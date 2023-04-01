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

#define BUILD_GRAPH_FOR_DEVICE
#define APP_LOADEDGES
#define APP_LOADSRCVERTICES
#define APP_LOADDESTVERTICES
#define APP_LOADMASKS
#define APP_LOADROOTVID
#define APP_LOADSTATSINFO
#define APP_LOADMESSAGES // THE CAUSE OF ERROR.
#define APP_RUNSWVERSION
#define APP_RUNHWVERSION 

#define NUM_KERNEL 24 //24
	
app::app(){		
	algorithmobj = new algorithm();
}
app::~app(){	
	cout<<"app::~app:: finish destroying memory structures... "<<endl;
}

universalparams_t app::get_universalparams(std::string algo, unsigned int numiterations, unsigned int rootvid, unsigned int num_vertices, unsigned int num_edges, bool graphisundirected){
	universalparams_t universalparams;
	// algorithm * algorithmobj = new algorithm();
	
	universalparams.ALGORITHM = algorithmobj->get_algorithm_id(algo);
	universalparams.NUM_ITERATIONS = numiterations; if(universalparams.ALGORITHM != BFS && universalparams.ALGORITHM != SSSP){ universalparams.NUM_ITERATIONS = 1; }
	universalparams.ROOTVID = rootvid;
	
	universalparams.NUM_VERTICES = num_vertices; 
	universalparams.NUM_EDGES = num_edges; 

	universalparams.NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	// if(universalparams.NUM_UPARTITIONS > MAX_NUM_UPARTITIONS){ universalparams.NUM_UPARTITIONS = MAX_NUM_UPARTITIONS; } // FIXME.
	universalparams.NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	
	universalparams.NUM_PARTITIONS = 16;
	
	return universalparams;
}

void print_globalparams(unsigned int globalparams[1024], universalparams_t universalparams, utility * utilityobj){
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTIONS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTIONS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CSRVPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CSREDGES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__VDATAS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__NFRONTIERS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTIONS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTIONS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CSREDGES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__VDATAS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS]<<endl;
	unsigned int lastww_addr = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__END: "<<lastww_addr<<" (of "<< ((1 << 28) / 4 / HBM_AXI_PACK_SIZE) <<" wide-words) ("<<lastww_addr * HBM_CHANNEL_PACK_SIZE * 4<<" bytes)"<<endl;
	utilityobj->checkoutofbounds("app::ERROR 2234::", lastww_addr, ((1 << 28) / 4 / HBM_AXI_PACK_SIZE), universalparams.NUM_APPLYPARTITIONS, MAX_APPLYPARTITION_VECSIZE, NAp);
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__PARAM__NUM_VERTICES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__PARAM__NUM_EDGES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES]<<endl;
	#endif 
	return;
	// exit(EXIT_SUCCESS);
}

unsigned int load_globalparams2(HBM_channelAXISW_t * HBM_axichannel[NUM_FPGAS][2][NUM_PEs], unsigned int globalparams[1024], universalparams_t universalparams, unsigned int rootvid, unsigned int max_degree, utility * utilityobj){
	globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES] = universalparams.NUM_VERTICES;
	globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES] = universalparams.NUM_EDGES;

	// load globalparams
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int i=0; i<NUM_PEs; i++){ 
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__ACTIONS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTIONS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS]; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__CSREDGES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__VDATAS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS];
			
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__ACTIONS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTIONS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__CSRVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS]; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__CSREDGES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__VDATAS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__WWSIZE__NFRONTIERS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
			
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__NUM_VERTICES].data[0] = universalparams.NUM_VERTICES;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__NUM_EDGES].data[0] = universalparams.NUM_EDGES;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS].data[0] = universalparams.NUM_UPARTITIONS;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS].data[0] = universalparams.NUM_APPLYPARTITIONS;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS].data[0] = 1;//universalparams.NUM_ITERATIONS; // FIXME.
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEFRONTIERSFORCONTROLSWITCH].data[0] = 128;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__MAXDEGREE].data[0] = max_degree;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__ALGORITHM].data[0] = universalparams.ALGORITHM;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__ROOTVID].data[0] = rootvid;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__RANGEPERCHANNEL].data[0] = universalparams.NUM_VERTICES / NUM_PEs;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEDSTVID].data[0] = 16;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__PARAM__NUM_RUNS].data[0] = 1; // 

			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___RESETBUFFERSATSTART].data[0] = 1;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PREPAREEDGEUPDATES].data[0] = 1; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGEUPDATES].data[0] = 1; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGES].data[0] = 1; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___READ_FRONTIER_PROPERTIES].data[0] = 1;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___VCPROCESSEDGES].data[0] = 1; 
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___ECUPDATEEDGES].data[0] = 1; 
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___SAVEVCUPDATES].data[0] = 1; // FIXME? CAUSE OF HANGING?
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___COLLECTACTIVEDSTVIDS].data[0] = 1;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___APPLYUPDATESMODULE].data[0] = 1; 
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___READ_DEST_PROPERTIES].data[0] = 1;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___APPLYUPDATES].data[0] = 1; ////////////////////
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES].data[0] = 1; 
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___SAVE_DEST_PROPERTIES].data[0] = 1; 
			
			//////////////////////////////////////////////////////////////////////////////////////////////
			
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___EXCHANGEFRONTIERINFOS].data[0] = 1;
			
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__ASYNC__BATCH].data[0] = 0;
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__ASYNC__BATCHSIZE].data[0] = universalparams.NUM_APPLYPARTITIONS;
			
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__COMMANDS__COMMAND0].data[0] = 1;
		}
	}
	
	unsigned int lastww_addr = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
	return lastww_addr;
	// exit(EXIT_SUCCESS);
}

unsigned int load_actpack_edges(HBM_channelAXISW_t * HBM_axicenter[NUM_FPGAS][2], HBM_channelAXISW_t * HBM_axichannel[NUM_FPGAS][2][NUM_PEs], 
		HBM_channelAXISW_t * HBM_import_export[NUM_FPGAS][2], 
		vector<edge3_type> (&partitioned_edges)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS],
		unsigned int rootvid, unsigned int max_degree,
		utility * utilityobj, universalparams_t universalparams, unsigned int globalparams[1024]){
	
	unsigned int report_statistics[64]; for(unsigned int t=0; t<64; t++){ report_statistics[t] = 0; }
			
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	unsigned int num_its = 2;
	#else 
	unsigned int num_its = 1;	
	#endif 
	unsigned int lenght[NUM_FPGAS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ lenght[fpga] = 0; }
	acts_kernel * acts = new acts_kernel(universalparams);
	
	unsigned int lastww_addr2 = load_globalparams2(HBM_axichannel, globalparams, universalparams, rootvid, max_degree, utilityobj);
	print_globalparams(globalparams, universalparams, utilityobj);
	
	map_t * edge_maps[NUM_FPGAS][NUM_PEs]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int i=0; i<NUM_PEs; i++){ edge_maps[fpga][i] = new map_t[MAX_NUM_UPARTITIONS * MAX_NUM_LLP_PER_UPARTITION]; }}
	map_t * vu_map[NUM_FPGAS][NUM_PEs]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int i=0; i<NUM_PEs; i++){ vu_map[fpga][i] = new map_t[NUM_FPGAS * MAX_NUM_LLPSETS]; }}

	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for(unsigned int c=0; c<num_its; c++){ 
			unsigned int start_pu = fpga; 
			unsigned int size_pu = universalparams.NUM_UPARTITIONS; 
			unsigned int skip_pu = NUM_FPGAS; 
			
			unsigned int offset_dest; 
			unsigned int offset_destptrs; 
			if(globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0] == 0){
				offset_dest = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES]; 
				offset_destptrs = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2]; 
			} else {
				offset_dest = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES]; 
				offset_destptrs = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS]; 
			}
			unsigned int offset_updatesptrs = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS]; 
			
			create_act_pack * create_act_pack_obj = new create_act_pack(universalparams);
			lenght[fpga] = create_act_pack_obj->create_actpack(
				partitioned_edges, HBM_axichannel[fpga][0], HBM_axichannel[fpga][1], edge_maps[fpga], vu_map[fpga], offset_dest,
				universalparams.NUM_UPARTITIONS, universalparams.NUM_APPLYPARTITIONS, fpga, universalparams.NUM_UPARTITIONS, NUM_FPGAS, globalparams[GLOBALPARAMSCODE__COMMANDS__COMMAND0]
				);
				
			// load edge map 
			for(unsigned int p_u=start_pu; p_u<start_pu + size_pu; p_u+=skip_pu){
				for(unsigned int llp_set=0; llp_set<universalparams.NUM_APPLYPARTITIONS; llp_set++){ 
					for(unsigned int llp_id=0; llp_id<NUM_LLP_PER_LLPSET; llp_id++){
						unsigned int index = ((p_u * MAX_NUM_LLP_PER_UPARTITION) + (llp_set * NUM_LLP_PER_LLPSET) + llp_id) * 2; // '*2' because data is dual: i.e., offset and size
						for(unsigned int i=0; i<NUM_VALID_PEs; i++){
							map_t edge_map = edge_maps[fpga][i][p_u*MAX_NUM_LLP_PER_UPARTITION + llp_set*NUM_LLP_PER_LLPSET + llp_id];
							HBM_axichannel[fpga][0][i][offset_destptrs + (index / HBM_AXI_PACK_SIZE)].data[(index % HBM_AXI_PACK_SIZE)] = edge_map.offset;
							HBM_axichannel[fpga][0][i][offset_destptrs + ((index + 1) / HBM_AXI_PACK_SIZE)].data[((index + 1) % HBM_AXI_PACK_SIZE)] = edge_map.size;
						}
					}
				}
			}
			
			// load vertex map
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
				for(unsigned int t=0; t<universalparams.NUM_APPLYPARTITIONS; t++){		
					unsigned int index = (fpga * MAX_NUM_LLPSETS) + t;
					for(unsigned int i=0; i<NUM_PEs; i++){
						HBM_axichannel[fpga][0][i][offset_updatesptrs + index].data[0] = vu_map[fpga][i][index].offset;
						HBM_axichannel[fpga][0][i][offset_updatesptrs + index].data[1] = vu_map[fpga][i][index].size;
					}
				}
			}
		}
	}
	unsigned int max_lenght = 0; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ if(max_lenght < lenght[fpga]){ max_lenght = lenght[fpga]; }}
	cout<<"------------------------ max_lenght: "<<max_lenght<<endl;
	return max_lenght;
}

void write_to_hbmchannel(unsigned int i, HBM_channelAXISW_t * HBM_axichannel[NUM_FPGAS][2][NUM_PEs], unsigned int offset, unsigned int index, unsigned int data){
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		HBM_axichannel[fpga][0][i][offset + (index / HBM_AXI_PACK_SIZE)].data[index % HBM_AXI_PACK_SIZE] = data; 
	}
}

void write2_to_hbmchannel(unsigned int i, HBM_channelAXISW_t * HBM_axichannel[NUM_FPGAS][2][NUM_PEs], unsigned int offset, unsigned int v, unsigned int data){
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		if(v>=0 && v<EDGE_PACK_SIZE){
			HBM_axichannel[fpga][0][i][offset].data[v] = data; 
		} else {
			HBM_axichannel[fpga][1][i][offset].data[v - EDGE_PACK_SIZE] = data; 
		}
	}
}

void app::run(std::string setup, std::string algo, unsigned int rootvid, string graph_path, int graphisundirected, unsigned int numiterations, std::string _binaryFile1){
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	// exit(EXIT_SUCCESS);
	
	std::string binaryFile[2]; binaryFile[0] = _binaryFile1;
	std::cout << std::setprecision(2) << std::fixed;
	
	vector<edge3_type> edgedatabuffer;
	vector<edge_t> vertexptrbuffer;
	
	vector<edge3_type> edgedatabuffers[NUM_PROCS];
	vector<edge_t> vertexptrbuffers[NUM_PROCS];
	
	HBM_channelAXISW_t * HBM_axichannel[NUM_FPGAS][2][NUM_PEs]; 
	HBM_channelAXISW_t * HBM_axicenter[NUM_FPGAS][2]; 
	HBM_channelAXISW_t * HBM_import_export[NUM_FPGAS][2]; 
	unsigned int globalparams[1024];
	vector<edge3_type> csr_pack_edges[NUM_PEs]; 

	// allocate AXI HBM memory
	cout<<"app: initializing HBM_axichannels..."<<endl;
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for(unsigned int n=0; n<2; n++){
			for(unsigned int i=0; i<NUM_PEs; i++){ HBM_axichannel[fpga][n][i] = new HBM_channelAXISW_t[HBM_CHANNEL_SIZE]; }
			for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axichannel[fpga][n][i][t].data[v] = 0; }}}
		}
	}
	
	cout<<"app: initializing HBM_axicenters"<<endl;
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for(unsigned int n=0; n<2; n++){
			HBM_axicenter[fpga][n] = new HBM_channelAXISW_t[HBM_CHANNEL_SIZE]; 
			for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axicenter[fpga][n][t].data[v] = 0; }}
		}
	}
	
	cout<<"app: initializing HBM_import_export"<<endl;
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for(unsigned int n=0; n<2; n++){
			HBM_import_export[fpga][n] = new HBM_channelAXISW_t[IMPORT_EXPORT_GRANULARITY_VECSIZE]; 
			for(unsigned int t=0; t<IMPORT_EXPORT_GRANULARITY_VECSIZE; t++){ for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_import_export[fpga][n][t].data[v] = 0; }}
		}
	}
	
	string GRAPH_NAME = ""; 
	string GRAPH_PATH = graph_path;
	bool graphisundirected_bool = true; if(graphisundirected == 0){ graphisundirected_bool = false; }
	
	prepare_graph * prepare_graphobj = new prepare_graph();
	prepare_graphobj->start(GRAPH_PATH, edgedatabuffer, vertexptrbuffer, graphisundirected_bool);
	unsigned int num_edges = edgedatabuffer.size();
	unsigned int num_vertices = vertexptrbuffer.size();
	cout<<"app:run: num_vertices: "<<num_vertices<<", num_edges: "<<num_edges<<endl;
	
	universalparams_t universalparams = get_universalparams(algo, numiterations, rootvid, num_vertices, num_edges, graphisundirected_bool);
	cout<<"app::run:: NUM_VERTICES: "<<universalparams.NUM_VERTICES<<", NUM_EDGES: "<<universalparams.NUM_EDGES<<", NUM_UPARTITIONS: "<<universalparams.NUM_UPARTITIONS<<", NUM_APPLYPARTITIONS: "<<universalparams.NUM_APPLYPARTITIONS<<", VERTEX RANGE: "<<universalparams.NUM_VERTICES / NUM_PEs<<endl;			
	utility * utilityobj = new utility(universalparams);
	// utilityobj->printallparameters();

	unsigned int __NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs

	// create csr format
	unsigned int * degrees[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ degrees[i] = new unsigned int[(universalparams.NUM_VERTICES / NUM_PEs) + 64]; }
	unsigned int * v_ptr[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ v_ptr[i] = new unsigned int[(universalparams.NUM_VERTICES / NUM_PEs) + 64]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<(universalparams.NUM_VERTICES / NUM_PEs) + 64; t++){ v_ptr[i][t] = 0; degrees[i][t] = 0; }}
	unsigned int H = 0;
	for(unsigned int vid=0; vid<universalparams.NUM_VERTICES-1; vid++){
		unsigned int vptr_begin = vertexptrbuffer[vid];
		unsigned int vptr_end = vertexptrbuffer[vid+1];
		unsigned int edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; }
		utilityobj->checkoutofbounds("app::ERROR 211::", vid / MAX_UPARTITION_SIZE, __NUM_UPARTITIONS, NAp, NAp, NAp);
		for(unsigned int i=0; i<edges_size; i++){
			unsigned int H_hash = H % NUM_PEs;
			edge3_type edge = edgedatabuffer[vptr_begin + i];
			csr_pack_edges[H_hash].push_back(edge);
			degrees[H_hash][edge.srcvid / NUM_PEs] += 1;
		}
		H += 1;
	}
	
	edge3_type first_edge = csr_pack_edges[0][0];
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ first_edge.srcvid: "<<first_edge.srcvid<<", first_edge.dstvid: "<<first_edge.dstvid<<endl;
	#endif 
	#ifdef ___FORCE_SUCCESS_SINGLE_CHANNEL___
	rootvid = first_edge.srcvid;
	universalparams.ROOTVID = first_edge.srcvid;
	cout<<"app::run:: [CHANGED] app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int max_degree = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ unsigned int index = 0, ind = 0; for(unsigned int vid=0; vid<universalparams.NUM_VERTICES; vid++){ if(vid % NUM_PEs == i){ 
		if(max_degree < degrees[i][vid / NUM_PEs]){ max_degree = degrees[i][vid / NUM_PEs]; }
		v_ptr[i][vid / NUM_PEs] = index; index += degrees[i][vid / NUM_PEs]; ind += 1; }}}			
	for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"csr-pack:: PE: 21: csr_pack_edges["<<i<<"].size(): "<<csr_pack_edges[i].size()<<endl; }
	cout<<"app::csr-pack:: max_degree: "<<max_degree<<endl;
	
	// load globalparams: {vptrs, edges, updatesptrs, updates, vertexprops, frontiers}
	cout<<"app: loading global addresses: {vptrs, edges, updates, vertexprops, frontiers}..."<<endl;
	unsigned int numcsredges = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(numcsredges < csr_pack_edges[i].size()){ numcsredges = csr_pack_edges[i].size(); }} 
	unsigned int numww_csredges = (numcsredges / EDGE_PACK_SIZE) + 16;
	
	unsigned int csrvptrsz_u32 = ((universalparams.NUM_VERTICES / NUM_PEs) + 64); 
	unsigned int vdatasz_u32 = __NUM_APPLYPARTITIONS * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	unsigned int cfrontiersz_u32 = 1 * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	unsigned int nfrontiersz_u32 = (__NUM_APPLYPARTITIONS * VDATA_SUBPARTITION_VECSIZE * NUM_SUBPARTITION_PER_PARTITION * EDGE_PACK_SIZE) * 2;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"--_________________________--------------------- nfrontiersz_u32: "<<nfrontiersz_u32<<", vdatasz_u32: "<<vdatasz_u32<<", globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS] = "<<((nfrontiersz_u32 / (HBM_CHANNEL_PACK_SIZE / 2)) + 16)<<" "<<endl;
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app: EDGE_PACK_SIZE: "<<EDGE_PACK_SIZE<<endl;
	cout<<"app: HBM_CHANNEL_PACK_SIZE: "<<HBM_CHANNEL_PACK_SIZE<<endl;
	cout<<"app: HBM_AXI_PACK_SIZE: "<<HBM_AXI_PACK_SIZE<<endl;
	cout<<"app: HBM_AXI_PACK_BITSIZE: "<<HBM_AXI_PACK_BITSIZE<<endl;
	cout<<"app: HBM_CHANNEL_BYTESIZE: "<<HBM_CHANNEL_BYTESIZE<<endl;
	cout<<"app: MAX_APPLYPARTITION_VECSIZE: "<<MAX_APPLYPARTITION_VECSIZE<<endl;
	cout<<"app: MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<endl;
	cout<<"app: MAX_UPARTITION_VECSIZE: "<<MAX_UPARTITION_VECSIZE<<endl;
	cout<<"app: MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<endl;
	cout<<"app: HBM_CHANNEL_BYTESIZE: "<<HBM_CHANNEL_BYTESIZE<<endl;
	cout<<"app: HBM_CHANNEL_SIZE: "<<HBM_CHANNEL_SIZE<<endl;
	cout<<"app: UPDATES_BUFFER_PACK_SIZE: "<<UPDATES_BUFFER_PACK_SIZE<<endl;
	cout<<"app: __NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<endl;
	cout<<"app: __NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<endl;
	cout<<"app: NUM_SUBPARTITION_PER_PARTITION: "<<NUM_SUBPARTITION_PER_PARTITION<<endl;
	cout<<"app: VDATA_SUBPARTITION_VECSIZE: "<<VDATA_SUBPARTITION_VECSIZE<<endl;
	cout<<"app: UPDATES_BUFFER_PACK_SIZE: "<<UPDATES_BUFFER_PACK_SIZE<<endl;
	#endif 
	
	// unsigned int globalparams[1024];
	for(unsigned int t=0; t<1024; t++){ globalparams[t] = 0; }

	// load actions 
	for(unsigned int i=0; i<NUM_PEs; i++){ globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTIONS] = 512; } 
	unsigned int size_u32 = 16 * EDGE_PACK_SIZE;	
	
	// load raw edge-update vptrs 
	for(unsigned int i=0; i<NUM_PEs; i++){  
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTIONS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16;  // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTIONS] + globalparams[GLOBALPARAMSCODE__WWSIZE__ACTIONS]; 
	}
	size_u32 = 0;
	vector<edge3_type> partitioned_edges[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS];
	unsigned int * final_edge_updates_offsets[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ final_edge_updates_offsets[i] = new unsigned int[MAX_NUM_UPARTITIONS * MAX_NUM_LLPSETS]; for(unsigned int t=0; t<MAX_NUM_UPARTITIONS * MAX_NUM_LLPSETS; t++){ final_edge_updates_offsets[i][t] = 0; } }
	act_pack * pack = new act_pack(universalparams);
	pack->load_edgeupdates(vertexptrbuffer, edgedatabuffer, partitioned_edges);	
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
			for(unsigned int llp_set=0; llp_set<MAX_NUM_LLPSETS; llp_set++){ 
				if(p_u*MAX_NUM_LLPSETS + llp_set > 0){ final_edge_updates_offsets[i][p_u*MAX_NUM_LLPSETS + llp_set] = final_edge_updates_offsets[i][p_u*MAX_NUM_LLPSETS + llp_set - 1] + (partitioned_edges[i][p_u][llp_set].size() / EDGE_PACK_SIZE); }
			}
		}
	}
	for(unsigned int i=0; i<NUM_PEs; i++){  // FIXME?
		unsigned int index = 0;
		for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
			for(unsigned int llp_set=0; llp_set<MAX_NUM_LLPSETS; llp_set++){ 
				write_to_hbmchannel(i, HBM_axichannel, globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS], index, final_edge_updates_offsets[i][(p_u * MAX_NUM_LLPSETS) + llp_set]);
				write_to_hbmchannel(i, HBM_axichannel, globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS], index + 1, partitioned_edges[i][p_u][llp_set].size() / EDGE_PACK_SIZE);
				index += 2;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load edge udpdates vptrs  
	for(unsigned int i=0; i<NUM_PEs; i++){  
		globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16;  // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS]; 
	}
	size_u32 = 0;
	size_u32 = MAX_NUM_UPARTITIONS * MAX_NUM_UPARTITIONS;

	// load csr vptrs 
	for(unsigned int i=0; i<NUM_PEs; i++){  
		globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16; // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS]; 
	}
	size_u32 = csrvptrsz_u32;
	
	// load act-pack vptrs  
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS]; 
	}
	size_u32 = 0;
	size_u32 = MAX_NUM_UPARTITIONS * MAX_NUM_LLPSETS;

	// load act-pack vptrs2
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16; // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS]; 
	}
	size_u32 = 0;
	size_u32 = MAX_NUM_UPARTITIONS * MAX_NUM_LLP_PER_UPARTITION;

	// load vertex-updates ptrs
	cout<<"loading vertex-updates ptrs..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2] = (size_u32 / HBM_AXI_PACK_SIZE) + 16; // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2] + globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2]; 
	}
	size_u32 = 0;	
	
	size_u32 = NUM_FPGAS * MAX_NUM_LLPSETS * HBM_AXI_PACK_SIZE;
	unsigned int max_num_updates = (universalparams.NUM_EDGES / NUM_PEs / EDGE_PACK_SIZE) + ((3 * 1024 * 1024) / 16);
	
	// load raw edge updates
	cout<<"loading raw edge updates..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16; // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS]; 
	}
	size_u32 = 0; 
	#ifndef ___CREATE_ACTPACK_FROM_VECTOR___
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int index_i = 0;
		for(unsigned int p_u=0; p_u<universalparams.NUM_UPARTITIONS; p_u++){
			for(unsigned int llp_set=0; llp_set<universalparams.NUM_APPLYPARTITIONS; llp_set++){
				unsigned int index = 0;
				unsigned int offset = final_edge_updates_offsets[i][p_u*MAX_NUM_LLPSETS + llp_set];
				for(unsigned int t=0; t<partitioned_edges[i][p_u][llp_set].size() / EDGE_PACK_SIZE; t++){ 
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){	
						utilityobj->checkoutofbounds("app::ERROR 23487::", globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES] + offset + t, HBM_CHANNEL_SIZE, globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES], offset, t);
						write2_to_hbmchannel(i, HBM_axichannel, globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES] + offset + t, 2 * v, partitioned_edges[i][p_u][llp_set][index].srcvid % MAX_UPARTITION_SIZE);
						write2_to_hbmchannel(i, HBM_axichannel globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES] + offset + t, 2 * v + 1, partitioned_edges[i][p_u][llp_set][index].dstvid % MAX_UPARTITION_SIZE);
						index += 1;
					}
					if(i==0){ size_u32 += EDGE_PACK_SIZE * 2; }		
				}
			}
		}
	}
	#endif 
	
	// allocate temp space
	cout<<"loading partial-processed edge updates..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES]; 
	}
	// size_u32 = EDGE_UPDATES_DRAMBUFFER_SIZE * 2 * HBM_CHANNEL_PACK_SIZE; // '2' is padding // FIXME.
	size_u32 = EDGE_UPDATES_DRAMBUFFER_LONGSIZE * HBM_CHANNEL_PACK_SIZE; // '2' is padding // FIXME.
	
	// allocate edge updates space (actpack format)
	cout<<"loading edge updates (actpack format)..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES]; 
	}
	size_u32 = 0; 
	unsigned int approx_actpacksz = (universalparams.NUM_EDGES / NUM_PEs / EDGE_PACK_SIZE) + (1024000 / EDGE_PACK_SIZE);
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	size_u32 += approx_actpacksz * EDGE_PACK_SIZE * 2;
	#endif

	// load edges (csr format)
	cout<<"loading load csr edges..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES]; 
	}
	size_u32 = 0; 
	#ifdef TRAVERSAL_ALGORITHM_TYPE
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES];
		for(unsigned int t=0; t<numww_csredges; t++){ 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				write2_to_hbmchannel(i, HBM_axichannel, base_offset + t, 2 * v, csr_pack_edges[i][index].srcvid);
				write2_to_hbmchannel(i, HBM_axichannel, base_offset + t, 2 * v + 1, (csr_pack_edges[i][index].dstvid << 1) | csr_pack_edges[i][index].valid);	
				index += 1;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	#endif 
	
	// allocate edges space (actpack format)
	cout<<"loading act-pack edges..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES] + globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES]; 
	}
	size_u32 = 0; unsigned int sourceid = 0; unsigned int destid = 0; unsigned int weight = 0;
	size_u32 = approx_actpacksz * EDGE_PACK_SIZE * 2; 
	
	// load edges and edge updates 
	unsigned int max_lenght = load_actpack_edges(HBM_axicenter, HBM_axichannel, 
		HBM_import_export,
		partitioned_edges,
		rootvid, max_degree,
		utilityobj, universalparams, globalparams);
	size_u32 = (max_lenght * EDGE_PACK_SIZE * 2) + (1024 * EDGE_PACK_SIZE * 2); 
	
	// load vertex updates 
	cout<<"loading vertex updates..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES] + 0; // act-pack edges occupy 1/2 of hbmchannel
	}
	
	// load vertex properties
	cout<<"loading vertex properties..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		// globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES] + (64 * 1024);
		globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES] + (128 * 1024);
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES]; 
	}
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS];
		for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ 
			for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){ 
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					unsigned int index = p*MAX_APPLYPARTITION_VECSIZE*EDGE_PACK_SIZE + t*EDGE_PACK_SIZE + v;
					write2_to_hbmchannel(i, HBM_axichannel, base_offset + (p * MAX_APPLYPARTITION_VECSIZE + t), 2 * v, algorithmobj->vertex_initdata(universalparams.ALGORITHM, index));
					write2_to_hbmchannel(i, HBM_axichannel, base_offset + (p * MAX_APPLYPARTITION_VECSIZE + t), 2 * v + 1, 0);
					if(i==0){ size_u32 += 2; }
				}
			}
		}
	}
	
	// cfrontier 
	cout<<"loading cfrontier..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS] = (vdatasz_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] + globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS]; 
	}
	// nfrontier
	cout<<"loading nfrontier..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP] = (cfrontiersz_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP] + globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP]; 
		globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS] = (nfrontiersz_u32 / (HBM_CHANNEL_PACK_SIZE / 2)) + 16;
	}	

	unsigned int lastww_addr2 = load_globalparams2(HBM_axichannel, globalparams, universalparams, rootvid, max_degree, utilityobj);
	print_globalparams(globalparams, universalparams, utilityobj);

	//Free 
	for(unsigned int i=0; i<NUM_PEs; i++){ csr_pack_edges[i].clear(); } 
	edgedatabuffer.clear();
	vertexptrbuffer.clear();
	// exit(EXIT_SUCCESS); 

	host * hostobj = new host(universalparams);
	hostobj->runapp(binaryFile, HBM_axichannel, HBM_axicenter, HBM_CHANNEL_SIZE, globalparams, universalparams, partitioned_edges);
	
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ for(unsigned int llp_set=0; llp_set<MAX_NUM_LLPSETS; llp_set++){ partitioned_edges[i][p_u][llp_set].clear(); }}}
	return;
}

void app::summary(){
	return;
}
















