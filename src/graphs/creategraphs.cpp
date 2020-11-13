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
#include "../../src/heuristics/heuristics.h"
#include "../../include/common.h"
#include "../../include/host_common.h"
#include "creategraphs.h"
using namespace std;
#define YES

creategraphs::creategraphs(unsigned int datasetid){
	algorithm * algorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	utilityobj = new utility();
	graphobj = new graph(algorithmobj, datasetid, heuristicsobj->getdefaultnumedgebanks(), true, false, false);
	
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
	// clearedges(edges2buffer);	
}

void creategraphs::run(){
	cout<<"creategraphs::run started."<<endl;
	
	#if NUMGROUPS>1
	analyzegraph();
	transformgraph();
	#endif 
	
	resetdatastructures(0);
	cout<<endl<< TIMINGRESULTSCOLOR << "creategraphs::start: creating graph for group 0..." << RESET <<endl;
	start();
	
	/* resetdatastructures(1);
	cout<<endl<< TIMINGRESULTSCOLOR << "creategraphs::start: creating graph for group 1..." << RESET <<endl;
	start(); */
	
	return;
}

void creategraphs::createdatastructures(){
	cout<<"~creategraphs::createdatastructures:: creating structures..."<<endl;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		lvertexoutdegrees[j] = new edge_t[KVDATA_RANGE];
		lvertexindegrees[j] = new edge_t[KVDATA_RANGE];
		vertexptrs[j] = new edge_t[KVDATA_RANGE];
	}
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ firstedge[i].srcvid = INVALIDDATA; firstedge[i].dstvid = INVALIDDATA;  }
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
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){ firstedge[i].srcvid = INVALIDDATA; firstedge[i].dstvid = INVALIDDATA;  }
	groupid = _groupid;
} 

void creategraphs::start(){
	cout<<"creategraphs::start: opening files..."<<endl;
	utilityobj->createdirectory(graphobj->getdatasetdir().c_str());
	graphobj->openfilesforwriting(groupid);
	graphobj->openfilesforreading(groupid);
	std::ifstream file_graph(graphobj->getdataset().graph_path);

	vertex_t srcv = 0;
	vertex_t dstv = 0;
	vertex_t ew = 1;
	vertex_t local_srcv = 0;
	vertex_t local_dstv = 0;
	edge_t linecount = 0;
	edge_t alllinecount = 0;
	vertex_t prevsrcv = 0;
	
	// create graph
	if (file_graph.is_open()) {
		std::string line;
		while (getline(file_graph, line)) {
			if (line.find("%") == 0){ continue; }
			if (alllinecount == 0){ alllinecount++; continue; } // first entry for flickr is stats
			if ((alllinecount % 1000000) == 0){ cout<<"creategraphs::start edge: ["<<srcv<<","<<dstv<<","<<ew<<"]. alllinecount: "<<alllinecount<<endl; }
			
			/* if(graphobj->getdataset().graphorder == SRC_DST){
				sscanf(line.c_str(), "%i %i", &srcv, &dstv);
			} else {
				sscanf(line.c_str(), "%i %i", &dstv, &srcv);
			} */
			
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
			if(srcv > graphobj->getdataset().num_vertices){ cout<<"creategraphs::start:: source vertex found greater than number of vertices specified in dataset. srcv: "<<srcv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<endl; exit(EXIT_FAILURE); }
			if(dstv > graphobj->getdataset().num_vertices){ cout<<"creategraphs::start:: destination vertex found greater than number of vertices specified in dataset. dstv: "<<dstv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<endl; exit(EXIT_FAILURE); }
			// if(alllinecount >= 2 && alllinecount < 128){ if(srcv < prevsrcv){ cout<<"creategraphs::start:: ERROR: source vertexid ("<<srcv<<") is less than previous source vertex id ("<<prevsrcv<<"). change graph direction. EXITING... "<<endl; exit(EXIT_FAILURE); } else { prevsrcv = srcv; }} else { prevsrcv = srcv; } 
			
			#if NUMGROUPS>1
			srcv = gettransformedglobalid(srcv);
			dstv = gettransformedglobalid(dstv);
			#endif
		
			if(getgroup(dstv) == groupid){
				local_srcv = srcv;
				#if NUMGROUPS==1
				local_dstv = dstv;
				#else 
				local_dstv = getlocalid(dstv);
				#endif
				
				unsigned int col = getbank(local_dstv);
				
				#ifdef _DEBUGMODE_CHECKS
				if (linecount < 16){ cout<<"creategraphs:: gedge: ["<<local_srcv<<","<<local_dstv<<"]"<<endl; }
				#endif 
				
				edge_type edge;
				edge2_type edge2;
				
				edge.srcvid = local_srcv;
				edge.dstvid = local_dstv;
				
				edge2.dstvid = local_dstv;
				
				if(firstedge[col].srcvid == INVALIDDATA){ firstedge[col] = edge; }
				
				edgesbuffer[col].push_back(edge); 
				// edges2buffer[col].push_back(edge2); 
				
				lvertexoutdegrees[col][local_srcv] += 1;
				lvertexindegrees[col][local_dstv] += 1;
				
				numedges[col] += 1;

				if((linecount++ % CREATENDGRAPH_BATCHSIZE) == 0){
					#ifdef _DEBUGMODE_HOSTPRINTS
					if(linecount < 2 * CREATENDGRAPH_BATCHSIZE){
					utilityobj->printedges("creategraphs::start", (edge_type *)(&edgesbuffer[0][0]), 16); 
					utilityobj->printedges("creategraphs::start", (edge_type *)(&edgesbuffer[1][0]), 16); }
					#endif 
					
					writeedgestofile(edgesbuffer); 
					clearedges(edgesbuffer); 

					// writeedgestofile(edges2buffer); 
					// clearedges(edges2buffer);
				}
			}
			alllinecount += 1;
		}
		cout<<"creategraphs:: finished processing edges: [valid edges processed: "<<linecount<<"][invalid edges processed: "<<(alllinecount - linecount)<<"][total edges processed: "<<alllinecount<<"][groupid: "<<groupid<<"][YDIMENSIONTHRESHOLD: "<<YDIMENSIONTHRESHOLD<<"]"<<endl;
	}
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printedges("creategraphs::start", (edge_type *)(&edgesbuffer[0][0]), 16); 
	utilityobj->printedges("creategraphs::start", (edge_type *)(&edgesbuffer[1][0]), 16);
	#endif
	
	writeedgestofile(edgesbuffer); 
	clearedges(edgesbuffer); 

	// writeedgestofile(edges2buffer); // FIXME.
	// clearedges(edges2buffer); 
	
	writevertexptrstofile();

	cout<<"creategraphs:: start finished. closing files... "<<endl;
	file_graph.close(); 
	graphobj->closefilesforwriting();
	graphobj->closefilesforreading();
	cout<<"creategraphs:: finished creating 2D graph from "<<graphobj->getdataset().graph_path<<endl;
	summary();
	return;
}

void creategraphs::analyzegraph(){
	cout<<endl<< TIMINGRESULTSCOLOR << "creategraphs::analyzegraph: analyzing graph..." << RESET <<endl;
	utilityobj->createdirectory(graphobj->getdatasetdir().c_str());
	std::ifstream file_graph(graphobj->getdataset().graph_path);
	
	cout<<"creategraphs::analyzegraph: initializing vertex out-degrees"<<endl;
	vertexindegrees = new unsigned int[KVDATA_RANGE]; 
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ vertexindegrees[i] = 0; }
	
	vertex_t srcv = 0;
	vertex_t dstv = 0;
	edge_t linecount = 0;
	vertex_t prevsrcv = 0;
	
	if (file_graph.is_open()) {
		std::string line;
		while (getline(file_graph, line)) {
			if (line.find("%") == 0){ continue; }
			if (linecount == 0){ linecount++; continue; } // first entry for flickr is stats
			if ((linecount % 1000000) == 0){ cout<<"creategraphs::analyzegraph edge: ["<<srcv<<", "<<dstv<<"]. linecount: "<<linecount<<endl; }
			
			if(graphobj->getdataset().graphorder == SRC_DST){
				sscanf(line.c_str(), "%i %i", &srcv, &dstv);
			} else {
				sscanf(line.c_str(), "%i %i", &dstv, &srcv);
			}
			#ifdef _DEBUGMODE_CHECKS
			cout<<"creategraphs: srcv: "<<srcv<<", dstv: "<<dstv<<endl;
			#endif 
			
			if(srcv > graphobj->getdataset().num_vertices){ cout<<"creategraphs::analyzegraph:: source vertex found greater than number of vertices specified in dataset. srcv: "<<srcv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<". EXITING..."<<endl; exit(EXIT_FAILURE); }
			if(dstv > graphobj->getdataset().num_vertices){ cout<<"creategraphs::analyzegraph:: destination vertex found greater than number of vertices specified in dataset. dstv: "<<dstv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<". EXITING..."<<endl; exit(EXIT_FAILURE); }						
			if(linecount >= 2 && linecount < 128){ if(srcv < prevsrcv){ cout<<"creategraphs::analyzegraph:: ERROR: source vertexid ("<<srcv<<") is less than previous source vertex id ("<<prevsrcv<<"). change graph direction. EXITING... "<<endl; exit(EXIT_FAILURE); } else { prevsrcv = srcv; }} else { prevsrcv = srcv; } 
			
			vertexindegrees[dstv] += 1;
			
			// if (linecount < 16){ cout<<"creategraphs:: gedge: ["<<srcv<<", "<<dstv<<"]"<<endl; }
			if (linecount < 16){ cout<<"analyzegraph:: line["<<linecount<<"]: "<<line<<endl; }
			linecount += 1;
		}
	} else {
		cout<<"creategraphs:: directory ("<<graphobj->getdatasetdir()<<") does not exist. EXITING... "<<endl;
		exit(EXIT_FAILURE);
	}
	
	cout<<"creategraphs:: closing vertices files... "<<endl;
	file_graph.close(); 
	
	utilityobj->printvalues("creategraphs::collectstats. vertexindegrees::", vertexindegrees, 256);
	utilityobj->printvaluesgreaterthan("creategraphs::collectstats. vertexindegrees::", vertexindegrees, graphobj->getdataset().num_vertices, YDIMENSIONTHRESHOLD);
	utilityobj->printvalueslessthan("creategraphs::collectstats. vertexindegrees::", vertexindegrees, graphobj->getdataset().num_vertices, 100);
	cout<<"creategraphs::analyzegraph finished analyzing "<<graphobj->getdataset().graph_path<<endl;
	return;
}
void creategraphs::transformgraph(){
	cout<<"creategraphs::transformgraph transforming large indegree vertices "<<endl;
	
	global_to_transfglobal_ids = new unsigned int[KVDATA_RANGE];
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ global_to_transfglobal_ids[i] = INVALIDDATA; }
	unsigned int nextfreevertexid = graphobj->getdataset().groupvoffset[1];
	
	for(unsigned int i=0; i<graphobj->getdataset().num_vertices; i++){ 
		if(vertexindegrees[i] >= YDIMENSIONTHRESHOLD){
			if(global_to_transfglobal_ids[i] == INVALIDDATA){
				global_to_transfglobal_ids[i] = nextfreevertexid; // re-assign
				vertexindegrees[nextfreevertexid] = vertexindegrees[i]; 
				vertexindegrees[i] = 0; 
				if((nextfreevertexid - graphobj->getdataset().num_vertices) < 16){ cout<<"creategraphs::transformgraph '> Threshold' FOUND i: "<<i<<" changed to "<<nextfreevertexid<<": vertexindegrees["<<i<<"]: "<<vertexindegrees[i]<<". vertexindegrees["<<nextfreevertexid<<"]: "<<vertexindegrees[nextfreevertexid]<<endl; }
				nextfreevertexid += 1;
			}
		}
	}
	cout<<"creategraphs::transformgraph finished transforming large indegree vertices. number of rehased items: "<<(nextfreevertexid - graphobj->getdataset().num_vertices)<<endl;
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

void creategraphs::writeedgestofile(std::vector<edge_type> (&edgesbuffer)[MAXNUMEDGEBANKS]){
	cout<<"creategraphs::writeedgestofile<edge_type>  started."<<endl;

	edge_t totalnumedges = 0;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		totalnumedgeswritten[j] += edgesbuffer[j].size();
		totalnumedges += edgesbuffer[j].size();
		if(edgesbuffer[j].size() > 0){ if(fwrite(edgesbuffer[j].data(), (edgesbuffer[j].size() * sizeof(edge_type)), 1, graphobj->getnvmeFd_edges_w()[j]) == 0){ cout<<"ERROR:writetofile:edgesbuffer: fwrite error 34"<<endl; exit(EXIT_FAILURE); }}
	}
	cout<<">>> creategraphs::writeedgestofile:: total number of edges written to all ["<<graphobj->getnumedgebanks()<<"] banks: "<<totalnumedges<<endl;
	return;
}
void creategraphs::writeedgestofile(std::vector<edge2_type> (&edges2buffer)[MAXNUMEDGEBANKS]){
	cout<<"creategraphs::writeedgestofile<edge2_type> started."<<endl;
	edge_t totalnumedges = 0;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		totalnumedgeswritten[j] += edges2buffer[j].size();
		totalnumedges += edges2buffer[j].size();
		if(edges2buffer[j].size() > 0){ if(fwrite(edges2buffer[j].data(), (edges2buffer[j].size() * sizeof(edge2_type)), 1, graphobj->getnvmeFd_edges_w()[j]) == 0){ cout<<"ERROR:writetofile:edges2buffer: fwrite error 34"<<endl; exit(EXIT_FAILURE); }}
	}
	cout<<">>> creategraphs::writeedgestofile:: total number of edges written to all ["<<graphobj->getnumedgebanks()<<"] banks: "<<totalnumedges<<endl;
	return;
}

void creategraphs::clearedges(std::vector<edge_type> (&edgesbuffer)[MAXNUMEDGEBANKS]){
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ edgesbuffer[j].clear(); }
	return;
}
void creategraphs::clearedges(std::vector<edge2_type> (&edges2buffer)[MAXNUMEDGEBANKS]){
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ edges2buffer[j].clear(); }
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
		// if((vertexptrs[i][KVDATA_RANGE-1] - firstedge[i].srcvid) != numedges[i]){ cout<<"creategraphs::writevertexptrstofile:ERROR: mismatch 34: vertexptrs["<<i<<"]["<<KVDATA_RANGE-1<<"]: "<<vertexptrs[i][KVDATA_RANGE-1]<<", numedges["<<i<<"]: "<<numedges[i]<<endl; exit(EXIT_FAILURE); }
		if(vertexptrs[i][KVDATA_RANGE-1] != numedges[i]){ cout<<"creategraphs::writevertexptrstofile:ERROR: mismatch 34: vertexptrs["<<i<<"]["<<KVDATA_RANGE-1<<"]: "<<vertexptrs[i][KVDATA_RANGE-1]<<", numedges["<<i<<"]: "<<numedges[i]<<endl; exit(EXIT_FAILURE); }		
	}
	cout<<">>> creategraphs::writevertexptrstofile:: total number of vertices out-degree information written to all ["<<graphobj->getnumedgebanks()<<"] banks: "<<totalnumvertices<<endl;
	return;
}

unsigned int creategraphs::getbank(vertex_t vertexid){
	unsigned int bank;
	#if NUMGROUPS==1
	bank = vertexid / KVDATA_RANGE_PERSSDPARTITION;
	#else 
	if(groupid == HIGHINDEGREESGROUPID){ bank = vertexid / (KVDATA_RANGE_PERSSDPARTITION / graphobj->getdataset().skewratio); }
	else { bank = vertexid / KVDATA_RANGE_PERSSDPARTITION; }
	#endif
	if(bank >= graphobj->getnumedgebanks()){ cout<<"creategraphs:: ERROR 32. invalid bank. bank: "<<bank<<", vertexid: "<<vertexid<<", graphobj->getdataset().num_vertices: "<<graphobj->getdataset().num_vertices<<", graphobj->getnumedgebanks(): "<<graphobj->getnumedgebanks()<<endl; exit(EXIT_FAILURE); }  	
	return bank;	
}
unsigned int creategraphs::getgroup(unsigned int vid){
	#if NUMGROUPS==1
	return 0;
	#else 
	if(vertexindegrees[vid] >= YDIMENSIONTHRESHOLD){ return HIGHINDEGREESGROUPID; }
	else { return LOWINDEGREESGROUPID; }
	#endif 
}
unsigned int creategraphs::gettransformedglobalid(unsigned int vertexid){
	if(global_to_transfglobal_ids[vertexid] == INVALIDDATA){ return vertexid; }
	else { return global_to_transfglobal_ids[vertexid]; }
}
unsigned int creategraphs::getlocalid(unsigned int vertexid){
	return vertexid - graphobj->getdataset().groupvoffset[groupid];
}


