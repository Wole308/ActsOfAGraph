#include "app.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}
// end-of-file

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

universalparams_t app::get_universalparams(std::string algo, unsigned int numiterations, unsigned int rootvid, unsigned int num_vertices, unsigned int num_edges){
	universalparams_t universalparams;
	algorithm * algorithmobj = new algorithm();
	
	universalparams.ALGORITHM = algorithmobj->get_algorithm_id(algo);
	universalparams.NUM_ITERATIONS = numiterations; if(universalparams.ALGORITHM != BFS || universalparams.ALGORITHM != SSSP){ universalparams.NUM_ITERATIONS = 1; }
	universalparams.ROOTVID = rootvid;
	
	universalparams.NUM_VERTICES = num_vertices; 
	universalparams.NUM_EDGES = num_edges; 
	
	// if(num_edges < 250000000){ universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM = 0; } 
	// else { universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM = 1; } // FIXME.
	universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM = 1; // NEWCHANGE.
	
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){ universalparams.KVDRAMWORKCAPACITY_BYTES = (1 << 28) / 2; } 
	else { universalparams.KVDRAMWORKCAPACITY_BYTES = 1 << 28; }
	
	universalparams.NUM_PARTITIONS_POW = 4;
	universalparams.NUM_PARTITIONS = 1 << universalparams.NUM_PARTITIONS_POW;

	#ifdef CONFIG_USEURAM_FOR_DESTVBUFFER//____UNUSED
		if(universalparams.ALGORITHM == CF || universalparams.ALGORITHM == HITS){ universalparams.RED_SRAMSZ_POW = MAX_RED_SRAMSZ_POW + 1 - 1; } else { universalparams.RED_SRAMSZ_POW = MAX_RED_SRAMSZ_POW + 1; } // URAM bit-width is 64 bit wide (i.e. can contain 2 uint32_ts)
			#else 
				if(universalparams.ALGORITHM == CF || universalparams.ALGORITHM == HITS){ universalparams.RED_SRAMSZ_POW = MAX_RED_SRAMSZ_POW - 1; } else { universalparams.RED_SRAMSZ_POW = MAX_RED_SRAMSZ_POW; }
					#endif 
	universalparams.RED_SRAMSZ = 1 << universalparams.RED_SRAMSZ_POW;
	
	#ifdef CONFIG_USEURAM_FOR_SRCVBUFFER//____UNUSED
		universalparams.PROC_SRAMSZ_POW = MAX_PROC_SRAMSZ_POW + 1; // URAM bit-width is 64 bit wide (i.e. can contain 2 uint32_ts)
			#else 
				universalparams.PROC_SRAMSZ_POW = MAX_PROC_SRAMSZ_POW;	
					#endif 
	universalparams.PROC_SRAMSZ = 1 << universalparams.PROC_SRAMSZ_POW;
	
	universalparams.KVDATA_RANGE_POW = ceil(log2((double)(universalparams.NUM_VERTICES)));
	universalparams.KVDATA_RANGE = universalparams.NUM_VERTICES;

	universalparams.BATCH_RANGE_POW = ceil(log2((double)(universalparams.NUM_VERTICES / NUM_PEs))); 
	// universalparams.BATCH_RANGE_POW = 22; // CRITICAL REMOVEME.
	universalparams.BATCH_RANGE = universalparams.NUM_VERTICES / NUM_PEs;
	universalparams.BATCH_RANGE_KVS = universalparams.BATCH_RANGE / VECTOR_SIZE;

	if(universalparams.ALGORITHM == BFS){ universalparams.TREE_DEPTH = 1; } 
	else { universalparams.TREE_DEPTH = ((universalparams.BATCH_RANGE_POW - universalparams.RED_SRAMSZ_POW) + (universalparams.NUM_PARTITIONS_POW - 1)) / universalparams.NUM_PARTITIONS_POW; }
	
	if(universalparams.ALGORITHM == BFS){ universalparams.REDUCESZ_POW = universalparams.RED_SRAMSZ_POW; } // MAX_RED_SRAMSZ_POW
	else { universalparams.REDUCESZ_POW = (universalparams.BATCH_RANGE_POW - (universalparams.TREE_DEPTH * universalparams.NUM_PARTITIONS_POW)); }
	universalparams.REDUCESZ = (1 << universalparams.REDUCESZ_POW);

	universalparams.REDUCEPARTITIONSZ = universalparams.REDUCESZ * VDATA_PACKINGSIZE;
	universalparams.REDUCEPARTITIONSZ_KVS2 = universalparams.REDUCEPARTITIONSZ / VECTOR2_SIZE;
	universalparams.NUMREDUCEPARTITIONS = ((universalparams.KVDATA_RANGE / NUM_PEs) + (universalparams.REDUCEPARTITIONSZ - 1)) / universalparams.REDUCEPARTITIONSZ;
	
	universalparams.PROCESSPARTITIONSZ = ((((universalparams.PROC_SRAMSZ / NUM_PEs) / VDATA_SHRINK_RATIO) * VDATA_SHRINK_RATIO) * NUM_PEs * VDATA_PACKINGSIZE);
	universalparams.SUB_CHUNK_HEIGHT = ((universalparams.PROC_SRAMSZ / NUM_PEs) / VDATA_SHRINK_RATIO);
	universalparams.PROCESSPARTITIONSZ_KVS2 = (universalparams.PROCESSPARTITIONSZ / VECTOR2_SIZE);
	universalparams.NUMPROCESSEDGESPARTITIONS = ((universalparams.KVDATA_RANGE + (universalparams.PROCESSPARTITIONSZ - 1)) / universalparams.PROCESSPARTITIONSZ);
	
	universalparams.NUMLASTLEVELPARTITIONS = (1 << (universalparams.NUM_PARTITIONS_POW * universalparams.TREE_DEPTH));

	universalparams.NUM_EDGE_BANKS = 1;

	universalparams.VPTR_SHRINK_RATIO = universalparams.PROCESSPARTITIONSZ;

	universalparams.MAXHBMCAPACITY_V = ((1 << 28) / 4); // (256MB/4=64M)
	universalparams.MAXHBMCAPACITY_KV = ((1 << 28) / 8); // (256MB/8=32M)
	universalparams.MAXHBMCAPACITY_VS = (universalparams.MAXHBMCAPACITY_V / VECTOR2_SIZE); // (64M/16=4M)
	universalparams.MAXHBMCAPACITY_KVS = (universalparams.MAXHBMCAPACITY_KV / VECTOR_SIZE); // (32M/8=4M)
	// universalparams.KVSOURCEDRAMSZ = (universalparams.MAXHBMCAPACITY_KVS * VECTOR_SIZE); // MAXHBMCAPACITY_KV // max HBM capacity (256MB)
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){ universalparams.KVSOURCEDRAMSZ = (universalparams.MAXHBMCAPACITY_KVS * VECTOR_SIZE) / 2; } 
	else { universalparams.KVSOURCEDRAMSZ = (universalparams.MAXHBMCAPACITY_KVS * VECTOR_SIZE); }

	universalparams.KVSOURCEDRAMSZ_KVS = (universalparams.KVSOURCEDRAMSZ / VECTOR_SIZE);
	universalparams.PADDEDKVSOURCEDRAMSZ = universalparams.KVSOURCEDRAMSZ;
	universalparams.PADDEDKVSOURCEDRAMSZ_KVS = (universalparams.PADDEDKVSOURCEDRAMSZ / VECTOR_SIZE);

	universalparams.VDRAMSZ = universalparams.KVSOURCEDRAMSZ;
	universalparams.PADDEDVDRAMSZ = universalparams.VDRAMSZ;
	universalparams.PADDEDVDRAMSZ_KVS = (universalparams.PADDEDVDRAMSZ / VECTOR_SIZE);
	
	universalparams.DRAMPADD = (16 * BLOCKRAM_SIZE * VECTOR_SIZE);
	universalparams.DRAMPADD_KVS = (universalparams.DRAMPADD / VECTOR_SIZE);

	universalparams.DRAMPADD_VPTRS = 2 * universalparams.VPTR_SHRINK_RATIO * VECTOR2_SIZE; 
	universalparams.DRAMPADD_VPTRS_KVS = universalparams.DRAMPADD_VPTRS / VECTOR2_SIZE; // to avoid any spill-overs 
	
	universalparams.SRCBUFFER_SIZE = BLOCKRAM_SIZE - (4 * 4);
	universalparams.WORKBUFFER_SIZE = universalparams.SRCBUFFER_SIZE - (universalparams.NUM_PARTITIONS*4);
	return universalparams;
}

void app::run(std::string setup, std::string algo, unsigned int numiterations, unsigned int rootvid, string graph_path, std::string _binaryFile1){
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph dir: "<<graph_path<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	
	#ifdef TESTKERNEL
	cout<< TIMINGRESULTSCOLOR <<"================================================================== APP: THIS IS A TEST RUN ==================================================================" << RESET<< endl;
	#endif 
	
	vector<edge2_type> edgedatabuffer;
	vector<edge_t> vertexptrbuffer;
	vector<edge3_type> edges_temp[MAXNUMSUBCPUTHREADS];
	long double edgesprocessed_totals[128];
	long double timeelapsed_totals[128][8];
	unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS];
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ num_edges_processed[iter] = 0; }
	container_t container;
	vector<value_t> actvvs; actvvs.push_back(rootvid);
	unsigned int * vpmaskbuffer[MAXNUMGRAPHITERATIONS]; 
	long double totaltime_ms = 0;
	binaryFile[0] = _binaryFile1;

	string GRAPH_NAME = ""; // "soc-orkut";
	string GRAPH_PATH = graph_path;
	
	prepare_graph * prepare_graphobj = new prepare_graph();
	prepare_graphobj->start(GRAPH_PATH, edgedatabuffer, vertexptrbuffer); // REMOVEME.
	unsigned int num_edges = edgedatabuffer.size();
	unsigned int num_vertices = vertexptrbuffer.size();
	cout<<"app:run: num_vertices: "<<num_vertices<<", num_edges: "<<num_edges<<endl;
	
	// universalparams_t universalparams;
	universalparams_t universalparams = get_universalparams(algo, numiterations, rootvid, num_vertices, num_edges);
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ vpmaskbuffer[i] = new unsigned int[universalparams.NUMPROCESSEDGESPARTITIONS]; }
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){ vpmaskbuffer[iter][t] = 0; }}
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
	for(unsigned int i=0; i<MAXNUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.MAXHBMCAPACITY_KVS * sizeof(uint512_vec_dt))); }				
	vdram = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.MAXHBMCAPACITY_KVS * sizeof(uint512_vec_dt)));
	mdram = (uint512_vec_dt *) aligned_alloc(4096, mdramsz_kvs * sizeof(uint512_vec_dt));
	#else
	for(unsigned int i=0; i<MAXNUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS]; }
	vdram = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS];
	mdram = new uint512_vec_dt[mdramsz_kvs]; // stores {edges, vptrs, active vertices} all in large DDR memory. '1000000' is padding
	#endif
	#endif 
	
	for(unsigned int i=0; i<MAXNUMSUBCPUTHREADS; i++){ // REMOVEME.
		for(unsigned int k=0; k<universalparams.MAXHBMCAPACITY_KVS; k++){ 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ kvbuffer[i][k].data[v].key = 0; kvbuffer[i][k].data[v].value = 0; }
		}
	}
	
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		#ifdef FPGA_IMPL
		for(unsigned int j=0; j<MAXNUMSUBCPUTHREADS; j++){ edges[j] = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }	// 2 because edge_type consist 4 bytes not 8 bytes			
		#else
		for(unsigned int j=0; j<MAXNUMSUBCPUTHREADS; j++){ edges[j] = new uint512_vec_dt[universalparams.PADDEDKVSOURCEDRAMSZ_KVS]; }
		#endif
	}
	
	// load workload information
	globalparams_TWOt globalparams;
	globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsK.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsK.SIZE_EDGES = 0;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsK.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsK.SIZE_VERTICESDATAMASK = 0;
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = 0;
	
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = 0;
		globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
		
		globalparams.globalparamsE.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
		globalparams.globalparamsE.SIZE_EDGES = 0;
		globalparams.globalparamsE.SIZE_VERTEXPTRS = 0;
		globalparams.globalparamsE.SIZE_SRCVERTICESDATA = 0;
		globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
		globalparams.globalparamsE.SIZE_ACTIVEVERTICES = 0;
		globalparams.globalparamsE.SIZE_VERTICESDATAMASK = 0;
		globalparams.globalparamsE.SIZE_KVSTATSDRAM = 0;
		globalparams.globalparamsE.SIZE_KVDRAM = 0;
		globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	}
	
	globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsV.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsV.SIZE_EDGES = 0;
	globalparams.globalparamsV.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsV.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsV.SIZE_VERTICESDATAMASK = 0;
	globalparams.globalparamsV.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsV.SIZE_KVDRAM = 0;
	globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsM.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsM.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsM.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsM.SIZE_EDGES = 0;
	globalparams.globalparamsM.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsM.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsM.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsM.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsM.SIZE_VERTICESDATAMASK = 0;
	globalparams.globalparamsM.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsM.SIZE_KVDRAM = 0;
	globalparams.globalparamsM.SIZE_KVDRAMWORKSPACE = 0;
	
	unsigned int num_iters = 0;
	unsigned int total_edges_processed = 0;
	
	// exit(EXIT_SUCCESS);
	
	// edges
	#ifdef APP_LOADEDGES
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		globalparams = loadedgesobj->start(0, vertexptrbuffer, edgedatabuffer, (vptr_type **)edges, (edge_type **)edges, edges_temp, &container, globalparams);
	} else {
		globalparams = loadedgesobj->start(0, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, edges_temp, &container, globalparams);
	}
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
	
	// src vertices data
	cout<<"app::loadvertexdata:: loading source vertex datas... "<<endl;
	globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR + ((globalparams.globalparamsK.SIZE_VERTEXPTRS/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsK.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR + ((globalparams.globalparamsE.SIZE_VERTEXPTRS/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR + ((globalparams.globalparamsV.SIZE_VERTEXPTRS/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsV.SIZE_SRCVERTICESDATA = NUM_PEs * universalparams.NUMREDUCEPARTITIONS * universalparams.REDUCEPARTITIONSZ_KVS2 * VECTOR2_SIZE;
				if(universalparams.ALGORITHM == HITS){ globalparams.globalparamsV.SIZE_SRCVERTICESDATA = globalparams.globalparamsV.SIZE_SRCVERTICESDATA * 2; }
	globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsM.BASEOFFSETKVS_VERTEXPTR + ((globalparams.globalparamsM.SIZE_VERTEXPTRS/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsM.SIZE_SRCVERTICESDATA = 0; 
	#ifdef APP_LOADSRCVERTICES
	loadgraphobj->loadvertexdata(algo, (keyvalue_t *)vdram, globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsV.SIZE_SRCVERTICESDATA, globalparams.globalparamsV, 0, SOURCE);
	for(unsigned int i = 0; i < NUM_PEs; i++){ loadgraphobj->loadvertexdata(algo, (keyvalue_t *)kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsK.SIZE_SRCVERTICESDATA, globalparams.globalparamsK, 0, SOURCE); }					
	#endif 
	// exit(EXIT_SUCCESS);
	
	// dest vertices data 
	cout<<"app::loadvertexdata:: loading dest vertex datas... "<<endl;
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + ((globalparams.globalparamsK.SIZE_SRCVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = (1 << universalparams.BATCH_RANGE_POW); // universalparams.BATCH_RANGE; 
		if(universalparams.ALGORITHM == HITS){ globalparams.globalparamsK.SIZE_DESTVERTICESDATA = globalparams.globalparamsK.SIZE_DESTVERTICESDATA * 2; }
	globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA + ((globalparams.globalparamsE.SIZE_SRCVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + ((globalparams.globalparamsV.SIZE_SRCVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsV.SIZE_DESTVERTICESDATA = 0; 
	globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsM.BASEOFFSETKVS_SRCVERTICESDATA + ((globalparams.globalparamsM.SIZE_SRCVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsM.SIZE_DESTVERTICESDATA = 0; 
	#ifdef APP_LOADDESTVERTICES
	loadgraphobj->loadvertexdata(algo, (keyvalue_t *)vdram, globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsV.SIZE_DESTVERTICESDATA, globalparams.globalparamsV, 0, DEST);
	for(unsigned int i = 0; i < NUM_PEs; i++){ loadgraphobj->loadvertexdata(algo, (keyvalue_t *)kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsK.SIZE_DESTVERTICESDATA, globalparams.globalparamsK, 0, DEST); }
	#endif 
	
	// active vertices & masks
	#ifdef APP_LOADMASKS
	cout<<"app::loadactvvertices:: loading active vertices... "<<endl;
	for(unsigned int i = 0; i < NUM_PEs; i++){ globalparams = loadgraphobj->loadactvvertices(actvvs, (keyy_t *)&kvbuffer[i], &container, globalparams); }
	cout<<"app::generatevmaskdata:: generating vmask... "<<endl;
	globalparams = loadgraphobj->generatevmaskdata(actvvs, kvbuffer, vdram, globalparams);
	#endif 
	globalparams.globalparamsM.BASEOFFSETKVS_ACTIVEVERTICES = globalparams.globalparamsM.BASEOFFSETKVS_DESTVERTICESDATA + ((globalparams.globalparamsM.SIZE_DESTVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsM.SIZE_ACTIVEVERTICES = CONFIG_HYBRIDGPMODE_MDRAMSECTIONSZ * MAXNUMGRAPHITERATIONS * 2; // (universalparams.NUM_VERTICES * 2); // current and next it active vertices
	// exit(EXIT_SUCCESS); //
	
	// setting root vid
	#ifdef APP_LOADROOTVID
	cout<<"app::setrootvid:: setting root vid(s)... "<<endl;
	loadgraphobj->setrootvid((uint512_vec_dt *)vdram, actvvs, globalparams.globalparamsV);
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// stats info 
	#ifdef APP_LOADSTATSINFO
	cout<<"app::loadoffsetmarkers:: loading offset markers... "<<endl;
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		globalparams = loadgraphobj->loadoffsetmarkers((edge_type **)edges, (keyvalue_t **)edges, edges_temp, &container, globalparams); 
		loadgraphobj->accumstats(kvbuffer, edges, globalparams); // NEWCHANGE.
	} else {
		globalparams = loadgraphobj->loadoffsetmarkers((edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, edges_temp, &container, globalparams); 
	}
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// messages
	#ifdef APP_LOADMESSAGES
	globalparams.globalparamsV = loadgraphobj->finishglobaparamsV(globalparams.globalparamsV);
	globalparams.globalparamsM = loadgraphobj->finishglobaparamsM(globalparams.globalparamsM);
	cout<<"app::loadmessages:: loading messages... "<<endl;
	globalparams = loadgraphobj->loadmessages(mdram, vdram, edges, kvbuffer, &container, universalparams.NUM_ITERATIONS, globalparams);
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// others
	cout<<"app::experiements: resetting kvdram & kvdram workspaces..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM], globalparams.globalparamsK.SIZE_KVDRAM);
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE], globalparams.globalparamsK.SIZE_KVDRAM);
	}
	
	// acts helper
	#ifdef APP_RUNSWVERSION
	num_iters = actshelperobj->extract_stats(actvvs, vertexptrbuffer, edgedatabuffer, edgesprocessed_totals, vpmaskbuffer, num_edges_processed);
	if(universalparams.ALGORITHM == BFS || universalparams.ALGORITHM == SSSP){ for(unsigned int t=0; t<num_iters; t++){ total_edges_processed += edgesprocessed_totals[t]; }} else { total_edges_processed = universalparams.NUM_EDGES; }
	#endif 
	
	// merge kv and edges dram for extra-large vertices 
	cout<<">>> swkernel::runapp: populating vdramA, vdramB and vdramC... "<<endl;
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<(universalparams.MAXHBMCAPACITY_KVS / 2); t++){ kvbuffer[i][(universalparams.MAXHBMCAPACITY_KVS / 2) + t] = edges[i][t]; }}
	}
	
	// run_hw
	#ifdef APP_RUNHWVERSION
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> app::run_hw: app started. ("<<actvvs.size()<<" active vertices)"<< RESET <<endl;
	cout<<">>> goclkernel::runapp:: runapp started -------------------------------. num_edges_processed[0]: "<<num_edges_processed[0]<<endl;
	long double total_time_elapsed = kernelobj->runapp(binaryFile, (uint512_vec_dt *)mdram, (uint512_vec_dt *)vdram, (uint512_vec_dt **)edges, (uint512_vec_dt **)kvbuffer, timeelapsed_totals, num_iters, 
		vpmaskbuffer, num_edges_processed, vertexptrbuffer, edgedatabuffer);
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// output
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<universalparams.NUM_ITERATIONS; i++){
		for(unsigned int analysis_i=0; analysis_i<3; analysis_i++){
			cout <<">>> app::run_hw: throughput["<<i<<"]["<<analysis_i<<"]("<<edgesprocessed_totals[i]/1000000<<"M edges,"<<timeelapsed_totals[i][analysis_i]<<" ms): "<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) * (1000))<<" edges/sec ("<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) / (1000))<<" million edges/sec)" <<endl;								
		}
	}
	#endif
	if(universalparams.ALGORITHM == BFS || universalparams.ALGORITHM == SSSP){
		cout<<endl<< TIMINGRESULTSCOLOR << ">>> app::run_hw: THROUGHPUT SUMMARY DETAILS: "<< RESET <<endl;
		for(unsigned int i=0; i<universalparams.NUM_ITERATIONS; i++){
			unsigned int total_edges = 0;
			long double total_time = 0;
			
			unsigned int analysis_begincount = 0;
			unsigned int analysis_icount = 3;
			for(unsigned int analysis_i=analysis_begincount; analysis_i<analysis_icount; analysis_i++){ total_time += timeelapsed_totals[i][analysis_i]; }
			
			if(edgesprocessed_totals[i] > 0){
				for(unsigned int analysis_i=analysis_begincount; analysis_i<analysis_icount; analysis_i++){ cout<<""<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) / (1000))<<", "; }
				cout<<"[Iter "<<i<<" throughput: "<<((edgesprocessed_totals[i] / total_time) / (1000))<<"]";
				cout<<"[Iter "<<i<<" edges processed: "<<edgesprocessed_totals[i]<<"]";
				cout<<endl;
			}
		}
		for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ cout<<">>> app::run_hw: edges processed in iteration: "<<iter<<" edges ("<<num_edges_processed[iter]<<" edges)"<<endl; }
	}
	unsigned int num_traversed_edges = actshelperobj->getfeedback("app", graph_path, vdram, vdram, vdram, vdram, kvbuffer);
	
	cout<<endl<<">>> app::run_hw: total_edges_processed: "<<total_edges_processed<<" edges ("<<total_edges_processed/1000000<<" million edges)"<<endl;
	cout<<">>> app::run_hw: num_traversed_edges: "<<num_traversed_edges<<" edges ("<<num_traversed_edges/1000000<<" million edges)"<<endl;
	cout<<">>> app::run_hw: total_time_elapsed: "<<total_time_elapsed<<" ms ("<<total_time_elapsed/1000<<" s)"<<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput: "<<((total_edges_processed / total_time_elapsed) * (1000))<<" edges/sec ("<<((total_edges_processed / total_time_elapsed) / (1000))<<" million edges/sec)"<< RESET <<endl;			
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput projection for 32 workers: ("<<((((total_edges_processed / total_time_elapsed) / (1000)) * 32) / NUM_PEs)<<" million edges/sec)"<< RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput (MPEPS): "<<((total_edges_processed / total_time_elapsed) * (1000))<<" PEPS ("<<((total_edges_processed / total_time_elapsed) / (1000))<<" MPEPS) ("<<((total_edges_processed / total_time_elapsed) / (1000000))<<" BPEPS)"<< RESET <<endl;			
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput (MTEPS): "<<((num_traversed_edges / total_time_elapsed) * (1000))<<" TEPS ("<<((num_traversed_edges / total_time_elapsed) / (1000))<<" MTEPS) ("<<((num_traversed_edges / total_time_elapsed) / (1000000))<<" BTEPS)"<< RESET <<endl;			
	
	actshelperobj->extract_stats(actvvs, vertexptrbuffer, edgedatabuffer, edgesprocessed_totals, vpmaskbuffer, num_edges_processed);
	verifyresults(GRAPH_PATH, vdram, globalparams.globalparamsV);
	
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
	
	#ifdef TESTKERNEL
	cout<< TIMINGRESULTSCOLOR <<"================================================================== APP: THIS WAS A TEST RUN ==================================================================" << RESET << endl;
	#endif 
	return;
}

void app::verifyresults(string graph_path, uint512_vec_dt * vdram, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<endl<<"app::verifyresults: verifying vertex data... "<<endl;
	#endif
	
	#ifdef NOT_USED
	unsigned int vdatas[64]; for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	unsigned int _BASEOFFSETKVS_SRCVERTICESDATA = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA].data[0].key;
	unsigned int _SIZE_SRCVERTICESDATA = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_SRCVERTICESDATA].data[0].key;
	
	for(unsigned int k=0; k<_SIZE_SRCVERTICESDATA / VECTOR2_SIZE; k++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			unsigned int vdata1 = utilityobj->READFROM_UINT(vdram[_BASEOFFSETKVS_SRCVERTICESDATA + k].data[v].key, OFFSETOF_VDATA, SIZEOF_VDATA);
			unsigned int vdata2 = utilityobj->READFROM_UINT(vdram[_BASEOFFSETKVS_SRCVERTICESDATA + k].data[v].value, OFFSETOF_VDATA, SIZEOF_VDATA);
			
			if(vdata1 < 64){
				vdatas[vdata1] += 1; 
			}
			if(vdata2 < 64){
				vdatas[vdata2] += 1; 
			}
		}
	}
	
	// verifying results 
	cout<<">>> app::verifyresults:: Printing results from app.cpp [@ counting from host]"<<endl;
	utilityobj->printvalues("app::verifyresults:: verifying results after kernel run (app.cpp)", vdatas, 16);
	#endif 
	
	#ifndef FPGA_IMPL
	utilityobj->printallfeedback("app", graph_path, vdram, vdram, vdram, vdram, kvbuffer);
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
















