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
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../../include/host_common.h"
#include "create2Dgraph.h"
using namespace std;
#define YES
#define THRESHOLDLINECNT 10000000

create2Dgraph::create2Dgraph(graph * _graphobj){
	graphobj = _graphobj;
	
	cout<<"create2Dgraph:: constructor called: "<<endl;
	cout<<"create2Dgraph:: dataset.graph_path: "<<graphobj->getdataset().graph_path<<endl;
	cout<<"create2Dgraph::  dataset.vertices_path: "<<graphobj->getdataset().vertices_path<<endl;
	cout<<"create2Dgraph::  dataset.vertices_path_bin: "<<graphobj->getdataset().vertices_path_bin<<endl;
	cout<<"create2Dgraph::  dataset.edges_path: "<<graphobj->getdataset().edges_path<<endl;
	cout<<"create2Dgraph::  dataset.edges_path_bin: "<<graphobj->getdataset().edges_path_bin<<endl;
	cout<<"create2Dgraph::  dataset.graphorder: "<<graphobj->getdataset().graphorder<<endl;
	cout<<endl;
	
	cout<<"create2Dgraph::create2Dgraph: initializing statistics variables..."<<endl;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ totalnumedgeswritten[j] = 0; }
	
	cout<<"create2Dgraph::create2Dgraph: initializing lvertexoutdegrees and edgeoffsets..."<<endl;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		lvertexoutdegrees[j] = new edge_t[KVDATA_RANGE];
		for(unsigned int k=0; k<KVDATA_RANGE; k++){ lvertexoutdegrees[j][k] = 0; }
		bank_col_numvertices[j] = 0;
		edgeoffsets[j] = new xvertexoffset_t[KVDATA_RANGE];
		bank_col_numedges[j] = 0;
	}
	
	cout<<"create2Dgraph::create2Dgraph: opening files..."<<endl;
	graphobj->openfilesforwriting();
	graphobj->openfilesforreading();
	
	utilityobj = new utility();
	cout<<"create2Dgraph::create2Dgraph: constructor finished"<<endl;
}
create2Dgraph::create2Dgraph(graph * _graphobj, unsigned int dummy){
	graphobj = _graphobj;
	
	cout<<"create2Dgraph:: constructor called: "<<endl;
	cout<<"create2Dgraph:: dataset.graph_path: "<<graphobj->getdataset().graph_path<<endl;
	cout<<"create2Dgraph::  dataset.vertices_path: "<<graphobj->getdataset().vertices_path<<endl;
	cout<<"create2Dgraph::  dataset.vertices_path_bin: "<<graphobj->getdataset().vertices_path_bin<<endl;
	cout<<"create2Dgraph::  dataset.edges_path: "<<graphobj->getdataset().edges_path<<endl;
	cout<<"create2Dgraph::  dataset.edges_path_bin: "<<graphobj->getdataset().edges_path_bin<<endl;
	cout<<"create2Dgraph::  dataset.graphorder: "<<graphobj->getdataset().graphorder<<endl;
	cout<<endl;
	
	cout<<"create2Dgraph::create2Dgraph: opening files..."<<endl;
	graphobj->openfilesforreading();
	
	utilityobj = new utility();
	cout<<"create2Dgraph::create2Dgraph: constructor finished"<<endl;
}
create2Dgraph::~create2Dgraph() {
	cout<<"~create2Dgraph:: destructor called... "<<endl;
	cout<<"create2Dgraph::~create2Dgraph deleting dynamic memories... "<<endl;
	
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ 
		delete [] lvertexoutdegrees[j]; 
		delete [] edgeoffsets[j];
	}
	
	#ifdef STREAMEDGESSETUP
	clearedges<edgeprop1_t>(edgepropertiesbuffer);
	#else 
	clearedges<edgeprop2_t>(edgepropertiesbuffer);	
	#endif
} 

unsigned int create2Dgraph::getbank(vertex_t vertexid){
	unsigned int bank = vertexid / KVDATA_RANGE_PERSSDPARTITION;
	if(bank >= graphobj->getnumedgebanks()){ cout<<"create2Dgraph:: ERROR 32. invalid bank. bank: "<<bank<<", vertexid: "<<vertexid<<", graphobj->getdataset().num_vertices: "<<graphobj->getdataset().num_vertices<<", graphobj->getnumedgebanks(): "<<graphobj->getnumedgebanks()<<endl; exit(EXIT_FAILURE); }  
	return bank;	
}
template <class T>
void create2Dgraph::clearedges(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS]){
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		edgepropertiesbuffer[j].clear();
	}
	return;
}
size_t create2Dgraph::hceildiv(size_t val1, size_t val2){
	return (val1 + (val2 - 1)) / val2;
}

void create2Dgraph::start(){
	std::ifstream file_graph(graphobj->getdataset().graph_path);

	vertex_t srcv = 0;
	vertex_t dstv = 0;
	edge_t linecount = 0;
	
	if (file_graph.is_open()) {
		std::string line;
		while (getline(file_graph, line)) {
			if (line.find("%") == 0){ continue; }
			if (linecount == 0){ linecount++; continue; } // first entry for flickr is stats
			if ((linecount % 1000000) == 0){ cout<<"create2Dgraph:: edge: ["<<srcv<<","<<dstv<<"]. linecount: "<<linecount<<endl; }
			
			if(graphobj->getdataset().graphorder == SRC_DST){
				sscanf(line.c_str(), "%i %i", &srcv, &dstv);
			} else {
				sscanf(line.c_str(), "%i %i", &dstv, &srcv);
			}
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph: srcv: "<<srcv<<", dstv: "<<dstv<<endl;
			#endif 
			
			if(srcv > graphobj->getdataset().num_vertices){ cout<<"create2Dgraph::start:: source vertex found greater than number of vertices spacified in dataset. srcv: "<<srcv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<endl; exit(EXIT_FAILURE); }
			if(dstv > graphobj->getdataset().num_vertices){ cout<<"create2Dgraph::start:: destination vertex found greater than number of vertices spacified in dataset. dstv: "<<dstv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<endl; exit(EXIT_FAILURE); }
	
			unsigned int col = getbank(dstv);
			
			if (linecount < 16){ cout<<"create2Dgraph:: gedge: ["<<srcv<<","<<dstv<<"]"<<endl; }
			
			#ifdef STREAMEDGESSETUP
			edgeprop1_t ledgeproperty;
			#else 
			edgeprop2_t ledgeproperty;	
			#endif 
			// T ledgeproperty;
			#ifndef STREAMEDGESSETUP
			ledgeproperty.srcvid = srcv;
			#endif 
			ledgeproperty.dstvid = dstv;
			
			edgepropertiesbuffer[col].push_back(ledgeproperty); 
			
			if(lvertexoutdegrees[col][srcv] == 0){ bank_col_numvertices[col] += 1; }
			lvertexoutdegrees[col][srcv] += 1;
			bank_col_numedges[col] += 1;

			if((linecount++ % THRESHOLDLINECNT) == 0){
				#ifdef STREAMEDGESSETUP
				writeedgestofile<edgeprop1_t>(edgepropertiesbuffer); 
				clearedges<edgeprop1_t>(edgepropertiesbuffer); 
				#else 
				writeedgestofile<edgeprop2_t>(edgepropertiesbuffer); 
				clearedges<edgeprop2_t>(edgepropertiesbuffer); 	
				#endif 
			}
		}
	}
	#ifdef STREAMEDGESSETUP
	writeedgestofile<edgeprop1_t>(edgepropertiesbuffer);
	clearedges<edgeprop1_t>(edgepropertiesbuffer);
	#else 
	writeedgestofile<edgeprop2_t>(edgepropertiesbuffer);
	clearedges<edgeprop2_t>(edgepropertiesbuffer);
	#endif 
	#ifdef STREAMEDGESSETUP
	writeedgeoffsetbitstofile();
	#else 
	writeedgeoffsetstofile();
	#endif 
	cout<<"create2Dgraph:: closing vertices files... "<<endl;
	file_graph.close(); 
	
	cout<<"create2Dgraph:: closing used files... "<<endl;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ fclose(graphobj->getnvmeFd_edgeproperties_w()[0][j]); }
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ fclose(graphobj->getnvmeFd_edgeoffsets_w()[0][j]); }
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ fclose(graphobj->getnvmeFd_edgeproperties_r()[0][j]); }
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){ fclose(graphobj->getnvmeFd_edgeoffsets_r()[0][j]); }
	cout<<"create2Dgraph:: finished creating 2D graph from "<<graphobj->getdataset().graph_path<<endl;
	return;
}
void create2Dgraph::analyzegraph(){
	cout<<"create2Dgraph::analyzegraph: analyzing graph started"<<endl;
	std::ifstream file_graph(graphobj->getdataset().graph_path);
	
	cout<<"create2Dgraph::analyzegraph: initializing vertex out-degrees"<<endl;
	vertexoutdegrees = new unsigned int[graphobj->getdataset().num_vertices];
	vertexindegrees = new unsigned int[graphobj->getdataset().num_vertices];
	for(unsigned int i=0; i<graphobj->getdataset().num_vertices; i++){ vertexoutdegrees[i] = 0; }
	for(unsigned int i=0; i<graphobj->getdataset().num_vertices; i++){ vertexindegrees[i] = 0; }
	
	vertex_t srcv = 0;
	vertex_t dstv = 0;
	edge_t linecount = 0;
	
	if (file_graph.is_open()) {
		std::string line;
		while (getline(file_graph, line)) {
			if (line.find("%") == 0){ continue; }
			if (linecount == 0){ linecount++; continue; } // first entry for flickr is stats
			if ((linecount % 1000000) == 0){ cout<<"create2Dgraph:: edge: ["<<srcv<<", "<<dstv<<"]. linecount: "<<linecount<<endl; }
			
			if(graphobj->getdataset().graphorder == SRC_DST){
				sscanf(line.c_str(), "%i %i", &srcv, &dstv);
			} else {
				sscanf(line.c_str(), "%i %i", &dstv, &srcv);
			}
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph: srcv: "<<srcv<<", dstv: "<<dstv<<endl;
			#endif 
			
			if(srcv > graphobj->getdataset().num_vertices){ cout<<"create2Dgraph::start:: source vertex found greater than number of vertices spacified in dataset. srcv: "<<srcv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<endl; exit(EXIT_FAILURE); }
			if(dstv > graphobj->getdataset().num_vertices){ cout<<"create2Dgraph::start:: destination vertex found greater than number of vertices spacified in dataset. dstv: "<<dstv<<", dataset.num_vertices: "<<graphobj->getdataset().num_vertices<<endl; exit(EXIT_FAILURE); }
	
			vertexoutdegrees[srcv] += 1;
			vertexindegrees[dstv] += 1;
			
			if (linecount < 16){ cout<<"create2Dgraph:: gedge: ["<<srcv<<", "<<dstv<<"]"<<endl; }
			linecount += 1;
			
			// if(linecount >= THRESHOLDLINECNT){ break; }
			// if(linecount >= 100000000){ break; }
		}
	}
	cout<<"create2Dgraph:: closing vertices files... "<<endl;
	file_graph.close(); 
	
	cout<<"create2Dgraph:: vertexoutdegrees[1]: "<<vertexoutdegrees[1]<<endl;
	
	// analyze
	utilityobj->printvaluesgreaterthan("create2Dgraph::collectstats. vertexoutdegrees::", vertexoutdegrees, graphobj->getdataset().num_vertices, 1000);
	utilityobj->printvalueslessthan("create2Dgraph::collectstats. vertexoutdegrees::", vertexoutdegrees, graphobj->getdataset().num_vertices, 100);
	
	utilityobj->printvaluesgreaterthan("create2Dgraph::collectstats. vertexindegrees::", vertexindegrees, graphobj->getdataset().num_vertices, 1000);
	utilityobj->printvalueslessthan("create2Dgraph::collectstats. vertexindegrees::", vertexindegrees, graphobj->getdataset().num_vertices, 100);
	
	cout<<"create2Dgraph:: finished analyzing "<<graphobj->getdataset().graph_path<<endl;
	return;
}
void create2Dgraph::summary(){
	edge_t _numwritten = 0;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		_numwritten += totalnumedgeswritten[j];
		#ifdef _DEBUGMODE_CHECKS
		cout<<">>> summary:: total number of edges written in volume ["<<j<<"]: "<<totalnumedgeswritten[j]<<endl;
		#endif 
	}
	cout<<">>> summary:: total number of edges written: "<<_numwritten<<endl;
	/* printworkloadstats();
	printglobaltolocaltranslator();
	checkedgeoffsets();
	checkedges();
	checkledges(); */
	cout<<">>> ===================== finished creating 2D graph ============================== "<<endl<<endl;
	return;
}

template <class T>
void create2Dgraph::writeedgestofile(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS]){
	cout<<"create2Dgraph::writeedgestofile started."<<endl;
	edge_t totalnumedges = 0;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		totalnumedgeswritten[j] += edgepropertiesbuffer[j].size();
		totalnumedges += edgepropertiesbuffer[j].size();
		if(edgepropertiesbuffer[j].size() > 0){ if(fwrite(edgepropertiesbuffer[j].data(), (edgepropertiesbuffer[j].size() * sizeof(T)), 1, graphobj->getnvmeFd_edgeproperties_w()[0][j]) == 0){ cout<<"ERROR:writetofile: fwrite error 34"<<endl; exit(EXIT_FAILURE); }}
	}
	cout<<">>> create2Dgraph::writeedgestofile:: total number of edges written to all ["<<graphobj->getnumedgebanks()<<"] banks: "<<totalnumedges<<endl;
	return;
}
void create2Dgraph::writeedgeoffsetbitstofile(){
	cout<<"create2Dgraph:: writeedgeoffsetbitstofile started."<<endl;
	size_t ledgeid = 0;	
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		ledgeid = 0;
		unsigned int edgeoffsetbitssz = (bank_col_numedges[j] + (NUMBITSINUNSIGNEDINT - 1)) / NUMBITSINUNSIGNEDINT;
		edgeoffsetbits[j] = new unsigned int[edgeoffsetbitssz];
		for(unsigned int k=0; k<edgeoffsetbitssz; k++){ edgeoffsetbits[j][k] = 0; }
		cout<<">>> create2Dgraph::writeedgeoffsetbitstofile:: bank_col_numedges["<<j<<"]: "<<bank_col_numedges[j]<<", edgeoffsetbitssz: "<<edgeoffsetbitssz<<endl;
	
		for(unsigned int k=0; k<KVDATA_RANGE; k++){
			for(unsigned int n=0; n<lvertexoutdegrees[j][k]; n++){
				if((ledgeid / NUMBITSINUNSIGNEDINT) >= edgeoffsetbitssz){ cout<<"create2Dgraph::writeedgeoffsetbitstofile:: ERROR 34. ledgeid >= edgeoffsetbitssz. ledgeid: "<<ledgeid<<", edgeoffsetbitssz: "<<edgeoffsetbitssz<<endl; exit(EXIT_FAILURE); }
				if(n==0){ utilityobj->InsertBit(edgeoffsetbits[j], ledgeid, 1); }
				else { utilityobj->InsertBit(edgeoffsetbits[j], ledgeid, 0); }
				ledgeid += 1;
				if(ledgeid > bank_col_numedges[j]){ cout<<"create2Dgraph::writeedgeoffsetbitstofile:: ERROR 34. ledgeid >= bank_col_numedges["<<j<<"]. ledgeid: "<<ledgeid<<", bank_col_numedges["<<j<<"]: "<<bank_col_numedges[j]<<", lvertexoutdegrees["<<j<<"]["<<k<<"]: "<<lvertexoutdegrees[j][k]<<endl; exit(EXIT_FAILURE); }					
			}
		}
		cout<<"create2Dgraph::writeedgeoffsetbitstofile:: edgeoffsetbitssz: "<<edgeoffsetbitssz<<endl;
		if(edgeoffsetbitssz > 0){ if(fwrite(edgeoffsetbits[j], sizeof(unsigned int), edgeoffsetbitssz, graphobj->getnvmeFd_edgeoffsets_w()[0][j]) == 0){ cout<<"--- ERROR:writeedgeoffsetbitstofile: fwrite error 34"<<endl; exit(EXIT_FAILURE); }}
		// delete edgeoffsetbits[j];
	}
	cout<<">>> create2Dgraph::writeedgeoffsetbitstofile:: finished. ledgeid: "<<ledgeid<<endl;
	return;
}
void create2Dgraph::generateedgeoffsets(){
	cout<<"create2Dgraph:: generateedgeoffsets... "<<endl;
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){			
		cout<<"create2Dgraph:: generateedgeoffsets: bank_col_numvertices["<<j<<"]: "<<bank_col_numvertices[j]<<endl;
		edgeoffsets[j][0] = 0;
		for(unsigned int k=1; k<KVDATA_RANGE; k++){
			edgeoffsets[j][k] = edgeoffsets[j][k-1] + lvertexoutdegrees[j][k-1];
		}
	}
	return;
}
void create2Dgraph::writeedgeoffsetstofile(){
	cout<<"create2Dgraph:: writeedgeoffsetstofile... "<<endl;
	vertex_t totalnumvertices = 0;
	generateedgeoffsets();	
	
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		if(fwrite(edgeoffsets[j], (KVDATA_RANGE * sizeof(xvertexoffset_t)), 1, graphobj->getnvmeFd_edgeoffsets_w()[0][j]) == 0){ cout<<"ERROR:writeedgeoffsetstofile: fwrite error 34"<<endl; exit(EXIT_FAILURE); }			
		#ifdef _DEBUGMODE_CHECKS
		cout<<"create2Dgraph::writeedgeoffsetstofile: number of vertices out-degree information written to bank["<<j<<"]: "<<KVDATA_RANGE<<endl;
		#endif 
		totalnumvertices += KVDATA_RANGE;
	}
	
	cout<<">>> create2Dgraph::writeedgeoffsetstofile:: total number of vertices out-degree information written to all ["<<graphobj->getnumedgebanks()<<"] banks: "<<totalnumvertices<<endl;
	return;
}

void create2Dgraph::printworkloadstats(){
	edge_t totalnumvertices = 0;
	edge_t totalnumedges = 0;
	edge_t totalnumedges2 = 0;
	
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		#ifdef _DEBUGMODE_CHECKS
		cout<<"create2Dgraph::printworkloadstats:: bank_col_numvertices["<<j<<"]: "<<bank_col_numvertices[j]<<endl;
		#endif 
		totalnumvertices += bank_col_numvertices[j];
	}

	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){	
		for(unsigned int k=0; k<KVDATA_RANGE; k++){	
			#ifdef _DEBUGMODE_CHECKS
			if(k < 4){ cout<<"create2Dgraph::printworkloadstats:: lvertexoutdegrees["<<j<<"]["<<k<<"]: "<<lvertexoutdegrees[j][k]<<endl; }
			#endif 
			totalnumedges += lvertexoutdegrees[j][k];
		}
		#ifdef _DEBUGMODE_CHECKS
		cout<<"create2Dgraph::printworkloadstats ... "<<endl;
		#endif 
	}

	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){
		#ifdef _DEBUGMODE_CHECKS
		cout<<"create2Dgraph::printworkloadstats:: bank_col_numedges["<<j<<"]: "<<bank_col_numedges[j]<<endl;
		#endif 
		totalnumedges2 += bank_col_numedges[j];
	}

	#ifdef _DEBUGMODE_CHECKS // NB: all banks have the same number of local vertices
	cout<<"create2Dgraph::printworkloadstats:: globaltolocalvid_weof: "<<globaltolocalvid_weof<<endl;
	#endif 

	#ifdef _DEBUGMODE_CHECKS
	cout<<"create2Dgraph:: globaltolocalvid_weof: "<<globaltolocalvid_weof<<endl;
	#endif 
	for(unsigned int j=0; j<graphobj->getnumedgebanks(); j++){	
		#ifdef _DEBUGMODE_CHECKS
		cout<<"$$$ create2Dgraph:: edgeoffsets["<<j<<"]["<<(globaltolocalvid_weof - 1)<<"].offset: "<<edgeoffsets[j][(globaltolocalvid_weof - 1)].offset<<", bank_col_numedges["<<j<<"]: "<<bank_col_numedges[j]<<endl;	
		#endif 
	}
	
	cout<<">>> create2Dgraph::printworkloadstats:: total number of vertices out-degree information written to all banks: "<<totalnumvertices<<endl;
	cout<<">>> create2Dgraph::printworkloadstats:: total number of edges written to all banks: "<<totalnumedges<<endl;
	cout<<">>> create2Dgraph::printworkloadstats:: total number of edges written to all banks: "<<totalnumedges2<<endl;
	return;
}
#ifndef STREAMEDGESSETUP
void create2Dgraph::checkedges(){
	#ifdef _DEBUGMODE_CHECKS
	cout<<"create2Dgraph::checkedges started. "<<endl;
	#endif
	edge_t totalcount = 0;
	
	edgeprop2_t firstedgeproperty; bool firstedgepropertyseen = false;
	edgeprop2_t lastedgeproperty;
	
	edgeprop2_t prevedgeproperty;
		for(unsigned int j=0; j<numedgebanks; j++){
			edgeprop2_t edgeproperty;
			edge_t count = 0;
			cout<<"create2Dgraph::checkedges : reading file ["<<j<<"]: "<<graphobj->getpath_edgesproperties(0, j)<<endl;
			while(fread(&edgeproperty, sizeof(edgeprop2_t), 1, graphobj->getnvmeFd_edgeproperties_r()[0][j])){
				if(count < 16){ cout<<"edgeproperty.srcvid: "<<edgeproperty.srcvid<<", edgeproperty.dstvid: "<<edgeproperty.dstvid<<endl;	}
				
				if(firstedgepropertyseen == false){ firstedgepropertyseen = true; firstedgeproperty = edgeproperty; }
				
				if(edgeproperty.srcvid > KVDATA_RANGE_PERSSDPARTITION){ cout<<"ERROR 34:create2Dgraph::checkedges: srcvid out of range. edgeproperty.srcvid: "<<edgeproperty.srcvid<<", dataset.num_vertices: "<<dataset.num_vertices<<", numedgebanks: "<<numedgebanks<<endl; exit(EXIT_FAILURE); }
				if(edgeproperty.dstvid > KVDATA_RANGE_PERSSDPARTITION){ cout<<"ERROR 34:create2Dgraph::checkedges: dstvid out of range. edgeproperty.dstvid: "<<edgeproperty.dstvid<<", dataset.num_vertices: "<<dataset.num_vertices<<", numedgebanks: "<<numedgebanks<<endl; exit(EXIT_FAILURE); }
				if((edgeproperty.srcvid < prevedgeproperty.srcvid) && (count > 0)){ cout<<"ERROR 34:create2Dgraph::checkedges: edgeproperty.srcvid < prevedgeproperty.srcvid. edgeproperty.srcvid: "<<edgeproperty.srcvid<<", prevedgeproperty.srcvid: "<<prevedgeproperty.srcvid<<", count: "<<count<<", totalcount: "<<totalcount<<endl; exit(EXIT_FAILURE); }
				prevedgeproperty = edgeproperty;
				
				count+=1;
				totalcount += 1;
			}
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::checkedges : count: "<<count<<endl;
			#endif 
			lastedgeproperty = edgeproperty;
			
			if((edgeoffsets[j][lastedgeproperty.srcvid] + lvertexoutdegrees[j][lastedgeproperty.srcvid]) != bank_col_numedges[j]){ 
				cout<<"ERROR 34:create2Dgraph::checkedges: ERROR 34. (firstedgeproperty.srcvid - lastedgeproperty.srcvid) != bank_col_numedges[j] "<<endl;
				cout<<"firstedgeproperty.srcvid: "<<firstedgeproperty.srcvid<<endl;
				cout<<"lastedgeproperty.srcvid: "<<lastedgeproperty.srcvid<<endl;
				cout<<"bank_col_numedges["<<j<<"]: "<<bank_col_numedges[j]<<endl;
				cout<<"edgeoffsets["<<j<<"][0] + lvertexoutdegrees["<<j<<"][0]: "<<(edgeoffsets[j][0] + lvertexoutdegrees[j][0])<<endl;
				cout<<"edgeoffsets["<<j<<"]["<<lastedgeproperty.srcvid<<"] + lvertexoutdegrees["<<j<<"]["<<lastedgeproperty.srcvid<<"]: "<<(edgeoffsets[j][lastedgeproperty.srcvid] + lvertexoutdegrees[j][lastedgeproperty.srcvid])<<endl;
				cout<<endl;
			}
			cout<<"SUCCESS:create2Dgraph::checkedges: (firstedgeproperty.srcvid - lastedgeproperty.srcvid) == bank_col_numedges[j] "<<endl;
			cout<<"firstedgeproperty.srcvid: "<<firstedgeproperty.srcvid<<endl;
			cout<<"lastedgeproperty.srcvid: "<<lastedgeproperty.srcvid<<endl;
			cout<<"bank_col_numedges["<<j<<"]: "<<bank_col_numedges[j]<<endl;
			cout<<"edgeoffsets["<<j<<"][0] + lvertexoutdegrees["<<j<<"][0]: "<<(edgeoffsets[j][0] + lvertexoutdegrees[j][0])<<endl;
			cout<<"edgeoffsets["<<j<<"]["<<lastedgeproperty.srcvid<<"] + lvertexoutdegrees["<<j<<"]["<<lastedgeproperty.srcvid<<"]: "<<(edgeoffsets[j][lastedgeproperty.srcvid] + lvertexoutdegrees[j][lastedgeproperty.srcvid])<<endl;
			cout<<endl;
		}
	cout<<">>> create2Dgraph::checkedges : total number of edges in all banks: "<<totalcount<<endl;
	return;
}
#endif 




