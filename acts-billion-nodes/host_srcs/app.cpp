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
	universalparams.NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	
	universalparams.NUM_PARTITIONS = 16;
	
	return universalparams;
}
void app::run(std::string setup, std::string algo, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1){
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	
	
	
	
	
	
	
	
	
	// unsigned int total_cycles_iter = 1140575; // 1777109;
	// float time_lapse = (total_cycles_iter * _NUMNANOSECONDS_PER_CLOCKCYCLE_) / 1000000;
	// cout<<"acts_kernel SUMMARY:: number of clock cycles seen: "<<total_cycles_iter<<" ("<<time_lapse<<" milliseconds)"<<endl;
	
	// float time_lapse1 = (total_cycles_iter / 1000000) * _NUMNANOSECONDS_PER_CLOCKCYCLE_;
	// cout<<"acts_kernel SUMMARY:: number of clock cycles seen: "<<total_cycles_iter<<" ("<<time_lapse1<<" milliseconds)"<<endl;
	
	// float time_lapse2 = (float)((total_cycles_iter * EDGE_PACK_SIZE) / 1000) / (float)ACTS_AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC;
	// cout<<"acts_kernel SUMMARY:: number of clock cycles seen: "<<total_cycles_iter<<" ("<<time_lapse2<<" milliseconds)"<<endl;
	// exit(EXIT_SUCCESS);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	std::cout << std::setprecision(2) << std::fixed;
	
	vector<edge3_type> edgedatabuffer;
	vector<edge_t> vertexptrbuffer;

	string GRAPH_NAME = ""; 
	string GRAPH_PATH = graph_path;
	bool graphisundirected_bool = true; if(graphisundirected == 0){ graphisundirected_bool = false; }
	
	prepare_graph * prepare_graphobj = new prepare_graph();
	prepare_graphobj->start(GRAPH_PATH, edgedatabuffer, vertexptrbuffer, graphisundirected_bool);
	unsigned int num_edges = edgedatabuffer.size();
	unsigned int num_vertices = vertexptrbuffer.size();
	cout<<"app:run: num_vertices: "<<num_vertices<<", num_edges: "<<num_edges<<endl;
	
	universalparams_t universalparams = get_universalparams(algo, numiterations, rootvid, num_vertices, num_edges, graphisundirected_bool);
	utility * utilityobj = new utility(universalparams);
	// utilityobj->printallparameters();
	
	unsigned int __NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	
	// create act-pack format
	map_t * vptr_actpack[NUM_PEs][MAX_NUM_UPARTITIONS];
	vector<edge3_vec_dt> act_pack_edges[NUM_PEs];
	edge3_vec_dt * act_pack_edges_arr[NUM_PEs];
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ vptr_actpack[i][p_u] = new map_t[MAX_NUM_LLPSETS]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ vptr_actpack[i][p_u][t].offset = 0; vptr_actpack[i][p_u][t].size = 0; }}}
	act_pack * pack = new act_pack(universalparams);
	pack->pack(vertexptrbuffer, edgedatabuffer, act_pack_edges, vptr_actpack);
	
	// create csr format
	vector<edge3_type> csr_pack_edges[NUM_PEs]; 
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
	for(unsigned int i=0; i<NUM_PEs; i++){ unsigned int index = 0, ind = 0; for(unsigned int vid=0; vid<universalparams.NUM_VERTICES; vid++){ if(vid % NUM_PEs == i){ v_ptr[i][vid / NUM_PEs] = index; index += degrees[i][vid / NUM_PEs]; ind += 1; }}}
	for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"csr-pack:: PE: 21: act_pack_edges["<<i<<"].size(): "<<csr_pack_edges[i].size()<<endl; }
	unsigned int max = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(max < act_pack_edges[i].size()){ max = act_pack_edges[i].size(); }} 
	for(unsigned int i=0; i<NUM_PEs; i++){ act_pack_edges_arr[i] = new edge3_vec_dt[max]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<max; t++){ act_pack_edges_arr[i][t] =  act_pack_edges[i][t]; }}
	
	HBM_channelX_t * HBM_center = new HBM_channelX_t[HBM_CHANNEL_SIZE];
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){ HBM_center[t].data[v] = 0; }}

	// allocate HBM memory
	HBM_channel_t * HBM_channel[NUM_PEs];// = new HBM_channel_t[NUM_PEs]; 
	cout<<"app: allocating HBM memory..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i] = new HBM_channel_t[HBM_CHANNEL_SIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){ HBM_channel[i][t].data[v] = 0; }}}
	
	// load globalparams: {vptrs, edges, updatesptrs, updates, vertexprops, frontiers}
	cout<<"app: loading global addresses: {vptrs, edges, updates, vertexprops, frontiers}..."<<endl;
	unsigned int numcsredges = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(numcsredges < csr_pack_edges[i].size()){ numcsredges = csr_pack_edges[i].size(); }} 
	unsigned int numww_csredges = (numcsredges / EDGE_PACK_SIZE) + 16;
	unsigned int numww_actpackedges = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(numww_actpackedges < act_pack_edges[i].size()){ numww_actpackedges = act_pack_edges[i].size(); }} 
	unsigned int csrvptrsz_u32 = ((universalparams.NUM_VERTICES / NUM_PEs) + 64); 
	unsigned int actpackvptrsz_u32 = (MAX_NUM_UPARTITIONS * MAX_NUM_LLPSETS); 
	unsigned int csredgessz_u32 = numcsredges * 2; 
	unsigned int actpackedgessz_u32 = numww_actpackedges * EDGE_PACK_SIZE * 2;
	unsigned int vdatasz_u32 = __NUM_APPLYPARTITIONS * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	unsigned int cfrontiersz_u32 = 1 * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	unsigned int nfrontiersz_u32 = __NUM_UPARTITIONS * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;

	unsigned int globalparams[1024];
	for(unsigned int t=0; t<1024; t++){ globalparams[t] = 0; }
	
	// load csr vptrs  
	unsigned int size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS] = 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS];
		unsigned int * T = (unsigned int *)&HBM_channel[i][base_offset].data[0];
		for(unsigned int t=0; t<csrvptrsz_u32; t++){
			T[index] = v_ptr[i][t]; // HBM_channel[i].v_ptr[t];
			index += 1;
			if(i==0){ size_u32 += 1; }
		}
	}
	
	// load act-pack vptrs  
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS]; 
	}
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS];
		for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
			for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
				HBM_channel[i][base_offset + (index / HBM_CHANNEL_PACK_SIZE)].data[index % HBM_CHANNEL_PACK_SIZE] = vptr_actpack[i][p_u][t].offset; // HBM_channel[i].vptr_actpack[p_u][t].offset; 
				HBM_channel[i][base_offset + ((index + 1) / HBM_CHANNEL_PACK_SIZE)].data[(index + 1) % HBM_CHANNEL_PACK_SIZE] = vptr_actpack[i][p_u][t].size; // HBM_channel[i].vptr_actpack[p_u][t].size; 
				index += 2;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load csr edges 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS]; 
	}
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES];
		for(unsigned int t=0; t<numww_csredges; t++){ 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				HBM_channel[i][base_offset + t].data[2 * v] = csr_pack_edges[i][index].srcvid;
				HBM_channel[i][base_offset + t].data[2 * v + 1] = (csr_pack_edges[i][index].dstvid << 1) | csr_pack_edges[i][index].valid;	
				index += 1;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load act-pack edges 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES] + globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES]; 
	}
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES];
		for(unsigned int t=0; t<numww_actpackedges; t++){ 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				HBM_channel[i][base_offset + t].data[2 * v] = act_pack_edges[i][t].data[v].srcvid;
				HBM_channel[i][base_offset + t].data[2 * v + 1] = (act_pack_edges[i][t].data[v].dstvid << 1) | act_pack_edges[i][t].data[v].valid; 
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load updatesptrs
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES] + globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES]; 
	}
	size_u32 = 0;
	keyvalue_t updateswwcount[NUM_PEs][MAX_NUM_LLPSETS]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ updateswwcount[i][t].value = 0; updateswwcount[i][t].key = 0; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS];
		for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
			for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
				updateswwcount[i][t].value += vptr_actpack[i][p_u][t].size;
			}
		}
	}
	for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ 
		if(false){ cout<<"--- updateswwcount[0]["<<t<<"].value: "<<updateswwcount[0][t].value<<endl; }
	}
	keyvalue_t maxs[MAX_NUM_LLPSETS]; for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ maxs[t].key = 0; maxs[t].value = 0; }
	for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ for(unsigned int n=0; n<NUM_PEs; n++){ if(maxs[t].value < updateswwcount[n][t].value){ maxs[t].value = updateswwcount[n][t].value; }}}
	for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
		cout<<"##### maxs["<<t<<"].key: "<<maxs[t].key<<", maxs["<<t<<"].value: "<<maxs[t].value<<endl; 
	}
	for(unsigned int t=1; t<MAX_NUM_LLPSETS; t++){ maxs[t].key = maxs[t-1].key + maxs[t-1].value + 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS];
		for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
			if(false){ if(i==0){ cout<<"maxs["<<t<<"].key: "<<maxs[t].key<<", maxs["<<t<<"].value: "<<maxs[t].value<<endl; }}
			HBM_channel[i][base_offset + t].data[0] = maxs[t].key;
			if(i==0){ size_u32 += HBM_CHANNEL_PACK_SIZE; }
		}
	}
	unsigned int max_num_updates = maxs[MAX_NUM_LLPSETS-1].key;
	if(false){ cout<<"-------------------------- max_num_updates: "<<max_num_updates<<endl; }
	
	// load updates (NAp)
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS]; 
	}
	
	// load vertex properties
	for(unsigned int i=0; i<NUM_PEs; i++){
		globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATES] = max_num_updates + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATES]; 
	}
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS];
		for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ 
			for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){ 
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					HBM_channel[i][base_offset + (p * MAX_APPLYPARTITION_VECSIZE + t)].data[2 * v] = algorithmobj->vertex_initdata(universalparams.ALGORITHM);
					HBM_channel[i][base_offset + (p * MAX_APPLYPARTITION_VECSIZE + t)].data[2 * v + 1] = 0;
					if(i==0){ size_u32 += 2; }
				}
			}
		}
	}
	
	// cfrontier 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS] = (vdatasz_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] + globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS]; 
	}
	
	// nfrontier
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP] = (cfrontiersz_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP] + globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP]; 
		globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS] = (nfrontiersz_u32 / (HBM_CHANNEL_PACK_SIZE / 2)) + 16;
	}
	
	// globalparams
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__CSREDGES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__UPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__VDATAS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS];
		
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__CSRVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__CSREDGES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__UPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__VDATAS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__NFRONTIERS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
		
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_VERTICES].data[0] = universalparams.NUM_VERTICES;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_EDGES].data[0] = universalparams.NUM_EDGES;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS].data[0] = universalparams.NUM_UPARTITIONS;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS].data[0] = universalparams.NUM_APPLYPARTITIONS;
	}

	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CSRVPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CSREDGES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__UPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__VDATAS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__NFRONTIERS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS]<<endl;
	
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CSREDGES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__UPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__VDATAS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS]<<endl;
	unsigned int lastww_addr = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__END: "<<lastww_addr<<" (of "<< ((1 << 28) / 4 / 16) <<" wide-words) ("<<lastww_addr * HBM_CHANNEL_PACK_SIZE * 4<<" bytes)"<<endl;
	utilityobj->checkoutofbounds("app::ERROR 2234::", lastww_addr, ((1 << 28) / 4 / 16), __NUM_APPLYPARTITIONS, MAX_APPLYPARTITION_VECSIZE, NAp);
	
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__PARAM__NUM_VERTICES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__PARAM__NUM_EDGES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES]<<endl;
	
	// clear 
	for(unsigned int i=0; i<NUM_PEs; i++){ csr_pack_edges[i].clear(); act_pack_edges[i].clear(); } // clear 
	
	// assign root vid 
	keyvalue_t root; root.key = rootvid; root.value = 0; keyvalue_t invalid; invalid.key = INVALIDDATA; invalid.value = INVALIDDATA; 
	// for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
		// if(v==1){ HBM_center[0].data[2*v] = root.key; HBM_center[0].data[2*v+1] = root.value; } 
		// else { HBM_center[0].data[2*v] = invalid.key; HBM_center[0].data[2*v+1] = invalid.value; } 
	// }	
	for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
		if(v==1){ HBM_center[0].data[v] = (root.value << 28) | root.key; } 
		else { HBM_center[0].data[v] = INVALIDDATA; } 
	}

	// allocate AXI HBM memory
	#ifdef ___USE_AXI_CHANNEL___
	HBM_axichannel_t * HBM_axichannel[2][NUM_PEs]; 
	cout<<"app: allocating HBM memory..."<<endl;
	for(unsigned int n=0; n<2; n++){
		for(unsigned int i=0; i<NUM_PEs; i++){ HBM_axichannel[n][i] = new HBM_axichannel_t[HBM_CHANNEL_SIZE]; }
		for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axichannel[n][i][t].data[v] = 0; }}}
	}
	
	cout<<"app: copying to axi-friendly channels..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				HBM_axichannel[0][i][t].data[v] = HBM_channel[i][t].data[v];
			}
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				HBM_axichannel[1][i][t].data[v] = HBM_channel[i][t].data[HBM_AXI_PACK_SIZE + v];
			}
		}
	}
	#endif 
	
	// run acts
	acts_kernel * acts = new acts_kernel(universalparams);
	#ifdef ___USE_AXI_CHANNEL___
	acts->top_function(HBM_center,
		HBM_axichannel[0][0], HBM_axichannel[1][0] 
		,HBM_axichannel[0][1], HBM_axichannel[1][1] 
		,HBM_axichannel[0][2], HBM_axichannel[1][2] 
		,HBM_axichannel[0][3], HBM_axichannel[1][3] 
		,HBM_axichannel[0][4], HBM_axichannel[1][4] 
		,HBM_axichannel[0][5], HBM_axichannel[1][5] 
		,HBM_axichannel[0][6], HBM_axichannel[1][6] 
		,HBM_axichannel[0][7], HBM_axichannel[1][7] 
		,HBM_axichannel[0][8], HBM_axichannel[1][8] 
		,HBM_axichannel[0][9], HBM_axichannel[1][9] 
		,HBM_axichannel[0][10], HBM_axichannel[1][10] 
		,HBM_axichannel[0][11], HBM_axichannel[1][11]
		#if NUM_PEs==24
		,HBM_axichannel[0][12], HBM_axichannel[1][12] 
		,HBM_axichannel[0][13], HBM_axichannel[1][13] 
		,HBM_axichannel[0][14], HBM_axichannel[1][14] 
		,HBM_axichannel[0][15], HBM_axichannel[1][15]
		,HBM_axichannel[0][16], HBM_axichannel[1][16]
		,HBM_axichannel[0][17], HBM_axichannel[1][17]
		,HBM_axichannel[0][18], HBM_axichannel[1][18] 
		,HBM_axichannel[0][19], HBM_axichannel[1][19] 
		,HBM_axichannel[0][20], HBM_axichannel[1][20] 
		,HBM_axichannel[0][21], HBM_axichannel[1][21] 
		,HBM_axichannel[0][22], HBM_axichannel[1][22] 
		,HBM_axichannel[0][23], HBM_axichannel[1][23]
		#endif 
		);
	#else
	acts->top_function(HBM_center, 
		HBM_channel[0], HBM_channel[0], 
		HBM_channel[1], HBM_channel[1], 
		HBM_channel[2], HBM_channel[2], 
		HBM_channel[3], HBM_channel[3], 
		HBM_channel[4], HBM_channel[4], 
		HBM_channel[5], HBM_channel[5], 
		HBM_channel[6], HBM_channel[6], 
		HBM_channel[7], HBM_channel[7], 
		HBM_channel[8], HBM_channel[8], 
		HBM_channel[9], HBM_channel[9], 
		HBM_channel[10], HBM_channel[10], 
		HBM_channel[11], HBM_channel[11], 
		HBM_channel[12], HBM_channel[12], 
		HBM_channel[13], HBM_channel[13], 
		HBM_channel[14], HBM_channel[14], 
		HBM_channel[15], HBM_channel[15], 
		HBM_channel[16], HBM_channel[16], 
		HBM_channel[17], HBM_channel[17], 
		HBM_channel[18], HBM_channel[18], 
		HBM_channel[19], HBM_channel[19], 
		HBM_channel[20], HBM_channel[20], 
		HBM_channel[21], HBM_channel[21], 
		HBM_channel[22], HBM_channel[22], 
		HBM_channel[23], HBM_channel[23]
		);
	#endif	
	return;
}

void app::summary(){
	return;
}
















