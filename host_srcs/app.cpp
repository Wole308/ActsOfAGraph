#include "app.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// active vertices area {actv vertices data}
// stats area {actv uprop blocks, active edge blocks, active update blocks}
// stats area {edges map, edge block map, vertex partition mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

// 1. Vitis profile summary 
// https://xilinx.github.io/Vitis-Tutorials/2020-1/docs/Pathway3/ProfileAndTraceReports.html

// 2. Select the Profile Summary report, a... (vitis profile summary has both Compute and Memory information to calculate Arithmetic Intensity)
// https://xilinx.github.io/Vitis-Tutorials/2020-1/docs/Pathway3/ProfileAndTraceReports.html 
// https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Profile-Summary-Report

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
	universalparams.PROC_SRAMSZ = (NUM_VERTICES_PER_UPROPBLOCK * NUM_UPROPBLOCKS_PER_VPARTITION) / VDATA_PACKINGSIZE; 

	universalparams.KVDATA_RANGE_POW = ceil(log2((double)(universalparams.NUM_VERTICES)));
	universalparams.KVDATA_RANGE = universalparams.NUM_VERTICES;

	universalparams.BATCH_RANGE = ((universalparams.NUM_VERTICES + (NUM_PEs - 1)) / NUM_PEs) + 1024; 
	universalparams.BATCH_RANGE_POW = 0; for(unsigned int t=0; t<64; t++){ if(pow (2, t) >= universalparams.BATCH_RANGE){ universalparams.BATCH_RANGE_POW = t; break; }}
	universalparams.BATCH_RANGE_KVS = universalparams.BATCH_RANGE / VECTOR_SIZE;
	cout<<"app: universalparams.BATCH_RANGE_POW: "<<universalparams.BATCH_RANGE_POW<<", universalparams.BATCH_RANGE: "<<universalparams.BATCH_RANGE<<", universalparams.BATCH_RANGE_KVS: "<<universalparams.BATCH_RANGE_KVS<<endl;
	
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
	
	// 8192 * 16;// 
	universalparams.REDUCEPARTITIONSZ = VDATA_SUBPARTITION_SIZE * NUM_SUBPARTITIONS_PER_VPARTITION * VECTOR2_SIZE; // CRITICAL FIXME NOW. 			// universalparams.RED_SRAMSZ * VDATA_PACKINGSIZE; // NEWCHANGE
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
	
	universalparams.PROCESSPARTITIONSZ = VDATA_SUBPARTITION_SIZE * NUM_SUBPARTITIONS_PER_VPARTITION * VECTOR2_SIZE; // CRITICAL FIXME NOW. 			// VDATA_SUBPARTITION_SIZE * NUM_PEs * VECTOR2_SIZE; // NUM_VERTICES_PER_UPROPBLOCK * NUM_UPROPBLOCKS_PER_VPARTITION; // (=122880vertices/vpartition)(=7680kvs/vpartition)(=320kvs/vpartition/channel)
	universalparams.PROCESSPARTITIONSZ_KVS2 = (universalparams.PROCESSPARTITIONSZ / VECTOR2_SIZE);
	universalparams.NUMPROCESSEDGESPARTITIONS = ((universalparams.KVDATA_RANGE + (universalparams.PROCESSPARTITIONSZ - 1)) / universalparams.PROCESSPARTITIONSZ);
	universalparams.NUMVERTEXBLOCKPARTITIONS = utilityobj->allignhigher_FACTOR(((universalparams.KVDATA_RANGE + (NUM_VERTICES_PER_UPROPBLOCK - 1)) / NUM_VERTICES_PER_UPROPBLOCK), VECTOR2_SIZE);
	#ifdef _DEBUGMODE_CHECKS3		
	utilityobj->checkoutofbounds("app::get_universalparams:: ERROR 27", universalparams.PROCESSPARTITIONSZ, 131072, NAp, NAp, NAp);									
	#endif	
									
	universalparams.NUMLASTLEVELPARTITIONS = (1 << (universalparams.NUM_PARTITIONS_POW * universalparams.TREE_DEPTH));

	universalparams.NUM_EDGE_BANKS = 1;

	universalparams.VPTR_SHRINK_RATIO = universalparams.PROCESSPARTITIONSZ;

	universalparams.MAXHBMCAPACITY_UINT32 = ((1 << 28) / 4); // (256MB/4=64M)
	// universalparams.MAXHBMCAPACITY_UINT32 = 50000000; // CRITICAL REMOVEME URGENT.
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
void app::run(std::string setup, std::string algo, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1){
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	
	std::cout << std::setprecision(2) << std::fixed;
	
	#ifdef TESTKERNEL
	cout<< TIMINGRESULTSCOLOR <<"================================================================== APP: THIS IS A TEST RUN ==================================================================" << RESET<< endl;
	#endif 
	
	vector<edge2_type> edgedatabuffer;
	vector<edge_t> vertexptrbuffer;
	vector<edge2_vec_dt> edges_final[MAXNUM_PEs];
	map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs];
	long double edgesprocessed_totals[128];
	long double timeelapsed_totals[128][8];
	unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS];
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ num_edges_processed[iter] = 0; }
	container_t container;
	vector<value_t> actvvs; actvvs.push_back(rootvid);
	tuple_t * vpartition_stats[MAXNUMGRAPHITERATIONS]; 
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
	
	universalparams_t universalparams = get_universalparams(algo, numiterations, rootvid, num_vertices, num_edges, graphisundirected_bool);
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ vpartition_stats[i] = new tuple_t[universalparams.NUMPROCESSEDGESPARTITIONS]; }
	for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int v_p=0; v_p<MAXNUM_VPs; v_p++){ edges_map[i][v_p] = new map_t[MAXNUM_LLPSETs]; }} // MAXNUM_LLPSETs, MAXNUM_LLPSETs // NEWCHANGE NOW.
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){ vpartition_stats[iter][t].A = 0; vpartition_stats[iter][t].B = 0; }}
	utility * utilityobj = new utility(universalparams);
	utilityobj->printallparameters();
	
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
	// exit(EXIT_SUCCESS);
	
	// load workload information
	globalparams_TWOt globalparams;
	globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsK.VOLUMEOFFSETKVS_WORKDATA = 0;
	globalparams.globalparamsK.VOLUMEOFFSETKVS_WORKSPACEDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGES0DATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEEDGEBLOCKS = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESMAP = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsK.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsK.VOLUMESIZEU32_WORKDATA = 0;
	globalparams.globalparamsK.VOLUMESIZEU32_WORKSPACEDATA = 0;
	globalparams.globalparamsK.SIZE_EDGES0 = 0;
	globalparams.globalparamsK.SIZE_EDGES = 0;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsK.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_ACTIVEEDGEBLOCKS = 0;
	globalparams.globalparamsK.SIZE_EDGESMAP = 0;
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsE.VOLUMEOFFSETKVS_WORKDATA = 0;
	globalparams.globalparamsE.VOLUMEOFFSETKVS_WORKSPACEDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGES0DATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEEDGEBLOCKS = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESMAP = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsE.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsE.VOLUMESIZEU32_WORKDATA = 0;
	globalparams.globalparamsE.VOLUMESIZEU32_WORKSPACEDATA = 0;
	globalparams.globalparamsE.SIZE_EDGES0 = 0;
	globalparams.globalparamsE.SIZE_EDGES = 0;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsE.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsE.SIZE_ACTIVEEDGEBLOCKS = 0;
	globalparams.globalparamsE.SIZE_EDGESMAP = 0;
	globalparams.globalparamsE.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsE.SIZE_KVDRAM = 0;
	globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsV.VOLUMEOFFSETKVS_WORKDATA = 0;
	globalparams.globalparamsV.VOLUMEOFFSETKVS_WORKSPACEDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGES0DATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEEDGEBLOCKS = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESMAP = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsV.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsV.VOLUMESIZEU32_WORKDATA = 0;
	globalparams.globalparamsV.VOLUMESIZEU32_WORKSPACEDATA = 0;
	globalparams.globalparamsV.SIZE_EDGES0 = 0;
	globalparams.globalparamsV.SIZE_EDGES = 0;
	globalparams.globalparamsV.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsV.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.SIZE_ACTIVEEDGEBLOCKS = 0;
	globalparams.globalparamsV.SIZE_EDGESMAP = 0;
	globalparams.globalparamsV.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsV.SIZE_KVDRAM = 0;
	globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsM.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsM.VOLUMEOFFSETKVS_WORKDATA = 0;
	globalparams.globalparamsM.VOLUMEOFFSETKVS_WORKSPACEDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_EDGES0DATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEEDGEBLOCKS = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_EDGESMAP = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsM.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsM.SIZE_EDGES0 = 0;
	globalparams.globalparamsM.SIZE_EDGES = 0;
	globalparams.globalparamsM.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsM.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsM.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsM.SIZE_ACTIVEEDGEBLOCKS = 0;
	globalparams.globalparamsM.SIZE_EDGESMAP = 0;
	globalparams.globalparamsM.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsM.SIZE_KVDRAM = 0;
	globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE = 0;
	
	unsigned int total_edges_processed = 0;
	
	// load edges
	#ifdef APP_LOADEDGES
	globalparams = loadedgesobj->start(0, vertexptrbuffer, edgedatabuffer, (vptr_type **)edges, edges, edges_final, edges_map, &container, globalparams);
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
	globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR + (globalparams.globalparamsK.SIZE_VERTEXPTRS / VALUEDATA_PACKINGSIZE) + 1 + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsK.SIZE_SRCVERTICESDATA = MAX_BLOCKRAM_VSRCDATA_SIZE * VDATA_PACKINGSIZE; // 0
	globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR + (globalparams.globalparamsE.SIZE_VERTEXPTRS / VALUEDATA_PACKINGSIZE) + 1 + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR + (globalparams.globalparamsV.SIZE_VERTEXPTRS / VALUEDATA_PACKINGSIZE) + 1 + universalparams.DRAMPADD_KVS;
	// globalparams.globalparamsV.SIZE_SRCVERTICESDATA = NUM_PEs * universalparams.NUMREDUCEPARTITIONS * universalparams.REDUCEPARTITIONSZ_KVS2 * VECTOR2_SIZE;
	globalparams.globalparamsV.SIZE_SRCVERTICESDATA = 256 * VECTOR2_SIZE; 
		if(globalparams.globalparamsV.SIZE_SRCVERTICESDATA > universalparams.MAXHBMCAPACITY_UINT32 - 4000000){ globalparams.globalparamsV.SIZE_SRCVERTICESDATA = universalparams.MAXHBMCAPACITY_UINT32 - 4000000; cout<<"app: RE-ASSIGN. globalparams.globalparamsV.SIZE_SRCVERTICESDATA. reduced to ensure fit."<<endl; } // FIXME.
	globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR + (globalparams.globalparamsM.SIZE_VERTEXPTRS / VALUEDATA_PACKINGSIZE) + 1 + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsM.SIZE_SRCVERTICESDATA = 0; 
	#ifdef APP_LOADSRCVERTICES
	loadgraphobj->loadvertexdata(algo, vdram, globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, globalparams.globalparamsV.SIZE_SRCVERTICESDATA, globalparams.globalparamsV, universalparams);
	for(unsigned int i = 0; i < NUM_PEs; i++){ loadgraphobj->loadvertexdata(algo, kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA, globalparams.globalparamsK.SIZE_SRCVERTICESDATA, globalparams.globalparamsK, universalparams); }					
	#endif 
	// exit(EXIT_SUCCESS);
	
	// dest vertices data 
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::loadvertexdata:: loading dest vertex datas... "<<endl;
	#endif 
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + (globalparams.globalparamsK.SIZE_SRCVERTICESDATA / VALUEDATA_PACKINGSIZE) + 1;
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = universalparams.BATCH_RANGE;
	globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA + (globalparams.globalparamsE.SIZE_SRCVERTICESDATA / VALUEDATA_PACKINGSIZE) + 1;
	globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparams.globalparamsV.SIZE_SRCVERTICESDATA / VALUEDATA_PACKINGSIZE) + 1;
	globalparams.globalparamsV.SIZE_DESTVERTICESDATA = 0; 
	globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA + (globalparams.globalparamsM.SIZE_SRCVERTICESDATA / VALUEDATA_PACKINGSIZE) + 1;
	globalparams.globalparamsM.SIZE_DESTVERTICESDATA = 0; 
	#ifdef APP_LOADDESTVERTICES
	loadgraphobj->loadvertexdata(algo, vdram, globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA, globalparams.globalparamsV.SIZE_DESTVERTICESDATA, globalparams.globalparamsV, universalparams);
	for(unsigned int i = 0; i < NUM_PEs; i++){ loadgraphobj->loadvertexdata(algo, kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, globalparams.globalparamsK.SIZE_DESTVERTICESDATA, globalparams.globalparamsK, universalparams); }
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// active vertices data 
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::loadvertexdata:: loading dest vertex datas... "<<endl;
	#endif 
	globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + (globalparams.globalparamsK.SIZE_DESTVERTICESDATA / VALUEDATA_PACKINGSIZE) + 1;
	globalparams.globalparamsK.SIZE_ACTIVEVERTICESDATA = universalparams.BATCH_RANGE; // 8192 * 16 * universalparams.NUMPROCESSEDGESPARTITIONS; ; // universalparams.BATCH_RANGE; 
	globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA + (globalparams.globalparamsE.SIZE_DESTVERTICESDATA / VALUEDATA_PACKINGSIZE) + 1;
	globalparams.globalparamsE.SIZE_ACTIVEVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + (globalparams.globalparamsV.SIZE_DESTVERTICESDATA / VALUEDATA_PACKINGSIZE) + 1;
	globalparams.globalparamsV.SIZE_ACTIVEVERTICESDATA = NUM_PEs * universalparams.NUMREDUCEPARTITIONS * universalparams.REDUCEPARTITIONSZ_KVS2 * VECTOR2_SIZE;; 
	globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEVERTICESDATA = globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA + (globalparams.globalparamsM.SIZE_DESTVERTICESDATA / VALUEDATA_PACKINGSIZE) + 1;
	globalparams.globalparamsM.SIZE_ACTIVEVERTICESDATA = 0; 
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
	globalparams = loadgraphobj->loadmaps(actvvs, vdram, edges, edges_map, globalparams, universalparams);
	#endif 
	globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEEDGEBLOCKS = globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA + (globalparams.globalparamsM.SIZE_DESTVERTICESDATA / VALUEDATA_PACKINGSIZE);
	globalparams.globalparamsM.SIZE_ACTIVEEDGEBLOCKS = 0; // NAp // CONFIG_HYBRIDGPMODE_MDRAMSECTIONSZ * MAXNUMGRAPHITERATIONS * 2; // (universalparams.NUM_VERTICES * 2); // current and next it active vertices
	// exit(EXIT_SUCCESS); //

	// setting root vid
	#ifdef APP_LOADROOTVID
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::setrootvid:: setting root vid(s)... "<<endl;
	#endif 
	loadgraphobj->setrootvid(algo, (uint512_vec_dt *)vdram, actvvs, globalparams.globalparamsV, universalparams);
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

	// messages
	#ifdef APP_LOADMESSAGES
	globalparams.globalparamsV = loadgraphobj->finishglobaparamsV(globalparams.globalparamsV, universalparams);
	globalparams.globalparamsM = loadgraphobj->finishglobaparamsM(globalparams.globalparamsM, universalparams);
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::loadmessages:: loading messages... "<<endl;
	#endif 
	globalparams = loadgraphobj->loadmessages(mdram, vdram, edges, kvbuffer, &container, globalparams, universalparams);
	#endif
	// exit(EXIT_SUCCESS); //
	
	// others
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app::experiements: resetting kvdram & kvdram workspaces..."<<endl;
	#endif 
	for(unsigned int i=0; i<NUM_PEs; i++){
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM], globalparams.globalparamsK.SIZE_KVDRAM);
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE], globalparams.globalparamsK.SIZE_KVDRAM);
	}

	// acts helper
	#ifdef APP_RUNSWVERSION
	universalparams.NUM_ITERATIONS = actshelperobj->extract_stats(vdram, kvbuffer, actvvs, vertexptrbuffer, edgedatabuffer, edgesprocessed_totals, vpartition_stats, num_edges_processed, globalparams);
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
	cout<<"app:: universalparams.MAXHBMCAPACITY_UINT32: "<<universalparams.MAXHBMCAPACITY_UINT32<<endl; 
	#endif 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<(universalparams.VOLUMESIZEU32_WORKDATA / VECTOR2_SIZE); t++){ kvbuffer[i][universalparams.VOLUMEOFFSETKVS_WORKDATA + t] = edges[i][t]; }}
	if(((universalparams.VOLUMEOFFSETKVS_WORKDATA + (universalparams.VOLUMESIZEU32_WORKDATA / VECTOR2_SIZE)) * sizeof(uint512_vec_dt)) > (universalparams.MAXHBMCAPACITY_UINT32 * sizeof(unsigned int))){ cout<<"app::ERROR: CAN NOT merge. dataset too large. EXITING... "<<endl; exit(EXIT_FAILURE); }
	// exit(EXIT_SUCCESS); //
	
	// run_hw
	#ifdef APP_RUNHWVERSION
	unsigned int num_runs_a = 1; unsigned int num_runs_b = 1; unsigned int num_runs_c = 1; 
	unsigned int skip_a = 0; unsigned int skip_b = 16; unsigned int skip_c = 16; // 0, 4, 8
	unsigned int maxlimit_actvedgeblocks_per_vpartition = 0; unsigned int maxlimit_actvupropblocks_per_vpartition = 0; unsigned int maxlimit_actvupdateblocks_per_vpartition = 0; 
	if(universalparams.ALGORITHM == BFS || universalparams.ALGORITHM == SSSP){} else { num_runs_a = 1; num_runs_b = 1; num_runs_c = 1; }
	long double total_time_elapsed = 0;
	for(unsigned int a = 0; a<num_runs_a; a++){
		for(unsigned int b = 0; b<num_runs_b; b++){
			for(unsigned int c = 0; c<num_runs_c; c++){
				
				#ifdef NOT___USED
				// reload src vertices 
				loadgraphobj->loadvertexdata(algo, vdram, globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA, globalparams.globalparamsV.SIZE_SRCVERTICESDATA, globalparams.globalparamsV, universalparams);
				for(unsigned int i = 0; i < NUM_PEs; i++){ loadgraphobj->loadvertexdata(algo, kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA, globalparams.globalparamsK.SIZE_SRCVERTICESDATA, globalparams.globalparamsK, universalparams); }					
				
				// reload dst vertices 
				loadgraphobj->loadvertexdata(algo, vdram, globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA, globalparams.globalparamsV.SIZE_DESTVERTICESDATA, globalparams.globalparamsV, universalparams);
				for(unsigned int i = 0; i < NUM_PEs; i++){ loadgraphobj->loadvertexdata(algo, kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, globalparams.globalparamsK.SIZE_DESTVERTICESDATA, globalparams.globalparamsK, universalparams); }
				
				// set root vid 
				loadgraphobj->setrootvid((uint512_vec_dt *)vdram, actvvs, globalparams.globalparamsV, universalparams);
				#endif 
				
				// set configuration. max {1024, 341, 1024}
				maxlimit_actvedgeblocks_per_vpartition = a*skip_a; maxlimit_actvupropblocks_per_vpartition = b*skip_b; maxlimit_actvupdateblocks_per_vpartition = c*skip_c; 
				
				// maxlimit_actvedgeblocks_per_vpartition = 0; maxlimit_actvupropblocks_per_vpartition = 0; maxlimit_actvupdateblocks_per_vpartition = 0;
				// maxlimit_actvedgeblocks_per_vpartition = 256; maxlimit_actvupropblocks_per_vpartition = 16; maxlimit_actvupdateblocks_per_vpartition = 64; 
				// maxlimit_actvedgeblocks_per_vpartition = 1024; maxlimit_actvupropblocks_per_vpartition = 128; maxlimit_actvupdateblocks_per_vpartition = 64; 
				maxlimit_actvedgeblocks_per_vpartition = 1024; maxlimit_actvupropblocks_per_vpartition = 1024; maxlimit_actvupdateblocks_per_vpartition = 0; 
				// maxlimit_actvedgeblocks_per_vpartition = 1024; maxlimit_actvupropblocks_per_vpartition = 1024; maxlimit_actvupdateblocks_per_vpartition = 64; 
				
				// uprop needs fixing...
				for(unsigned int i=0; i<NUM_PEs; i++){
					kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION].data[0].key = maxlimit_actvupdateblocks_per_vpartition; // 0;
					kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION].data[0].key = maxlimit_actvedgeblocks_per_vpartition;
					kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION].data[0].key = maxlimit_actvupropblocks_per_vpartition;
				}
				vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION].data[0].key = maxlimit_actvupdateblocks_per_vpartition; // 0;
				vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION].data[0].key = maxlimit_actvedgeblocks_per_vpartition;
				vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION].data[0].key = maxlimit_actvupropblocks_per_vpartition;
				
				#ifdef _DEBUGMODE_HOSTPRINTS4
				unsigned int numkvs_vertices_per_updateblock = ((1 << universalparams.BATCH_RANGE_POW) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; // 16
				unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock; // 512
				cout<<endl<< TIMINGRESULTSCOLOR <<">>> app::run_hw: app started. ("<<a<<", "<<b<<", "<<c<<")("<<maxlimit_actvedgeblocks_per_vpartition<<", "<<maxlimit_actvupropblocks_per_vpartition<<", "<<maxlimit_actvupdateblocks_per_vpartition<<")(of "<<MAXNUM_EDGEBLOCKS_PER_VPARTITION<<", "<<NUM_UPROPBLOCKS_PER_VPARTITION<<", "<<MAXNUM_UPDATEBLOCKS_PER_VPARTITION<<"). ("<<actvvs.size()<<" active vertices)"<< RESET <<endl;
				cout<<"[maxlimit_actvedgeblocks_per_vpartition: "<<maxlimit_actvedgeblocks_per_vpartition<<", maxlimit_actvupropblocks_per_vpartition "<<maxlimit_actvupropblocks_per_vpartition<<", maxlimit_actvupdateblocks_per_vpartition: "<<maxlimit_actvupdateblocks_per_vpartition<< "]" <<endl;
				#endif 
				total_time_elapsed = kernelobj->runapp(binaryFile, (uint512_vec_dt *)vdram, (uint512_vec_dt **)edges, (uint512_vec_dt **)kvbuffer, timeelapsed_totals, 
					num_edges_processed, vertexptrbuffer, edgedatabuffer, universalparams);
					
				float total__latency_ms = actshelperobj->get_results("app", graph_path, vdram, vdram, vdram, vdram, kvbuffer, universalparams);
				// actshelperobj->verifyresults2(vdram, globalparams.globalparamsV, universalparams);
				actshelperobj->verifyresults3(vdram, globalparams.globalparamsV, universalparams);
				total_time_elapsed = total__latency_ms;
			}
		}
	}
	#endif 
	
	// output
	unsigned int num_traversed_edges = NAp; 
	#ifdef _DEBUGMODE_HOSTPRINTS//4
	cout<<endl<<">>> app::run_hw: total_edges_processed: "<<total_edges_processed<<" edges ("<<total_edges_processed/1000000<<" million edges)"<<endl;
	cout<<">>> app::run_hw: num_traversed_edges: "<<num_traversed_edges<<" edges ("<<num_traversed_edges/1000000<<" million edges)"<<endl;
	cout<<">>> app::run_hw: total_time_elapsed: "<<total_time_elapsed<<" ms ("<<total_time_elapsed/1000<<" s)"<<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput: "<<((total_edges_processed / total_time_elapsed) * (1000))<<" Edges / sec, "<<((total_edges_processed / total_time_elapsed) / (1000))<<" Million edges / sec, "<<((total_edges_processed / total_time_elapsed) / (1000000))<<" Billion edges / sec"<< RESET <<endl;	
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput projection for 32 workers: "<<((((total_edges_processed / total_time_elapsed) / (1000)) * 32) / NUM_PEs)<<" Million edges / sec"<< RESET <<endl;
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS//3
	actshelperobj->extract_stats(vdram, kvbuffer, actvvs, vertexptrbuffer, edgedatabuffer, edgesprocessed_totals, vpartition_stats, num_edges_processed, globalparams);
	summary(GRAPH_PATH, vdram, globalparams.globalparamsV);
	#endif 
	
	cout<<"------------------------------------------ app::printdataset: VDATA_SUBPARTITION_SIZE: "<<VDATA_SUBPARTITION_SIZE<<endl;
	cout<<"------------------------------------------ app::printdataset: NUM_VERTICES_PER_UPROPBLOCK: "<<NUM_VERTICES_PER_UPROPBLOCK<<endl;
	cout<<"------------------------------------------ app::printdataset: NUM_VERTICESKVS_PER_UPROPBLOCK: "<<NUM_VERTICESKVS_PER_UPROPBLOCK<<endl;
	cout<<"------------------------------------------ app::printdataset: NUM_UPROPBLOCKS_PER_VPARTITION: "<<NUM_UPROPBLOCKS_PER_VPARTITION<<endl;
	cout<<"------------------------------------------ app::printdataset: VDATA_SUBPARTITION_SIZE: "<<VDATA_SUBPARTITION_SIZE<<endl;
	cout<<"------------------------------------------ app::printdataset: VDATA_SUBPARTITION_SIZE: "<<VDATA_SUBPARTITION_SIZE<<endl;

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
	cout<<">>> app:: graph direction: "<<graphisundirected<<endl;
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
















