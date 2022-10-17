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
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_v=0; p_v<MAX_NUM_UPARTITIONS; p_v++){ vptr_actpack[i][p_v] = new map_t[MAX_NUM_LLPSETS]; }}
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
	
	HBM_center_t HBM_center;
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ HBM_center.cfrontier_dram[v][p] = new keyvalue_t[MAX_UPARTITION_SIZE]; }}
	
	HBM_channel_t HBM_channel[NUM_PEs]; 
	// load all datas to HBM
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].v_ptr = new unsigned int[(universalparams.NUM_VERTICES / NUM_PEs) + 64]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<(universalparams.NUM_VERTICES / NUM_PEs) + 64; t++){ HBM_channel[i].v_ptr[t] = v_ptr[i][t];; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_v=0; p_v<MAX_NUM_UPARTITIONS; p_v++){ HBM_channel[i].vptr_actpack[p_v] = new map_t[MAX_NUM_LLPSETS]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_v=0; p_v<MAX_NUM_UPARTITIONS; p_v++){ for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ HBM_channel[i].vptr_actpack[p_v][t] = vptr_actpack[i][p_v][t]; }}}
	unsigned int numcsredges = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(numcsredges < csr_pack_edges[i].size()){ numcsredges = csr_pack_edges[i].size(); }} 
	unsigned int numww_csredges = (numcsredges / EDGE_PACK_SIZE) + 16;
		for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].csr_pack_edges = new edge3_vec_dt[numww_csredges]; }
		for(unsigned int i=0; i<NUM_PEs; i++){ unsigned int index = 0; for(unsigned int t=0; t<numcsredges; t++){ HBM_channel[i].csr_pack_edges[index / EDGE_PACK_SIZE].data[index % EDGE_PACK_SIZE] = csr_pack_edges[i][t]; index += 1; }}
	unsigned int numww_actpackedges = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(numww_actpackedges < act_pack_edges[i].size()){ numww_actpackedges = act_pack_edges[i].size(); }} 
		for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].act_pack_edges = new edge3_vec_dt[numww_actpackedges]; }
		for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<numww_actpackedges; t++){ HBM_channel[i].act_pack_edges[t] =  act_pack_edges[i][t]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ HBM_channel[i].vdatas_dram[p] = new vprop_vec_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ HBM_channel[i].vdatas_dram[p][t].data[v].prop = algorithmobj->vertex_initdata(universalparams.ALGORITHM); HBM_channel[i].vdatas_dram[p][t].data[v].mask = 0; }}}}		
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].cfrontier_dram_tmp = new uint512_vec_dt[MAX_APPLYPARTITION_VECSIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ HBM_channel[i].nfrontier_dram[p] = new uint512_vec_dt[MAX_APPLYPARTITION_VECSIZE]; }} // FIXME.
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ HBM_channel[i].updates_dram[p] = new uint512_vec_dt[HBM_CHANNEL_SIZE]; }}
	
	// allocate HBM memory
	cout<<"app: allocating HBM memory..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams = new uint512_ivec_dt[1024]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<1024; t++){ for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){ HBM_channel[i].globalparams[t].data[v] = 0; }}}
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].HBM = new uint512_ivec_dt[HBM_CHANNEL_SIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){ HBM_channel[i].HBM[t].data[v] = 0; }}}
	
	// load globalparams: {vptrs, edges, updates, vertexprops, frontiers}
	cout<<"app: loading global addresses: {vptrs, edges, updates, vertexprops, frontiers}..."<<endl;
	unsigned int csrvptrsz_u32 = ((universalparams.NUM_VERTICES / NUM_PEs) + 64); //  / EDGE_PACK_SIZE;
	unsigned int actpackvptrsz_u32 = (MAX_NUM_UPARTITIONS * MAX_NUM_LLPSETS); // / (HBM_CHANNEL_PACK_SIZE / 2);
	unsigned int csredgessz_u32 = numcsredges * 2; // * HBM_CHANNEL_PACK_SIZE;
	unsigned int actpackedgessz_u32 = numww_actpackedges * EDGE_PACK_SIZE * 2;
	unsigned int updatessz_u32 = actpackedgessz_u32 + (1 << 20);
	unsigned int vdatasz_u32 = __NUM_APPLYPARTITIONS * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	unsigned int cfrontiersz_u32 = 1 * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	unsigned int nfrontiersz_u32 = __NUM_UPARTITIONS * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	cout<<"------------- app:: [csrvptrsz_u32: "<<csrvptrsz_u32<<"]"<<endl;
	cout<<"------------- app:: [actpackvptrsz_u32: "<<actpackvptrsz_u32<<"]"<<endl;
	cout<<"------------- app:: [csredgessz_u32: "<<csredgessz_u32<<"]"<<endl;
	cout<<"------------- app:: [actpackedgessz_u32: "<<actpackedgessz_u32<<"]"<<endl;
	cout<<"------------- app:: [updatessz_u32: "<<updatessz_u32<<"]"<<endl;
	cout<<"------------- app:: [vdatasz_u32: "<<vdatasz_u32<<"]"<<endl;
	cout<<"------------- app:: [cfrontiersz_u32: "<<cfrontiersz_u32<<"]"<<endl;
	cout<<"------------- app:: [nfrontiersz_u32: "<<nfrontiersz_u32<<"]"<<endl;
	cout<<"------------- app:: [total: "<<csrvptrsz_u32 + actpackvptrsz_u32 + csredgessz_u32 + actpackedgessz_u32 + updatessz_u32 + vdatasz_u32 + cfrontiersz_u32 + nfrontiersz_u32<<"]"<<endl;
	cout<<"------------- app:: [total (uint 32): "<<csrvptrsz_u32 + actpackvptrsz_u32 + csredgessz_u32 + actpackedgessz_u32 + updatessz_u32 + vdatasz_u32 + cfrontiersz_u32 + nfrontiersz_u32<<"]"<<endl;
		
	/* // load act-pack edges 
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = 0;
		for(unsigned int t=0; t<numww_actpackedges; t++){ 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				HBM_channel[i].HBM[base_offset + t].data[2 * v] = HBM_channel[i].act_pack_edges[t].data[v].srcvid;
				// HBM_channel[i].HBM[base_offset + t].data[2 * v + 1] = HBM_channel[i].act_pack_edges[t].data[v].dstvid;
				HBM_channel[i].HBM[base_offset + t].data[2 * v + 1] = (HBM_channel[i].act_pack_edges[t].data[v].dstvid << 1) | HBM_channel[i].act_pack_edges[t].data[v].valid; 
			}
		}
	} */
		
	// #ifdef ___NOT___USED___
	// load csr vptrs  
	unsigned int size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams[GLOBALPARAMSCODE__CSRVPTRS].data[0] = 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = HBM_channel[i].globalparams[GLOBALPARAMSCODE__CSRVPTRS].data[0];
		unsigned int * T = (unsigned int *)&HBM_channel[i].HBM[0];
		for(unsigned int t=0; t<csrvptrsz_u32; t++){
			T[index] = HBM_channel[i].v_ptr[base_offset + t];
			index += 1;
			if(i==0){ size_u32 += 1; }
		}
	}
	
	// load act-pack vptrs  
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams[GLOBALPARAMSCODE__ACTPACKVPTRS].data[0] = HBM_channel[i].globalparams[GLOBALPARAMSCODE__CSRVPTRS].data[0] + (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16; }
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = HBM_channel[i].globalparams[GLOBALPARAMSCODE__ACTPACKVPTRS].data[0];
		unsigned int * T = (unsigned int *)&HBM_channel[i].HBM[0];
		for(unsigned int p_v=0; p_v<MAX_NUM_UPARTITIONS; p_v++){ 
			for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
				T[2 * index] = HBM_channel[i].vptr_actpack[p_v][base_offset + t].offset;
				T[2 * index + 1] = HBM_channel[i].vptr_actpack[p_v][base_offset + t].size;
				index += 2;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load csr edges 
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams[GLOBALPARAMSCODE__CSREDGES].data[0] = HBM_channel[i].globalparams[GLOBALPARAMSCODE__ACTPACKVPTRS].data[0] + (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16; }
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int base_offset = HBM_channel[i].globalparams[GLOBALPARAMSCODE__CSREDGES].data[0];
		for(unsigned int t=0; t<numww_csredges; t++){ 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				HBM_channel[i].HBM[base_offset + t].data[2 * v] = HBM_channel[i].csr_pack_edges[t].data[v].srcvid;
				// HBM_channel[i].HBM[base_offset + t].data[2 * v + 1] = HBM_channel[i].csr_pack_edges[t].data[v].dstvid;
				HBM_channel[i].HBM[base_offset + t].data[2 * v + 1] = (HBM_channel[i].csr_pack_edges[t].data[v].dstvid << 1) | HBM_channel[i].csr_pack_edges[t].data[v].valid; 
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load act-pack edges 
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams[GLOBALPARAMSCODE__ACTPACKEDGES].data[0] = HBM_channel[i].globalparams[GLOBALPARAMSCODE__CSREDGES].data[0] + (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16; }
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = HBM_channel[i].globalparams[GLOBALPARAMSCODE__ACTPACKEDGES].data[0];
		for(unsigned int t=0; t<numww_actpackedges; t++){ 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				HBM_channel[i].HBM[base_offset + t].data[2 * v] = HBM_channel[i].act_pack_edges[t].data[v].srcvid;
				// HBM_channel[i].HBM[base_offset + t].data[2 * v + 1] = HBM_channel[i].act_pack_edges[t].data[v].dstvid;
				HBM_channel[i].HBM[base_offset + t].data[2 * v + 1] = (HBM_channel[i].act_pack_edges[t].data[v].dstvid << 1) | HBM_channel[i].act_pack_edges[t].data[v].valid; 
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load updates (NAp)
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams[GLOBALPARAMSCODE__UPDATES].data[0] = HBM_channel[i].globalparams[GLOBALPARAMSCODE__ACTPACKEDGES].data[0] + (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16; }
	
	// load vertex properties
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams[GLOBALPARAMSCODE__VDATAS].data[0] = HBM_channel[i].globalparams[GLOBALPARAMSCODE__UPDATES].data[0] + (updatessz_u32 / HBM_CHANNEL_PACK_SIZE) + 16; }
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = HBM_channel[i].globalparams[GLOBALPARAMSCODE__VDATAS].data[0];
		for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ 
			for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){ 
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					HBM_channel[i].HBM[base_offset + (p * MAX_APPLYPARTITION_VECSIZE + t)].data[2 * v] = HBM_channel[i].vdatas_dram[p][t].data[v].prop;
					HBM_channel[i].HBM[base_offset + (p * MAX_APPLYPARTITION_VECSIZE + t)].data[2 * v + 1] = HBM_channel[i].vdatas_dram[p][t].data[v].mask;
					if(i==0){ size_u32 += 2; }
				}
			}
		}
	}
	
	// cfrontier 
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams[GLOBALPARAMSCODE__CFRONTIERSTMP].data[0] = HBM_channel[i].globalparams[GLOBALPARAMSCODE__VDATAS].data[0] + (vdatasz_u32 / HBM_CHANNEL_PACK_SIZE) + 16; }
	
	// nfrontier
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i].globalparams[GLOBALPARAMSCODE__NFRONTIERS].data[0] = HBM_channel[i].globalparams[GLOBALPARAMSCODE__CFRONTIERSTMP].data[0] + (cfrontiersz_u32 / HBM_CHANNEL_PACK_SIZE) + 16; }
	
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__CSRVPTRS: "<<HBM_channel[0].globalparams[GLOBALPARAMSCODE__CSRVPTRS].data[0]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__ACTPACKVPTRS: "<<HBM_channel[0].globalparams[GLOBALPARAMSCODE__ACTPACKVPTRS].data[0]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__CSREDGES: "<<HBM_channel[0].globalparams[GLOBALPARAMSCODE__CSREDGES].data[0]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__ACTPACKEDGES: "<<HBM_channel[0].globalparams[GLOBALPARAMSCODE__ACTPACKEDGES].data[0]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__UPDATES: "<<HBM_channel[0].globalparams[GLOBALPARAMSCODE__UPDATES].data[0]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__VDATAS: "<<HBM_channel[0].globalparams[GLOBALPARAMSCODE__VDATAS].data[0]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__CFRONTIERSTMP: "<<HBM_channel[0].globalparams[GLOBALPARAMSCODE__CFRONTIERSTMP].data[0]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__NFRONTIERS: "<<HBM_channel[0].globalparams[GLOBALPARAMSCODE__NFRONTIERS].data[0]<<endl;
	unsigned int lastww_addr = HBM_channel[0].globalparams[GLOBALPARAMSCODE__NFRONTIERS].data[0] + (nfrontiersz_u32 / HBM_CHANNEL_PACK_SIZE / 2);
	cout<<"------------- app:: last address in HBM 0: "<<lastww_addr<<" ("<<lastww_addr * HBM_CHANNEL_PACK_SIZE * 4<<" bytes)"<<endl;
	utilityobj->checkoutofbounds("app::ERROR 2234::", lastww_addr * HBM_CHANNEL_PACK_SIZE * 4, (1 << 28), __NUM_APPLYPARTITIONS, MAX_APPLYPARTITION_VECSIZE, NAp);
	// #endif 
	
	// clear 
	for(unsigned int i=0; i<NUM_PEs; i++){ csr_pack_edges[i].clear(); act_pack_edges[i].clear(); } // clear 
	
	// run acts
	acts_sw * acts = new acts_sw(universalparams);
	acts->run(v_ptr, vptr_actpack, &HBM_center, HBM_channel);
	return;
}

void app::summary(){
	return;
}
















