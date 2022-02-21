value_t acts_all::PROCESSP0_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	value_t res = 0;
	#ifdef CUSTOMLOGICFOREACHALGORITHM
		#if defined(PR_ALGORITHM)
			res = udata + edgew;
		#elif defined(CF_ALGORITHM)
			// source: https://mrmgroup.cs.princeton.edu/papers/taejun_micro16.pdf (Graphicionado)
			// process edge & reduce combined here (source: graphicionado paper)
			/* --- Collaborative Filtering ---:
			Process Edges: Executed in Reduce. ({Ew, Uprop} from source is sent to destination vertex)
			Reduce: Function of (uprop, Ew, Vprop) is executed 
			Apply: Function of (Vprop, Vtemp) is executed 
			Finish: */
			res = udata;
		#elif defined(CC_ALGORITHM)
			// source: https://www.baeldung.com/cs/graph-connected-components
			// source: https://cs.usm.maine.edu/~briggs/webPage/c161/projects/graphColoring.htmls
			// source: https://www.usenix.org/system/files/conference/osdi12/osdi12-final-126.pdf (GraphChi)
			/* --- Connected Components ---:
			Process Edges: Each vertex writes its id ("label") to its edges. 
			Reduce: Vertex chooses the minimum label of its neighbors; 
			Apply: A neighbor is scheduled only if a label in a connecting edge changes, which we implement by using selective scheduling. 
			Finish: sets of vertices with equal labels are interpreted as connected components or communities, respectively. */
			res = udata;
		#elif defined(BFS_ALGORITHM)
			res = NAp;
		#elif defined(SSSP_ALGORITHM)
			res = udata + edgew;
		#else
			NOT DEFINED.
		#endif
	#else 
	if(GraphAlgo == PAGERANK){
		res = udata + edgew;
	} else if(GraphAlgo == CF){
		res = udata;
	} else if(GraphAlgo == CC){
		res = udata;
	} else if(GraphAlgo == BFS){
		res = NAp;
	} else if(GraphAlgo == SSSP){
		res = udata + edgew;
	} else {
		res = NAp;
	}
	#endif
	return res;
}

value_t acts_all::REDUCEP0_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef CUSTOMLOGICFOREACHALGORITHM
		#if defined(PR_ALGORITHM)
			temp = vtemp + res;
		#elif defined(CF_ALGORITHM)
			unsigned int ew = 1;
			unsigned int lamda = 1;
			temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
		#elif defined(CC_ALGORITHM)
			temp = UTILP0_amin(vtemp, res);
		#elif defined(BFS_ALGORITHM)
			temp = UTILP0_amin(vtemp, GraphIter);
		#elif defined(SSSP_ALGORITHM)
			temp = UTILP0_amin(vtemp, res);
		#else 
			NOT DEFINED.
		#endif
	#else 
	if(GraphAlgo == PAGERANK){
		temp = vtemp + res;
	} else if(GraphAlgo == CF){
		temp = vtemp + ((ew - vtemp*res)*res - lamda*vtemp);
	} else if(GraphAlgo == CC){
		temp = UTILP0_amin(vtemp, res);
	} else if(GraphAlgo == BFS){
		temp = UTILP0_amin(vtemp, GraphIter);
	} else if(GraphAlgo == SSSP){
		temp = UTILP0_amin(vtemp, res);
	} else {
		NOT DEFINED
	}
	#endif
	return temp;
}
