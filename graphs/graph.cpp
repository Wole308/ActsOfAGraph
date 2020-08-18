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
#include <vector> 

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <fstream> // looks like we need this too (edit by π)

#include "graph.h"
#include "../heuristics/heuristics.h"
#include "../algorithm/algorithm.h"
#include "../src/common.h"
#include "../src/host_common.h"
using namespace std;

#ifdef AWS_PLATFORM
string datasetRootDir = "/oj2zf/";
string tmpDatarootDir = "/oj2zf/";
#else
string datasetRootDir = "/localtmp/oj2zf/";
string tmpDatarootDir = "/localtmp/oj2zf/";
#endif
	
graph::graph(algorithm * _algorithmobj, unsigned int datasetid, unsigned int _numvertexbanks, unsigned int _numedgebanks){
	cout<<"graph::graph:: datasetid: "<<datasetid<<", _numvertexbanks: "<<_numvertexbanks<<", _numedgebanks: "<<_numedgebanks<<endl;
	algorithmobj = _algorithmobj;
	loadalldatasets();
	setdataset(datasetid);
	
	numvertexbanks = _numvertexbanks;
	numedgebanks = _numedgebanks;
	numverticespervertexbank = KVDATA_RANGE / numvertexbanks;
	
	nvmeFd_verticesdata_r2 = new int[MAXNUMEDGEBANKS]; // temporary vertex data
	nvmeFd_verticesdata_w2 = new int[MAXNUMEDGEBANKS];
	nvmeFd_vertexoutdegrees_r2 = new int[MAXNUMEDGEBANKS]; 
	nvmeFd_vertexproperties_r2 = new int[MAXNUMEDGEBANKS]; // vertex properties
	nvmeFd_vertexproperties_w2 = new int[MAXNUMEDGEBANKS];	
	nvmeFd_edgeproperties_r2 = new int*[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeproperties_r2[i] = new int[MAXNUMEDGEBANKS]; } // edge properties
	nvmeFd_edgeproperties_w2 = new int*[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeproperties_w2[i] = new int[MAXNUMEDGEBANKS]; }	
	nvmeFd_edgeproperties_r = new FILE**[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeproperties_r[i] = new FILE*[MAXNUMEDGEBANKS]; }
	nvmeFd_edgeproperties_w = new FILE**[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeproperties_w[i] = new FILE*[MAXNUMEDGEBANKS]; }	
	nvmeFd_edgeoffsets_r2 = new int*[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeoffsets_r2[i] = new int[MAXNUMEDGEBANKS]; } // edge offsets
	nvmeFd_edgeoffsets_w2 = new int*[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeoffsets_w2[i] = new int[MAXNUMEDGEBANKS]; }	
	nvmeFd_edgeoffsets_r = new FILE**[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeoffsets_r[i] = new FILE*[MAXNUMEDGEBANKS]; }
	nvmeFd_edgeoffsets_w = new FILE**[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeoffsets_w[i] = new FILE*[MAXNUMEDGEBANKS]; }	
	// nvmeFd_activevertexids_r2 = new int[MAXNUMVERTEXBANKS]; // active vertices
	// nvmeFd_activevertexids_w2 = new int[MAXNUMVERTEXBANKS];
	// nvmeFd_activevertexids_w = new FILE*[MAXNUMVERTEXBANKS];
	nvmeFd_vertexupdates_w = new FILE*[MAXNUMEDGEBANKS]; // temporary vertex updates
}
graph::graph(unsigned int datasetid, vertex_t vbatchsz){
	cout<<"graph::graph:: datasetid: "<<datasetid<<", vbatchsz: "<<vbatchsz<<endl;
	loadalldatasets();
	setdataset(datasetid);
	
	heuristics * heuristicsobj = new heuristics();
	numvertexbanks = heuristicsobj->getdefaultnumvertexbanks();
	numedgebanks = heuristicsobj->getdefaultnumedgebanks();
	numverticespervertexbank = KVDATA_RANGE / numvertexbanks;
}
graph::~graph(){}

size_t graph::get_num_vertices(){
	return thisdataset.num_vertices;
}
size_t graph::get_num_edges(){
	return thisdataset.num_edges;
}
size_t graph::getnumedgebanks(){
	return numedgebanks;
}
void graph::setnumedgebanks(unsigned int _numedgebanks){
	numedgebanks = _numedgebanks;
}
size_t graph::getnumvertexbanks(){
	return numvertexbanks;
}
void graph::setnumvertexbanks(unsigned int _numvertexbanks){
	numvertexbanks = _numvertexbanks;
}
void graph::setbanks(unsigned int _numvertexbanks, unsigned int _numedgebanks){
	numvertexbanks = _numvertexbanks;
	numedgebanks = _numedgebanks;
	numverticespervertexbank = KVDATA_RANGE / numvertexbanks;
}

void graph::openfilesforreading(){
	cout<<"graph::openfilesforreading : opening files for reading"<<endl;
	// vertex offsets (pread)
	for(unsigned int i=0; i<numedgebanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgeoffsets(i, j); 	
			nvmeFd_edgeoffsets_r2[i][j] = open(path.c_str(), O_RDONLY);
			if (nvmeFd_edgeoffsets_r2[i][j] < 0) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
	// edge properties (pread)
	for(unsigned int i=0; i<numedgebanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgesproperties(i, j);
			nvmeFd_edgeproperties_r2[i][j] = open(path.c_str(), O_RDONLY); 
			if (nvmeFd_edgeproperties_r2[i][j] < 0) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
	return;
}
void graph::opentemporaryfilesforreading(){
	cout<<"graph::opentemporaryfilesforreading : opening temporary files for reading"<<endl;
	// vertex data (pread)
	for(unsigned int i=0; i<numvertexbanks; i++){
		string path = getpath_vertexdata(i); 
		nvmeFd_verticesdata_r2[i] = open(path.c_str(), O_RDONLY); 
		if (nvmeFd_verticesdata_r2[i] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	#ifdef NOTUSED
	// temporary vertex data (pread)
	for(unsigned int i=0; i<numvertexbanks; i++){
		string path = getpath_tempvertexdata(i); 
		nvmeFd_tempverticesdata_r2[i] = open(path.c_str(), O_RDONLY); 
		if (nvmeFd_tempverticesdata_r2[i] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	#endif 
	// vertex properties (pread)
	for(unsigned int i=0; i<numvertexbanks; i++){	
		string path = getpath_vertexproperties(i); 
		nvmeFd_vertexproperties_r2[i] = open(path.c_str(), O_RDONLY); 
		if (nvmeFd_vertexproperties_r2[i] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	return;
}
void graph::opentemporaryfilesforwriting(){
	cout<<"graph::opentemporaryfilesforwriting : opening temporary files for writing..."<<endl;
	// vertex data (pwrite)
	for(unsigned int i=0; i<numvertexbanks; i++){
		string path = getpath_vertexdata(i); 
		std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
		nvmeFd_verticesdata_w2[i] = open(path.c_str(), O_WRONLY); 
		if (nvmeFd_verticesdata_w2[i] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	#ifdef NOTUSED
	// temporary vertex data (pwrite)
	for(unsigned int i=0; i<numvertexbanks; i++){
		string path = getpath_tempvertexdata(i); 
		std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
		nvmeFd_tempverticesdata_w2[i] = open(path.c_str(), O_WRONLY); 
		if (nvmeFd_tempverticesdata_w2[i] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	#endif 
	// vertex properties (pwrite)
	for(unsigned int i=0; i<numvertexbanks; i++){
		string path = getpath_vertexproperties(i); 
		std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();
		nvmeFd_vertexproperties_w2[i] = open(path.c_str(), O_WRONLY); 
		if (nvmeFd_vertexproperties_w2[i] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	return;
}
void graph::closefilesforreading(){
	cout<<"graph::closefilesforreading : close files for reading..."<<endl;
	for(unsigned int i=0; i<numedgebanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_edgeoffsets_r2[i][j]); }
		for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_edgeproperties_r2[i][j]); }
	}
	return;
}
void graph::closetemporaryfilesforreading(){
	cout<<"graph::closetemporaryfilesforreading : close files for reading..."<<endl;
	for(unsigned int i=0; i<numvertexbanks; i++){
		close(nvmeFd_verticesdata_r2[i]);
		#ifdef NOTUSED
		close(nvmeFd_tempverticesdata_r2[i]);
		#endif
		close(nvmeFd_vertexproperties_r2[i]);
	}
	#ifndef PR_ALGORITHM
	close(nvmeFd_activevertexids_r2);	
	#endif
	return;
}
void graph::closetemporaryfilesforwriting(){
	cout<<"graph::closetemporaryfilesforwriting : close temporary files for writing..."<<endl;
	for(unsigned int i=0; i<numvertexbanks; i++){
		close(nvmeFd_verticesdata_w2[i]);
		close(nvmeFd_vertexproperties_w2[i]);
		#ifdef NOTUSED
		close(nvmeFd_tempverticesdata_w2[i]);
		#endif 
	}
	#ifndef PR_ALGORITHM
	close(nvmeFd_activevertexids_w2);
	#endif 
	return;
}

void graph::openactiveverticesfilesforreading(unsigned int graph_iterationidx){
	cout<<"graph::openactiveverticesfilesforreading : open active vertices (iteration "<<graph_iterationidx<<") files for reading ..."<<endl;
	string path = getpath_activevertices(graph_iterationidx);
	nvmeFd_activevertexids_r2 = open(path.c_str(), O_RDONLY); 
	if (nvmeFd_activevertexids_r2 < 0) {
		cout << "ERR: open " << path << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path << endl;
	
	path = getpath_vertexisactive(graph_iterationidx);
	nvmeFd_vertexisactive_r2 = open(path.c_str(), O_RDONLY); 
	if (nvmeFd_vertexisactive_r2 < 0) {
		cout << "ERR: open " << path << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path << endl;
	return;
}
void graph::openactiveverticesfilesforwriting(unsigned int graph_iterationidx){
	cout<<"graph::openactiveverticesfilesforwriting : open active vertices (iteration "<<graph_iterationidx<<") files for writing..."<<endl;
	string path = getpath_activeverticesW(graph_iterationidx);
	std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();
	nvmeFd_activevertexids_w = fopen(path.c_str(), "w"); 
	if (nvmeFd_activevertexids_w == NULL) {
		cout << "ERR: open " << path << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path << endl;
	
	path = getpath_vertexisactive(graph_iterationidx);
	std::ofstream ofs2; ofs2.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs2.close();
	nvmeFd_vertexisactive_w2 = open(path.c_str(), O_WRONLY); 
	if (nvmeFd_vertexisactive_w2 < 0) {
		cout << "ERR: open " << path << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path << endl;
	return;
}
void graph::closeactiveverticesfilesforreading(){
	cout<<"graph::closeactiveverticesfilesforreading : close active vertices files for writing..."<<endl;
	close(nvmeFd_activevertexids_r2);
	close(nvmeFd_vertexisactive_r2);
	return;
}
void graph::closeactiveverticesfilesforwriting(){
	cout<<"graph::closeactiveverticesfilesforwriting : close active vertices files for writing..."<<endl;
	fclose(nvmeFd_activevertexids_w);
	close(nvmeFd_vertexisactive_w2);
	return;
}
void graph::openedgesfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgesproperties(i,j);
			nvmeFd_edgeproperties_w[i][j] = fopen(path.c_str(), "w"); 
			if (nvmeFd_edgeproperties_w[i][j] == NULL) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
}
void graph::openedgeoffsetsfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgeoffsets(i,j); 
			nvmeFd_edgeoffsets_w[i][j] = fopen(path.c_str(), "w"); 
			if (nvmeFd_edgeoffsets_w[i][j] == NULL) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
}
void graph::openedgesfileforreading(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgesproperties(i,j);
			nvmeFd_edgeproperties_r[i][j] = fopen(path.c_str(), "r"); 
			if (nvmeFd_edgeproperties_r[i][j] == NULL) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
}
void graph::openedgeoffsetsfileforreading(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgeoffsets(i,j); 
			nvmeFd_edgeoffsets_r[i][j] = fopen(path.c_str(), "r"); 
			if (nvmeFd_edgeoffsets_r[i][j] == NULL) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
}

void graph::closeedgesfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeoffsets_w[i][j]); }}
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeproperties_w[i][j]); }}
}
void graph::closeedgesfileforreading(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeoffsets_r[i][j]); }}
}
void graph::closeedgeoffsetsfileforwriting(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeoffsets_w[i][j]); }}
}
void graph::closeedgeoffsetsfileforreading(unsigned int startbank, unsigned int numbanks, unsigned int numedgebanks){
	for(unsigned int i=startbank; i<(startbank+numbanks); i++){ for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edgeoffsets_r[i][j]); }}
}

string graph::getpath_vertexdata(unsigned int i){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numvertexbanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + ".vdata";		
}
string graph::getpath_tempvertexdata(unsigned int i){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numvertexbanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + ".tempvdata";		
}
string graph::getpath_vertexproperties(unsigned int i){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numvertexbanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + ".vproperties";		
}
string graph::getpath_edgesproperties(unsigned int i, unsigned int j){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numedgebanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + "_" + std::to_string(j) + ".edges";		
}
string graph::getpath_edgeoffsets(unsigned int i, unsigned int j){
	// #ifdef STREAMEDGESSETUP
	// return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numedgebanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + "_" + std::to_string(j) + ".edgeoffsetbits";		
	// #else 
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numedgebanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + "_" + std::to_string(j) + ".edgeoffsets";		
	// #endif 
}
string graph::getpath_activevertices(unsigned int graph_iterationidx){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + "activevertices" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".actvvertices"; // CORRECTONE.		
	// return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + "activeverticesW" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".actvvertices";
}
string graph::getpath_activeverticesW(unsigned int graph_iterationidx){
	#ifdef FORCEDFINISH_DONTCAREABOUTACTIVEVERTICESGENERATED
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + "activeverticesW" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".actvvertices";	
	#else 
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + "activevertices" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".actvvertices";	
	#endif 
}
string graph::getpath_vertexisactive(unsigned int graph_iterationidx){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + "activeverticesW" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".vertexisactive";	
}
string graph::getpath_vertexupdates(unsigned int i){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numedgebanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + ".vupdates";		
}
string graph::gettmp_dir(){
	return datasetRootDir + "tmpdata_grafboost";		
}
string graph::getidx_path(){
	return datasetRootDir + "datasets/" + thisdataset.graphname + "/ridx.dat";
}
string graph::getmat_path(){
	return datasetRootDir + "datasets/" + thisdataset.graphname + "/matrix.dat";	
}
string graph::gettmppath_parentvid(){
	return datasetRootDir + "tmpdata" + "/" + "parentvids.vertex";
}
string graph::gettmppath_childrenstats(){
	return datasetRootDir + "tmpdata" + "/" + "children.vertex";
}

int * graph::getnvmeFd_verticesdata_r2(){ return nvmeFd_verticesdata_r2; } // vertex data
int * graph::getnvmeFd_verticesdata_w2(){ return nvmeFd_verticesdata_w2; }
int * graph::getnvmeFd_tempverticesdata_r2(){ return nvmeFd_tempverticesdata_r2; } // temporary vertex data
int * graph::getnvmeFd_tempverticesdata_w2(){ return nvmeFd_tempverticesdata_w2; }
int * graph::getnvmeFd_vertexoutdegrees_r2(){ return nvmeFd_vertexoutdegrees_r2; }
int * graph::getnvmeFd_vertexproperties_r2(){ return nvmeFd_vertexproperties_r2; } // vertex properties
int * graph::getnvmeFd_vertexproperties_w2(){ return nvmeFd_vertexproperties_w2; }
int ** graph::getnvmeFd_edgeproperties_r2(){ return nvmeFd_edgeproperties_r2; } // edge properties
int ** graph::getnvmeFd_edgeproperties_w2(){ return nvmeFd_edgeproperties_w2; }
FILE *** graph::getnvmeFd_edgeproperties_r(){ return nvmeFd_edgeproperties_r; }
FILE *** graph::getnvmeFd_edgeproperties_w(){ return nvmeFd_edgeproperties_w; }
int ** graph::getnvmeFd_edgeoffsets_r2(){ return nvmeFd_edgeoffsets_r2; }
int ** graph::getnvmeFd_edgeoffsets_w2(){ return nvmeFd_edgeoffsets_w2; }
FILE *** graph::getnvmeFd_edgeoffsets_r(){ return nvmeFd_edgeoffsets_r; }
FILE *** graph::getnvmeFd_edgeoffsets_w(){ return nvmeFd_edgeoffsets_w; }
int graph::getnvmeFd_activevertexids_r2(){ return nvmeFd_activevertexids_r2; } // active vertices
int graph::getnvmeFd_activevertexids_w2(){ return nvmeFd_activevertexids_w2; }
FILE * graph::getnvmeFd_activevertexids_w(){ return nvmeFd_activevertexids_w; }
int graph::getnvmeFd_vertexisactive_r2(){ return nvmeFd_vertexisactive_r2; } // vertex-is-active 
int graph::getnvmeFd_vertexisactive_w2(){ return nvmeFd_vertexisactive_w2; }
FILE ** graph::getnvmeFd_vertexupdates_w(){ return nvmeFd_vertexupdates_w; } // temporary vertex updates
int graph::gettmpnvmeFd_parents_w2(){ return tmpnvmeFd_parents_w2; } // files for createsmartgraph.cpp
int graph::gettmpnvmeFd_parents_r2(){ return tmpnvmeFd_parents_r2; }
int graph::gettmpnvmeFd_children_w2(){ return tmpnvmeFd_children_w2; }
int graph::gettmpnvmeFd_children_r2(){ return tmpnvmeFd_children_r2; }

void graph::generateverticesdata(){
	cout<<"generating vertices data... "<<endl;
	for(unsigned int i=0; i<numvertexbanks; i++){
		value_t * verticesdata = new value_t[numverticespervertexbank]();
		for(unsigned int k=0; k<numverticespervertexbank; k++){ verticesdata[k] = algorithmobj->vertex_initdata(); } 
		if(pwrite(nvmeFd_verticesdata_w2[i], verticesdata, (size_t)(numverticespervertexbank * sizeof(value_t)), 0) < 0){ cout<<"hostprocess::generateverticesdata::ERROR 36. numvertexbanks: "<<numvertexbanks<<", numverticespervertexbank: "<<numverticespervertexbank<<endl; exit(EXIT_FAILURE); }			
		delete [] verticesdata;
	}
	cout<<"finished generating vertices data"<<endl;
	return;
}
void graph::generatetempverticesdata(){
	cout<<"generating vertices data... "<<endl;
	for(unsigned int i=0; i<numvertexbanks; i++){
		value_t * tempverticesdata = new value_t[numverticespervertexbank]();
		for(unsigned int k=0; k<numverticespervertexbank; k++){ tempverticesdata[k] = algorithmobj->vertex_inittempdata(); } 
		if(pwrite(nvmeFd_tempverticesdata_w2[i], tempverticesdata, (size_t)(numverticespervertexbank * sizeof(value_t)), 0) < 0){ cout<<"hostprocess::generateverticesdata::ERROR 36. numvertexbanks: "<<numvertexbanks<<", numverticespervertexbank: "<<numverticespervertexbank<<endl; exit(EXIT_FAILURE); }			
		delete [] tempverticesdata;
	}
	cout<<"finished generating vertices data"<<endl;
	return;
}
void graph::generatevertexoutdegrees(vertex_t * vertexoutdegrees, unsigned int bank){
	cout<<"graph::generating vertex outdegrees... "<<endl;	
	for(unsigned int i=0; i<numverticespervertexbank; i++){
		vertexoutdegrees[i] = 1; // FIXME.
	}
	return;
}
void graph::generatevertexproperties(){
	cout<<"generating vertex properties... "<<endl;	
	for(unsigned int i=0; i<numvertexbanks; i++){	
		vertex_t * vertexoutdegrees = new vertex_t[numverticespervertexbank];
		generatevertexoutdegrees(vertexoutdegrees, i);
		
		vertexprop_t * vertexproperties = new vertexprop_t[numverticespervertexbank];
		for(unsigned int k=0; k<numverticespervertexbank; k++){
			vertexproperties[k].outdegree = vertexoutdegrees[k];
		}
		if(pwrite(nvmeFd_vertexproperties_w2[i], vertexproperties, (size_t)(numverticespervertexbank * sizeof(vertexprop_t)), 0) < 0){ cout<<"hostprocess::generatevertexproperties::ERROR 36"<<endl; exit(EXIT_FAILURE); }		
		// close(nvmeFd_vertexproperties_w2[i]);
		delete [] vertexoutdegrees;
		delete [] vertexproperties;
	}
	cout<<"finished generating vertex properties data"<<endl;
	return;
}
vertex_t graph::getnumactivevertices(){
	size_t numactivevertices = lseek(nvmeFd_activevertexids_r2, 0, SEEK_END) / sizeof(keyvalue_t);
	cout<<"graph::getnumactivevertices : number of active vertices for current graph iteration : "<<numactivevertices<<endl;
	// exit(EXIT_SUCCESS);
	return numactivevertices;
}
void graph::saveactiveverticestofile(unsigned int graph_iterationidx, int fd){
	cout<<"graph::saveactiveverticestofile : saving active vertices to file..."<<endl;	
	string path = getpath_activeverticesW(graph_iterationidx);
	std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
	int actvfd = open(path.c_str(), O_WRONLY); 
	if (actvfd < 0) {
		cout << "ERR: open " << path << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path << endl;
	
	int n;
    char buf[4096];
	while ((n=read(fd , buf, 4096)) > 0){
		if ( write(actvfd , buf, n) != n )
			printf("Error writing to file.\n");
	}
	cout << "graph::saveactiveverticestofile: Successfully saved active vertices for iteration: " << graph_iterationidx << " to "<<path<<endl;
	return;
}
void graph::saveactiveverticestofile(unsigned int graph_iterationidx, keyvalue_t keyvalue, bool save){
	cout<<"graph::saveactiveverticestofile : saving active vertices to file..."<<endl;	
	string path = getpath_activeverticesW(graph_iterationidx);
	std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
	int actvfd = open(path.c_str(), O_WRONLY); 
	if (actvfd < 0) {
		cout << "ERR: open " << path << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path << endl;
	
	if(save == true){ pwrite(actvfd, &keyvalue, sizeof(keyvalue_t), 0); }
	#ifdef _DEBUGMODE_CHECKS2
	cout << "graph::saveactiveverticestofile: Successfully saved active vertices for iteration: " << graph_iterationidx << " to "<<path<<endl;
	#endif 
	return;
}
void graph::saveactiveverticestofile(FILE * nvmeFd_activevertexids_w, vector<keyvalue_t> & activeverticesbuffer, unsigned int graph_iterationidx){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"graph::saveactiveverticestofile : saving active vertices (graph iteration "<<graph_iterationidx<<") to file..."<<endl;	
	#endif 
	fwrite (&activeverticesbuffer[0], sizeof(keyvalue_t), activeverticesbuffer.size(), nvmeFd_activevertexids_w);
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout << "graph::saveactiveverticestofile: Successfully saved active vertices for iteration: " << graph_iterationidx<<endl;
	#endif 
	return;
}
void graph::savevertexisactiveinfostofile(int fd, unsigned int offset, unsigned int * buffer, vertex_t bufferoffset, vertex_t size, unsigned int graph_iterationidx){
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << "graph::savevertexisactiveinfostofile. writing vertex-is-active-bits (graph iteration "<<graph_iterationidx<<") to file... bufferoffset: "<<bufferoffset<<", size: "<<size<<", offset: "<<offset<<endl;
	#endif 
	
	if(pwrite(fd, &buffer[bufferoffset], ((size / NUMBITSINUNSIGNEDINT) * sizeof(unsigned int)), ((offset / NUMBITSINUNSIGNEDINT) * sizeof(unsigned int))) <= 0){ cout<<"graph::savevertexisactiveinfostofile::ERROR (graph iteration "<<graph_iterationidx<<") 35"<<endl; exit(EXIT_FAILURE); }		

	#ifdef _DEBUGMODE_HOSTPRINTS
	// utilityobj->printbits(fd, offset, 4);
	// exit(EXIT_SUCCESS);
	#endif
	return;
}
void graph::writerootvertextoactiveverticesfiles(keyy_t key, value_t value){
	cout<<"graph::writerootvertextoactiveverticesfiles : write root active vertex."<<endl;
	keyvalue_t kv;
	kv.key = key;
	kv.value = value;
	openactiveverticesfilesforwriting(0);
	fwrite(&kv, sizeof(keyvalue_t), 1, nvmeFd_activevertexids_w);
	closeactiveverticesfilesforwriting();
	cout<<"graph::writerootvertextoactiveverticesfiles : finished writing root active vertex..."<<endl;
}
void graph::loadalldatasets(){
	cout<<"graph::loadalldatasets:: "<<endl;
	
	#ifdef AWS_PLATFORM
	string rootDir = "/oj2zf/";
	#else
	string rootDir = "/localtmp/oj2zf/";
	#endif
	
	_datasets[0].graphname = "holes";
	_datasets[0].graph_path = rootDir + "datasets/holes/holes.graph";
	_datasets[0].vertices_path = rootDir + "datasets/holes/holes.vertices";
	_datasets[0].edges_path =rootDir + "datasets/holes/holes.edges";	
	_datasets[0].vertices_path_bin = rootDir + "datasets/holes/holes_bin.vertices";
	_datasets[0].edges_path_bin = rootDir + "datasets/holes/holes_bin.edges";
	_datasets[0].min_vertex = 0;
	_datasets[0].max_vertex = 0;
	_datasets[0].num_vertices = 0;
	_datasets[0].num_edges = 0;
	_datasets[0].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[0].graphorder = SRC_DST;
	
	_datasets[1].graphname = "flickr";
	_datasets[1].graph_path = rootDir + "datasets/flickr/flickr.graph";
	_datasets[1].vertices_path = rootDir + "datasets/flickr/flickr.vertices";
	_datasets[1].edges_path = rootDir + "datasets/flickr/flickr.edges";
	_datasets[1].vertices_path_bin = rootDir + "datasets/flickr/flickr_bin.vertices";
	_datasets[1].edges_path_bin = rootDir + "datasets/flickr/flickr_bin.edges";
	_datasets[1].min_vertex = 0;
	_datasets[1].max_vertex = 820878;
	_datasets[1].num_vertices = 820879;
	_datasets[1].num_edges = 9837214;
	_datasets[1].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[1].graphorder = DST_SRC;
	
	_datasets[2].graphname = "twitter";
	_datasets[2].graph_path = rootDir + "datasets/twitter/twitter.graph";
	_datasets[2].vertices_path = rootDir + "datasets/twitter/twitter.vertices";
	_datasets[2].edges_path = rootDir + "datasets/twitter/twitter.edges";	
	_datasets[2].vertices_path_bin = rootDir + "datasets/twitter/twitter_bin.vertices"; 
	_datasets[2].edges_path_bin = rootDir + "datasets/twitter/twitter_bin.edges";
	_datasets[2].min_vertex = 0;	
	_datasets[2].max_vertex = 41652230; // -- extract_vertices says 41652240
	_datasets[2].num_vertices = 41652230; // FIXME
	// _datasets[2].max_vertex = 41652240; // -- extract_vertices says 41652240
	// _datasets[2].num_vertices = 41652240; // FIXME
	_datasets[2].num_edges = 1468365184; // 1468365184
	_datasets[2].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[2].graphorder = DST_SRC;
	
	_datasets[4].graphname = "kronecker26";
	_datasets[4].graph_path = rootDir + "datasets/kronecker26/kronecker26.graph";
	_datasets[4].vertices_path = rootDir + "datasets/kronecker26/kronecker26.vertices";
	_datasets[4].edges_path = rootDir + "datasets/kronecker26/kronecker26.edges";	
	_datasets[4].vertices_path_bin = rootDir + "datasets/kronecker26/kronecker26_bin.vertices";
	_datasets[4].edges_path_bin = rootDir + "datasets/kronecker26/kronecker26_bin.edges";
	_datasets[4].min_vertex = 0;
	_datasets[4].max_vertex = 67108864; 
	_datasets[4].num_vertices = 67108864;  // 67108864, 67108816(prev)*
	_datasets[4].num_edges = 1073741824; // 1073741824
	_datasets[4].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[4].graphorder = SRC_DST;
	
	_datasets[5].graphname = "kronecker28";
	_datasets[5].graph_path = rootDir + "datasets/kronecker28/kronecker28.graph";
	_datasets[5].vertices_path = rootDir + "datasets/kronecker28/kronecker28.vertices";
	_datasets[5].edges_path = rootDir + "datasets/kronecker28/kronecker28.edges";	
	_datasets[5].vertices_path_bin = rootDir + "datasets/kronecker28/kronecker28_bin.vertices";
	_datasets[5].edges_path_bin = rootDir + "datasets/kronecker28/kronecker28_bin.edges";
	_datasets[5].min_vertex = 0;
	_datasets[5].max_vertex = 268435456; // 268435456 // 268431360(prev)
	_datasets[5].num_vertices = 268435456; 
	_datasets[5].num_edges = 4294967290;
	_datasets[5].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[5].graphorder = SRC_DST;
	
	_datasets[6].graphname = "kronecker30";
	_datasets[6].graph_path = rootDir + "datasets/kronecker30/kronecker30.graph";
	_datasets[6].vertices_path = rootDir + "datasets/kronecker30/kronecker30.vertices";
	_datasets[6].edges_path = rootDir + "datasets/kronecker30/kronecker30.edges";	
	_datasets[6].vertices_path_bin = rootDir + "datasets/kronecker30/kronecker30_bin.vertices"; 
	_datasets[6].edges_path_bin = rootDir + "datasets/kronecker30/kronecker30_bin.edges"; 
	_datasets[6].min_vertex = 0;
	_datasets[6].max_vertex = 1073741824; // 1073741824 // 1041451012(prev)
	_datasets[6].num_vertices = 1073741824;
	_datasets[6].num_edges = 17179869184;
	_datasets[6].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[6].graphorder = SRC_DST;
	
	_datasets[7].graphname = "kronecker32";
	_datasets[7].graph_path = rootDir + "datasets/kronecker32/kronecker32.graph";
	_datasets[7].vertices_path = rootDir + "datasets/kronecker32/kronecker32.vertices";
	_datasets[7].edges_path = rootDir + "datasets/kronecker32/kronecker32.edges";	
	_datasets[7].vertices_path_bin = rootDir + "datasets/kronecker32/kronecker32_bin.vertices"; 
	_datasets[7].edges_path_bin = rootDir + "datasets/kronecker32/kronecker32_bin.edges"; 
	_datasets[7].min_vertex = 0;
	_datasets[7].max_vertex = 4294967296; 
	_datasets[7].num_vertices = 4294967296;
	_datasets[7].num_edges = 68719476736;
	_datasets[7].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[7].graphorder = SRC_DST;
	
	_datasets[10].graphname = "wdc";
	_datasets[10].graph_path = rootDir + "datasets/wdc/wdc.graph";
	_datasets[10].vertices_path = rootDir + "datasets/wdc/wdc.vertices";
	_datasets[10].edges_path = rootDir + "datasets/wdc/wdc.edges";	
	_datasets[10].vertices_path_bin = "/projects/DPE/wole/wdc/wdc_bin.vertices"; 
	_datasets[10].edges_path_bin = "/projects/DPE/wole/wdc/wdc_bin.edges";
	_datasets[10].min_vertex = 0;
	_datasets[10].max_vertex = 3315452338;
	_datasets[10].num_vertices = 3315452338;
	_datasets[10].num_edges = 0;
	_datasets[10].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[10].graphorder = SRC_DST;
	return;
}
void graph::setdataset(unsigned int id){
	#ifdef _LARGEDATASET_67M
	// if(id != 2 || id != 4){ cout<<"WARNING:DATASETS GRAPH "<<id<<"'S SIZE DOES NOT MATCH SPECIFIED CONFIGURATION. PLEASE SET CORRECT CONFIGURATION OR USE CORRECT GRAPH SIZE"<<endl; exit(EXIT_FAILURE); }
	if(id == 2 || id == 4){} else { cout<<"WARNING:DATASETS GRAPH "<<id<<"'S SIZE DOES NOT MATCH SPECIFIED CONFIGURATION. PLEASE SET CORRECT CONFIGURATION OR USE CORRECT GRAPH SIZE"<<endl; exit(EXIT_FAILURE); }
	#endif
	#ifdef _LARGEDATASET_268M
	if(id != 5){ cout<<"WARNING:DATASETS GRAPH "<<id<<"'S SIZE DOES NOT MATCH SPECIFIED CONFIGURATION. PLEASE SET CORRECT CONFIGURATION OR USE CORRECT GRAPH SIZE"<<endl; exit(EXIT_FAILURE); }
	#endif 
	#ifdef _LARGEDATASET_1B
	if(id != 6){ cout<<"WARNING:DATASETS GRAPH "<<id<<"'S SIZE DOES NOT MATCH SPECIFIED CONFIGURATION. PLEASE SET CORRECT CONFIGURATION OR USE CORRECT GRAPH SIZE"<<endl; exit(EXIT_FAILURE); }
	#endif 
	#ifdef _LARGEDATASET_4B
	if(id != 7){ cout<<"WARNING:DATASETS GRAPH "<<id<<"'S SIZE DOES NOT MATCH SPECIFIED CONFIGURATION. PLEASE SET CORRECT CONFIGURATION OR USE CORRECT GRAPH SIZE"<<endl; exit(EXIT_FAILURE); }
	#endif	
	thisdataset = _datasets[id];
	return;
}
dataset_t graph::getdataset(){
	return thisdataset;
}

// functions for createsmartgraph.cpp
void graph::getedges(vertex_t vid, vector<edgeprop2_t> & edges){
	return getedges(vid, 0, 0, edges);
}
void graph::getedges(vertex_t vid, unsigned int bank, unsigned int col, vector<edgeprop2_t> & edges){
	edges.clear();
	addedges(vid, bank, col, edges);
	return;
}
void graph::addedges(vertex_t vid, vector<edgeprop2_t> & edges){
	return addedges(vid, 0, 0, edges);
}
void graph::addedges(vertex_t vid, unsigned int bank, unsigned int col, vector<edgeprop2_t> & edges){	
	// cout<<"graph::addedges: vid: "<<vid<<endl;
	size_t byte_offset = ((size_t)vid) * sizeof(xvertexoffset_t);
	xvertexoffset_t mp_vidx_buffer[2];
	pread(nvmeFd_edgeoffsets_r2[bank][col], mp_vidx_buffer, (2 * sizeof(xvertexoffset_t)), byte_offset);
	uint32_t fanout = mp_vidx_buffer[1] - mp_vidx_buffer[0];

	edgeprop2_t * eB = new edgeprop2_t[fanout]; 
	pread(nvmeFd_edgeproperties_r2[bank][col], eB, (fanout * sizeof(edgeprop2_t)), (mp_vidx_buffer[0] * sizeof(edgeprop2_t)));	
	for(unsigned int i=0; i<fanout; i++){ edges.push_back(eB[i]); }
	delete eB;
	return;
}
void graph::initializecommunity(){
	cout<<"graph::initializecommunity::"<<endl;
	#ifdef XXX
	// parent vertexids (tmpdata)(pwrite)
	string path1 = gettmppath_parentvid(); 
	std::ofstream ofs; ofs.open(path1.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
	tmpnvmeFd_parents_w2 = open(path1.c_str(), O_WRONLY); 
	if (tmpnvmeFd_parents_w2 < 0) {
		cout << "ERR: open " << path1 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path1 << endl;
	
	// parent vertexids (tmpdata)(pread)
	string path2 = gettmppath_parentvid(); 
	tmpnvmeFd_parents_r2 = open(path2.c_str(), O_RDONLY); 
	if (tmpnvmeFd_parents_r2 < 0) {
		cout << "ERR: open " << path2 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path2 << endl;
	#endif 
	
	parents = new int[KVDATA_RANGE];
	childrencount = new int[KVDATA_RANGE];
	
	// write default values to parent & childrencount
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ parents[i] = -1; }
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ childrencount[i] = 0; }
	
	#ifdef XXX
	// write values to parent 
	int * defaultparents = new int[KVDATA_RANGE];
	for(unsigned int i=0; i<KVDATA_RANGE; i++){defaultparents[i] = -1; }
	if(pwrite(tmpnvmeFd_parents_w2, defaultparents, (size_t)(KVDATA_RANGE * sizeof(int)), 0) <= 0){ cout<<"hostprocess::initializecommunity::ERROR 36"<<endl; exit(EXIT_FAILURE); }		
	delete defaultparents;
	cout<<"graph::initializecommunity:: finished writing "<<KVDATA_RANGE<<" default values (-1) to "<<path1<<endl;
	#endif 
	return;
}
int graph::getparent(int childvid){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"graph::getparent:: getparent called: childvid: "<<childvid<<endl;
	#endif 
	
	return parents[childvid];
	
	// cout<<"graph::getparent::------------------ getparent called: childvid: "<<childvid<<endl;
	// exit(EXIT_SUCCESS);
	
	#ifdef XXX
	size_t byte_offset = ((size_t)childvid) * sizeof(int);
	int parent;
	if(pread(tmpnvmeFd_parents_r2, &parent, sizeof(int), byte_offset) <= 0){ cout<<"graph::getparent:: ERROR in getparent. childvid: "<<childvid<<", byte_offset: "<<byte_offset<<endl; exit(EXIT_FAILURE); }		
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"graph::getparent:: getparent called: parent: "<<parent<<endl;
	#endif
	return parent;
	#endif 
}
void graph::setparent(int childvid, int parentvid){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"graph::setparent:: setparent called: childvid: "<<childvid<<endl;
	#endif 
	
	// set parent 
	parents[childvid] = parentvid;	
	// set child
	childrencount[parentvid] += 1;
	
	#ifdef XXX
	// set parent 
	size_t byte_offset = ((size_t)childvid) * sizeof(int);
	int parent = parentvid;
	if(pwrite(tmpnvmeFd_parents_w2, &parent, sizeof(int), byte_offset) <= 0){ cout<<"graph::setparent:: ERROR in setparent. childvid: "<<childvid<<", parentvid: "<<parentvid<<", byte_offset: "<<byte_offset<<endl; exit(EXIT_FAILURE); }			
	#endif 
	return;
}
void graph::removeparent(int childvid){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"graph::removeparent:: removeparent called: childvid: "<<childvid<<endl;
	#endif 
	
	// get parent
	int parentvid = parents[childvid];
	// unset parent 
	parents[childvid] = -1;	
	// unset child
	childrencount[parentvid] -= 1;
	return;
}
unsigned int graph::getnumchildren(unsigned int parentvid){
	return childrencount[parentvid];
}
void graph::printparents(unsigned int start, unsigned int size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"graph::printparents:: printparents called: start: "<<start<<", size: "<<size<<endl;
	#endif 
	
	for(unsigned int i=0; i<size; i++){ cout<<"graph::printparents:: printparents called: parents["<<i<<"]: "<<parents[i]<<endl; }
	
	#ifdef XXX
	size_t byte_offset = ((size_t)start) * sizeof(int);
	int * parents = new int[size];
	if(pread(tmpnvmeFd_parents_r2, parents, (size * sizeof(int)), byte_offset) <= 0){ cout<<"graph::printparents:: ERROR in printparents. start: "<<start<<", size: "<<size<<endl; exit(EXIT_FAILURE); }		
	// #ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<size; i++){ cout<<"graph::printparents:: printparents called: parents["<<i<<"]: "<<parents[i]<<endl; }
	// #endif 
	#endif 
	return;
}
void graph::printchildrencount(unsigned int start, unsigned int size){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"graph::printchildrencount:: printchildrencount called: start: "<<start<<", size: "<<size<<endl;
	#endif 
	
	for(unsigned int i=0; i<size; i++){ cout<<"graph::printchildrencount:: printchildrencount called: childrencount["<<i<<"]: "<<childrencount[i]<<endl; }
	return;
}












