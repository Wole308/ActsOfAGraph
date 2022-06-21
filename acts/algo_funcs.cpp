/* 
All algorithms: 
https://neo4j.com/docs/graph-data-science/current/algorithms/
https://iss.oden.utexas.edu/?p=projects/galois/analytics/betweenness_centrality

*/

value_t PROCESSP0_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	#pragma HLS INLINE // NEWCHANGE.
	value_t res = 0;
	#ifdef CONFIG_GOLDENKERNEL
	// res = udata + edgew;
	if(GraphAlgo == PAGERANK){
		res = udata + edgew;
	} else if(GraphAlgo == CF){
		res = udata;
	} else if(GraphAlgo == SPMV){
		res = udata;
	} else if(GraphAlgo == BFS){
		res = NAp;
	} else if(GraphAlgo == SSSP){
		res = udata + edgew;
	} else {
		res = NAp;
	}
	#else
		#if defined(PR_ALGORITHM)
			res = udata + edgew;
		#elif defined(CF_ALGORITHM)
			// source: https://mrmgroup.cs.princeton.edu/papers/taejun_micro16.pdf (Graphicionado)
			// process edge & reduce combined here (source: graphicionado paper)
			// --- Collaborative Filtering ---:
			// Process Edges: Executed in Reduce. ({Ew, Uprop} from source is sent to destination vertex)
			// Reduce: Function of (uprop, Ew, Vprop) is executed 
			// Apply: Function of (Vprop, Vtemp) is executed 
			// Finish:
			res = udata;
		#elif defined(HITS_ALGORITHM)	
			// https://www.geeksforgeeks.org/hyperlink-induced-topic-search-hits-algorithm-using-networxx-module-python/
			res = udata;
		#elif defined(SPMV_ALGORITHM)
			// source: https://www.baeldung.com/cs/graph-connected-components
			// source: https://cs.usm.maine.edu/~briggs/webPage/c161/projects/graphColoring.htmls
			// source: https://www.usenix.org/system/files/conference/osdi12/osdi12-final-126.pdf (GraphChi)
			// --- Connected Components ---:
			// Process Edges: Each vertex writes its id ("label") to its edges. 
			// Reduce: Vertex chooses the minimum label of its neighbors; 
			// Apply: A neighbor is scheduled only if a label in a connecting edge changes, which we implement by using selective scheduling. 
			// Finish: sets of vertices with equal labels are interpreted as connected components or communities, respectively.
			res = udata;
		#elif defined(BFS_ALGORITHM)
			res = NAp;
		#elif defined(SSSP_ALGORITHM)
			res = udata + edgew;
		#else
			NOT DEFINED.
		#endif
	#endif
	return res;
}

value_t REDUCEP0_reducefunc(value_t vtemp, value_t vdata, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	#pragma HLS INLINE // NEWCHANGE.
	value_t temp = 0;
	#ifdef CONFIG_GOLDENKERNEL
	// temp = UTILP0_amax(vtemp, 1);
	unsigned int lamda = 1;
	unsigned int ew=1;
	if(GraphAlgo == PAGERANK){
		temp = vtemp + res;
	} else if(GraphAlgo == CF){
		temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
	} else if(GraphAlgo == HITS){
		temp = vtemp + res;
	} else if(GraphAlgo == SPMV){
		temp = UTILP0_amin(vtemp, res);
	} else if(GraphAlgo == BFS){
		temp = UTILP0_amin(vtemp, GraphIter);
	} else if(GraphAlgo == SSSP){
		temp = UTILP0_amin(vtemp, res);
	} else {
		// NOT DEFINED
		temp = 0;
	}
	#else
		#if defined(PR_ALGORITHM)
			// Atomic Compute
			unsigned int alpha = 0.5;
			unsigned int vdeg = 1;
			temp = ((alpha + (1-alpha)*vtemp) / vdeg) + res;
			// temp = vtemp + res;
		#elif defined(CF_ALGORITHM)
			// Atomic Compute
			unsigned int ew = 1;
			unsigned int lamda = 0.5;
			unsigned int gamma = 0.5;
			temp = vdata + gamma*(vtemp + ((ew - vtemp*res)*res - lamda*vdata));
		#elif defined(HITS_ALGORITHM)
			// Atomic Add
			temp = vtemp + res;
			// atomicAdd(&hrank_next[src], arank_curr[dest]);
			// atomicAdd(&arank_next[dest], hrank_curr[src]);
		#elif defined(SPMV_ALGORITHM)
			// Atomic Min
			temp = UTILP0_amin(vtemp, res);
		#elif defined(BFS_ALGORITHM)
			// Atomic Min
			#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
			temp = UTILP0_amax(vtemp, 1);
			#else 
			temp = UTILP0_amin(vtemp, GraphIter);
			#endif 
		#elif defined(SSSP_ALGORITHM)
			// Atomic Min
			temp = UTILP0_amin(vtemp, res);
		#else 
			NOT DEFINED.
		#endif
	#endif
	return temp;
}

