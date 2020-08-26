#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include "../../include/host_common.h"
#include "../../include/common.h"
#include "dataset.h"
using namespace std;

dataset::dataset(){}
dataset::~dataset(){} 

unsigned int dataset::getdatasetid(){
	unsigned int datasetid = 0;
	#ifdef _LARGEDATASET_1M
	datasetid = 1;
	#endif
	#ifdef _TWITTERDATASET_67M
	datasetid = 2;
	#endif
	#ifdef _LARGEDATASET_67M
	datasetid = 4;
	#endif
	#ifdef _LARGEDATASET_268M
	datasetid = 5;
	#endif
	#ifdef _LARGEDATASET_1B
	datasetid = 6;
	#endif
	#ifdef _LARGEDATASET_4B
	datasetid = 7;
	#endif
	return datasetid;
}
void dataset::loadalldatasets(){
	cout<<"graph::loadalldatasets:: "<<endl;
	
	#ifdef AWS_PLATFORM
	string rootDir = "/oj2zf/";
	#else
	string rootDir = "/localtmp/oj2zf/";
	#endif
	
	datasets[0].graphname = "holes";
	datasets[0].graph_path = rootDir + "datasets/holes/holes.graph";
	datasets[0].vertices_path = rootDir + "datasets/holes/holes.vertices";
	datasets[0].edges_path =rootDir + "datasets/holes/holes.edges";	
	datasets[0].vertices_path_bin = rootDir + "datasets/holes/holes_bin.vertices";
	datasets[0].edges_path_bin = rootDir + "datasets/holes/holes_bin.edges";
	datasets[0].min_vertex = 0;
	datasets[0].max_vertex = 0;
	datasets[0].num_vertices = 0;
	datasets[0].num_edges = 0;
	datasets[0].graphdirectiontype = DIRECTEDGRAPH;
	datasets[0].graphorder = SRC_DST;
	
	datasets[1].graphname = "flickr";
	datasets[1].graph_path = rootDir + "datasets/flickr/flickr.graph";
	datasets[1].vertices_path = rootDir + "datasets/flickr/flickr.vertices";
	datasets[1].edges_path = rootDir + "datasets/flickr/flickr.edges";
	datasets[1].vertices_path_bin = rootDir + "datasets/flickr/flickr_bin.vertices";
	datasets[1].edges_path_bin = rootDir + "datasets/flickr/flickr_bin.edges";
	datasets[1].min_vertex = 0;
	datasets[1].max_vertex = 820878;
	datasets[1].num_vertices = 820879;
	datasets[1].num_edges = 9837214;
	datasets[1].graphdirectiontype = DIRECTEDGRAPH;
	datasets[1].graphorder = DST_SRC;
	
	datasets[2].graphname = "twitter";
	datasets[2].graph_path = rootDir + "datasets/twitter/twitter.graph";
	datasets[2].vertices_path = rootDir + "datasets/twitter/twitter.vertices";
	datasets[2].edges_path = rootDir + "datasets/twitter/twitter.edges";	
	datasets[2].vertices_path_bin = rootDir + "datasets/twitter/twitter_bin.vertices"; 
	datasets[2].edges_path_bin = rootDir + "datasets/twitter/twitter_bin.edges";
	datasets[2].min_vertex = 0;	
	datasets[2].max_vertex = 41652230; // -- extract_vertices says 41652240
	datasets[2].num_vertices = 41652230; // FIXME
	// datasets[2].max_vertex = 41652240; // -- extract_vertices says 41652240
	// datasets[2].num_vertices = 41652240; // FIXME
	datasets[2].num_edges = 1468365184; // 1468365184
	datasets[2].graphdirectiontype = UNDIRECTEDGRAPH;
	datasets[2].graphorder = DST_SRC;
	
	datasets[4].graphname = "kronecker26";
	datasets[4].graph_path = rootDir + "datasets/kronecker26/kronecker26.graph";
	datasets[4].vertices_path = rootDir + "datasets/kronecker26/kronecker26.vertices";
	datasets[4].edges_path = rootDir + "datasets/kronecker26/kronecker26.edges";	
	datasets[4].vertices_path_bin = rootDir + "datasets/kronecker26/kronecker26_bin.vertices";
	datasets[4].edges_path_bin = rootDir + "datasets/kronecker26/kronecker26_bin.edges";
	datasets[4].min_vertex = 0;
	datasets[4].max_vertex = 67108864; 
	datasets[4].num_vertices = 67108864;  // 67108864, 67108816(prev)*
	datasets[4].num_edges = 1073741824; // 1073741824
	datasets[4].graphdirectiontype = DIRECTEDGRAPH;
	datasets[4].graphorder = SRC_DST;
	
	datasets[5].graphname = "kronecker28";
	datasets[5].graph_path = rootDir + "datasets/kronecker28/kronecker28.graph";
	datasets[5].vertices_path = rootDir + "datasets/kronecker28/kronecker28.vertices";
	datasets[5].edges_path = rootDir + "datasets/kronecker28/kronecker28.edges";	
	datasets[5].vertices_path_bin = rootDir + "datasets/kronecker28/kronecker28_bin.vertices";
	datasets[5].edges_path_bin = rootDir + "datasets/kronecker28/kronecker28_bin.edges";
	datasets[5].min_vertex = 0;
	datasets[5].max_vertex = 268435456; // 268435456 // 268431360(prev)
	datasets[5].num_vertices = 268435456; 
	datasets[5].num_edges = 4294967290;
	datasets[5].graphdirectiontype = DIRECTEDGRAPH;
	datasets[5].graphorder = SRC_DST;
	
	datasets[6].graphname = "kronecker30";
	datasets[6].graph_path = rootDir + "datasets/kronecker30/kronecker30.graph";
	datasets[6].vertices_path = rootDir + "datasets/kronecker30/kronecker30.vertices";
	datasets[6].edges_path = rootDir + "datasets/kronecker30/kronecker30.edges";	
	datasets[6].vertices_path_bin = rootDir + "datasets/kronecker30/kronecker30_bin.vertices"; 
	datasets[6].edges_path_bin = rootDir + "datasets/kronecker30/kronecker30_bin.edges"; 
	datasets[6].min_vertex = 0;
	datasets[6].max_vertex = 1073741824; // 1073741824 // 1041451012(prev)
	datasets[6].num_vertices = 1073741824;
	datasets[6].num_edges = 17179869184;
	datasets[6].graphdirectiontype = DIRECTEDGRAPH;
	datasets[6].graphorder = SRC_DST;
	
	datasets[7].graphname = "kronecker32";
	datasets[7].graph_path = rootDir + "datasets/kronecker32/kronecker32.graph";
	datasets[7].vertices_path = rootDir + "datasets/kronecker32/kronecker32.vertices";
	datasets[7].edges_path = rootDir + "datasets/kronecker32/kronecker32.edges";	
	datasets[7].vertices_path_bin = rootDir + "datasets/kronecker32/kronecker32_bin.vertices"; 
	datasets[7].edges_path_bin = rootDir + "datasets/kronecker32/kronecker32_bin.edges"; 
	datasets[7].min_vertex = 0;
	datasets[7].max_vertex = 4294967296; 
	datasets[7].num_vertices = 4294967296;
	datasets[7].num_edges = 68719476736;
	datasets[7].graphdirectiontype = DIRECTEDGRAPH;
	datasets[7].graphorder = SRC_DST;
	
	datasets[10].graphname = "wdc";
	datasets[10].graph_path = rootDir + "datasets/wdc/wdc.graph";
	datasets[10].vertices_path = rootDir + "datasets/wdc/wdc.vertices";
	datasets[10].edges_path = rootDir + "datasets/wdc/wdc.edges";	
	datasets[10].vertices_path_bin = "/projects/DPE/wole/wdc/wdc_bin.vertices"; 
	datasets[10].edges_path_bin = "/projects/DPE/wole/wdc/wdc_bin.edges";
	datasets[10].min_vertex = 0;
	datasets[10].max_vertex = 3315452338;
	datasets[10].num_vertices = 3315452338;
	datasets[10].num_edges = 0;
	datasets[10].graphdirectiontype = DIRECTEDGRAPH;
	datasets[10].graphorder = SRC_DST;
	return;
}










