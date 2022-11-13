#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "utility.h"
// #include "app.h"

#include "utility.h"
#include "act_pack.h"
#include "prepare_graph.h"
#include "algorithm.h"
// #include "app_hw.h"

#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <mutex>
#include "algorithm.h"
#include "utility.h"
#include "../include/common.h"

// #ifdef FPGA_IMPL
#include "xcl2.hpp"
#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <CL/cl_ext_xilinx.h>
// #endif 

#include "utility.h"
#include "algorithm.h"
#include "../include/common.h"
using namespace std;
using namespace std;

#define YYYYYYYYYYYYYYYYYYYY

#define NUM_KERNEL 1
// HBM Pseudo-channel(PC) requirements
#define MAX_HBM_PC_COUNT 32
#define PC_NAME(n) n | XCL_MEM_TOPOLOGY
const int pc[MAX_HBM_PC_COUNT] = {
    PC_NAME(0),  PC_NAME(1),  PC_NAME(2),  PC_NAME(3),  PC_NAME(4),  PC_NAME(5),  PC_NAME(6),  PC_NAME(7),
    PC_NAME(8),  PC_NAME(9),  PC_NAME(10), PC_NAME(11), PC_NAME(12), PC_NAME(13), PC_NAME(14), PC_NAME(15),
    PC_NAME(16), PC_NAME(17), PC_NAME(18), PC_NAME(19), PC_NAME(20), PC_NAME(21), PC_NAME(22), PC_NAME(23),
    PC_NAME(24), PC_NAME(25), PC_NAME(26), PC_NAME(27), PC_NAME(28), PC_NAME(29), PC_NAME(30), PC_NAME(31)};

int main(int argc, char** argv){
	cout<<"Hostprocess:: Graph Analytics Started..."<<endl;
	
	std::string binaryFile1;
	universalparams_t universalparams;
	// utility * utilityobj = new utility();
	algorithm * algorithmobj = new algorithm();
	
	#ifdef FPGA_IMPL
	if (argc < 2) { // 2,3
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
	binaryFile1 = argv[5];
	#endif
	
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime_overallexecution = std::chrono::steady_clock::now();
	#endif
	
	// app * appobj = new app();	///////////////
	// void run(std::string algorithm, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1);
	// appobj->run(argv[1], argv[2], stoi(argv[3]), stoi(argv[4]), argv[5], stoi(argv[6]), argv[7]); /////////////
	// appobj->run("nap", "sssp", 16, 1, "/home/oj2zf/Documents/dataset/soc-LiveJournal1.mtx", 1, "NAp");
	// appobj->run("nap", "sssp", 2, 1, "/home/oj2zf/Documents/dataset/rmat_32m_256m.mtx", 1, "NAp");
	// appobj->run("nap", "sssp", 12, 1, "/home/oj2zf/Documents/dataset/com-Orkut.mtx", 1, "NAp");
	// appobj->run("nap", "pr", 8, 1, "/home/oj2zf/dataset/uk-2002.mtx", 1, "NAp");
	
	std::string setup = argv[1];
	std::string algo = argv[2]; 
	unsigned int numiterations = stoi(argv[3]); 
	unsigned int rootvid = stoi(argv[4]); 
	string graph_path = argv[5]; 
	int graphisundirected = stoi(argv[6]); 
	std::string _binaryFile1 = argv[7];
	
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;

	// std::string binaryFile[2]; binaryFile[0] = _binaryFile1;
	
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
	
	// universalparams_t universalparams = get_universalparams(algo, numiterations, rootvid, num_vertices, num_edges, graphisundirected_bool);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// universalparams_t universalparams;
	// utility * utilityobj = new utilityobj();;
	// algorithm * algorithmobj = new algorithm();;
	// algorithm * algorithmobj = new algorithm();
	
	universalparams.ALGORITHM = algorithmobj->get_algorithm_id(algo);
	universalparams.NUM_ITERATIONS = numiterations; if(universalparams.ALGORITHM != BFS && universalparams.ALGORITHM != SSSP){ universalparams.NUM_ITERATIONS = 1; }
	universalparams.ROOTVID = rootvid;
	
	universalparams.NUM_VERTICES = num_vertices; 
	universalparams.NUM_EDGES = num_edges; 

	universalparams.NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	universalparams.NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	
	universalparams.NUM_PARTITIONS = 16;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	// act_pack * pack = new act_pack(universalparams);
	// pack->pack(vertexptrbuffer, edgedatabuffer, act_pack_edges, vptr_actpack); // CRITICAL REMOVEME.
	// edge3_vec_dt edge_vec3; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ edge_vec3.data[v].srcvid = 7; edge_vec3.data[v].dstvid = 7; edge_vec3.data[v].valid = 1; } // CRITICAL REMOVEME.
	// for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<10240; t++){ act_pack_edges[i].push_back(edge_vec3); }} // CRITICAL REMOVEME.
	
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
	unsigned int max_degree = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ unsigned int index = 0, ind = 0; for(unsigned int vid=0; vid<universalparams.NUM_VERTICES; vid++){ if(vid % NUM_PEs == i){ 
		if(max_degree < degrees[i][vid / NUM_PEs]){ max_degree = degrees[i][vid / NUM_PEs]; }
		v_ptr[i][vid / NUM_PEs] = index; index += degrees[i][vid / NUM_PEs]; ind += 1; }}}			
	for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"csr-pack:: PE: 21: act_pack_edges["<<i<<"].size(): "<<csr_pack_edges[i].size()<<endl; }
	cout<<"csr-pack:: max_degree______________________________________________________: "<<max_degree<<endl;
	unsigned int max = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(max < act_pack_edges[i].size()){ max = act_pack_edges[i].size(); }} 
	for(unsigned int i=0; i<NUM_PEs; i++){ act_pack_edges_arr[i] = new edge3_vec_dt[max]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<max; t++){ act_pack_edges_arr[i][t] =  act_pack_edges[i][t]; }}
	
	HBM_channel_t * HBM_center = new HBM_channel_t[HBM_CHANNEL_SIZE];
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
	// for(unsigned int i=0; i<NUM_PEs; i++){ globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS] = 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){ globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS] = 512; } // CRITICAL FIXME_HARDWARE
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
				// unsigned int isvalid = act_pack_edges[i][t].data[v].valid;
				// if(isvalid==1){
					// HBM_channel[i][base_offset + t].data[2 * v] = act_pack_edges[i][t].data[v].srcvid;
					// HBM_channel[i][base_offset + t].data[2 * v + 1] = act_pack_edges[i][t].data[v].dstvid; 
				// } else {
					// HBM_channel[i][base_offset + t].data[2 * v] = INVALIDDATA;
					// HBM_channel[i][base_offset + t].data[2 * v + 1] = act_pack_edges[i][t].data[v].dstvid; 
				// }	
				
				HBM_channel[i][base_offset + t].data[2 * v] = 0;
				HBM_channel[i][base_offset + t].data[2 * v + 1] = 0; 
					
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
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS].data[0] = universalparams.NUM_ITERATIONS; // FIXME_HARDWARE.
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEFRONTIERSFORCONTROLSWITCH].data[0] = 128;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__MAXDEGREE].data[0] = max_degree;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__ALGORITHM].data[0] = universalparams.ALGORITHM;

		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___RESETBUFFERSATSTART].data[0] = 1;
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGES].data[0] = 0;
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___READ_FRONTIER_PROPERTIES].data[0] = 1;
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___VCPROCESSEDGES].data[0] = 0; 
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___ECPROCESSEDGES].data[0] = 1; 
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___SAVEVCUPDATES].data[0] = 1; // FIXME? CAUSE OF HANGING?
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___COLLECTACTIVEDSTVIDS].data[0] = 1;
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___APPLYUPDATESMODULE].data[0] = 0; 
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___READ_DEST_PROPERTIES].data[0] = 0;
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___APPLYUPDATES].data[0] = 0; 
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES].data[0] = 0; 
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___SAVE_DEST_PROPERTIES].data[0] = 0; 
		// HBM_channel[i][GLOBALPARAMSCODE___ENABLE___EXCHANGEFRONTIERINFOS].data[0] = 0; 
		
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___RESETBUFFERSATSTART].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGES].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___READ_FRONTIER_PROPERTIES].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___VCPROCESSEDGES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___ECPROCESSEDGES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___SAVEVCUPDATES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___COLLECTACTIVEDSTVIDS].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___APPLYUPDATESMODULE].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___READ_DEST_PROPERTIES].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___APPLYUPDATES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___SAVE_DEST_PROPERTIES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___EXCHANGEFRONTIERINFOS].data[0] = 1; 
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
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__END: "<<lastww_addr<<" (of "<< ((1 << 28) / 4 / HBM_AXI_PACK_SIZE) <<" wide-words) ("<<lastww_addr * HBM_CHANNEL_PACK_SIZE * 4<<" bytes)"<<endl;
	utilityobj->checkoutofbounds("app::ERROR 2234::", lastww_addr, ((1 << 28) / 4 / HBM_AXI_PACK_SIZE), __NUM_APPLYPARTITIONS, MAX_APPLYPARTITION_VECSIZE, NAp);
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__PARAM__NUM_VERTICES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__PARAM__NUM_EDGES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES]<<endl;
	// exit(EXIT_SUCCESS);
	
	// clear 
	for(unsigned int i=0; i<NUM_PEs; i++){ csr_pack_edges[i].clear(); act_pack_edges[i].clear(); } // clear 
	
	// assign root vid 
	keyvalue_t root; root.key = rootvid; root.value = 0; keyvalue_t invalid; invalid.key = INVALIDDATA; invalid.value = INVALIDDATA; 
	for(unsigned int t=0; t<16; t++){ 
		for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){ 
			HBM_center[t].data[v] = INVALIDDATA;
		} 
	}
	unsigned int index = 2*root.key;
	HBM_center[index / HBM_CHANNEL_PACK_SIZE].data[index % HBM_CHANNEL_PACK_SIZE] = root.key; 
	HBM_center[(index + 1) / HBM_CHANNEL_PACK_SIZE].data[(index + 1) % HBM_CHANNEL_PACK_SIZE] = root.value; 

	// allocate AXI HBM memory
	#ifdef ___USE_AXI_CHANNEL___
	HBM_channelAXISW_t * HBM_axichannel[2][NUM_PEs]; 
	cout<<"app: allocating HBM memory..."<<endl;
	for(unsigned int n=0; n<2; n++){
		for(unsigned int i=0; i<NUM_PEs; i++){ HBM_axichannel[n][i] = new HBM_channelAXISW_t[HBM_CHANNEL_SIZE]; }
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
	
	HBM_channelAXISW_t * HBM_axicenter[2]; 
	for(unsigned int n=0; n<2; n++){
		HBM_axicenter[n] = new HBM_channelAXISW_t[HBM_CHANNEL_SIZE]; 
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axicenter[n][t].data[v] = 0; }}
	}
	cout<<"app: copying to axi-friendly channels..."<<endl;
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			HBM_axicenter[0][t].data[v] = HBM_center[t].data[v];
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			HBM_axicenter[1][t].data[v] = HBM_center[t].data[HBM_AXI_PACK_SIZE + v];
		}
	}
	#endif 
	
	// run acts
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef YYYYYYYYYYYYYYYYYYYY
	unsigned int num_axi_interfaces = (2 * VALID_NUMPEs) + 2;
	// unsigned int num_axi_vertex_interfaces = 2 * VALID_NUMPEs;
	unsigned int total_num_axi_interfaces = num_axi_interfaces; // (2 * NUM_PEs) + 2;
	// unsigned int total_num_axi_vertex_interfaces = 2 * NUM_PEs;
	unsigned int dataSize = 64 * 1024 * 1024;
	
	cout<<"--------------------------- VALID_NUMPEs: "<<VALID_NUMPEs<<" ----------------------------------"<<endl;

    std::string binaryFile = _binaryFile1; // binaryFile__[0]; 
    cl_int err;
    cl::CommandQueue q;
    std::string krnl_name = "top_function";
    std::vector<cl::Kernel> krnls(NUM_KERNEL);
    cl::Context context;
	std::vector<cl::Event> kernel_events(48);
	std::vector<cl::Event> write_events(48);
	std::vector<cl::Event> read_events(48);
	
	std::vector<int, aligned_allocator<int> > source_in1[32]; for(unsigned int i=0; i<total_num_axi_interfaces; i++){ source_in1[i] = std::vector<int, aligned_allocator<int> >(HBM_CHANNEL_INTSIZE); }				
	unsigned int bank_index = 0;
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ 
		cout<<"filling host buffer "<<bank_index<<"..."<<endl;
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			if(t<64 && false){ cout<<t<<": "; for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ cout<<HBM_axichannel[0][i][t].data[v]<<", "; } cout<<endl; }
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				source_in1[bank_index][t*HBM_AXI_PACK_SIZE + v] = 7;//HBM_axichannel[0][i][t].data[v]; 
				if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 23: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
			}
		}
		bank_index += 1;
		cout<<"filling host buffer "<<bank_index<<"..."<<endl;
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				source_in1[bank_index][t*HBM_AXI_PACK_SIZE + v] = 0;//HBM_axichannel[1][i][t].data[v];
				if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 24: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
			}
		}
		bank_index += 1;
	}
	cout<<"filling host buffer "<<bank_index<<"..."<<endl;
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			source_in1[bank_index][t*HBM_AXI_PACK_SIZE + v] = 0;//HBM_axicenter[0][t].data[v];
			if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 25: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
		}
	} 
	bank_index += 1;
	cout<<"filling host buffer "<<bank_index<<"..."<<endl;
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			source_in1[bank_index][t*HBM_AXI_PACK_SIZE + v] = 0;//HBM_axicenter[1][t].data[v];
			if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 26: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
		}
	}
	bank_index += 1;
    // for (int i = 0; i < total_num_axi_interfaces; i++) { std::generate(source_in1[i].begin(), source_in1[i].end(), std::rand); }

    // OPENCL HOST CODE AREA START
    // The get_xil_devices will return vector of Xilinx Devices
    auto devices = xcl::get_xil_devices();

    // read_binary_file() command will find the OpenCL binary file created using the V++ compiler load into OpenCL Binary and return pointer to file buffer.
    auto fileBuf = xcl::read_binary_file(binaryFile);

    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        OCL_CHECK(err, q = cl::CommandQueue(context, device,
                                            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err));

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            // Creating Kernel object using Compute unit names
            for (int i = 0; i < NUM_KERNEL; i++) {
                std::string cu_id = std::to_string(i + 1);
                std::string krnl_name_full = krnl_name + ":{" + "top_function_" + cu_id + "}";
                printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i + 1);
                OCL_CHECK(err, krnls[0] = cl::Kernel(program, krnl_name_full.c_str(), &err));
            }
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    std::vector<cl_mem_ext_ptr_t> inBufExt1(total_num_axi_interfaces);
    std::vector<cl::Buffer> buffer_input1(total_num_axi_interfaces);

    // For Allocating Buffer to specific Global Memory PC, user has to use cl_mem_ext_ptr_t and provide the PCs
    for (int i = 0; i < total_num_axi_interfaces; i++) {
		cout<<"attaching inBufExt1["<<i<<"] @ source_in1["<<i<<"] to pc["<<i<<"]... "<<endl;
		inBufExt1[i].obj = source_in1[i].data();
		// inBufExt1[i].obj = source_master.data();// FIXME_HARDWARE
		inBufExt1[i].param = 0;
		inBufExt1[i].flags = pc[i];
    }

    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    // Creating Buffers
    for (int i = 0; i < total_num_axi_interfaces; i++) {
		cout<<"+++ app_hw: creating buffer_input1["<<i<<"]... "<<endl;
		OCL_CHECK(err,
			buffer_input1[i] = cl::Buffer(context, CL_MEM_READ_ONLY  | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
												HBM_CHANNEL_BYTESIZE, &inBufExt1[i], &err));
	}

    // Copy input data to Device Global Memory
    // for (int i = 0; i < total_num_axi_interfaces; i++) {
		// OCL_CHECK(err,
			// err = q.enqueueMigrateMemObjects({buffer_input1[i]}, 0 /* 0 means from host*/));
	// }
	cout<<"app_hw:: Migrating input data from host memory to device global memory..."<<endl;
	#if VALID_NUMPEs==1
	cout<<"app_hw:: Migrating input data from host memory to device global memory (B)..."<<endl;
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({												
													buffer_input1[0], buffer_input1[1],
													buffer_input1[2], buffer_input1[3]}, 
													0, NULL, &write_events[0]));
	#endif 
	#if VALID_NUMPEs==2
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5]}, 
													0, NULL, &write_events[0]));
	#endif 
	#if VALID_NUMPEs==4
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5], 
													buffer_input1[6], buffer_input1[7], 
													buffer_input1[8], buffer_input1[9]}, 
													0, NULL, &write_events[0]));
	#endif 
	#if VALID_NUMPEs==6
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5], 
													buffer_input1[6], buffer_input1[7], 
													buffer_input1[8], buffer_input1[9], 
													buffer_input1[10], buffer_input1[11], 
													buffer_input1[12], buffer_input1[13]}, 
													0, NULL, &write_events[0]));
	#endif 
	OCL_CHECK(err, err = write_events[0].wait());
    q.finish();

    double kernel_time_in_sec = 0, result = 0;

    std::chrono::duration<double> kernel_time(0);

    auto kernel_start = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();	
    // Setting the k_vadd Arguments
	for (int i = 0; i < num_axi_interfaces; i++) {
		cout<<"setting argument krnls[0].setArg("<<i<<", buffer_input1["<<i<<"])"<<endl;
        OCL_CHECK(err, err = krnls[0].setArg(i, buffer_input1[i]));
    }
	q.finish();
	
	// Invoking the kernel
	cout<<"Invoking the kernel..."<<endl;
    // OCL_CHECK(err, err = q.enqueueTask(krnls[0]));
	OCL_CHECK(err, err = q.enqueueTask(krnls[0], NULL, &kernel_events[0])); 
	OCL_CHECK(err, err = kernel_events[0].wait());
    q.finish();
	double end_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time).count()) / 1000;			
	std::cout <<">>> kernel time elapsed for all iterations : "<<end_time<<" ms, "<<(end_time * 1000)<<" microsecs, "<<std::endl;
    auto kernel_end = std::chrono::high_resolution_clock::now();
	// exit(EXIT_SUCCESS); ///////////////////

    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);

    kernel_time_in_sec = kernel_time.count();
    kernel_time_in_sec /= NUM_KERNEL;
	std::cout << "THEORETICAL THROUGHPUT = " << (universalparams.NUM_EDGES / kernel_time_in_sec) / 1000000 << " MTEPS" << std::endl; // kernel_time_in_sec
	std::cout << "ACTUAL THROUGHPUT = " << ((universalparams.NUM_EDGES / kernel_time_in_sec) / 1000000) * (16 / EDGE_PACK_SIZE) << " MTEPS" << std::endl; 
	
    // Copy Result from Device Global Memory to Host Local Memory
	cout<<"app_hw:: Migrating input data from device global memory to host memory..."<<endl;
    // for (int i = 0; i < NUM_KERNEL; i++) {
        // OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input1[i]},
                                                        // CL_MIGRATE_MEM_OBJECT_HOST));
    // }
	#if VALID_NUMPEs==1
	cout<<"app_hw:: Migrating input data from device global memory to host memory (B)..."<<endl;
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({												
													buffer_input1[0], buffer_input1[1],
													buffer_input1[2], buffer_input1[3]}, 
													CL_MIGRATE_MEM_OBJECT_HOST));
	#endif 
	#if VALID_NUMPEs==2
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5]}, 
													CL_MIGRATE_MEM_OBJECT_HOST));
	#endif 
	#if VALID_NUMPEs==4
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5], 
													buffer_input1[6], buffer_input1[7], 
													buffer_input1[8], buffer_input1[9]}, 
													CL_MIGRATE_MEM_OBJECT_HOST));
	#endif 
	#if VALID_NUMPEs==6
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5], 
													buffer_input1[6], buffer_input1[7], 
													buffer_input1[8], buffer_input1[9], 
													buffer_input1[10], buffer_input1[11], 
													buffer_input1[12], buffer_input1[13]}, 
													CL_MIGRATE_MEM_OBJECT_HOST));
	#endif
    q.finish();
	#endif 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef ZZZZZZZZZZZZZZZZZZZZ
    unsigned int dataSize = 64 * 1024 * 1024; // taking maximum possible data size value for an HBM bank
    unsigned int num_times = 1024;            // num_times specify, number of times a kernel
                                              // will execute the same operation. This is
                                              // needed
    // to keep the kernel busy to test the actual bandwidth of all banks running
    // concurrently.

    // reducing the test data capacity to run faster in emulation mode
    if (xcl::is_emulation()) {
        dataSize = 1024;
        num_times = 64;
    }

    std::string binaryFile = argv[1];
    cl_int err;
    cl::CommandQueue q;
    std::string krnl_name = "krnl_vaddmul";
    std::vector<cl::Kernel> krnls(NUM_KERNEL);
    cl::Context context;
    std::vector<int, aligned_allocator<int> > source_in1(dataSize);
    std::vector<int, aligned_allocator<int> > source_in2(dataSize);
    std::vector<int, aligned_allocator<int> > source_sw_add_results(dataSize);
    std::vector<int, aligned_allocator<int> > source_sw_mul_results(dataSize);

    std::vector<int, aligned_allocator<int> > source_hw_add_results[NUM_KERNEL];
    std::vector<int, aligned_allocator<int> > source_hw_mul_results[NUM_KERNEL];

    for (int i = 0; i < NUM_KERNEL; i++) {
        source_hw_add_results[i].resize(dataSize);
        source_hw_mul_results[i].resize(dataSize);
    }

    // Create the test data
    std::generate(source_in1.begin(), source_in1.end(), std::rand);
    std::generate(source_in2.begin(), source_in2.end(), std::rand);
    for (size_t i = 0; i < dataSize; i++) {
        source_sw_add_results[i] = source_in1[i] + source_in2[i];
        source_sw_mul_results[i] = source_in1[i] * source_in2[i];
    }

    // Initializing output vectors to zero
    for (size_t i = 0; i < NUM_KERNEL; i++) {
        std::fill(source_hw_add_results[i].begin(), source_hw_add_results[i].end(), 0);
        std::fill(source_hw_mul_results[i].begin(), source_hw_mul_results[i].end(), 0);
    }

    // OPENCL HOST CODE AREA START
    // The get_xil_devices will return vector of Xilinx Devices
    auto devices = xcl::get_xil_devices();

    // read_binary_file() command will find the OpenCL binary file created using
    // the
    // V++ compiler load into OpenCL Binary and return pointer to file buffer.
    auto fileBuf = xcl::read_binary_file(binaryFile);

    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        OCL_CHECK(err, q = cl::CommandQueue(context, device,
                                            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err));

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            // Creating Kernel object using Compute unit names

            for (int i = 0; i < NUM_KERNEL; i++) {
                std::string cu_id = std::to_string(i + 1);
                std::string krnl_name_full = krnl_name + ":{" + "top_function_" + cu_id + "}";

                printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i + 1);

                // Here Kernel object is created by specifying kernel name along with
                // compute unit.
                // For such case, this kernel object can only access the specific
                // Compute unit

                OCL_CHECK(err, krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
            }
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    std::vector<cl_mem_ext_ptr_t> inBufExt1(NUM_KERNEL);
    std::vector<cl_mem_ext_ptr_t> inBufExt2(NUM_KERNEL);
    std::vector<cl_mem_ext_ptr_t> outAddBufExt(NUM_KERNEL);
    std::vector<cl_mem_ext_ptr_t> outMulBufExt(NUM_KERNEL);

    std::vector<cl::Buffer> buffer_input1(NUM_KERNEL);
    std::vector<cl::Buffer> buffer_input2(NUM_KERNEL);
    std::vector<cl::Buffer> buffer_output_add(NUM_KERNEL);
    std::vector<cl::Buffer> buffer_output_mul(NUM_KERNEL);

    // For Allocating Buffer to specific Global Memory PC, user has to use
    // cl_mem_ext_ptr_t
    // and provide the PCs
    for (int i = 0; i < NUM_KERNEL; i++) {
        inBufExt1[i].obj = source_in1.data();
        inBufExt1[i].param = 0;
        inBufExt1[i].flags = pc[i * 4];

        inBufExt2[i].obj = source_in2.data();
        inBufExt2[i].param = 0;
        inBufExt2[i].flags = pc[(i * 4) + 1];

        outAddBufExt[i].obj = source_hw_add_results[i].data();
        outAddBufExt[i].param = 0;
        outAddBufExt[i].flags = pc[(i * 4) + 2];

        outMulBufExt[i].obj = source_hw_mul_results[i].data();
        outMulBufExt[i].param = 0;
        outMulBufExt[i].flags = pc[(i * 4) + 3];
    }

    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    // Creating Buffers
    for (int i = 0; i < NUM_KERNEL; i++) {
        OCL_CHECK(err,
                  buffer_input1[i] = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
                                                sizeof(uint32_t) * dataSize, &inBufExt1[i], &err));
        OCL_CHECK(err,
                  buffer_input2[i] = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
                                                sizeof(uint32_t) * dataSize, &inBufExt2[i], &err));
        OCL_CHECK(err, buffer_output_add[i] =
                           cl::Buffer(context, CL_MEM_WRITE_ONLY | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
                                      sizeof(uint32_t) * dataSize, &outAddBufExt[i], &err));
        OCL_CHECK(err, buffer_output_mul[i] =
                           cl::Buffer(context, CL_MEM_WRITE_ONLY | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
                                      sizeof(uint32_t) * dataSize, &outMulBufExt[i], &err));
    }

    // Copy input data to Device Global Memory
    for (int i = 0; i < NUM_KERNEL; i++) {
        OCL_CHECK(err,
                  err = q.enqueueMigrateMemObjects({buffer_input1[i], buffer_input2[i]}, 0 /* 0 means from host*/));
    }
    q.finish();

    double kernel_time_in_sec = 0, result = 0;

    std::chrono::duration<double> kernel_time(0);

    auto kernel_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_KERNEL; i++) {
        // Setting the k_vadd Arguments
        OCL_CHECK(err, err = krnls[i].setArg(0, buffer_input1[i]));
        OCL_CHECK(err, err = krnls[i].setArg(1, buffer_input2[i]));
        OCL_CHECK(err, err = krnls[i].setArg(2, buffer_output_add[i]));
        OCL_CHECK(err, err = krnls[i].setArg(3, buffer_output_mul[i]));
        OCL_CHECK(err, err = krnls[i].setArg(4, dataSize));
        OCL_CHECK(err, err = krnls[i].setArg(5, num_times));

        // Invoking the kernel
        OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
    }
    q.finish();
    auto kernel_end = std::chrono::high_resolution_clock::now();

    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);

    kernel_time_in_sec = kernel_time.count();
    kernel_time_in_sec /= NUM_KERNEL;

    // Copy Result from Device Global Memory to Host Local Memory
    for (int i = 0; i < NUM_KERNEL; i++) {
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output_add[i], buffer_output_mul[i]},
                                                        CL_MIGRATE_MEM_OBJECT_HOST));
    }
    q.finish();
	#endif 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	#ifdef _DEBUGMODE_TIMERS3
	std::cout << endl << "TEST FINISHED" << std::endl; 
	utilityobj->stopTIME("HOSTPROCESS:: TIMING SUMMARY: TOTAL TIME ELAPSED: ", begintime_overallexecution, NAp);
	#endif
	return EXIT_SUCCESS;
}

