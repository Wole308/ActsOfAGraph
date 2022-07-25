#include "app.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, edges map, active edges map, vertex partition mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

/*
// datasets 
snap.stanford.edu/snap/download.html (downloading snap)
python -m pip install snap-stanford (installing snap)
https://groups.google.com/g/snap-datasets/c/4_sMYlFRf9Q?pli=1 (smaller twitter7 dataset)
graphchallenge.mit.edu/data-sets
dango.rocks/datasets
http://konect.cc/networks/wikipedia_link_en/
http://konect.cc/categories/Hyperlink/
http://konect.cc/networks/delicious-ti/ (pr: 500, bfs: 726 MiEdges/s)
lgylym.github.io/big-graph/dataset.html
https://github.com/snap-stanford/snap-python/blob/master/examples/benchmark.py (SNAP graph in python)
https://snap.stanford.edu/snappy/index.html
https://snap.stanford.edu/snappy/index.html (snap RMAT? datasets)
Degree-aware Hybrid Graph Traversal on FPGA-HMC Platform (Jing (Jane) Li)(https://dl.acm.org/doi/pdf/10.1145/3174243.3174245)
Large-Scale Graph Processing on FPGAs with Caches for Thousands of Simultaneous Misses (https://www.epfl.ch/labs/lap/wp-content/uploads/2021/10/AsiaticiJun21_LargeScaleGraphProcessingOnFpgasWithCachesForThousandsOfSimultaneousMisses_ISCA21.pdf)
FDGLib: A Communication Library for Efficient Large-Scale Graph Processing in FPGA-Accelerated Data Centers (https://link.springer.com/content/pdf/10.1007/s11390-021-1242-y.pdf)

graphalytics.org/datasets (*)
github.com/GRAND-Lab/graph_datasets

# lightweight graph re-ordering: https://github.com/faldupriyank/dbg 
*/

#define APP_LOADEDGES
#define APP_LOADSRCVERTICES
#define APP_LOADDESTVERTICES
#define APP_LOADMASKS
#define APP_LOADROOTVID
#define APP_LOADSTATSINFO
#define APP_LOADMESSAGES // THE CAUSE OF ERROR.
#define APP_RUNSWVERSION
#define APP_RUNHWVERSION 
	
app::app(){}
app::~app(){
	cout<<"app::~app:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}

universalparams_t app::get_universalparams(std::string algo, unsigned int numiterations, unsigned int rootvid, unsigned int num_vertices, unsigned int num_edges, bool graphisundirected){
	universalparams_t universalparams;
	algorithm * algorithmobj = new algorithm();
	
	universalparams.ALGORITHM = algorithmobj->get_algorithm_id(algo);
	universalparams.NUM_ITERATIONS = numiterations; if(universalparams.ALGORITHM != BFS && universalparams.ALGORITHM != SSSP){ universalparams.NUM_ITERATIONS = 1; }
	universalparams.ROOTVID = rootvid;
	
	universalparams.NUM_VERTICES = num_vertices; 
	universalparams.NUM_EDGES = num_edges; 
	universalparams.AVERAGENUM_WORKEDGES_PER_CHANNEL = num_edges / NUM_PEs;
	
	universalparams.ISUNDIRECTEDGRAPH = graphisundirected;
	
	universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM = 1; // NEWCHANGE.
	
	universalparams.NUM_PARTITIONS_POW = 4;
	universalparams.NUM_PARTITIONS = 1 << universalparams.NUM_PARTITIONS_POW;

	universalparams.RED_SRAMSZ_POW = MAX_RED_SRAMSZ_POW;
	universalparams.RED_SRAMSZ = 1 << universalparams.RED_SRAMSZ_POW;
	
	universalparams.PROC_SRAMSZ_POW = MAX_PROC_SRAMSZ_POW;
	// universalparams.PROC_SRAMSZ = 1 << universalparams.PROC_SRAMSZ_POW;
	universalparams.PROC_SRAMSZ = ((1 << universalparams.PROC_SRAMSZ_POW) / NUM_PEs) * NUM_PEs;
	
	universalparams.KVDATA_RANGE_POW = ceil(log2((double)(universalparams.NUM_VERTICES)));
	universalparams.KVDATA_RANGE = universalparams.NUM_VERTICES;

	unsigned int ceilnum_reduce_partitions = 0;
	unsigned int num_reduce_partitions = (universalparams.NUM_VERTICES / NUM_PEs) / (universalparams.RED_SRAMSZ * VDATA_PACKINGSIZE); // PROC_SRAMSZ
	if(num_reduce_partitions == 0){ ceilnum_reduce_partitions = 1; }
		else if(num_reduce_partitions >= 1 && num_reduce_partitions < 16){ ceilnum_reduce_partitions = 16; }
			else if(num_reduce_partitions >= 16 && num_reduce_partitions < 256){ ceilnum_reduce_partitions = 256; }
				else { ceilnum_reduce_partitions = 4096; }
	cout<<"---+++++++++++++++--------------- num_reduce_partitions: "<<num_reduce_partitions<<", ceilnum_reduce_partitions: "<<ceilnum_reduce_partitions<<endl;
	universalparams.BATCH_RANGE_POW = ceil(log2((double)(ceilnum_reduce_partitions))) + ceil(log2((double)((universalparams.RED_SRAMSZ * VDATA_PACKINGSIZE)))); // PROC_SRAMSZ
	universalparams.BATCH_RANGE = universalparams.NUM_VERTICES / NUM_PEs;
	universalparams.BATCH_RANGE_KVS = universalparams.BATCH_RANGE / VECTOR_SIZE;

	if(universalparams.ALGORITHM == BFS){ 
		#ifdef BIT_TRAVERSAL_ALGORITHM
		universalparams.TREE_DEPTH = 1;
		#else 
		universalparams.TREE_DEPTH = 2;
		#endif
		universalparams.TREE_DEPTH = 1; } 
	else { universalparams.TREE_DEPTH = 2; }
	
	universalparams.REDUCESZ_POW = universalparams.RED_SRAMSZ_POW; // NEWCHANGE
	universalparams.REDUCESZ = (1 << universalparams.REDUCESZ_POW); 
	
	universalparams.REDUCEPARTITIONSZ = universalparams.RED_SRAMSZ * VDATA_PACKINGSIZE; // NEWCHANGE
	if(universalparams.REDUCEPARTITIONSZ > universalparams.BATCH_RANGE){ universalparams.REDUCEPARTITIONSZ = universalparams.BATCH_RANGE; } ///// NEWCHANGE.
	universalparams.REDUCEPARTITIONSZ_KVS2 = universalparams.REDUCEPARTITIONSZ / VECTOR2_SIZE;
	if(universalparams.ALGORITHM == BFS){ 
		#ifdef BIT_TRAVERSAL_ALGORITHM
		universalparams.NUMREDUCEPARTITIONS = 1;
		#else 
		universalparams.NUMREDUCEPARTITIONS = ((universalparams.KVDATA_RANGE / NUM_PEs) + (universalparams.REDUCEPARTITIONSZ - 1)) / universalparams.REDUCEPARTITIONSZ;
		#endif 
	}
	else { universalparams.NUMREDUCEPARTITIONS = ((universalparams.KVDATA_RANGE / NUM_PEs) + (universalparams.REDUCEPARTITIONSZ - 1)) / universalparams.REDUCEPARTITIONSZ; }
	
	universalparams.PROCESSPARTITIONSZ = universalparams.PROC_SRAMSZ * VDATA_PACKINGSIZE; // CRITICAL FIXME.
	universalparams.SUB_CHUNK_HEIGHT = ((universalparams.PROC_SRAMSZ / NUM_PEs) / VDATA_SHRINK_RATIO);
	universalparams.PROCESSPARTITIONSZ_KVS2 = (universalparams.PROCESSPARTITIONSZ / VECTOR2_SIZE);
	universalparams.NUMPROCESSEDGESPARTITIONS = ((universalparams.KVDATA_RANGE + (universalparams.PROCESSPARTITIONSZ - 1)) / universalparams.PROCESSPARTITIONSZ);
	
	universalparams.NUMLASTLEVELPARTITIONS = (1 << (universalparams.NUM_PARTITIONS_POW * universalparams.TREE_DEPTH));

	universalparams.NUM_EDGE_BANKS = 1;

	universalparams.VPTR_SHRINK_RATIO = universalparams.PROCESSPARTITIONSZ;

	universalparams.MAXHBMCAPACITY_UINT32 = ((1 << 28) / 4); // (256MB/4=64M)
	universalparams.MAXHBMCAPACITY_KVS2 = universalparams.MAXHBMCAPACITY_UINT32 / HBM_CHANNEL_VECTOR_SIZE;

	universalparams.KVSOURCEDRAMSZ = universalparams.MAXHBMCAPACITY_UINT32;
	universalparams.KVSOURCEDRAMSZ_KVS = (universalparams.KVSOURCEDRAMSZ / HBM_CHANNEL_VECTOR_SIZE);
	universalparams.PADDEDKVSOURCEDRAMSZ = universalparams.KVSOURCEDRAMSZ;
	universalparams.PADDEDKVSOURCEDRAMSZ_KVS = (universalparams.PADDEDKVSOURCEDRAMSZ / HBM_CHANNEL_VECTOR_SIZE);
	
	universalparams.HBMWORKCAPACITY_UINT32 = ((1 << 28) / 4) / 2;
	universalparams.VOLUMESIZEU32_WORKSPACEDATA = universalparams.HBMWORKCAPACITY_UINT32; 
	universalparams.VOLUMESIZEU32_WORKDATA = universalparams.HBMWORKCAPACITY_UINT32; 
	unsigned int EDGES_space_u32 = (universalparams.NUM_EDGES * 2) / NUM_PEs;
	unsigned int VPTR_space_u32 = universalparams.NUM_VERTICES / universalparams.VPTR_SHRINK_RATIO;
	cout<<">>> app:get_universalparams:[RE-ASSIGN CHECK]: EDGES_space_u32: "<<EDGES_space_u32<<", VPTR_space_u32: "<<VPTR_space_u32<<endl;
	if(EDGES_space_u32 + VPTR_space_u32 >= universalparams.VOLUMESIZEU32_WORKDATA){ // override
		cout<<"app: RE-ASSIGN. EDGES space("<<EDGES_space_u32<<") + VPTR space("<<VPTR_space_u32<<")(="<<EDGES_space_u32 + VPTR_space_u32<<") >= universalparams.VOLUMESIZEU32_WORKDATA("<<universalparams.VOLUMESIZEU32_WORKDATA<<"). OVERRIDING SET VALUES..."<<endl; 
		universalparams.VOLUMESIZEU32_WORKDATA = utilityobj->allignhigher_FACTOR((EDGES_space_u32 + VPTR_space_u32 + (MESSAGESDRAMSZ * 16) + ((1 << 16) * 16)), 16); // last is padding
		universalparams.VOLUMESIZEU32_WORKSPACEDATA = universalparams.MAXHBMCAPACITY_UINT32 - universalparams.VOLUMESIZEU32_WORKDATA;
		std::cout<<">> app[size]:: universalparams.VOLUMESIZEU32_WORKSPACEDATA: "<<universalparams.VOLUMESIZEU32_WORKSPACEDATA<<std::endl;
		std::cout<<">> app[size]:: universalparams.VOLUMESIZEU32_WORKDATA: "<<universalparams.VOLUMESIZEU32_WORKDATA<<std::endl;
	}
	universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA = 0; 
	universalparams.VOLUMEOFFSETKVS_WORKDATA = universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA + (universalparams.VOLUMESIZEU32_WORKSPACEDATA / VECTOR2_SIZE); 
	
	universalparams.VDRAMSZ = universalparams.KVSOURCEDRAMSZ;
	universalparams.PADDEDVDRAMSZ = universalparams.VDRAMSZ;
	universalparams.PADDEDVDRAMSZ_KVS = (universalparams.PADDEDVDRAMSZ / VECTOR_SIZE);
	
	universalparams.DRAMPADD = (16 * BLOCKRAM_SIZE * VECTOR_SIZE);
	universalparams.DRAMPADD_KVS = (universalparams.DRAMPADD / VECTOR_SIZE);

	universalparams.DRAMPADD_VPTRS = 1024; // 2 * universalparams.VPTR_SHRINK_RATIO * VECTOR2_SIZE; 
	universalparams.DRAMPADD_VPTRS_KVS = universalparams.DRAMPADD_VPTRS / VECTOR2_SIZE; // to avoid any spill-overs 
	
	universalparams.SRCBUFFER_SIZE = BLOCKRAM_SIZE - (4 * 4);
	universalparams.WORKBUFFER_SIZE = universalparams.SRCBUFFER_SIZE - (universalparams.NUM_PARTITIONS*4);
	return universalparams;
}
void app::print_active_partitions(unsigned int GraphIter, uint512_vec_dt * dram, universalparams_t universalparams, unsigned int num_partitions, bool printA, bool printB){
	if(universalparams.ALGORITHM == BFS || universalparams.ALGORITHM == SSSP){ 
		cout<< endl << TIMINGRESULTSCOLOR << ">>> app::print_active_vpartitions: printing active vertex partitions...: "<< RESET <<endl;
		
		pmask_dt * pmask0; pmask0 = new pmask_dt[512];
		unsigned int * emask0; emask0 = new pmask_dt[512];
		uint512_ivec_dt * tempdram = (uint512_ivec_dt *)dram;		
		unsigned int dram_BASEOFFSETKVS_VERTICESPARTITIONMASK = dram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		unsigned int dram_BASEOFFSETKVS_ACTIVEVERTICES = dram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key;
		unsigned int offset_kvs = dram_BASEOFFSETKVS_VERTICESPARTITIONMASK;
		unsigned int offset2_kvs = dram_BASEOFFSETKVS_ACTIVEVERTICES;
		for (buffer_type i=0; i<num_partitions; i++){
			pmask0[i] = tempdram[offset_kvs + i].data[GraphIter];
			emask0[i] = tempdram[offset2_kvs + i].data[GraphIter];
		}
		
		unsigned int total_num_actvvs = 0;
		#if defined(_DEBUGMODE_HOSTPRINTS3)
		if(printA==true){
			unsigned int num_actvps = 0;
			for(unsigned int t=0; t<num_partitions; t++){
				// if(pmask0[t] > 0  && t < 16){ cout<<t<<", "; }
				if(pmask0[t] > 0){ num_actvps += 1; }
				total_num_actvvs += pmask0[t];
			}
			cout<<"+++ iter "<<GraphIter<<": total number of active vertices in all HBM channels (varA): "<<total_num_actvvs<<" ("<<num_actvps<<" active partitions +++"<<endl;
		}
		if(printB==true){
			unsigned int num_actvedges = 0;
			unsigned int num_actv_edges = 0;
			for(unsigned int t=0; t<num_partitions; t++){
				// if(emask0[t] > 0  && t < 16){ cout<<emask0[t]<<", "; }
				// if(emask0[t] > 0){ num_actvedges += emask0[t]; num_actvps += 1; }
				num_actv_edges += emask0[t];
			}
			cout<<"^^^ iter "<<GraphIter<<": average number of active edges per HBM channel (varB): "<<num_actv_edges<<" ^^^"<<endl;
		}
		#endif 
	}
}	
void app::run(std::string setup, std::string algo, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1){
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	
	#ifdef TESTKERNEL
	cout<< TIMINGRESULTSCOLOR <<"================================================================== APP: THIS IS A TEST RUN ==================================================================" << RESET<< endl;
	#endif 
	
	vector<edge2_type> edgedatabuffer;
	vector<edge_t> vertexptrbuffer;
	vector<edge2_vec_dt> edges_final[MAXNUM_PEs];
	map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs]; // [MAXNUM_LLPs];
	keyvalue_t * edgeblock_map[MAXNUM_PEs][MAXNUM_VPs]; // FIXME.  triple_t * edgeblock_map[MAXNUM_PEs][MAXNUM_VPs]
	long double edgesprocessed_totals[128];
	long double timeelapsed_totals[128][8];
	unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS];
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ num_edges_processed[iter] = 0; }
	container_t container;
	vector<value_t> actvvs; actvvs.push_back(rootvid);
	tuple_t * vpartition_stats[MAXNUMGRAPHITERATIONS]; 
	tuple_t * vpmaskstats_merge[MAXNUMGRAPHITERATIONS][NUM_PEs]; 
	long double totaltime_ms = 0;
	binaryFile[0] = _binaryFile1;

	string GRAPH_NAME = ""; // "soc-orkut";
	string GRAPH_PATH = graph_path;
	bool graphisundirected_bool = true; if(graphisundirected == 0){ graphisundirected_bool = false; }
	
	prepare_graph * prepare_graphobj = new prepare_graph();
	// prepare_graphobj->create_graph(GRAPH_PATH, edgedatabuffer, vertexptrbuffer);
	prepare_graphobj->start(GRAPH_PATH, edgedatabuffer, vertexptrbuffer, graphisundirected_bool);
	unsigned int num_edges = edgedatabuffer.size();
	unsigned int num_vertices = vertexptrbuffer.size();
	cout<<"app:run: num_vertices: "<<num_vertices<<", num_edges: "<<num_edges<<endl;
	// exit(EXIT_SUCCESS); //
	
	// universalparams_t universalparams;
	universalparams_t universalparams = get_universalparams(algo, numiterations, rootvid, num_vertices, num_edges, graphisundirected_bool);
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ vpartition_stats[i] = new tuple_t[universalparams.NUMPROCESSEDGESPARTITIONS]; }
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ for(unsigned int j=0; j<NUM_PEs; j++){ vpmaskstats_merge[i][j] = new tuple_t[universalparams.NUMREDUCEPARTITIONS]; }}
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){ vpartition_stats[iter][t].A = 0; vpartition_stats[iter][t].B = 0; }}
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ for(unsigned int j=0; j<NUM_PEs; j++){ for(unsigned int t=0; t<universalparams.NUMREDUCEPARTITIONS; t++){ vpmaskstats_merge[iter][j][t].A = 0; vpmaskstats_merge[iter][j][t].B = 0; }}}
	utility * utilityobj = new utility(universalparams);
	utilityobj->printallparameters();
	// exit(EXIT_SUCCESS); //
	
	utilityobj = new utility(universalparams);
	loadgraphobj = new loadgraph(universalparams);
	loadedgesobj = new loadedges(universalparams);
	swkernelobj = new swkernel(universalparams);
	#ifdef FPGA_IMPL
	kernelobj = new goclkernel(universalparams);
	#else 
	kernelobj = new swkernel(universalparams);
	#endif
	acts_helper * actshelperobj = new acts_helper(universalparams);
	
	unsigned int mdramsz_kvs = (universalparams.NUM_EDGES + universalparams.NUM_VERTICES + (universalparams.NUM_VERTICES*2) + 1000000) / VECTOR2_SIZE;
	#ifndef SW_IMPL 
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<MAXNUM_PEs; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.MAXHBMCAPACITY_KVS2 * sizeof(uint512_vec_dt))); }				
	vdram = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.MAXHBMCAPACITY_KVS2 * sizeof(uint512_vec_dt)));
	mdram = (uint512_vec_dt *) aligned_alloc(4096, mdramsz_kvs * sizeof(uint512_vec_dt));
	#else
	for(unsigned int i=0; i<MAXNUM_PEs; i++){ kvbuffer[i] = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS2]; }
	vdram = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS2];
	mdram = new uint512_vec_dt[mdramsz_kvs]; // stores {edges, vptrs, active vertices} all in large DDR memory. '1000000' is padding
	#endif
	#endif 
	
	#ifdef FPGA_IMPL
	for(unsigned int j=0; j<MAXNUM_PEs; j++){ edges[j] = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }	// 2 because edge_type consist 4 bytes not 8 bytes			
	#else
	for(unsigned int j=0; j<MAXNUM_PEs; j++){ edges[j] = new uint512_vec_dt[universalparams.PADDEDKVSOURCEDRAMSZ_KVS]; }
	#endif
	
	for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int k=0; k<universalparams.MAXHBMCAPACITY_KVS2; k++){ for(unsigned int v=0; v<KEYVALUEDATA_PACKINGSIZE; v++){ kvbuffer[i][k].data[v].key = 0; kvbuffer[i][k].data[v].value = 0; }}} // REMOVEME.
	for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int v_p=0; v_p<MAXNUM_VPs; v_p++){ edges_map[i][v_p] = new map_t[MAXNUM_LLPs]; }}
	// for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int v_p=0; v_p<MAXNUM_VPs; v_p++){ edgeblock_map[i][v_p] = new keyvalue_t[MAXNUM_EDGEBLOCKS_PER_VPARTITION]; for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){ edgeblock_map[i][v_p][t].key = 0; edgeblock_map[i][v_p][t].value = 0; }}}		
	for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int v_p=0; v_p<MAXNUM_VPs; v_p++){ edgeblock_map[i][v_p] = new keyvalue_t[ALLIGNED_MAXNUM_EDGEBLOCKS_PER_VPARTITION]; for(unsigned int t=0; t<ALLIGNED_MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){ edgeblock_map[i][v_p][t].key = 0; edgeblock_map[i][v_p][t].value = 0; }}}		
	// cout<<"app: MAXNUM_EDGEBLOCKS_PER_VPARTITION: "<<MAXNUM_EDGEBLOCKS_PER_VPARTITION<<endl;
	// cout<<"app: ALLIGNED_MAXNUM_EDGEBLOCKS_PER_VPARTITION: "<<ALLIGNED_MAXNUM_EDGEBLOCKS_PER_VPARTITION<<endl;
	// exit(EXIT_SUCCESS);
	
	// load workload information
	globalparams_TWOt globalparams;
	globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsK.VOLUMEOFFSETKVS_WORKDATA = 0;
	globalparams.globalparamsK.VOLUMEOFFSETKVS_WORKSPACEDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsK.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsK.VOLUMESIZEU32_WORKDATA = 0;
	globalparams.globalparamsK.VOLUMESIZEU32_WORKSPACEDATA = 0;
	globalparams.globalparamsK.SIZE_EDGES = 0;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsK.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsK.SIZE_EDGESMAP = 0;
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsE.VOLUMEOFFSETKVS_WORKDATA = 0;
	globalparams.globalparamsE.VOLUMEOFFSETKVS_WORKSPACEDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsE.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsE.VOLUMESIZEU32_WORKDATA = 0;
	globalparams.globalparamsE.VOLUMESIZEU32_WORKSPACEDATA = 0;
	globalparams.globalparamsE.SIZE_EDGES = 0;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsE.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsE.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsE.SIZE_EDGESMAP = 0;
	globalparams.globalparamsE.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsE.SIZE_KVDRAM = 0;
	globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsV.VOLUMEOFFSETKVS_WORKDATA = 0;
	globalparams.globalparamsV.VOLUMEOFFSETKVS_WORKSPACEDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsV.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsV.VOLUMESIZEU32_WORKDATA = 0;
	globalparams.globalparamsV.VOLUMESIZEU32_WORKSPACEDATA = 0;
	globalparams.globalparamsV.SIZE_EDGES = 0;
	globalparams.globalparamsV.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsV.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsV.SIZE_EDGESMAP = 0;
	globalparams.globalparamsV.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsV.SIZE_KVDRAM = 0;
	globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsM.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsM.VOLUMEOFFSETKVS_WORKDATA = 0;
	globalparams.globalparamsM.VOLUMEOFFSETKVS_WORKSPACEDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_EDGESMAP = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsM.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsM.SIZE_EDGES = 0;
	globalparams.globalparamsM.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsM.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsM.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsM.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsM.SIZE_EDGESMAP = 0;
	globalparams.globalparamsM.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsM.SIZE_KVDRAM = 0;
	globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE = 0;
	
	unsigned int total_edges_processed = 0;
	
	// edges
	#ifdef APP_LOADEDGES
	globalparams = loadedgesobj->start(0, vertexptrbuffer, edgedatabuffer, (vptr_type **)edges, edges, edges_final, edges_map, edgeblock_map, &container, globalparams);
	// exit(EXIT_SUCCESS);
	#endif 
	
	globalparams.globalparamsV.SIZE_EDGES = 0; 
	globalparams.globalparamsV.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA = MESSAGES_BASEOFFSETKVS_MESSAGESDATA + MESSAGESDRAMSZ;
	globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA + (globalparams.globalparamsV.SIZE_EDGES / VECTOR2_SIZE);
	globalparams.globalparamsM.SIZE_EDGES = universalparams.NUM_EDGES + 1000; //
	globalparams.globalparamsM.SIZE_VERTEXPTRS = universalparams.NUM_VERTICES + 1000; //
	globalparams.globalparamsM.BASEOFFSETKVS_EDGESDATA = MESSAGES_BASEOFFSETKVS_MESSAGESDATA + MESSAGESDRAMSZ;
	globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsM.BASEOFFSETKVS_EDGESDATA + (globalparams.globalparamsM.SIZE_EDGES / VECTOR2_SIZE);
	// exit(EXIT_SUCCESS);
	
	// adjust workspace and workload capacities
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"app::: [after load edges] checking to re-assign workspace and workload capacities... "<<endl;
	#endif 
	unsigned int EDGES_space_u32 = edges_final[0].size() * EDGEDATA_PACKINGSIZE * (HBM_BITWIDTHx32 / EDGEDATA_PACKINGSIZE); // 2;
	unsigned int VPTR_space_u32 = universalparams.NUM_VERTICES / universalparams.VPTR_SHRINK_RATIO;
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<">>> app:[RE-ASSIGN CHECK]: EDGES_space_u32: "<<EDGES_space_u32<<", VPTR_space_u32: "<<VPTR_space_u32<<", universalparams.VOLUMESIZEU32_WORKDATA: "<<universalparams.VOLUMESIZEU32_WORKDATA<<endl;
	#endif 
	if(EDGES_space_u32 + VPTR_space_u32 >= universalparams.VOLUMESIZEU32_WORKDATA){ // edges per channel might have changed when loading edges, reassign it again
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"app[RE-ASSIGN]. EDGES space("<<EDGES_space_u32<<") + VPTR space("<<VPTR_space_u32<<")("<<EDGES_space_u32 + VPTR_space_u32<<") >= universalparams.VOLUMESIZEU32_WORKDATA("<<universalparams.VOLUMESIZEU32_WORKDATA<<"). OVERRIDING SET VALUES..."<<endl; 
		#endif
		universalparams.VOLUMESIZEU32_WORKDATA = utilityobj->allignhigher_FACTOR((EDGES_space_u32 + VPTR_space_u32 + (MESSAGESDRAMSZ * 16) + ((1 << 16) * 16)), 16); // last is padding
		universalparams.VOLUMESIZEU32_WORKSPACEDATA = universalparams.MAXHBMCAPACITY_UINT32 - universalparams.VOLUMESIZEU32_WORKDATA;
		std::cout<< TIMINGRESULTSCOLOR << ">> app[RE-ASSIGN]:: universalparams.VOLUMESIZEU32_WORKDATA: "<<universalparams.VOLUMESIZEU32_WORKDATA<<", universalparams.VOLUMESIZEU32_WORKSPACEDATA: "<<universalparams.VOLUMESIZEU32_WORKSPACEDATA<< RESET <<std::endl;
	}
	universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA = 0; 
	universalparams.VOLUMEOFFSETKVS_WORKDATA = universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA + (universalparams.VOLUMESIZEU32_WORKSPACEDATA / VECTOR2_SIZE); 
	
	// src vertices data
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::loadvertexdata:: loading source vertex datas... "<<endl;
	#endif 
	globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR + (globalparams.globalparamsK.SIZE_VERTEXPTRS / VALUEDATA_PACKINGSIZE) + universalparams.DRAMPADD_KVS;
	// globalparams.globalparamsK.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_SRCVERTICESDATA = MAX_BLOCKRAM_VSRCDATA_SIZE * VDATA_PACKINGSIZE;
	globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR + (globalparams.globalparamsE.SIZE_VERTEXPTRS / VALUEDATA_PACKINGSIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR + (globalparams.globalparamsV.SIZE_VERTEXPTRS / VALUEDATA_PACKINGSIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsV.SIZE_SRCVERTICESDATA = NUM_PEs * universalparams.NUMREDUCEPARTITIONS * universalparams.REDUCEPARTITIONSZ_KVS2 * VECTOR2_SIZE;
		if(globalparams.globalparamsV.SIZE_SRCVERTICESDATA > universalparams.MAXHBMCAPACITY_UINT32 - 4000000){ globalparams.globalparamsV.SIZE_SRCVERTICESDATA = universalparams.MAXHBMCAPACITY_UINT32 - 4000000; cout<<"app: RE-ASSIGN. globalparams.globalparamsV.SIZE_SRCVERTICESDATA. reduced to ensure fit."<<endl; } // FIXME.
	globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR + (globalparams.globalparamsM.SIZE_VERTEXPTRS / VALUEDATA_PACKINGSIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsM.SIZE_SRCVERTICESDATA = 0; 
	#ifdef APP_LOADSRCVERTICES
	loadgraphobj->loadvertexdata(algo, (keyvalue_t *)vdram, globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsV.SIZE_SRCVERTICESDATA, globalparams.globalparamsV, 0, SOURCE, universalparams);
	for(unsigned int i = 0; i < NUM_PEs; i++){ loadgraphobj->loadvertexdata(algo, (keyvalue_t *)kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsK.SIZE_SRCVERTICESDATA, globalparams.globalparamsK, 0, SOURCE, universalparams); }					
	#endif 
	// exit(EXIT_SUCCESS);
	
	// dest vertices data 
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::loadvertexdata:: loading dest vertex datas... "<<endl;
	#endif 
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + (globalparams.globalparamsK.SIZE_SRCVERTICESDATA / VALUEDATA_PACKINGSIZE);
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = universalparams.BATCH_RANGE; // (1 << universalparams.BATCH_RANGE_POW); // universalparams.BATCH_RANGE; 
	globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA + (globalparams.globalparamsE.SIZE_SRCVERTICESDATA / VALUEDATA_PACKINGSIZE);
	globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparams.globalparamsV.SIZE_SRCVERTICESDATA / VALUEDATA_PACKINGSIZE);
	globalparams.globalparamsV.SIZE_DESTVERTICESDATA = 0; 
	globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA + (globalparams.globalparamsM.SIZE_SRCVERTICESDATA / VALUEDATA_PACKINGSIZE);
	globalparams.globalparamsM.SIZE_DESTVERTICESDATA = 0; 
	#ifdef APP_LOADDESTVERTICES
	loadgraphobj->loadvertexdata(algo, (keyvalue_t *)vdram, globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsV.SIZE_DESTVERTICESDATA, globalparams.globalparamsV, 0, DEST, universalparams);
	for(unsigned int i = 0; i < NUM_PEs; i++){ loadgraphobj->loadvertexdata(algo, (keyvalue_t *)kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsK.SIZE_DESTVERTICESDATA, globalparams.globalparamsK, 0, DEST, universalparams); }
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// load maps 
	#ifdef APP_LOADMASKS
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::loadactvvertices:: loading maps ... "<<endl;
	#endif 
	for(unsigned int i = 0; i < NUM_PEs; i++){ globalparams = loadgraphobj->loadactvvertices(vdram, actvvs, globalparams, universalparams); } // (keyy_t *)&kvbuffer[i], &container, 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"app::loadmaps:: generating vmask... "<<endl;
	#endif 
	globalparams = loadgraphobj->loadmaps(actvvs, vdram, edges, edges_map, edgeblock_map, globalparams, universalparams);
	#endif 
	globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA + (globalparams.globalparamsM.SIZE_DESTVERTICESDATA / VALUEDATA_PACKINGSIZE);
	globalparams.globalparamsM.SIZE_ACTIVEVERTICES = CONFIG_HYBRIDGPMODE_MDRAMSECTIONSZ * MAXNUMGRAPHITERATIONS * 2; // (universalparams.NUM_VERTICES * 2); // current and next it active vertices
	// exit(EXIT_SUCCESS); //
	
	/* ////////////////////////////////////////////////
	for(unsigned int t=0; t<4; t++){
		for(unsigned int v=0; v<8; v++){
			cout<<"app1: vdram["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].key: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].key<<endl; 
			cout<<"app1: vdram["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].value: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].value<<endl; 
		}
	}
	// exit(EXIT_SUCCESS); //
	//////////////////////////////////////////////// */
	
	/* for(unsigned int t=0; t<16; t++){
		for(unsigned int v=0; v<8; v++){
			vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].key = 777; 
			vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].value = 777;
		}
	}
	for(unsigned int i = 0; i < NUM_PEs; i++){
		for(unsigned int t=0; t<16; t++){
			for(unsigned int v=0; v<8; v++){
				kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].key = 777; 
				kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].value = 777;
			}
		}
	} */
	
	// setting root vid
	#ifdef APP_LOADROOTVID
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::setrootvid:: setting root vid(s)... "<<endl;
	#endif 
	loadgraphobj->setrootvid((uint512_vec_dt *)vdram, actvvs, globalparams.globalparamsV, universalparams);
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// stats info 
	#ifdef APP_LOADSTATSINFO
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::loadoffsetmarkers:: loading offset markers... "<<endl;
	#endif 
	globalparams = loadgraphobj->loadoffsetmarkers((keyvalue_t **)edges, edges_final, &container, globalparams, universalparams); 
	loadgraphobj->accumstats(kvbuffer, edges, globalparams, universalparams); // NEWCHANGE. // OBSOLETE.
	#endif 
	// exit(EXIT_SUCCESS); //
	
	/* ////////////////////////////////////////////////
	for(unsigned int t=0; t<4; t++){
		for(unsigned int v=0; v<8; v++){
			cout<<"app2: vdram["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].key: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].key<<endl; 
			cout<<"app2: vdram["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].value: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].value<<endl; 
		}
	}
	// exit(EXIT_SUCCESS); //
	//////////////////////////////////////////////// */

	// messages
	#ifdef APP_LOADMESSAGES
	globalparams.globalparamsV = loadgraphobj->finishglobaparamsV(globalparams.globalparamsV, universalparams);
	globalparams.globalparamsM = loadgraphobj->finishglobaparamsM(globalparams.globalparamsM, universalparams);
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::loadmessages:: loading messages... "<<endl;
	#endif 
	globalparams = loadgraphobj->loadmessages(mdram, vdram, edges, kvbuffer, &container, globalparams, universalparams);
	#endif

	// others
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::experiements: resetting kvdram & kvdram workspaces..."<<endl;
	#endif 
	for(unsigned int i=0; i<NUM_PEs; i++){
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM], globalparams.globalparamsK.SIZE_KVDRAM);
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE], globalparams.globalparamsK.SIZE_KVDRAM);
	}
	
	/* ////////////////////////////////////////////////
	for(unsigned int t=0; t<4; t++){
		for(unsigned int v=0; v<8; v++){
			cout<<"app3: vdram["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].key: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].key<<endl; 
			cout<<"app3: vdram["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].value: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].value<<endl; 
		}
	}
	// exit(EXIT_SUCCESS); //
	//////////////////////////////////////////////// */

	// acts helper
	#ifdef APP_RUNSWVERSION
	universalparams.NUM_ITERATIONS = actshelperobj->extract_stats(actvvs, vertexptrbuffer, edgedatabuffer, edgesprocessed_totals, vpartition_stats, vpmaskstats_merge, num_edges_processed);
	if(numiterations < universalparams.NUM_ITERATIONS){ universalparams.NUM_ITERATIONS = numiterations; }
	cout<<"app:: extract_stats finsished successfully. "<<universalparams.NUM_ITERATIONS<<" iterations run."<<endl;
	if(universalparams.ALGORITHM == BFS || universalparams.ALGORITHM == SSSP){ for(unsigned int t=0; t<MAXNUMGRAPHITERATIONS; t++){ total_edges_processed += edgesprocessed_totals[t]; }} else { total_edges_processed = universalparams.NUM_EDGES; }
	#endif
	
	// merge kv and edges dram for extra-large vertices 
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<">>> app: populating vdramA, vdramB and vdramC... "<<endl;
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"app:: universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA: "<<universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA<<endl;
	cout<<"app:: universalparams.VOLUMESIZEU32_WORKSPACEDATA: "<<universalparams.VOLUMESIZEU32_WORKSPACEDATA<<endl;
	cout<<"app:: universalparams.VOLUMEOFFSETKVS_WORKDATA: "<<universalparams.VOLUMEOFFSETKVS_WORKDATA<<endl;
	cout<<"app:: universalparams.VOLUMESIZEU32_WORKDATA: "<<universalparams.VOLUMESIZEU32_WORKDATA<<endl;
	cout<<"app::ERROR: universalparams.MAXHBMCAPACITY_UINT32: "<<universalparams.MAXHBMCAPACITY_UINT32<<endl; 
	#endif 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<(universalparams.VOLUMESIZEU32_WORKDATA / VECTOR2_SIZE); t++){ kvbuffer[i][universalparams.VOLUMEOFFSETKVS_WORKDATA + t] = edges[i][t]; }}
	if(((universalparams.VOLUMEOFFSETKVS_WORKDATA + (universalparams.VOLUMESIZEU32_WORKDATA / VECTOR2_SIZE)) * sizeof(uint512_vec_dt)) > (universalparams.MAXHBMCAPACITY_UINT32 * sizeof(unsigned int))){ cout<<"app::ERROR: CAN NOT merge. dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
	// exit(EXIT_SUCCESS);
	
	/* ////////////////////////////////////////////////
	for(unsigned int t=0; t<4; t++){
		for(unsigned int v=0; v<8; v++){
			cout<<"app4: vdram["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].key: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].key<<endl; 
			cout<<"app4: vdram["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].value: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].value<<endl; 
		}
	}
	// exit(EXIT_SUCCESS); //
	//////////////////////////////////////////////// */

	// load partitions stats
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<">>> app: populating active streaming partitions... "<<endl;		
	#endif 
	uint512_ivec_dt * tempvdram = (uint512_ivec_dt *)vdram;		
	unsigned int vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	// unsigned int vdram_BASEOFFSETKVS_ACTIVEVERTICES = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key;
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ // MAXNUMGRAPHITERATIONS
		for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){ tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + t].data[iter] = vpartition_stats[iter][t].A; }
		// for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){ tempvdram[vdram_BASEOFFSETKVS_ACTIVEVERTICES + t].data[iter] = vpartition_stats[iter][t].B / NUM_PEs; }
		
		// for(unsigned int i=0; i<NUM_PEs; i++){ 
			// uint512_ivec_dt * tempkvdram = (uint512_ivec_dt *)kvbuffer[i];	
			// unsigned int kvdram_BASEOFFSETKVS_ACTIVEVERTICES = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key;
			// for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){ tempkvdram[kvdram_BASEOFFSETKVS_ACTIVEVERTICES + t].data[iter] = vpmaskstats_merge[iter][i][t].A; }
		// }		
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"----------------------- app: printing active partitions for acts process-partition-reduce stage... -----------------------"<<endl;
	for(unsigned int GraphIter=0; GraphIter<MAXNUMGRAPHITERATIONS; GraphIter++){ print_active_partitions(GraphIter, vdram, universalparams, universalparams.NUMPROCESSEDGESPARTITIONS, true, true); }
	cout<<"----------------------- app: printing active partitions for merge stage... -----------------------"<<endl;
	for(unsigned int i=0; i<1; i++){ for(unsigned int GraphIter=0; GraphIter<MAXNUMGRAPHITERATIONS; GraphIter++){ print_active_partitions(GraphIter, kvbuffer[i], universalparams, universalparams.NUMREDUCEPARTITIONS, true, true); }} // NUM_PEs
	#endif
	#endif
	// exit(EXIT_SUCCESS); //
	
	/* ////////////////////////////////////////////////
	for(unsigned int t=0; t<16; t++){
		for(unsigned int v=0; v<8; v++){
			cout<<"app30: vdram3["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].key: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].key<<endl; 
			cout<<"app30: vdram3["<<globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t<<"].data["<<v<<"].value: "<<vdram[globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + t].data[v].value<<endl; 
		}
	}
	exit(EXIT_SUCCESS); //
	//////////////////////////////////////////////// */
	
	// run_hw
	#ifdef APP_RUNHWVERSION
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> app::run_hw: app started. ("<<actvvs.size()<<" active vertices)"<< RESET <<endl;
	long double total_time_elapsed = kernelobj->runapp(binaryFile, (uint512_vec_dt *)vdram, (uint512_vec_dt **)edges, (uint512_vec_dt **)kvbuffer, timeelapsed_totals, 
		num_edges_processed, vertexptrbuffer, edgedatabuffer, universalparams);
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// output
	unsigned int num_traversed_edges = actshelperobj->getfeedback("app", graph_path, vdram, vdram, vdram, vdram, kvbuffer, universalparams);
	actshelperobj->verifyresults(vdram, globalparams.globalparamsV, universalparams);
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<endl<<">>> app::run_hw: total_edges_processed: "<<total_edges_processed<<" edges ("<<total_edges_processed/1000000<<" million edges)"<<endl;
	cout<<">>> app::run_hw: num_traversed_edges: "<<num_traversed_edges<<" edges ("<<num_traversed_edges/1000000<<" million edges)"<<endl;
	cout<<">>> app::run_hw: total_time_elapsed: "<<total_time_elapsed<<" ms ("<<total_time_elapsed/1000<<" s)"<<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput: "<<((total_edges_processed / total_time_elapsed) * (1000))<<" Edges / sec, "<<((total_edges_processed / total_time_elapsed) / (1000))<<" Million edges / sec, "<<((total_edges_processed / total_time_elapsed) / (1000000))<<" Billion edges / sec"<< RESET <<endl;	
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput projection for 32 workers: "<<((((total_edges_processed / total_time_elapsed) / (1000)) * 32) / NUM_PEs)<<" Million edges / sec"<< RESET <<endl;
	#endif 
	
	actshelperobj->extract_stats(actvvs, vertexptrbuffer, edgedatabuffer, edgesprocessed_totals, vpartition_stats, vpmaskstats_merge, num_edges_processed);
	summary(GRAPH_PATH, vdram, globalparams.globalparamsV);
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<endl;
	cout<<"app::printdataset: printing dataset parameters..."<<endl;
	cout<<">>> app:: algo: "<<universalparams.ALGORITHM<<endl;
	if(universalparams.ALGORITHM==PAGERANK){ cout<<">>> app:: algo: PAGERANK"<<endl; }
	else if(universalparams.ALGORITHM==CF){ cout<<">>> app:: algo: CF"<<endl; }
	else if(universalparams.ALGORITHM==HITS){ cout<<">>> app:: algo: HITS"<<endl; }
	else if(universalparams.ALGORITHM==SPMV){ cout<<">>> app:: algo: SPMV"<<endl; }
	else if(universalparams.ALGORITHM==BFS){ cout<<">>> app:: algo: BFS"<<endl; }
	else if(universalparams.ALGORITHM==SSSP){ cout<<">>> app:: algo: SSSP"<<endl; }
	else{ cout<<">>> app:: algo: not specified. EXITING..."<<endl; exit(EXIT_FAILURE); }
	cout<<">>> app:: num iterations: "<<universalparams.NUM_ITERATIONS<<endl;
	cout<<">>> app:: graphname: "<<GRAPH_NAME<<endl;
	cout<<">>> app:: GRAPH_PATH: "<<GRAPH_PATH<<endl;
	cout<<">>> app:: NUM_VERTICES: "<<universalparams.NUM_VERTICES<<endl; 
	cout<<">>> app:: NUM_EDGES: "<<universalparams.NUM_EDGES<<endl;
	#endif 
	
	#ifdef TESTKERNEL
	cout<< TIMINGRESULTSCOLOR <<"================================================================== APP: THIS WAS A TEST RUN ==================================================================" << RESET << endl;
	#endif 
	return;
}

void app::summary(string graph_path, uint512_vec_dt * vdram, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<endl<<"app::summary: verifying vertex data... "<<endl;
	#endif

	cout<<endl;
	#ifdef CONFIG_ACTS_MEMORYLAYOUT
	std::cout<< TIMINGRESULTSCOLOR << "app:: CONFIG_ACTS_MEMORYLAYOUT DEFINED: "<< RESET << std::endl;	
	#endif 
	#ifdef CONFIG_ACTS_PARTITIONINGLOGIC
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_ACTS_PARTITIONINGLOGIC DEFINED: "<<std::endl;	
	#endif
	#ifdef CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC DEFINED: "<< RESET << std::endl;	
	#endif
	#ifdef CONFIG_ACTS_HYBRIDLOGIC
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_ACTS_HYBRIDLOGIC DEFINED: "<< RESET << std::endl;	
	#endif
	#ifdef CONFIG_ACTS_VERYBARE
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_ACTS_VERYBARE DEFINED: "<< RESET << std::endl;	
	#endif

	#ifndef CONFIG_ACTS_MEMORYLAYOUT
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_TRADITIONAL_MEMORYLAYOUT DEFINED: "<< RESET << std::endl;	
	#endif 
	#ifndef CONFIG_ACTS_PARTITIONINGLOGIC
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_TRADITIONAL_PARTITIONINGLOGIC DEFINED: "<< RESET << std::endl;	
	#endif
	#ifndef CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_TRADITIONAL_RECURSIVEPARTITIONINGLOGIC DEFINED: "<< RESET << std::endl;	
	#endif
	#ifndef CONFIG_ACTS_HYBRIDLOGIC
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_TRADITIONAL_HYBRIDLOGIC DEFINED: "<< RESET << std::endl;	
	#endif
	#ifndef CONFIG_ACTS_VERYBARE
	std::cout<< TIMINGRESULTSCOLOR <<"app:: CONFIG_ACTS_VERYBARE NOT DEFINED: "<< RESET << std::endl;	
	#endif
	cout<<endl;
	return;
}
















