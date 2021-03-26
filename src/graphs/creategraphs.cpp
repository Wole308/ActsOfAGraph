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
#include "mysort.h"
#include "creategraphs.h"
using namespace std;
#define YES

unsigned int globalerrorfound1;

creategraphs::creategraphs(unsigned int datasetid){
	algorithm * algorithmobj = new algorithm();
	utilityobj = new utility();
	graphobj = new graph(algorithmobj, datasetid, 1, true, false, false);
	mysortobj = new mysort(graphobj);
	
	cout<<"creategraphs:: constructor called: creating structures... "<<endl;
	cout<<"creategraphs:: dataset.graph_path: "<<graphobj->getdataset().graph_path<<endl;
	cout<<"creategraphs:: dataset.vertices_path: "<<graphobj->getdataset().vertices_path<<endl;
	cout<<"creategraphs:: dataset.vertices_path_bin: "<<graphobj->getdataset().vertices_path_bin<<endl;
	cout<<"creategraphs:: dataset.edges_path: "<<graphobj->getdataset().edges_path<<endl;
	cout<<"creategraphs:: dataset.edges_path_bin: "<<graphobj->getdataset().edges_path_bin<<endl;
	cout<<"creategraphs:: dataset.graphorder: "<<graphobj->getdataset().graphorder<<endl;
	cout<<endl;
	
	createdatastructures();
	cout<<"creategraphs::creategraphs: constructor finished"<<endl;
}
creategraphs::~creategraphs() {
	cout<<"~creategraphs:: destructor called... "<<endl;
	cout<<"creategraphs::~creategraphs deleting dynamic memories... "<<endl;
	
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ 
		delete [] lvertexoutdegrees[j]; 
		delete [] vertexptrs[j];
	}
	
	clearedges(edgesbuffer);
}

void creategraphs::run(){
	cout<<"creategraphs::run started."<<endl;

	resetdatastructures(0);
	cout<<endl<< TIMINGRESULTSCOLOR << "creategraphs::start: creating graph for group 0..." << RESET <<endl;
	start();
	return;
}

void creategraphs::createdatastructures(){
	cout<<"~creategraphs::createdatastructures:: creating structures..."<<endl;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		lvertexoutdegrees[j] = new edge_t[KVDATA_RANGE];
		lvertexindegrees[j] = new edge_t[KVDATA_RANGE];
		vertexptrs[j] = new edge_t[KVDATA_RANGE];
	}
	firstedge[0].srcvid = INVALIDDATA; 
	firstedge[0].dstvid = INVALIDDATA; 
} 
void creategraphs::resetdatastructures(unsigned int _groupid){
	cout<<"~creategraphs::resetting structures..."<<endl;
	for(unsigned int i=0; i<graphobj->getnumedgebanks(); i++){
		for(unsigned int k=0; k<KVDATA_RANGE; k++){ 
			lvertexoutdegrees[i][k] = 0; 
			lvertexindegrees[i][k] = 0; 
			vertexptrs[i][k] = 0; 
		}
		numedges[i] = 0;
		totalnumedgeswritten[i] = 0;
	}
	firstedge[0].srcvid = INVALIDDATA; 
	firstedge[0].dstvid = INVALIDDATA; 
	groupid = _groupid;
} 

void creategraphs::start(){
	cout<<"creategraphs::start: opening files..."<<endl;
	utilityobj->createdirectory(graphobj->getdatasetdir().c_str());
	graphobj->openfilesforwriting(0);
	graphobj->openfilesforreading(0);
	std::ifstream file_graph(graphobj->getdataset().graph_path);

	vertex_t srcv = 0;
	vertex_t dstv = 0;
	vertex_t ew = 1;
	vertex_t local_srcv = 0;
	vertex_t local_dstv = 0;
	edge_t linecount = 0;
	edge_t alllinecount = 0;
	vertex_t prevsrcv = 0;
	
	globalerrorfound1 = 0;
	
	// create graph
	if (file_graph.is_open()) {
		std::string line;
		while (getline(file_graph, line)) {
			if (line.find("%") == 0){ continue; }
			if (alllinecount == 0){ alllinecount++; continue; } // first entry for flickr is stats
			if ((alllinecount % 1000000) == 0){ cout<<"creategraphs::start edge: ["<<srcv<<","<<dstv<<","<<ew<<"]. alllinecount: "<<alllinecount<<endl; }
			
			if(graphobj->getdataset().graphorder == SRC_DST){
				sscanf(line.c_str(), "%i %i", &srcv, &dstv);
			} else if(graphobj->getdataset().graphorder == DST_SRC){
				sscanf(line.c_str(), "%i %i", &dstv, &srcv);
			} else if(graphobj->getdataset().graphorder == DST_SRC_EDGEW){
				sscanf(line.c_str(), "%i %i %i", &dstv, &srcv, &ew);
			} else {}
			
			#ifdef _DEBUGMODE_CHECKS
			cout<<"creategraphs: srcv: "<<srcv<<", dstv: "<<dstv<<endl; if(alllinecount >= 100){ break; }
			#endif 
			#ifdef ENABLE_PERFECTACCURACY // graphobj->getdataset().num_vertices
			if(srcv >= KVDATA_RANGE){ cout<<"creategraphs::start:: source vertex found greater than number of vertices specified in dataset. srcv: "<<srcv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<endl; exit(EXIT_FAILURE); }
			if(dstv >= KVDATA_RANGE){ cout<<"creategraphs::start:: destination vertex found greater than number of vertices specified in dataset. dstv: "<<dstv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<endl; exit(EXIT_FAILURE); }
			#else 
			if(srcv >= KVDATA_RANGE){ srcv = graphobj->getdataset().max_vertex; }
			if(dstv >= KVDATA_RANGE){ dstv = graphobj->getdataset().max_vertex; }
			#endif
		
			local_srcv = srcv;
			local_dstv = dstv;
			
			unsigned int col = getbank(local_dstv);
			if(col > 0){ continue; } // FIXME.
			
			#ifdef _DEBUGMODE_CHECKS
			if (linecount < 16){ cout<<"creategraphs:: gedge: ["<<local_srcv<<","<<local_dstv<<"]"<<endl; }
			#endif 
			
			edge2_type edge;
			edge2_type edge2;
			#ifdef GRAPHISUNDIRECTED
			edge2_type edge_dup;
			edge2_type edge2_dup;
			#endif 
			
			edge.srcvid = local_srcv;
			edge.dstvid = local_dstv;
			#ifdef GRAPHISUNDIRECTED
			edge_dup.srcvid = local_dstv;
			edge_dup.dstvid = local_srcv;
			#endif 
			
			edge2.dstvid = local_dstv;
			#ifdef GRAPHISUNDIRECTED
			edge2_dup.dstvid = local_srcv;
			#endif 
			
			if(firstedge[col].srcvid == INVALIDDATA){ firstedge[col] = edge; }
			
			edgesbuffer[col].push_back(edge); 
			// edges2buffer[col].push_back(edge2); 
			#ifdef GRAPHISUNDIRECTED
			edgesbuffer[col].push_back(edge_dup); 
			// edges2buffer[col].push_back(edge2_dup); 
			#endif 
			
			lvertexoutdegrees[col][local_srcv] += 1;
			lvertexindegrees[col][local_dstv] += 1;
			#ifdef GRAPHISUNDIRECTED
			lvertexoutdegrees[col][local_dstv] += 1;
			lvertexindegrees[col][local_srcv] += 1;
			#endif 
			
			numedges[col] += 1;

			if((linecount++ % CREATENDGRAPH_BATCHSIZE) == 0){
				#ifdef _DEBUGMODE_HOSTPRINTS
				if(linecount < 2 * CREATENDGRAPH_BATCHSIZE){
				utilityobj->printedges("creategraphs::start", (edge2_type *)(&edgesbuffer[0][0]), 16); 
				utilityobj->printedges("creategraphs::start", (edge2_type *)(&edgesbuffer[1][0]), 16); }
				#endif 
				
				writeedgestofile(edgesbuffer); 
				clearedges(edgesbuffer); 

				// writeedgestofile(edges2buffer); 
				// clearedges(edges2buffer);
			}
				
			alllinecount += 1;
										// if(linecount > 1000000){ break; } // CRITICAL REMOVEME..................................................
		}
		cout<<"creategraphs:: finished processing edges: [valid edges processed: "<<linecount<<"][invalid edges processed: "<<(alllinecount - linecount)<<"][total edges processed: "<<alllinecount<<"][groupid: "<<groupid<<"][YDIMENSIONTHRESHOLD: "<<YDIMENSIONTHRESHOLD<<"]"<<endl;
	}
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printedges("creategraphs::start", (edge2_type *)(&edgesbuffer[0][0]), 16); 
	utilityobj->printedges("creategraphs::start", (edge2_type *)(&edgesbuffer[1][0]), 16);
	#endif
	
	writeedgestofile(edgesbuffer); 
	clearedges(edgesbuffer); 

	writevertexptrstofile();

	cout<<"creategraphs:: start finished. closing files... "<<endl;
	file_graph.close(); 
	graphobj->closefilesforwriting();
	graphobj->closefilesforreading();
	cout<<"creategraphs:: finished creating 2D graph from "<<graphobj->getdataset().graph_path<<endl;
	summary();
	return;
}

void creategraphs::summary(){
	cout<<endl;
	unsigned int totalnumedges = 0;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		cout<<">>> creategraphs::summary:: numedges["<<j<<"]: "<<numedges[j]<<endl;
		totalnumedges += numedges[j];
	}
	cout<<">>> creategraphs::summary:: totalnumedges: "<<totalnumedges<<endl;
	cout<<endl;
	return;
}

void creategraphs::writeedgestofile(std::vector<edge2_type> (&edgesbuffer)[MAXNUMEDGEBANKS]){
	cout<<"creategraphs::writeedgestofile<edge2_type>  started."<<endl;

	edge_t totalnumedges = 0;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		totalnumedgeswritten[j] += edgesbuffer[j].size();
		totalnumedges += edgesbuffer[j].size();
		if(edgesbuffer[j].size() > 0){ if(fwrite(edgesbuffer[j].data(), (edgesbuffer[j].size() * sizeof(edge2_type)), 1, graphobj->getnvmeFd_edges_w()[j]) == 0){ cout<<"ERROR:writetofile:edgesbuffer: fwrite error 34"<<endl; exit(EXIT_FAILURE); }}
	}
	cout<<">>> creategraphs::writeedgestofile:: total number of edges written to all ["<<graphobj->getnumedgebanks()<<"] banks: "<<totalnumedges<<endl;
	return;
}

void creategraphs::clearedges(std::vector<edge2_type> (&edgesbuffer)[MAXNUMEDGEBANKS]){
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ edgesbuffer[j].clear(); }
	return;
}

void creategraphs::writevertexptrstofile(){
	cout<<"creategraphs:: writevertexptrstofile... "<<endl;
	vertex_t totalnumvertices = 0;

	for(unsigned int i=0; i<graphobj->getnumedgebanks(); i++){
		cout<<endl;
		for(unsigned int k=0; k<16; k++){ cout<<"creategraphs::writevertexptrstofile:: lvertexoutdegrees["<<i<<"]["<<k<<"]: "<<lvertexoutdegrees[i][k]<<endl; }
	}
	
	cout<<"creategraphs::writevertexptrstofile:: generating vertex pointers... "<<endl;
	for(unsigned int i=0; i<graphobj->getnumedgebanks(); i++){
		unsigned int firstN = 0; 
		cout<<endl;
		cout<<"creategraphs::writevertexptrstofile:: firstedge["<<i<<"].srcvid: "<<firstedge[i].srcvid<<endl;
		vertexptrs[i][0] = 0;// firstedge[i].srcvid;
		for(unsigned int k=1; k<KVDATA_RANGE; k++){
			vertexptrs[i][k] = vertexptrs[i][k-1] + lvertexoutdegrees[i][k-1];
			if(vertexptrs[i][k] < vertexptrs[i][k-1]){ cout<<"creategraphs::writevertexptrstofile:ERROR: non-increasing vertex ptrs: vertexptrs["<<i<<"]["<<k<<"]: "<<vertexptrs[i][k]<<", vertexptrs["<<i<<"]["<<k-1<<"]: "<<vertexptrs[i][k-1]<<endl; exit(EXIT_FAILURE); }
			#ifdef _DEBUGMODE_HOSTPRINTS3
			// if(firstN < 8 && vertexptrs[i][k] > 0){ cout<<"creategraphs::writevertexptrstofile:: vertexptrs["<<i<<"]["<<k-1<<"]: "<<vertexptrs[i][k-1]<<endl; firstN++; }
			if(firstN < 16){ cout<<"vertexptrs["<<i<<"]["<<k-1<<"]: "<<vertexptrs[i][k-1]<<endl; firstN++; }
			#endif
		}
	}

	cout<<"creategraphs:: writing vertex pointers to file... "<<endl;
	for(unsigned int i=0; i<graphobj->getnumedgebanks(); i++){
		if(fwrite(vertexptrs[i], (KVDATA_RANGE * sizeof(edge_t)), 1, graphobj->getnvmeFd_vertexptrs_w()[i]) == 0){ cout<<"ERROR:writevertexptrstofile: fwrite error 34"<<endl; exit(EXIT_FAILURE); }			
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"creategraphs::writevertexptrstofile: number of vertices out-degree information written to bank["<<i<<"]: "<<KVDATA_RANGE<<endl;
		#endif 
		totalnumvertices += KVDATA_RANGE;
	}
	
	for(unsigned int i=0; i<graphobj->getnumedgebanks(); i++){
		cout <<"creategraphs:: KVDATA_RANGE: "<<KVDATA_RANGE <<endl;
		cout <<"creategraphs:: number of vertices: "<<graphobj->get_num_vertices() <<endl;
		cout <<"creategraphs:: number of edges: "<<graphobj->get_num_edges() <<endl;
		cout << TIMINGRESULTSCOLOR <<"creategraphs:: number of edges in column "<<i<<": "<<numedges[i]<< RESET <<endl;
		cout <<"creategraphs:: first offset: vertexptrs["<<i<<"][0]: "<<vertexptrs[i][0] <<endl;
		cout <<"creategraphs:: last offset: vertexptrs["<<i<<"]["<<graphobj->get_num_vertices()-1<<"]: "<<vertexptrs[i][graphobj->get_num_vertices()-1] <<endl;
		cout <<"creategraphs:: last+1 offset: vertexptrs["<<i<<"]["<<graphobj->get_num_vertices()<<"]: "<<vertexptrs[i][graphobj->get_num_vertices()] <<endl;
		cout << TIMINGRESULTSCOLOR <<"creategraphs:: end offset: vertexptrs["<<i<<"]["<<KVDATA_RANGE-1<<"]: "<<vertexptrs[i][KVDATA_RANGE-1]<< RESET <<endl;
		cout << TIMINGRESULTSCOLOR <<"creategraphs:: firstedge["<<i<<"].srcvid: "<<firstedge[i].srcvid<< RESET <<endl;
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"creategraphs:: printing vertex out degrees... "<<endl;
		for(unsigned int k=0; k<8; k++){ cout <<k<<": "<< lvertexoutdegrees[i][k]<<endl; }
		cout<<"creategraphs:: printing vertex ptrs... "<<endl;
		for(unsigned int k=0; k<8; k++){ cout <<k<<": "<< vertexptrs[i][k]<<endl; }
		#endif 
		cout<<"creategraphs:: checking col "<<i<<"... "<<endl;

		#ifdef GRAPHISUNDIRECTED
		if(vertexptrs[i][KVDATA_RANGE-1] != 2*numedges[i]){ cout<<"creategraphs::writevertexptrstofile:ERROR: mismatch 34: vertexptrs["<<i<<"]["<<KVDATA_RANGE-1<<"]: "<<vertexptrs[i][KVDATA_RANGE-1]<<", numedges["<<i<<"]: "<<numedges[i]<<endl; exit(EXIT_FAILURE); }		
		#else 
		if(vertexptrs[i][KVDATA_RANGE-1] != numedges[i]){ cout<<"creategraphs::writevertexptrstofile:ERROR: mismatch 34: vertexptrs["<<i<<"]["<<KVDATA_RANGE-1<<"]: "<<vertexptrs[i][KVDATA_RANGE-1]<<", numedges["<<i<<"]: "<<numedges[i]<<endl; exit(EXIT_FAILURE); }		
		#endif 
	}
	cout<<">>> creategraphs::writevertexptrstofile:: total number of vertices out-degree information written to all ["<<graphobj->getnumedgebanks()<<"] banks: "<<totalnumvertices<<endl;
	return;
}

unsigned int creategraphs::getbank(vertex_t vertexid){
	unsigned int bank;
	bank = vertexid / KVDATA_RANGE;
	if(bank >= graphobj->getnumedgebanks()){
		cout<<"creategraphs:: ERROR 32. invalid bank. bank: "<<bank<<", vertexid: "<<vertexid<<", graphobj->getdataset().num_vertices: "<<graphobj->getdataset().num_vertices<<", graphobj->getnumedgebanks(): "<<graphobj->getnumedgebanks()<<endl;
		if(globalerrorfound1++ > 10000){ exit(EXIT_FAILURE); }
		// exit(EXIT_FAILURE); 
		bank = 0;
	}  	
	return bank;	
}


