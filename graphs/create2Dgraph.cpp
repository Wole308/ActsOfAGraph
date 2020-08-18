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
#include "../src/host_common.h"
#include "create2Dgraph.h"
using namespace std;
#define YES
#define THRESHOLDLINECNT 10000000

create2Dgraph::create2Dgraph(graph * _graphobj, unsigned int _startbank, unsigned int _numbanks){
	graphobj = _graphobj;
	dataset = graphobj->getdataset();
	
	cout<<"create2Dgraph:: constructor called: "<<endl;
	cout<<"create2Dgraph:: dataset.graph_path: "<<graphobj->getdataset().graph_path<<endl;
	cout<<"create2Dgraph::  dataset.vertices_path: "<<graphobj->getdataset().vertices_path<<endl;
	cout<<"create2Dgraph::  dataset.vertices_path_bin: "<<graphobj->getdataset().vertices_path_bin<<endl;
	cout<<"create2Dgraph::  dataset.edges_path: "<<graphobj->getdataset().edges_path<<endl;
	cout<<"create2Dgraph::  dataset.edges_path_bin: "<<graphobj->getdataset().edges_path_bin<<endl;
	cout<<"create2Dgraph::  dataset.graphorder: "<<graphobj->getdataset().graphorder<<endl;
	cout<<"create2Dgraph::  _startbank: "<<_startbank<<endl;
	cout<<"create2Dgraph::  _numbanks: "<<_numbanks<<endl;
	cout<<endl;
	
	startbank = _startbank;
	numbanks = _numbanks;

	numvertexbanks = graphobj->getnumvertexbanks();
	numedgebanks = graphobj->getnumedgebanks();
	numverticespervertexbank = KVDATA_RANGE / graphobj->getnumvertexbanks();
	
	cout<<"create2Dgraph::create2Dgraph: initializing statistics variables..."<<endl;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ totalnumedgeswritten[i][j] = 0; }}	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ globaltolocalvid_weof[i] = 0; }
	
	cout<<"create2Dgraph::create2Dgraph: initializing lvertexoutdegrees and edgeoffsets..."<<endl;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			lvertexoutdegrees[i-startbank][j] = new edge_t[numverticespervertexbank];
			for(unsigned int k=0; k<numverticespervertexbank; k++){ lvertexoutdegrees[i-startbank][j][k] = 0; }
			bank_col_numvertices[i-startbank][j] = 0;
			edgeoffsets[i-startbank][j] = new xvertexoffset_t[numverticespervertexbank];
			bank_col_numedges[i-startbank][j] = 0;
		}
	}
	
	cout<<"create2Dgraph::create2Dgraph: opening files..."<<endl;
	graphobj->openedgesfileforwriting(startbank, numbanks, numedgebanks);
	graphobj->openedgeoffsetsfileforwriting(startbank, numbanks, numedgebanks);
	
	graphobj->openedgesfileforreading(startbank, numbanks, numedgebanks);
	graphobj->openedgeoffsetsfileforreading(startbank, numbanks, numedgebanks);
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ nvmeFd_edgeproperties_w[i][j] = graphobj->getnvmeFd_edgeproperties_w()[i][j]; }}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ nvmeFd_edgeoffsets_w[i][j] = graphobj->getnvmeFd_edgeoffsets_w()[i][j]; }}
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ nvmeFd_edgeproperties_r[i][j] = graphobj->getnvmeFd_edgeproperties_r()[i][j]; }}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ nvmeFd_edgeoffsets_r[i][j] = graphobj->getnvmeFd_edgeoffsets_r()[i][j]; }}
	
	utilityobj = new utility();
	cout<<"create2Dgraph::create2Dgraph: constructor finished"<<endl;
}
create2Dgraph::~create2Dgraph() {
	cout<<"~create2Dgraph:: destructor called... "<<endl;
	/** graphobj->closeedgesfileforwriting(startbank, numbanks, numedgebanks);
	graphobj->closeedgeoffsetsfileforwriting(startbank, numbanks, numedgebanks);	
	graphobj->closeedgesfileforreading(startbank, numbanks, numedgebanks);
	graphobj->closeedgeoffsetsfileforreading(startbank, numbanks, numedgebanks); */
	
	cout<<"create2Dgraph::~create2Dgraph deleting dynamic memories... "<<endl;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){ 
			delete [] lvertexoutdegrees[i-startbank][j]; 
			delete [] edgeoffsets[i-startbank][j];
		}
	}
	#ifdef STREAMEDGESSETUP
	clearedges<edgeprop1_t>(edgepropertiesbuffer);
	#else 
	clearedges<edgeprop2_t>(edgepropertiesbuffer);	
	#endif
} 
void create2Dgraph::reload(unsigned int _startbank, unsigned int _numbanks){
	cout<<"create2Dgraph::reload constructor called: "<<endl;
	cout<<"create2Dgraph::reload dataset.graph_path: "<<graphobj->getdataset().graph_path<<endl;
	cout<<"create2Dgraph::reload dataset.vertices_path: "<<graphobj->getdataset().vertices_path<<endl;
	cout<<"create2Dgraph::reload dataset.vertices_path_bin: "<<graphobj->getdataset().vertices_path_bin<<endl;
	cout<<"create2Dgraph::reload dataset.edges_path: "<<graphobj->getdataset().edges_path<<endl;
	cout<<"create2Dgraph::reload dataset.edges_path_bin: "<<graphobj->getdataset().edges_path_bin<<endl;
	cout<<"create2Dgraph::reload dataset.graphorder: "<<graphobj->getdataset().graphorder<<endl;
	cout<<"create2Dgraph::reload _startbank: "<<_startbank<<endl;
	cout<<"create2Dgraph::reload _numbanks: "<<_numbanks<<endl;
	cout<<endl;
	
	startbank = _startbank;
	numbanks = _numbanks;

	numvertexbanks = graphobj->getnumvertexbanks();
	numedgebanks = graphobj->getnumedgebanks();
	numverticespervertexbank = KVDATA_RANGE / graphobj->getnumvertexbanks();
	
	cout<<"create2Dgraph::reload: initializing statistics variables..."<<endl;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ totalnumedgeswritten[i][j] = 0; }}	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ globaltolocalvid_weof[i] = 0; }
	
	cout<<"create2Dgraph::reload: initializing lvertexoutdegrees and edgeoffsets..."<<endl;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			for(unsigned int k=0; k<numverticespervertexbank; k++){ lvertexoutdegrees[i-startbank][j][k] = 0; }
		}
	}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			bank_col_numvertices[i-startbank][j] = 0;
		}
	}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			bank_col_numedges[i-startbank][j] = 0;
		}
	}
	
	cout<<"create2Dgraph::reload: opening files..."<<endl;
	graphobj->openedgesfileforwriting(startbank, numbanks, numedgebanks);
	graphobj->openedgeoffsetsfileforwriting(startbank, numbanks, numedgebanks);
	
	graphobj->openedgesfileforreading(startbank, numbanks, numedgebanks);
	graphobj->openedgeoffsetsfileforreading(startbank, numbanks, numedgebanks);
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ nvmeFd_edgeproperties_w[i][j] = graphobj->getnvmeFd_edgeproperties_w()[i][j]; }}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ nvmeFd_edgeoffsets_w[i][j] = graphobj->getnvmeFd_edgeoffsets_w()[i][j]; }}
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ nvmeFd_edgeproperties_r[i][j] = graphobj->getnvmeFd_edgeproperties_r()[i][j]; }}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ nvmeFd_edgeoffsets_r[i][j] = graphobj->getnvmeFd_edgeoffsets_r()[i][j]; }}
	
	for(unsigned int i=0; i<MAXNUMEDGEBANKS; i++){ for(unsigned int j=0; j<MAXNUMEDGEBANKS; j++){ edgepropertiesbuffer[i][j].clear(); }}
	cout<<"create2Dgraph::create2Dgraph: constructor finished"<<endl;
}

void create2Dgraph::start(){
	std::ifstream file_graph(dataset.graph_path);

	vertex_t srcv = 0;
	vertex_t dstv = 0;
	edge_t linecount = 0;
	
	if (file_graph.is_open()) {
		std::string line;
		while (getline(file_graph, line)) {
			if (line.find("%") == 0){ continue; }
			if (linecount == 0){ linecount++; continue; } // first entry for flickr is stats
			if ((linecount % 1000000) == 0){ cout<<"create2Dgraph:: edge: ["<<srcv<<","<<dstv<<"]. linecount: "<<linecount<<endl; }
			// if (linecount >= 1024){ cout<<"create2Dgraph:: test ended."<<endl; exit(EXIT_SUCCESS); } 
			// linecount++; // REMOVEME
			// if ((linecount % 1000000) == 0){ cout<<"create2Dgraph:: edge: ["<<srcv<<","<<dstv<<"]. linecount: "<<linecount<<endl; break; } // FIXME.
			
			if(dataset.graphorder == SRC_DST){
				sscanf(line.c_str(), "%i %i", &srcv, &dstv);
			} else {
				sscanf(line.c_str(), "%i %i", &dstv, &srcv);
			}
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph: srcv: "<<srcv<<", dstv: "<<dstv<<endl;
			#endif 
			
			if(srcv > dataset.num_vertices){ cout<<"create2Dgraph::start:: source vertex found greater than number of vertices spacified in dataset. srcv: "<<srcv<<", dataset.num_vertices: "<<dataset.num_vertices<<endl; exit(EXIT_FAILURE); }
			if(dstv > dataset.num_vertices){ cout<<"create2Dgraph::start:: destination vertex found greater than number of vertices spacified in dataset. dstv: "<<dstv<<", dataset.num_vertices: "<<dataset.num_vertices<<endl; exit(EXIT_FAILURE); }
	
			unsigned int bank = getbank(srcv);
			unsigned int col = getbank(dstv);
			
			if((bank < startbank) || (bank >= (startbank + numbanks))){ continue; }
			
			vertex_t local_srcvid = getlocalvid(bank, srcv); // local to the source vertex's bank
			vertex_t local_dstvid = getlocalvid(col, dstv); // local to the destination vertex's bank
			if (linecount < 16){ cout<<"create2Dgraph:: ledge: ["<<local_srcvid<<","<<local_dstvid<<"], gedge: ["<<srcv<<","<<dstv<<"]"<<endl; }
			
			#ifdef STREAMEDGESSETUP
			edgeprop1_t ledgeproperty;
			#else 
			edgeprop2_t ledgeproperty;	
			#endif 
			// T ledgeproperty;
			#ifndef STREAMEDGESSETUP
			ledgeproperty.srcvid = local_srcvid;
			#endif 
			ledgeproperty.dstvid = local_dstvid;
			
			edgepropertiesbuffer[bank][col].push_back(ledgeproperty); 
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph: srcv: "<<srcv<<", dstv: "<<dstv<<", local_srcvid[^"<<bank<<"]: "<<local_srcvid<<", local_dstvid[^"<<col<<"]: "<<local_dstvid<<endl;
			#endif 
			
			unsigned int srcbank = bank;
			if(lvertexoutdegrees[srcbank-startbank][col][local_srcvid] == 0){ bank_col_numvertices[srcbank-startbank][col] += 1; }
			lvertexoutdegrees[srcbank-startbank][col][local_srcvid] += 1;
			bank_col_numedges[srcbank-startbank][col] += 1;

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
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeproperties_w[i][j]); }}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeoffsets_w[i][j]); }}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeproperties_r[i][j]); }}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeoffsets_r[i][j]); }}
	cout<<"create2Dgraph:: finished creating 2D graph from "<<dataset.graph_path<<endl;
	// summary();
	return;
}
void create2Dgraph::summary(){
	edge_t _numwritten = 0;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			_numwritten += totalnumedgeswritten[i][j];
			#ifdef _DEBUGMODE_CHECKS
			cout<<">>> summary:: total number of edges written in volume ["<<i<<","<<j<<"]: "<<totalnumedgeswritten[i][j]<<endl;
			#endif 
		}
	}
	cout<<">>> summary:: total number of edges written: "<<_numwritten<<endl;
	printworkloadstats();
	printglobaltolocaltranslator();
	checkedgeoffsets();
	checkedges();
	checkledges();
	cout<<">>> ===================== finished creating 2D graph ============================== "<<endl<<endl;
	return;
}

void create2Dgraph::writeedgeoffsetstofile(){
	cout<<"create2Dgraph:: writeedgeoffsetstofile... "<<endl;
	vertex_t totalnumvertices = 0;
	generateedgeoffsets();	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			if(fwrite(edgeoffsets[i-startbank][j], (numverticespervertexbank * sizeof(xvertexoffset_t)), 1, nvmeFd_edgeoffsets_w[i][j]) == 0){ cout<<"ERROR:writeedgeoffsetstofile: fwrite error 34"<<endl; exit(EXIT_FAILURE); }			
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::writeedgeoffsetstofile: number of vertices out-degree information written to bank["<<i<<"]["<<j<<"]: "<<numverticespervertexbank<<endl;
			#endif 
			totalnumvertices += numverticespervertexbank;
		}
	}
	cout<<">>> create2Dgraph::writeedgeoffsetstofile:: total number of vertices out-degree information written to all ["<<numedgebanks<<"]x["<<numedgebanks<<"] banks: "<<totalnumvertices<<endl;
	return;
}
void create2Dgraph::generateedgeoffsets(){
	cout<<"create2Dgraph:: generateedgeoffsets... "<<endl;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){			
			// #ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph:: generateedgeoffsets: bank_col_numvertices["<<i-startbank<<"]["<<j<<"]: "<<bank_col_numvertices[i-startbank][j]<<endl;
			// #endif
			edgeoffsets[i-startbank][j][0] = 0;
			for(unsigned int k=1; k<numverticespervertexbank; k++){
				edgeoffsets[i-startbank][j][k] = edgeoffsets[i-startbank][j][k-1] + lvertexoutdegrees[i-startbank][j][k-1];
			}
		}
	}
	return;
}

void create2Dgraph::writeedgeoffsetbitstofile(){
	cout<<"create2Dgraph:: writeedgeoffsetbitstofile... "<<endl;
	size_t ledgeid = 0;	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			ledgeid = 0;
			unsigned int edgeoffsetbitssz = (bank_col_numedges[i-startbank][j] + (NUMBITSINUNSIGNEDINT - 1)) / NUMBITSINUNSIGNEDINT;
			edgeoffsetbits[i-startbank][j] = new unsigned int[edgeoffsetbitssz];
			for(unsigned int k=0; k<edgeoffsetbitssz; k++){ edgeoffsetbits[i-startbank][j][k] = 0; }
			cout<<">>> create2Dgraph::writeedgeoffsetbitstofile:: bank_col_numedges["<<i-startbank<<"]["<<j<<"]: "<<bank_col_numedges[i-startbank][j]<<", edgeoffsetbitssz: "<<edgeoffsetbitssz<<endl;
		
			for(unsigned int k=0; k<numverticespervertexbank; k++){
				for(unsigned int n=0; n<lvertexoutdegrees[i-startbank][j][k]; n++){
					if((ledgeid / NUMBITSINUNSIGNEDINT) >= edgeoffsetbitssz){ cout<<"create2Dgraph::writeedgeoffsetbitstofile:: ERROR 34. ledgeid >= edgeoffsetbitssz. ledgeid: "<<ledgeid<<", edgeoffsetbitssz: "<<edgeoffsetbitssz<<endl; exit(EXIT_FAILURE); }
					if(n==0){ utilityobj->InsertBit(edgeoffsetbits[i-startbank][j], ledgeid, 1); }
					else { utilityobj->InsertBit(edgeoffsetbits[i-startbank][j], ledgeid, 0); }
					ledgeid += 1;
					if(ledgeid > bank_col_numedges[i-startbank][j]){ cout<<"create2Dgraph::writeedgeoffsetbitstofile:: ERROR 34. ledgeid >= bank_col_numedges["<<i-startbank<<"]["<<j<<"]. ledgeid: "<<ledgeid<<", bank_col_numedges["<<i-startbank<<"]["<<j<<"]: "<<bank_col_numedges[i-startbank][j]<<", lvertexoutdegrees["<<i-startbank<<"]["<<j<<"]["<<k<<"]: "<<lvertexoutdegrees[i-startbank][j][k]<<endl; exit(EXIT_FAILURE); }					
				}
			}
			cout<<"create2Dgraph::writeedgeoffsetbitstofile:: edgeoffsetbitssz: "<<edgeoffsetbitssz<<endl;
			if(fwrite(edgeoffsetbits[i-startbank][j], sizeof(unsigned int), edgeoffsetbitssz, nvmeFd_edgeoffsets_w[i][j]) == 0){ cout<<"--- ERROR:writeedgeoffsetstofile: fwrite error 34"<<endl; exit(EXIT_FAILURE); }	
			// delete edgeoffsetbits[i][j];
		}
	}
	cout<<">>> create2Dgraph::writeedgeoffsetbitstofile:: finished. ledgeid: "<<ledgeid<<endl;
	// exit(EXIT_SUCCESS);
	return;
}
template <class T>
void create2Dgraph::writeedgestofile(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS]){
	#ifdef _DEBUGMODE_CHECKS
	cout<<"create2Dgraph:: writeedgestofile... "<<endl;
	#endif 

	edge_t totalnumedges = 0;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			totalnumedgeswritten[i][j] += edgepropertiesbuffer[i][j].size();
			totalnumedges += edgepropertiesbuffer[i][j].size();
			if(edgepropertiesbuffer[i][j].size() > 0){ if(fwrite(edgepropertiesbuffer[i][j].data(), (edgepropertiesbuffer[i][j].size() * sizeof(T)), 1, nvmeFd_edgeproperties_w[i][j]) == 0){ cout<<"ERROR:writetofile: fwrite error 34"<<endl; exit(EXIT_FAILURE); }}
		}
	}
	cout<<">>> create2Dgraph::writeedgestofile:: total number of edges written to all ["<<numedgebanks<<"]x["<<numedgebanks<<"] banks: "<<totalnumedges<<endl;
	return;
}

vertex_t create2Dgraph::getlocalvid(unsigned int bank, vertex_t globalvid){
	#ifdef _DEBUGMODE_CHECKS
	cout<<"getlocalvid: bank: "<<bank<<", globalvid: "<<globalvid<<", globaltolocalvid_weof["<<bank<<"]: "<<globaltolocalvid_weof[bank]<<endl;
	#endif 
	vertex_t localvid;

	if(globalvid < (bank * hceildiv(dataset.num_vertices, numedgebanks))){ cout<<"create2Dgraph::getlocalvid:: ERROR 35. globalvid < (bank * hceildiv(dataset.num_vertices, numedgebanks)). globalvid: "<<globalvid<<", bank: "<<bank<<", number of vertices per workload bank: "<<hceildiv(dataset.num_vertices, numedgebanks)<<endl; exit(EXIT_FAILURE); }
	localvid = (globalvid - (bank * hceildiv(dataset.num_vertices, numedgebanks)));	
	
	return localvid;
}
unsigned int create2Dgraph::getbank(vertex_t vertexid){
	#ifdef _DEBUGMODE_CHECKS
	cout<<"create2Dgraph::getbank: vertexid: "<<vertexid<<", dataset.num_vertices: "<<dataset.num_vertices<<", numedgebanks: "<<numedgebanks<<endl;
	#endif 
	
	unsigned int bank = vertexid / hceildiv(dataset.num_vertices, numedgebanks);
	if(bank >= numedgebanks){ cout<<"create2Dgraph:: ERROR 32. invalid bank. bank: "<<bank<<", vertexid: "<<vertexid<<", dataset.num_vertices: "<<dataset.num_vertices<<", numedgebanks: "<<numedgebanks<<endl; exit(EXIT_FAILURE); }  
	return bank;	
}
template <class T>
void create2Dgraph::clearedges(std::vector<T> (&edgepropertiesbuffer)[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS]){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			edgepropertiesbuffer[i][j].clear();
		}
	}
	return;
}
size_t create2Dgraph::hceildiv(size_t val1, size_t val2){
	return (val1 + (val2 - 1)) / val2;
}
void create2Dgraph::printworkloadstats(){
	edge_t totalnumvertices = 0;
	edge_t totalnumedges = 0;
	edge_t totalnumedges2 = 0;
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::printworkloadstats:: bank_col_numvertices["<<i-startbank<<"]["<<j<<"]: "<<bank_col_numvertices[i-startbank][j]<<endl;
			#endif 
			totalnumvertices += bank_col_numvertices[i-startbank][j];
		}
	}
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){	
			for(unsigned int k=0; k<numverticespervertexbank; k++){	
				#ifdef _DEBUGMODE_CHECKS
				if(k < 4){ cout<<"create2Dgraph::printworkloadstats:: lvertexoutdegrees["<<i-startbank<<"]["<<j<<"]["<<k<<"]: "<<lvertexoutdegrees[i-startbank][j][k]<<endl; }
				#endif 
				totalnumedges += lvertexoutdegrees[i-startbank][j][k];
			}
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::printworkloadstats ... "<<endl;
			#endif 
		}
	}
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::printworkloadstats:: bank_col_numedges["<<i-startbank<<"]["<<j<<"]: "<<bank_col_numedges[i-startbank][j]<<endl;
			#endif 
			totalnumedges2 += bank_col_numedges[i-startbank][j];
		}
	}
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		#ifdef _DEBUGMODE_CHECKS // NB: all banks have the same number of local vertices
		cout<<"create2Dgraph::printworkloadstats:: globaltolocalvid_weof["<<i<<"]: "<<globaltolocalvid_weof[i]<<endl;
		#endif 
	}
	
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		#ifdef _DEBUGMODE_CHECKS
		cout<<"create2Dgraph:: globaltolocalvid_weof["<<i<<"]: "<<globaltolocalvid_weof[i]<<endl;
		#endif 
		for(unsigned int j=0; j<numedgebanks; j++){	
			#ifdef _DEBUGMODE_CHECKS
			cout<<"$$$ create2Dgraph:: edgeoffsets["<<i-startbank<<"]["<<j<<"]["<<(globaltolocalvid_weof[i] - 1)<<"].offset: "<<edgeoffsets[i-startbank][j][(globaltolocalvid_weof[i] - 1)].offset<<", bank_col_numedges["<<i-startbank<<"]["<<j<<"]: "<<bank_col_numedges[i-startbank][j]<<endl;	
			#endif 
		}
	}
	
	cout<<">>> create2Dgraph::printworkloadstats:: total number of vertices out-degree information written to all banks: "<<totalnumvertices<<endl;
	cout<<">>> create2Dgraph::printworkloadstats:: total number of edges written to all banks: "<<totalnumedges<<endl;
	cout<<">>> create2Dgraph::printworkloadstats:: total number of edges written to all banks: "<<totalnumedges2<<endl;
	return;
}
void create2Dgraph::printglobaltolocaltranslator(){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		#ifdef _DEBUGMODE_CHECKS
		cout<<"create2Dgraph::printglobaltolocaltranslator:: globaltolocalvid_weof["<<i<<"]: "<<globaltolocalvid_weof[i]<<endl;
		#endif 
		for(unsigned int j=0; j<globaltolocalvid_weof[i]; j++){
			#ifdef _DEBUGMODE_CHECKS
			if(j<4){ cout<<"create2Dgraph::printglobaltolocaltranslator:: globaltolocalvid["<<i<<"]["<<j<<"]: "<<globaltolocalvid[i][j]<<endl; }
			#endif 
		}
	}
	return;
}
#ifdef STREAMEDGESSETUP
void create2Dgraph::checkedgeoffsets(){
	cout<<"create2Dgraph::checkedgeoffsets started. "<<endl;
	/* vertex_t totalcount = 0;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			size_t count = 0;
			char data;
			while(fread(&data, sizeof(char), 1, nvmeFd_edgeoffsets_r[i][j])){
				// #ifdef _DEBUGMODE_CHECKS
				if(count < 4){ for(unsigned int k=0; k<NUMBITSINCHAR; k++){ cout<<READFROM(data, k, 1)<<", "; } cout<<endl; }
				// #endif
				count += 1;
			}
		}
	}
	cout<<">>> create2Dgraph::checkedgeoffsets : total number of vertices (local) in all banks: "<<totalcount<<endl;
	// exit(EXIT_SUCCESS); */
	return;
}
#else 
void create2Dgraph::checkedgeoffsets(){
	cout<<"create2Dgraph::checkedgeoffsets started. dataset.num_vertices: "<<dataset.num_vertices<<endl;	
	vertex_t totalcount = 0;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){			
			xvertexoffset_t vertexoffset;
			vertex_t count = 0;
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::checkedgeoffsets : reading file ["<<i<<"]["<<j<<"]"<<graphobj->getpath_edgeoffsets(i, j)<<endl;
			#endif 
			while(fread(&vertexoffset, sizeof(xvertexoffset_t), 1, nvmeFd_edgeoffsets_r[i][j])){
				#ifdef _DEBUGMODE_CHECKS
				if(count < 8){
					cout<<"vertexoffset: "<<vertexoffset;
					cout<<endl;	}
				#endif 
				if(vertexoffset > bank_col_numedges[i-startbank][j]){ cout<<"ERROR 34:create2Dgraph::checkedgeoffsets: vertexoffset out of range. vertexoffset: "<<vertexoffset<<", bank_col_numedges["<<i-startbank<<"]["<<j<<"]: "<<bank_col_numedges[i-startbank][j]<<endl; exit(EXIT_FAILURE); }
				
				count+=1;
				totalcount += 1;
			}
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::checkedgeoffsets : count: "<<count<<", bank_col_numedges["<<i-startbank<<"]["<<j<<"]: "<<bank_col_numedges[i-startbank][j]<<endl;
			#endif 
		}
	}
	cout<<">>> create2Dgraph::checkedgeoffsets : total number of vertices (local) in all banks: "<<totalcount<<endl;
	return;
}
#endif 
#ifdef STREAMEDGESSETUP
void create2Dgraph::checkedges(){ return; }
#else 
void create2Dgraph::checkedges(){
	#ifdef _DEBUGMODE_CHECKS
	cout<<"create2Dgraph::checkedges started. (dataset.num_vertices / numedgebanks): "<<hceildiv(dataset.num_vertices, numedgebanks)<<endl;
	#endif
	edge_t totalcount = 0;
	
	edgeprop2_t firstedgeproperty; bool firstedgepropertyseen = false; //
	edgeprop2_t lastedgeproperty; //
	
	edgeprop2_t prevedgeproperty;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			edgeprop2_t edgeproperty;
			edge_t count = 0;
			// #ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::checkedges : reading file ["<<i<<"]["<<j<<"]: "<<graphobj->getpath_edgesproperties(i, j)<<endl;
			// #endif 
			while(fread(&edgeproperty, sizeof(edgeprop2_t), 1, nvmeFd_edgeproperties_r[i][j])){
				// #ifdef _DEBUGMODE_CHECKS
				if(count < 16){
					cout<<"edgeproperty.srcvid: "<<edgeproperty.srcvid<<", ";
					cout<<"edgeproperty.dstvid: "<<edgeproperty.dstvid;
					cout<<endl;	}
				// #endif 
				
				if(firstedgepropertyseen == false){ firstedgepropertyseen = true; firstedgeproperty = edgeproperty; }
				
				if(edgeproperty.srcvid > hceildiv(dataset.num_vertices, numedgebanks)){ cout<<"ERROR 34:create2Dgraph::checkedges: srcvid out of range. edgeproperty.srcvid: "<<edgeproperty.srcvid<<", dataset.num_vertices: "<<dataset.num_vertices<<", numedgebanks: "<<numedgebanks<<endl; exit(EXIT_FAILURE); }
				if(edgeproperty.dstvid > hceildiv(dataset.num_vertices, numedgebanks)){ cout<<"ERROR 34:create2Dgraph::checkedges: dstvid out of range. edgeproperty.dstvid: "<<edgeproperty.dstvid<<", dataset.num_vertices: "<<dataset.num_vertices<<", numedgebanks: "<<numedgebanks<<endl; exit(EXIT_FAILURE); }
				if((edgeproperty.srcvid < prevedgeproperty.srcvid) && (count > 0)){ cout<<"ERROR 34:create2Dgraph::checkedges: edgeproperty.srcvid < prevedgeproperty.srcvid. edgeproperty.srcvid: "<<edgeproperty.srcvid<<", prevedgeproperty.srcvid: "<<prevedgeproperty.srcvid<<", count: "<<count<<", totalcount: "<<totalcount<<endl; exit(EXIT_FAILURE); }
				prevedgeproperty = edgeproperty;
				
				count+=1;
				totalcount += 1;
				// if(count >= 8){ break; }
			}
			#ifdef _DEBUGMODE_CHECKS
			cout<<"create2Dgraph::checkedges : count: "<<count<<endl;
			#endif 
			lastedgeproperty = edgeproperty;
			
			if((edgeoffsets[i][j][lastedgeproperty.srcvid] + lvertexoutdegrees[i][j][lastedgeproperty.srcvid]) != bank_col_numedges[i][j]){ 
				cout<<"ERROR 34:create2Dgraph::checkedges: ERROR 34. (firstedgeproperty.srcvid - lastedgeproperty.srcvid) != bank_col_numedges[i][j] "<<endl;
				cout<<"firstedgeproperty.srcvid: "<<firstedgeproperty.srcvid<<endl;
				cout<<"lastedgeproperty.srcvid: "<<lastedgeproperty.srcvid<<endl;
				cout<<"bank_col_numedges["<<i<<"]["<<j<<"]: "<<bank_col_numedges[i][j]<<endl;
				cout<<"edgeoffsets["<<i<<"]["<<j<<"][0] + lvertexoutdegrees["<<i<<"]["<<j<<"][0]: "<<(edgeoffsets[i][j][0] + lvertexoutdegrees[i][j][0])<<endl;
				cout<<"edgeoffsets["<<i<<"]["<<j<<"]["<<lastedgeproperty.srcvid<<"] + lvertexoutdegrees["<<i<<"]["<<j<<"]["<<lastedgeproperty.srcvid<<"]: "<<(edgeoffsets[i][j][lastedgeproperty.srcvid] + lvertexoutdegrees[i][j][lastedgeproperty.srcvid])<<endl;
				cout<<endl;
			}
			cout<<"SUCCESS:create2Dgraph::checkedges: (firstedgeproperty.srcvid - lastedgeproperty.srcvid) == bank_col_numedges[i][j] "<<endl;
			cout<<"firstedgeproperty.srcvid: "<<firstedgeproperty.srcvid<<endl;
			cout<<"lastedgeproperty.srcvid: "<<lastedgeproperty.srcvid<<endl;
			cout<<"bank_col_numedges["<<i<<"]["<<j<<"]: "<<bank_col_numedges[i][j]<<endl;
			cout<<"edgeoffsets["<<i<<"]["<<j<<"][0] + lvertexoutdegrees["<<i<<"]["<<j<<"][0]: "<<(edgeoffsets[i][j][0] + lvertexoutdegrees[i][j][0])<<endl;
			cout<<"edgeoffsets["<<i<<"]["<<j<<"]["<<lastedgeproperty.srcvid<<"] + lvertexoutdegrees["<<i<<"]["<<j<<"]["<<lastedgeproperty.srcvid<<"]: "<<(edgeoffsets[i][j][lastedgeproperty.srcvid] + lvertexoutdegrees[i][j][lastedgeproperty.srcvid])<<endl;
			cout<<endl;
		}
	}
	cout<<">>> create2Dgraph::checkedges : total number of edges in all banks: "<<totalcount<<endl;
	return;
}
#endif 
void create2Dgraph::checkledges(){
	unsigned int ledgeid = 0;
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){	
			ledgeid = 0;
			for(unsigned int k=0; k<numverticespervertexbank; k++){
					ledgeid += lvertexoutdegrees[i][j][k];
			}
			cout<<"create2Dgraph:: ledgeid: "<<ledgeid<<", bank_col_numedges["<<i<<"]["<<j<<"]: "<<bank_col_numedges[i][j]<<endl;
			if(ledgeid > bank_col_numedges[i][j]){ cout<<"create2Dgraph::generateedgeoffsetbits:: ERROR 34. ledgeid >= bank_col_numedges["<<i<<"]["<<j<<"]. ledgeid: "<<ledgeid<<", bank_col_numedges["<<i<<"]["<<j<<"]: "<<bank_col_numedges[i][j]<<endl; exit(EXIT_FAILURE); }
		}
	}
	return;
}






