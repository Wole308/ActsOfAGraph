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
#include <bits/stdc++.h>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../include/common.h"
#include "../../include/host_common.h"
#include "createundirectedgraph.h"
using namespace std;

/** 
This class creates an undirected graph:
Input - Graph in file 
Output - Undirected Graph 
*/

#ifdef AWS_PLATFORM
string datasetRootDir_createundirgraph = "/oj2zf/";
#else
string datasetRootDir_createundirgraph = "/home/oj2zf/";
#endif

createundirectedgraph::createundirectedgraph(unsigned int datasetid){
	algorithm * algorithmobj = new algorithm();
	utilityobj = new utility();
	graphobj = new graph(algorithmobj, datasetid, 1, true, true, true);
	
	unsigned int num_edges = graphobj->getedgessize(0);
	edgedatabuffer_dup = new edge2_type[2 * num_edges];
	edgedatabuffer = new edge2_type[num_edges];
	
	vertexptrbuffer_dup = new edge_t[KVDATA_RANGE];
	inoutdegree_dup = new int[KVDATA_RANGE];
	
	cout<<"createundirectedgraph:: constructor called: creating structures... "<<endl;
	cout<<"createundirectedgraph:: dataset.graph_path: "<<graphobj->getdataset().graph_path<<endl;
	cout<<"createundirectedgraph:: dataset.vertices_path: "<<graphobj->getdataset().vertices_path<<endl;
	cout<<"createundirectedgraph:: dataset.vertices_path_bin: "<<graphobj->getdataset().vertices_path_bin<<endl;
	cout<<"createundirectedgraph:: dataset.edges_path: "<<graphobj->getdataset().edges_path<<endl;
	cout<<"createundirectedgraph:: dataset.edges_path_bin: "<<graphobj->getdataset().edges_path_bin<<endl;
	cout<<"createundirectedgraph:: dataset.graphorder: "<<graphobj->getdataset().graphorder<<endl;
	cout<<"createundirectedgraph::createundirectedgraph: constructor finished"<<endl;
}
createundirectedgraph::~createundirectedgraph() {
	cout<<"~createundirectedgraph:: destructor called... "<<endl;
	cout<<"createundirectedgraph::~createundirectedgraph deleting dynamic memories... "<<endl;
	
	delete [] edgedatabuffer_dup; 
	delete [] edgedatabuffer;
	
	delete [] vertexptrbuffer_dup;
	delete [] vertexptrbuffer;
	
	delete [] inoutdegree_dup;
}

void createundirectedgraph::start(){
	cout<<"createundirectedgraph::start: opening files..."<<endl;
	utilityobj->createdirectory(graphobj->getdatasetdir().c_str());
	graphobj->openfilesforreading(0);
	
	vertex_t srcv = 0;
	vertex_t dstv = 0;
	vertex_t ew = NAp;
	vertex_t local_srcv = 0;
	vertex_t local_dstv = 0;
	unsigned int edgedatabuffer_dup_size = 0;
	
	unsigned int num_edges = graphobj->getedgessize(0);
	
	// populate edges 
	std::ifstream file1_graph(graphobj->getdataset().graph_path);
	if (file1_graph.is_open()) {
		std::string line;
		unsigned int linecount = 0;
		unsigned int alllinecount = 0;
		while (getline(file1_graph, line)) {
			if (line.find("%") == 0){ continue; }
			if(graphobj->getdataset().graphgroup == SNAP){ if (alllinecount == 0){ alllinecount++; continue; }} // first entry for flickr is stats
			
			if (linecount < 128){ cout<<"createundirectedgraph::~start edge (A): ["<<srcv<<","<<dstv<<","<<ew<<"]. alllinecount: "<<alllinecount<<endl; }
			if ((alllinecount % 1000000) == 0){ cout<<"createundirectedgraph::~start edge (A): ["<<srcv<<","<<dstv<<","<<ew<<"]. alllinecount: "<<alllinecount<<endl; }
			
			if(graphobj->getdataset().graphorder == SRC_DST){
				sscanf(line.c_str(), "%i %i", &srcv, &dstv);
			} else if(graphobj->getdataset().graphorder == DST_SRC){
				sscanf(line.c_str(), "%i %i", &dstv, &srcv);
			} else if(graphobj->getdataset().graphorder == DST_SRC_EDGEW){
				sscanf(line.c_str(), "%i %i %i", &dstv, &srcv, &ew);
			} else {}
			
			if(srcv >= KVDATA_RANGE){ srcv = graphobj->getdataset().max_vertex; }
			if(dstv >= KVDATA_RANGE){ dstv = graphobj->getdataset().max_vertex; }
			
			edgedatabuffer[linecount].srcvid = srcv;
			edgedatabuffer[linecount].dstvid = dstv;
			
			linecount += 1;
			alllinecount += 1;
			
			if(linecount > num_edges){ cout<<"createundirectedgraph: linecount("<<linecount<<") > num_edges("<<num_edges<<"). srcv: "<<srcv<<", dstv: "<<dstv<<endl; exit(EXIT_FAILURE); }
		}
		
		cout<<"SUMMARY: alllinecount: "<<alllinecount<<endl;
		cout<<"SUMMARY: linecount: "<<linecount<<endl;
		cout<<"SUMMARY: num_edges: "<<num_edges<<endl;
	}
	file1_graph.close();
	
	for(unsigned int i=0; i<KVDATA_RANGE; i++){
		inoutdegree_dup[i] = 0;
		vertexptrbuffer_dup[i] = 0;
	}
	
	// populate inout degrees of all vertices 
	for(unsigned int i=0; i<num_edges; i++){ // graphobj->get_num_edges()
		if(edgedatabuffer[i].srcvid >= KVDATA_RANGE){ edgedatabuffer[i].srcvid = graphobj->getdataset().max_vertex; }
		if(edgedatabuffer[i].dstvid >= KVDATA_RANGE){ edgedatabuffer[i].dstvid = graphobj->getdataset().max_vertex; }
			
		inoutdegree_dup[edgedatabuffer[i].srcvid] += 1;
		inoutdegree_dup[edgedatabuffer[i].dstvid] += 1;
	}
	
	for(unsigned int k=0; k<10; k++){
		cout<<"createundirectedgraph::start:: inoutdegree_dup["<<k<<"]: "<<inoutdegree_dup[k]<<endl;
	}
	
	// create new vertex pointers from inout degrees
	vertexptrbuffer_dup[0] = 0;
	for(unsigned int k=1; k<KVDATA_RANGE; k++){
		vertexptrbuffer_dup[k] = vertexptrbuffer_dup[k-1] + inoutdegree_dup[k-1];
		if(k<16 || k>KVDATA_RANGE-16){ cout<<"createundirectedgraph::start:: vertexptrbuffer_dup["<<k<<"]: "<<vertexptrbuffer_dup[k]<<endl; }
		if(vertexptrbuffer_dup[k] < vertexptrbuffer_dup[k-1]){ cout<<"creategraphs::writevertexptrstofile:ERROR: non-increasing vertex ptrs: vertexptrbuffer_dup["<<k<<"]: "<<vertexptrbuffer_dup[k]<<", vertexptrbuffer_dup["<<k-1<<"]: "<<vertexptrbuffer_dup[k-1]<<endl; exit(EXIT_FAILURE); }
	}
	cout<<"createundirectedgraph::start:: last: vertexptrbuffer_dup["<<KVDATA_RANGE-1<<"]: "<<vertexptrbuffer_dup[KVDATA_RANGE-1]<<endl;
	
	// checking new vertex pointers created...
	cout<<"createundirectedgraph::start:: checking..."<<endl;
	for(unsigned int k=0; k<KVDATA_RANGE-2; k++){
		if(vertexptrbuffer_dup[k] + inoutdegree_dup[k] > vertexptrbuffer_dup[k+1]){ 
			cout<<"createundirectedgraph::start:: ERROR 2A. vertexptrbuffer_dup["<<k<<"]("<<vertexptrbuffer_dup[k]<<") + inoutdegree_dup["<<k<<"]("<<inoutdegree_dup[k]<<") >= vertexptrbuffer_dup["<<k+1<<"]("<<vertexptrbuffer_dup[k+1]<<")"<<endl; 
			exit(EXIT_FAILURE); 
		}
	}
	cout<<"createundirectedgraph::start:: finished checking."<<endl;
			
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ inoutdegree_dup[i] = 0; }
	
	// re-writing graph with: 1) new vertex pointers
	std::ifstream file2_graph(graphobj->getdataset().graph_path);
	if (file2_graph.is_open()) {
		std::string line;
		unsigned int linecount = 0;
		unsigned int alllinecount = 0;
		while (getline(file2_graph, line)) {
			if (line.find("%") == 0){ continue; }
			if(graphobj->getdataset().graphgroup == SNAP){ if (alllinecount == 0){ alllinecount++; continue; }} // first entry for flickr is stats
			
			if ((alllinecount % 1000000) == 0){ cout<<"createundirectedgraph::~start edge (B): ["<<srcv<<","<<dstv<<","<<ew<<"]. alllinecount: "<<alllinecount<<endl; }
			
			if(graphobj->getdataset().graphorder == SRC_DST){
				sscanf(line.c_str(), "%i %i", &srcv, &dstv);
			} else if(graphobj->getdataset().graphorder == DST_SRC){
				sscanf(line.c_str(), "%i %i", &dstv, &srcv);
			} else if(graphobj->getdataset().graphorder == DST_SRC_EDGEW){
				sscanf(line.c_str(), "%i %i %i", &dstv, &srcv, &ew);
			} else {}
			
			#ifdef _DEBUGMODE_CHECKS
			cout<<"createundirectedgraph: srcv: "<<srcv<<", dstv: "<<dstv<<endl; if(alllinecount >= 100){ break; }
			#endif 
			
			if(srcv >= KVDATA_RANGE){ srcv = graphobj->getdataset().max_vertex; }
			if(dstv >= KVDATA_RANGE){ dstv = graphobj->getdataset().max_vertex; }
			
			local_srcv = srcv;
			local_dstv = dstv;
			
			edge2_type edge1; edge1.srcvid = local_srcv; edge1.dstvid = local_dstv;
			edge2_type edge2; edge2.srcvid = local_dstv; edge2.dstvid = local_srcv;

			edgedatabuffer_dup[vertexptrbuffer_dup[edge1.srcvid] + inoutdegree_dup[edge1.srcvid]] = edge1;
			inoutdegree_dup[edge1.srcvid] += 1;
			
			edgedatabuffer_dup[vertexptrbuffer_dup[edge2.srcvid] + inoutdegree_dup[edge2.srcvid]] = edge2;
			inoutdegree_dup[edge2.srcvid] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(edge1.srcvid < graphobj->get_num_vertices()-1 && vertexptrbuffer_dup[edge1.srcvid] + inoutdegree_dup[edge1.srcvid] > vertexptrbuffer_dup[edge1.srcvid+1]){ 
				cout<<"createundirectedgraph::start:: ERROR 1A. vertexptrbuffer_dup["<<edge1.srcvid<<"]("<<vertexptrbuffer_dup[edge1.srcvid]<<") + inoutdegree_dup["<<edge1.srcvid<<"]("<<inoutdegree_dup[edge1.srcvid]<<") >= vertexptrbuffer_dup["<<edge1.srcvid+1<<"]("<<vertexptrbuffer_dup[edge1.srcvid+1]<<")"<<endl; 
				exit(EXIT_FAILURE); 
			}
			if(edge2.srcvid < graphobj->get_num_vertices()-1 && vertexptrbuffer_dup[edge2.srcvid] + inoutdegree_dup[edge2.srcvid] > vertexptrbuffer_dup[edge2.srcvid+1]){ 
				cout<<"createundirectedgraph::start:: ERROR 2A. vertexptrbuffer_dup["<<edge2.srcvid<<"]("<<vertexptrbuffer_dup[edge2.srcvid]<<") + inoutdegree_dup["<<edge2.srcvid<<"]("<<inoutdegree_dup[edge2.srcvid]<<") >= vertexptrbuffer_dup["<<edge2.srcvid+1<<"]("<<vertexptrbuffer_dup[edge2.srcvid+1]<<")"<<endl; 
				exit(EXIT_FAILURE); 
			}
			#endif 
			
			edgedatabuffer_dup_size += 2;
				
			alllinecount += 1;
		}
		cout<<"createundirectedgraph:: finished processing edges: [valid edges processed: "<<linecount<<"][invalid edges processed: "<<(alllinecount - linecount)<<"][total edges processed: "<<alllinecount<<"]"<<endl;
	}
	file2_graph.close(); 
	
	cout<<"createundirectedgraph:: saving edge data... "<<endl;
	string edgespath = datasetRootDir_createundirgraph + "dataset" + "/" + graphobj->getdataset().graphtopname + "/" + graphobj->getdataset().graphtopname + "_" + std::to_string(1) + "by" +  std::to_string(1) + "/" + graphobj->getdataset().graphname + "_dup" + "_" + std::to_string(0) + "_" + std::to_string(0) + ".edges";
	std::ofstream ofs1; ofs1.open(edgespath.c_str(), std::ofstream::out | std::ofstream::trunc); ofs1.close();	
	nvmeFd_edges_w = fopen(edgespath.c_str(), "w"); 
	if(fwrite(edgedatabuffer_dup, (edgedatabuffer_dup_size * sizeof(edge2_type)), 1, nvmeFd_edges_w) == 0){ cout<<"ERROR:createundirectedgraph: fwrite error 34"<<endl; exit(EXIT_FAILURE); }	

	cout<<"createundirectedgraph:: saving vertex ptrs... "<<endl;
	string vptrspath = datasetRootDir_createundirgraph + "dataset" + "/" + graphobj->getdataset().graphtopname + "/" + graphobj->getdataset().graphtopname + "_" + std::to_string(1) + "by" +  std::to_string(1) + "/" + graphobj->getdataset().graphname + "_dup" + "_" + std::to_string(0) + "_" + std::to_string(0) + ".vertexptrs";
	std::ofstream ofs2; ofs2.open(vptrspath.c_str(), std::ofstream::out | std::ofstream::trunc); ofs2.close();	
	nvmeFd_vertexptrs_w = fopen(vptrspath.c_str(), "w"); 
	if(fwrite(vertexptrbuffer_dup, (KVDATA_RANGE * sizeof(edge_t)), 1, nvmeFd_vertexptrs_w) == 0){ cout<<"ERROR:createundirectedgraph: fwrite error 34"<<endl; exit(EXIT_FAILURE); }	
		
	cout<<"createundirectedgraph:: start finished. closing files... "<<endl;
	graphobj->closefilesforreading();
	cout<<"createundirectedgraph:: finished creating undirected 2D graph from "<<graphobj->getdataset().graph_path<<endl;
	return;
}




