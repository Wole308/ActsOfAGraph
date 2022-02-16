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
#include <algorithm>
#include <iterator>
#include <bits/stdc++.h> 
#include <iostream> 
#include <sys/stat.h> 
#include <sys/types.h> 
// #include "EdgeProcess.h" 
// #include "VertexValues.h" 
// #include "sortreduce.h" 
// #include "filekvreader.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/utility/utility.h"
#include "../../include/host_common.h"
#include "../../include/common.h"
#include "graph.h"
using namespace std;

#ifdef AWS_PLATFORM
string datasetRootDir = "/oj2zf/";
string tmpDatarootDir = "/oj2zf/";
#else
string datasetRootDir = "/home/oj2zf/";
string tmpDatarootDir = "/home/oj2zf/";
#endif
	
graph::graph(algorithm * _algorithmobj, unsigned int datasetid, unsigned int _numedgebanks, bool _initfiles, bool _initgraphstructures, bool _initstatstructures){
	cout<<"graph::graph constructor:: datasetid: "<<datasetid<<", _numedgebanks: "<<_numedgebanks<<endl;
	
	algorithmobj = _algorithmobj;
	utilityobj = new utility();
	
	loadalldatasets();
	setdataset(datasetid);
	numedgebanks = _numedgebanks;
	
	if(_initfiles == true){ initializefiles(); }
	if(_initgraphstructures == true){ initgraphstructures(); }
	if(_initstatstructures == true){ initstatstructures(); }
	
	srand (0);
}
graph::~graph(){}

void graph::initializefiles(){
	nvmeFd_edges_r2 = new int[MAXNUMEDGEBANKS];
	nvmeFd_edges_w2 = new int[MAXNUMEDGEBANKS]; 	
	
	nvmeFd_edges_r = new FILE*[MAXNUMEDGEBANKS];
	nvmeFd_edges_w = new FILE*[MAXNUMEDGEBANKS]; 
	
	nvmeFd_vertexptrs_r2 = new int[MAXNUMEDGEBANKS]; 
	nvmeFd_vertexptrs_w2 = new int[MAXNUMEDGEBANKS]; 
	
	nvmeFd_vertexptrs_r = new FILE*[MAXNUMEDGEBANKS]; 
	nvmeFd_vertexptrs_w = new FILE*[MAXNUMEDGEBANKS];
	return;
}
void graph::initgraphstructures(){
	vertexdatabuffer = new value_t[KVDATA_RANGE];
	tempvertexdatabuffer = new value_t[KVDATA_RANGE];
	vertexptrbuffer = new edge_t[KVDATA_RANGE]; 
	return;
}
void graph::initstatstructures(){
	totalkeyvaluesread[0] = new unsigned long[1];
	totalkeyvaluesread[0][0] = 0;
	return;
}

size_t graph::get_num_vertices(){
	return thisdataset.num_vertices;
}
size_t graph::get_num_edges(){
	return thisdataset.num_edges;
}
void graph::setnumvertices(unsigned int num_vertices){
	thisdataset.num_vertices = num_vertices;
}

size_t graph::getnumedgebanks(){
	return numedgebanks;
}
void graph::setnumedgebanks(unsigned int _numedgebanks){
	numedgebanks = _numedgebanks;
}
void graph::setbanks(unsigned int _numedgebanks){
	numedgebanks = _numedgebanks;
}

void graph::openfilesforreading(unsigned int groupid){
	cout<<"graph::openfilesforreading : opening files for reading"<<endl;
	// vertex pointers (pread)
	for(unsigned int j=0; j<numedgebanks; j++){
		string path = getpath_vertexptrs(groupid, j); 	
		nvmeFd_vertexptrs_r2[j] = open(path.c_str(), O_RDONLY);
		if (nvmeFd_vertexptrs_r2[j] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	
	// vertex pointers (fread)
	for(unsigned int j=0; j<numedgebanks; j++){
		string path = getpath_vertexptrs(groupid,j); 
		nvmeFd_vertexptrs_r[j] = fopen(path.c_str(), "r"); 
		if (nvmeFd_vertexptrs_r[j] == NULL) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	
	// edges (pread)
	for(unsigned int j=0; j<numedgebanks; j++){
		string path = getpath_edges(groupid, j);
		nvmeFd_edges_r2[j] = open(path.c_str(), O_RDONLY); 
		if (nvmeFd_edges_r2[j] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	
	// edges (fread)
	for(unsigned int j=0; j<numedgebanks; j++){
		string path = getpath_edges(groupid,j);
		nvmeFd_edges_r[j] = fopen(path.c_str(), "r"); 
		if (nvmeFd_edges_r[j] == NULL) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	return;
}
void graph::openfilesforwriting(unsigned int groupid){
	cout<<"graph::openfilesforwriting : opening files for writing"<<endl;
	utilityobj->createdirectory(getdatasetdir().c_str());
	
	// vertex pointers (pwrite)
	for(unsigned int j=0; j<numedgebanks; j++){
		string path = getpath_vertexptrs(groupid, j); 
		std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();				
		nvmeFd_vertexptrs_w2[j] = open(path.c_str(), O_WRONLY);
		if (nvmeFd_vertexptrs_w2[j] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	
	// vertex pointers (fwrite)
	for(unsigned int j=0; j<numedgebanks; j++){
		string path = getpath_vertexptrs(groupid, j); 
		std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
		nvmeFd_vertexptrs_w[j] = fopen(path.c_str(), "w"); 
		if (nvmeFd_vertexptrs_w[j] == NULL) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	
	// edges (pwrite)
	for(unsigned int j=0; j<numedgebanks; j++){
		string path = getpath_edges(groupid, j);
		std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
		nvmeFd_edges_w2[j] = open(path.c_str(), O_WRONLY); 
		if (nvmeFd_edges_w2[j] < 0) {
			cout << "ERR: open " << path << " failed: "
				<< strerror(errno) << endl;
			exit(EXIT_FAILURE);
		}
		cout << "INFO: Successfully opened " << path << endl;
	}
	
	// edges (fwrite)
	for(unsigned int j=0; j<numedgebanks; j++){
		string path = getpath_edges(groupid, j);
		std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
		nvmeFd_edges_w[j] = fopen(path.c_str(), "w"); 
		if (nvmeFd_edges_w[j] == NULL) {
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
	for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_vertexptrs_r2[j]); }
	for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_vertexptrs_r[j]); }
	for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_edges_r2[j]); }
	for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edges_r[j]); }
	return;
}
void graph::closefilesforwriting(){
	cout<<"graph::closefilesforreading : close files for writing..."<<endl;
	for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_vertexptrs_w2[j]); }
	for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_vertexptrs_w[j]); }
	for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_edges_w2[j]); }
	for(unsigned int j=0; j<numedgebanks; j++){ fclose(nvmeFd_edges_w[j]); }
	return;
}

void graph::opentemporaryfilesforreading(){
	cout<<"graph::opentemporaryfilesforreading : opening temporary files for reading"<<endl;
	// vertex data (pread)
	string path1 = getpath_vertexdata(); 
	nvmeFd_verticesdata_r2 = open(path1.c_str(), O_RDONLY); 
	if (nvmeFd_verticesdata_r2 < 0) {
		cout << "ERR: open " << path1 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path1 << endl;
	
	#ifdef NOTUSED
	// temporary vertex data (pread)
	string path2 = getpath_tempvertexdata(); 
	nvmeFd_tempverticesdata_r2 = open(path2.c_str(), O_RDONLY); 
	if (nvmeFd_tempverticesdata_r2 < 0) {
		cout << "ERR: open " << path2 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path2 << endl;
	#endif 
	
	// vertex properties (pread)
	string path3 = getpath_vertexproperties(); 
	nvmeFd_vertexproperties_r2 = open(path3.c_str(), O_RDONLY); 
	if (nvmeFd_vertexproperties_r2 < 0) {
		cout << "ERR: open " << path3 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path3 << endl;
	return;
}
void graph::opentemporaryfilesforwriting(){
	cout<<"graph::opentemporaryfilesforwriting : opening temporary files for writing..."<<endl;
	utilityobj->createdirectory(getdatasetdir().c_str());
	
	// vertex data (pwrite)
	string path1 = getpath_vertexdata(); 
	std::ofstream ofs1; ofs1.open(path1.c_str(), std::ofstream::out | std::ofstream::trunc); ofs1.close();	
	nvmeFd_verticesdata_w2 = open(path1.c_str(), O_WRONLY); 
	if (nvmeFd_verticesdata_w2 < 0) {
		cout << "ERR: open " << path1 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path1 << endl;

	// exit(EXIT_SUCCESS);
	#ifdef NOTUSED
	// temporary vertex data (pwrite)
	string path2 = getpath_tempvertexdata(); 
	std::ofstream ofs2; ofs2.open(path2.c_str(), std::ofstream::out | std::ofstream::trunc); ofs2.close();	
	nvmeFd_tempverticesdata_w2 = open(path2.c_str(), O_WRONLY); 
	if (nvmeFd_tempverticesdata_w2 < 0) {
		cout << "ERR: open " << path2 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path2 << endl;
	#endif 
	
	// vertex properties (pwrite)
	string path3 = getpath_vertexproperties(); 
	std::ofstream ofs3; ofs3.open(path3.c_str(), std::ofstream::out | std::ofstream::trunc); ofs3.close();
	nvmeFd_vertexproperties_w2 = open(path3.c_str(), O_WRONLY); 
	if (nvmeFd_vertexproperties_w2 < 0) {
		cout << "ERR: open " << path3 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path3 << endl;
	return;
}

void graph::closetemporaryfilesforreading(){
	cout<<"graph::closetemporaryfilesforreading : close files for reading..."<<endl;
	close(nvmeFd_verticesdata_r2);
	#ifdef NOTUSED
	close(nvmeFd_tempverticesdata_r2);
	#endif
	close(nvmeFd_vertexproperties_r2);
	#ifndef PR_ALGORITHM
	close(nvmeFd_activevertexids_r2);	
	#endif
	return;
}
void graph::closetemporaryfilesforwriting(){
	cout<<"graph::closetemporaryfilesforwriting : close temporary files for writing..."<<endl;
	close(nvmeFd_verticesdata_w2);
	close(nvmeFd_vertexproperties_w2);
	#ifdef NOTUSED
	close(nvmeFd_tempverticesdata_w2);
	#endif 
	#ifndef PR_ALGORITHM
	close(nvmeFd_activevertexids_w2);
	#endif 
	return;
}

void graph::openactiveverticesfilesforreading(unsigned int graph_iterationidx){
	cout<<"graph::openactiveverticesfilesforreading : open active vertices (iteration "<<graph_iterationidx<<") files for reading ..."<<endl;
	string path1 = getpath_activevertices(graph_iterationidx);
	nvmeFd_activevertexids_r2 = open(path1.c_str(), O_RDONLY); 
	if (nvmeFd_activevertexids_r2 < 0) {
		cout << "ERR: open " << path1 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path1 << endl;
	
	string path2 = getpath_vertexisactive(graph_iterationidx);
	nvmeFd_vertexisactive_r2 = open(path2.c_str(), O_RDONLY); 
	if (nvmeFd_vertexisactive_r2 < 0) {
		cout << "ERR: open " << path2 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path2 << endl;
	return;
}
void graph::openactiveverticesfilesforwriting(unsigned int graph_iterationidx){
	cout<<"graph::openactiveverticesfilesforwriting : open active vertices (iteration "<<graph_iterationidx<<") files for writing..."<<endl;
	utilityobj->createdirectory(getdatasetdir().c_str());
	
	string path1 = getpath_activeverticesW(graph_iterationidx);
	std::ofstream ofs1; ofs1.open(path1.c_str(), std::ofstream::out | std::ofstream::trunc); ofs1.close();
	nvmeFd_activevertexids_w = fopen(path1.c_str(), "w"); 
	if (nvmeFd_activevertexids_w == NULL) {
		cout << "ERR: open " << path1 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path1 << endl;
	
	string path2 = getpath_vertexisactive(graph_iterationidx);
	std::ofstream ofs2; ofs2.open(path2.c_str(), std::ofstream::out | std::ofstream::trunc); ofs2.close();
	nvmeFd_vertexisactive_w2 = open(path2.c_str(), O_WRONLY); 
	if (nvmeFd_vertexisactive_w2 < 0) {
		cout << "ERR: open " << path2 << " failed: "
			<< strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	cout << "INFO: Successfully opened " << path2 << endl;
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

string graph::getdatasetdir(){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + thisdataset.graphtopname  + "_" + std::to_string(1) + "by" +  std::to_string(numedgebanks);// + "/" + thisdataset.graphname + "_" + std::to_string(groupid) + "_" + std::to_string(j) + ".vertexptrs";		
}
string graph::getpath_vertexdata(){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + thisdataset.graphtopname + "_" + std::to_string(1) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + ".vdata";		
}
string graph::getpath_tempvertexdata(){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + thisdataset.graphtopname + "_" + std::to_string(1) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + ".tempvdata";		
}
string graph::getpath_vertexproperties(){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + thisdataset.graphtopname + "_" + std::to_string(1) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + ".vproperties";		
}
string graph::getpath_edges(unsigned int groupid, unsigned int j){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + thisdataset.graphtopname + "_" + std::to_string(1) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(groupid) + "_" + std::to_string(j) + ".edges";		
}
string graph::getpath_vertexptrs(unsigned int groupid, unsigned int j){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + thisdataset.graphtopname + "_" + std::to_string(1) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(groupid) + "_" + std::to_string(j) + ".vertexptrs";		
}
string graph::getpath_activevertices(unsigned int graph_iterationidx){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + "activevertices" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".actvvertices"; // CORRECTONE.		
	// return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + "activeverticesW" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".actvvertices";
}
string graph::getpath_activeverticesW(unsigned int graph_iterationidx){
	#ifdef FORCEDFINISH_DONTCAREABOUTACTIVEVERTICESGENERATED
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + "activeverticesW" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".actvvertices";	
	#else 
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + "activevertices" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".actvvertices";	
	#endif 
}
string graph::getpath_vertexisactive(unsigned int graph_iterationidx){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + "activeverticesW" + "/" + "iteration" + std::to_string(graph_iterationidx) + ".vertexisactive";	
}
string graph::getpath_vertexupdates(){
	return datasetRootDir + "dataset" + "/" + thisdataset.graphtopname + "/" + thisdataset.graphtopname + "_" + std::to_string(1) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + ".vupdates";		
}
string graph::gettmp_dir(){
	return datasetRootDir + "tmpdata_grafboost";		
}
string graph::getidx_path(){
	return datasetRootDir + "dataset/" + thisdataset.graphtopname + "/ridx.dat";
}
string graph::getmat_path(){
	return datasetRootDir + "dataset/" + thisdataset.graphtopname + "/matrix.dat";	
}

int graph::getnvmeFd_verticesdata_r2(){ return nvmeFd_verticesdata_r2; } 
int graph::getnvmeFd_verticesdata_w2(){ return nvmeFd_verticesdata_w2; }
int graph::getnvmeFd_tempverticesdata_r2(){ return nvmeFd_tempverticesdata_r2; } 
int graph::getnvmeFd_tempverticesdata_w2(){ return nvmeFd_tempverticesdata_w2; }
int graph::getnvmeFd_vertexoutdegrees_r2(){ return nvmeFd_vertexoutdegrees_r2; }
int graph::getnvmeFd_vertexproperties_r2(){ return nvmeFd_vertexproperties_r2; } 
int graph::getnvmeFd_vertexproperties_w2(){ return nvmeFd_vertexproperties_w2; }
int * graph::getnvmeFd_edges_r2(){ return nvmeFd_edges_r2; } 
int * graph::getnvmeFd_edges_w2(){ return nvmeFd_edges_w2; }
FILE ** graph::getnvmeFd_edges_r(){ return nvmeFd_edges_r; }
FILE ** graph::getnvmeFd_edges_w(){ return nvmeFd_edges_w; }
int * graph::getnvmeFd_vertexptrs_r2(){ return nvmeFd_vertexptrs_r2; }
int * graph::getnvmeFd_vertexptrs_w2(){ return nvmeFd_vertexptrs_w2; }
FILE ** graph::getnvmeFd_vertexptrs_r(){ return nvmeFd_vertexptrs_r; }
FILE ** graph::getnvmeFd_vertexptrs_w(){ return nvmeFd_vertexptrs_w; }
int graph::getnvmeFd_activevertexids_r2(){ return nvmeFd_activevertexids_r2; } 
int graph::getnvmeFd_activevertexids_w2(){ return nvmeFd_activevertexids_w2; }
FILE * graph::getnvmeFd_activevertexids_w(){ return nvmeFd_activevertexids_w; }
int graph::getnvmeFd_vertexisactive_r2(){ return nvmeFd_vertexisactive_r2; }
int graph::getnvmeFd_vertexisactive_w2(){ return nvmeFd_vertexisactive_w2; }

edge_t * graph::loadvertexptrsfromfile(int col){ 
	if(pread(nvmeFd_vertexptrs_r2[col], vertexptrbuffer, KVDATA_RANGE * sizeof(edge_t), 0) <= 0){ cout<<"graph::loadvertexptrsfromfile::ERROR LOADING FILE. COL("<<col<<"). EXITING. 36. KVDATA_RANGE: "<<KVDATA_RANGE<<endl; exit(EXIT_FAILURE); }
	return vertexptrbuffer;
}
void graph::loadvertexptrsfromfile(int col, size_t fdoffset, edge_t * buffer, vertex_t bufferoffset, vertex_t size){
	if(size > 0){ if(pread(nvmeFd_vertexptrs_r2[col], &buffer[bufferoffset], size * sizeof(edge_t), fdoffset * sizeof(edge_t)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient vertexptrs at col["<<col<<"]. EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }}
	return;
}
value_t * graph::generateverticesdata(unsigned int Algo){ 
	cout<<"generating vertices data... "<<endl;
	for(unsigned int k=0; k<KVDATA_RANGE; k++){
		vertexdatabuffer[k] = algorithmobj->vertex_initdata(Algo, k); 
	} 
	return vertexdatabuffer;
}
value_t * graph::generatetempverticesdata(unsigned int Algo){ 
	cout<<"generating temp vertices data... "<<endl;
	for(unsigned int k=0; k<KVDATA_RANGE; k++){ tempvertexdatabuffer[k] = algorithmobj->vertex_initdata(Algo, k); }					 
	return tempvertexdatabuffer;
}
void graph::loadedgesfromfile(int col, size_t fdoffset, edge2_type * buffer, vertex_t bufferoffset, vertex_t size){
	// if(size > 0){ if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset], (size * sizeof(edge2_type)), fdoffset * sizeof(edge2_type)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"]. EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }}					
	
	unsigned int szA = size/2;
	unsigned int szB = size - szA;
	
	if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset], (szA * sizeof(edge2_type)), fdoffset * sizeof(edge2_type)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](A). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset + szA], (szB * sizeof(edge2_type)), (fdoffset + szA) * sizeof(edge2_type)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](B). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	
	
	// if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset], ((size/2) * sizeof(edge2_type)), fdoffset * sizeof(edge2_type)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](A). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	// if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset + (size/2)], ((size/2) * sizeof(edge2_type)), (fdoffset + (size/2)) * sizeof(edge2_type)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](B). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	
	return;
}
edge_t graph::getedgessize(int col){ 
	edge_t size = lseek(nvmeFd_edges_r2[col], 0, SEEK_END) / sizeof(edge2_type);
	return size;
}

void graph::generatevertexoutdegrees(vertex_t * vertexoutdegrees){
	cout<<"graph::generating vertex outdegrees... "<<endl;	
	for(unsigned int i=0; i<KVDATA_RANGE; i++){
		vertexoutdegrees[i] = 1; // FIXME.
	}
	return;
}
void graph::generatevertexproperties(){
	cout<<"generating vertex properties... "<<endl;	
	vertex_t * vertexoutdegrees = new vertex_t[KVDATA_RANGE];
	generatevertexoutdegrees(vertexoutdegrees);
	
	vertexprop_t * vertexproperties = new vertexprop_t[KVDATA_RANGE];
	for(unsigned int k=0; k<KVDATA_RANGE; k++){
		vertexproperties[k].outdegree = vertexoutdegrees[k];
	}
	if(pwrite(nvmeFd_vertexproperties_w2, vertexproperties, (size_t)(KVDATA_RANGE * sizeof(vertexprop_t)), 0) < 0){ cout<<"hostprocess::generatevertexproperties::ERROR 36"<<endl; exit(EXIT_FAILURE); }		
	// close(nvmeFd_vertexproperties_w2);
	delete [] vertexoutdegrees;
	delete [] vertexproperties;
	cout<<"finished generating vertex properties data"<<endl;
	return;
}

void graph::loadalldatasets(){
	cout<<"graph::loadalldatasets:: "<<endl;
	
	#ifdef AWS_PLATFORM
	string rootDir = "/oj2zf/";
	#else
	string rootDir = "/home/oj2zf/"; // /home/oj2zf
	#endif
	
	load_realworld_datasets();
	load_synthetic_datasets();
	load_large_datasets();

	for(unsigned int i=0; i<128; i++){
		_datasets[i].groupvoffset[0] = 0;
		_datasets[i].groupvoffset[1] = _datasets[i].num_vertices;
	}
	
	cout<<"graph::loadalldatasets finished. "<<endl;
	return;
}
void graph::load_realworld_datasets(){
	cout<<"graph::loadalldatasets:: "<<endl;
	
	#ifdef AWS_PLATFORM
	string rootDir = "/oj2zf/";
	#else
	string rootDir = "/home/oj2zf/"; // /home/oj2zf
	#endif
	
	// small dataset
	_datasets[1].graphtopname = "soc-orkut";
	_datasets[1].graphname = "soc-orkut";
	_datasets[1].graph_path = rootDir + "dataset/soc-orkut/soc-orkut.mtx";
	_datasets[1].vertices_path = rootDir + "dataset/soc-orkut/soc-orkut.vertices";
	_datasets[1].edges_path = rootDir + "dataset/soc-orkut/soc-orkut.edges";	
	_datasets[1].vertices_path_bin = rootDir + "dataset/soc-orkut/soc-orkut_bin.vertices";
	_datasets[1].edges_path_bin = rootDir + "dataset/soc-orkut/soc-orkut_bin.edges";
	_datasets[1].min_vertex = 0;
	_datasets[1].max_vertex = 2997166; 
	_datasets[1].num_vertices = 2997166;
	_datasets[1].num_edges = 106349209;
	_datasets[1].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[1].graphorder = DST_SRC;
	_datasets[1].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[2].graphtopname = "hollywood-2009";
	_datasets[2].graphname = "hollywood-2009";
	_datasets[2].graph_path = rootDir + "dataset/hollywood-2009/hollywood-2009.mtx";
	_datasets[2].vertices_path = rootDir + "dataset/hollywood-2009/hollywood-2009.vertices";
	_datasets[2].edges_path = rootDir + "dataset/hollywood-2009/hollywood-2009.edges";	
	_datasets[2].vertices_path_bin = rootDir + "dataset/hollywood-2009/hollywood-2009_bin.vertices";
	_datasets[2].edges_path_bin = rootDir + "dataset/hollywood-2009/hollywood-2009_bin.edges";
	_datasets[2].min_vertex = 0;
	_datasets[2].max_vertex = 1139905; 
	_datasets[2].num_vertices = 1139905;
	_datasets[2].num_edges = 57515616;
	_datasets[2].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[2].graphorder = DST_SRC;
	_datasets[2].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[3].graphtopname = "kron_g500-logn20";
	_datasets[3].graphname = "kron_g500-logn20";
	_datasets[3].graph_path = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20.mtx";
	_datasets[3].vertices_path = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20.vertices";
	_datasets[3].edges_path = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20.edges";	
	_datasets[3].vertices_path_bin = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20_bin.vertices";
	_datasets[3].edges_path_bin = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20_bin.edges";
	_datasets[3].min_vertex = 0;
	_datasets[3].max_vertex = 1048576;
	_datasets[3].num_vertices = 1048576;
	_datasets[3].num_edges = 44620272;
	_datasets[3].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[3].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[3].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[4].graphtopname = "kron_g500-logn21";
	_datasets[4].graphname = "kron_g500-logn21";
	_datasets[4].graph_path = rootDir + "dataset/kron_g500-logn21/kron_g500-logn21.mtx";
	_datasets[4].vertices_path = rootDir + "dataset/kron_g500-logn21/kron_g500-logn21.vertices";
	_datasets[4].edges_path = rootDir + "dataset/kron_g500-logn21/kron_g500-logn21.edges";	
	_datasets[4].vertices_path_bin = rootDir + "dataset/kron_g500-logn21/kron_g500-logn21_bin.vertices";
	_datasets[4].edges_path_bin = rootDir + "dataset/kron_g500-logn21/kron_g500-logn21_bin.edges";
	_datasets[4].min_vertex = 0;
	_datasets[4].max_vertex = 2097151; // 2097152, 2097151
	_datasets[4].num_vertices = 2097151; // 2097152, 2097151
	_datasets[4].num_edges = 91042010;
	_datasets[4].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[4].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[4].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[5].graphtopname = "kron_g500-logn22";
	_datasets[5].graphname = "kron_g500-logn22";
	_datasets[5].graph_path = rootDir + "dataset/kron_g500-logn22/kron_g500-logn22.mtx";
	_datasets[5].vertices_path = rootDir + "dataset/kron_g500-logn22/kron_g500-logn22.vertices";
	_datasets[5].edges_path = rootDir + "dataset/kron_g500-logn22/kron_g500-logn22.edges";	
	_datasets[5].vertices_path_bin = rootDir + "dataset/kron_g500-logn22/kron_g500-logn22.vertices";
	_datasets[5].edges_path_bin = rootDir + "dataset/kron_g500-logn22/kron_g500-logn22.edges";
	_datasets[5].min_vertex = 0;
	_datasets[5].max_vertex = (1 << 22);
	_datasets[5].num_vertices = (1 << 22);
	_datasets[5].num_edges = ((1 << 22) * 35);
	_datasets[5].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[5].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[5].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[6].graphtopname = "rgg_n_2_24_s0";
	_datasets[6].graphname = "rgg_n_2_24_s0";
	_datasets[6].graph_path = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0.mtx";
	_datasets[6].vertices_path = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0.vertices";
	_datasets[6].edges_path = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0.edges";	
	_datasets[6].vertices_path_bin = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0_bin.vertices";
	_datasets[6].edges_path_bin = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0_bin.edges";
	_datasets[6].min_vertex = 0;
	_datasets[6].max_vertex = 16777216; 
	_datasets[6].num_vertices = 16777216;
	_datasets[6].num_edges = 265114400 / 2;
	_datasets[6].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[6].graphorder = DST_SRC;
	_datasets[6].graphgroup = SNAP;
	
	_datasets[7].graphtopname = "europe_osm";
	_datasets[7].graphname = "europe_osm";
	_datasets[7].graph_path = rootDir + "dataset/europe_osm/europe_osm.mtx";
	_datasets[7].vertices_path = rootDir + "dataset/europe_osm/europe_osm.vertices";
	_datasets[7].edges_path = rootDir + "dataset/europe_osm/europe_osm.edges";	
	_datasets[7].vertices_path_bin = rootDir + "dataset/europe_osm/europe_osm_bin.vertices";
	_datasets[7].edges_path_bin = rootDir + "dataset/europe_osm/europe_osm_bin.edges";
	_datasets[7].min_vertex = 0;
	_datasets[7].max_vertex = 50912018; 
	_datasets[7].num_vertices = 50912018;
	_datasets[7].num_edges = 108109320 / 2;
	_datasets[7].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[7].graphorder = DST_SRC;
	_datasets[7].graphgroup = SNAP;
	
	_datasets[8].graphtopname = "kmer_V2a";
	_datasets[8].graphname = "kmer_V2a";
	_datasets[8].graph_path = rootDir + "dataset/kmer_V2a/kmer_V2a.mtx";
	_datasets[8].vertices_path = rootDir + "dataset/kmer_V2a/kmer_V2a.vertices";
	_datasets[8].edges_path = rootDir + "dataset/kmer_V2a/kmer_V2a.edges";	
	_datasets[8].vertices_path_bin = rootDir + "dataset/kmer_V2a/kmer_V2a_bin.vertices";
	_datasets[8].edges_path_bin = rootDir + "dataset/kmer_V2a/kmer_V2a_bin.edges";
	_datasets[8].min_vertex = 0;
	_datasets[8].max_vertex = 55042369; 
	_datasets[8].num_vertices = 55042369;
	_datasets[8].num_edges = 117217600 / 2;
	_datasets[8].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[8].graphorder = DST_SRC;
	_datasets[8].graphgroup = SNAP;
	
	_datasets[9].graphtopname = "hugebubbles_00020";
	_datasets[9].graphname = "hugebubbles_00020";
	_datasets[9].graph_path = rootDir + "dataset/hugebubbles_00020/hugebubbles_00020.mtx";
	_datasets[9].vertices_path = rootDir + "dataset/hugebubbles_00020/hugebubbles_00020.vertices";
	_datasets[9].edges_path = rootDir + "dataset/hugebubbles_00020/hugebubbles_00020.edges";	
	_datasets[9].vertices_path_bin = rootDir + "dataset/hugebubbles_00020/hugebubbles_00020_bin.vertices";
	_datasets[9].edges_path_bin = rootDir + "dataset/hugebubbles_00020/hugebubbles_00020_bin.edges";
	_datasets[9].min_vertex = 0;
	_datasets[9].max_vertex = 21198119; 
	_datasets[9].num_vertices = 21198119;
	_datasets[9].num_edges = 63580358 / 2;
	_datasets[9].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[9].graphorder = DST_SRC;
	_datasets[9].graphgroup = SNAP;
	
	_datasets[10].graphtopname = "kmer_U1a";
	_datasets[10].graphname = "kmer_U1a";
	_datasets[10].graph_path = rootDir + "dataset/kmer_U1a/kmer_U1a.mtx";
	_datasets[10].vertices_path = rootDir + "dataset/kmer_U1a/kmer_U1a.vertices";
	_datasets[10].edges_path = rootDir + "dataset/kmer_U1a/kmer_U1a.edges";	
	_datasets[10].vertices_path_bin = rootDir + "dataset/kmer_U1a/kmer_U1a_bin.vertices";
	_datasets[10].edges_path_bin = rootDir + "dataset/kmer_U1a/kmer_U1a_bin.edges";
	_datasets[10].min_vertex = 0;
	_datasets[10].max_vertex = 67716231; 
	_datasets[10].num_vertices = 67716231;
	_datasets[10].num_edges = 138778562 / 2;
	_datasets[10].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[10].graphorder = DST_SRC;
	_datasets[10].graphgroup = SNAP;
	
	// _datasets[].graphtopname = "?";
	// _datasets[].graphname = "?";
	// _datasets[].graph_path = rootDir + "dataset/?/?.mtx";
	// _datasets[].vertices_path = rootDir + "dataset/?/?.vertices";
	// _datasets[].edges_path = rootDir + "dataset/?/?.edges";	
	// _datasets[].vertices_path_bin = rootDir + "dataset/?/?_bin.vertices";
	// _datasets[].edges_path_bin = rootDir + "dataset/?/?_bin.edges";
	// _datasets[].min_vertex = 0;
	// _datasets[].max_vertex = ?; 
	// _datasets[].num_vertices = ?;
	// _datasets[].num_edges = ?;
	// _datasets[].graphdirectiontype = UNDIRECTEDGRAPH;
	// _datasets[].graphorder = DST_SRC;
	// _datasets[].graphgroup = SNAP;
	
	cout<<"graph::loadalldatasets finished. "<<endl;
	return;
}
void graph::load_synthetic_datasets(){
	cout<<"graph::loadalldatasets:: "<<endl;
	
	#ifdef AWS_PLATFORM
	string rootDir = "/oj2zf/";
	#else
	string rootDir = "/home/oj2zf/"; // /home/oj2zf
	#endif
	
	_datasets[30].graphtopname = "RMAT22_SPARSE0";
	_datasets[30].graphname = "RMAT22_SPARSE0";
	_datasets[30].graph_path = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.mtx";
	_datasets[30].vertices_path = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.vertices";
	_datasets[30].edges_path = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.edges";	
	_datasets[30].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.vertices";
	_datasets[30].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.edges";
	_datasets[30].min_vertex = 0;
	_datasets[30].max_vertex = (1 << 22);
	_datasets[30].num_vertices = (1 << 22);
	_datasets[30].num_edges = ((1 << 22) * 30);
	_datasets[30].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[30].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[30].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[31].graphtopname = "RMAT22_SPARSE1";
	_datasets[31].graphname = "RMAT22_SPARSE1";
	_datasets[31].graph_path = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.mtx";
	_datasets[31].vertices_path = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.vertices";
	_datasets[31].edges_path = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.edges";	
	_datasets[31].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.vertices";
	_datasets[31].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.edges";
	_datasets[31].min_vertex = 0;
	_datasets[31].max_vertex = (1 << 22);
	_datasets[31].num_vertices = (1 << 22);
	_datasets[31].num_edges = ((1 << 22) * 30);
	_datasets[31].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[31].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[31].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[32].graphtopname = "RMAT22_SPARSE2";
	_datasets[32].graphname = "RMAT22_SPARSE2";
	_datasets[32].graph_path = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.mtx";
	_datasets[32].vertices_path = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.vertices";
	_datasets[32].edges_path = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.edges";	
	_datasets[32].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.vertices";
	_datasets[32].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.edges";
	_datasets[32].min_vertex = 0;
	_datasets[32].max_vertex = (1 << 22);
	_datasets[32].num_vertices = (1 << 22);
	_datasets[32].num_edges = ((1 << 22) * 30);
	_datasets[32].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[32].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[32].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[33].graphtopname = "RMAT22_SPARSE3";
	_datasets[33].graphname = "RMAT22_SPARSE3";
	_datasets[33].graph_path = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.mtx";
	_datasets[33].vertices_path = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.vertices";
	_datasets[33].edges_path = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.edges";	
	_datasets[33].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.vertices";
	_datasets[33].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.edges";
	_datasets[33].min_vertex = 0;
	_datasets[33].max_vertex = (1 << 22);
	_datasets[33].num_vertices = (1 << 22);
	_datasets[33].num_edges = ((1 << 22) * 30);
	_datasets[33].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[33].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[33].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[34].graphtopname = "RMAT22_SPARSE4";
	_datasets[34].graphname = "RMAT22_SPARSE4";
	_datasets[34].graph_path = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.mtx";
	_datasets[34].vertices_path = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.vertices";
	_datasets[34].edges_path = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.edges";	
	_datasets[34].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.vertices";
	_datasets[34].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.edges";
	_datasets[34].min_vertex = 0;
	_datasets[34].max_vertex = (1 << 22);
	_datasets[34].num_vertices = (1 << 22);
	_datasets[34].num_edges = ((1 << 22) * 30);
	_datasets[34].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[34].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[34].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[35].graphtopname = "RMAT22_SPARSE5";
	_datasets[35].graphname = "RMAT22_SPARSE5";
	_datasets[35].graph_path = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.mtx";
	_datasets[35].vertices_path = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.vertices";
	_datasets[35].edges_path = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.edges";	
	_datasets[35].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.vertices";
	_datasets[35].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.edges";
	_datasets[35].min_vertex = 0;
	_datasets[35].max_vertex = (1 << 22);
	_datasets[35].num_vertices = (1 << 22);
	_datasets[35].num_edges = ((1 << 22) * 30);
	_datasets[35].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[35].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[35].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[36].graphtopname = "RMAT22_SPARSE6";
	_datasets[36].graphname = "RMAT22_SPARSE6";
	_datasets[36].graph_path = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.mtx";
	_datasets[36].vertices_path = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.vertices";
	_datasets[36].edges_path = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.edges";	
	_datasets[36].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.vertices";
	_datasets[36].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.edges";
	_datasets[36].min_vertex = 0;
	_datasets[36].max_vertex = (1 << 22);
	_datasets[36].num_vertices = (1 << 22);
	_datasets[36].num_edges = ((1 << 22) * 30);
	_datasets[36].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[36].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[36].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[37].graphtopname = "RMAT22_SPARSE7";
	_datasets[37].graphname = "RMAT22_SPARSE7";
	_datasets[37].graph_path = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.mtx";
	_datasets[37].vertices_path = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.vertices";
	_datasets[37].edges_path = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.edges";	
	_datasets[37].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.vertices";
	_datasets[37].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.edges";
	_datasets[37].min_vertex = 0;
	_datasets[37].max_vertex = (1 << 22);
	_datasets[37].num_vertices = (1 << 22);
	_datasets[37].num_edges = ((1 << 22) * 30);
	_datasets[37].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[37].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[37].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	///////////////////////////////////////////////////////////////////// synthetic datasets (II)
	
	_datasets[40].graphtopname = "RMAT_RANGE0";
	_datasets[40].graphname = "RMAT_RANGE0";
	_datasets[40].graph_path = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.mtx";
	_datasets[40].vertices_path = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.vertices";
	_datasets[40].edges_path = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.edges";	
	_datasets[40].vertices_path_bin = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.vertices";
	_datasets[40].edges_path_bin = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.edges";
	_datasets[40].min_vertex = 0;
	_datasets[40].max_vertex = (1 << 20);
	_datasets[40].num_vertices = (1 << 20);
	_datasets[40].num_edges = (1 << 26);
	_datasets[40].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[40].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[40].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[41].graphtopname = "RMAT_RANGE1";
	_datasets[41].graphname = "RMAT_RANGE1";
	_datasets[41].graph_path = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.mtx";
	_datasets[41].vertices_path = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.vertices";
	_datasets[41].edges_path = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.edges";	
	_datasets[41].vertices_path_bin = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.vertices";
	_datasets[41].edges_path_bin = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.edges";
	_datasets[41].min_vertex = 0;
	_datasets[41].max_vertex = (1 << 21);
	_datasets[41].num_vertices = (1 << 21);
	_datasets[41].num_edges = (1 << 26);
	_datasets[41].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[41].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[41].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[42].graphtopname = "RMAT_RANGE2";
	_datasets[42].graphname = "RMAT_RANGE2";
	_datasets[42].graph_path = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.mtx";
	_datasets[42].vertices_path = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.vertices";
	_datasets[42].edges_path = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.edges";	
	_datasets[42].vertices_path_bin = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.vertices";
	_datasets[42].edges_path_bin = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.edges";
	_datasets[42].min_vertex = 0;
	_datasets[42].max_vertex = (1 << 22);
	_datasets[42].num_vertices = (1 << 22);
	_datasets[42].num_edges = (1 << 26);
	_datasets[42].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[42].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[42].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[43].graphtopname = "RMAT_RANGE3";
	_datasets[43].graphname = "RMAT_RANGE3";
	_datasets[43].graph_path = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.mtx";
	_datasets[43].vertices_path = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.vertices";
	_datasets[43].edges_path = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.edges";	
	_datasets[43].vertices_path_bin = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.vertices";
	_datasets[43].edges_path_bin = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.edges";
	_datasets[43].min_vertex = 0;
	_datasets[43].max_vertex = (1 << 23);
	_datasets[43].num_vertices = (1 << 23);
	_datasets[43].num_edges = (1 << 26);
	_datasets[43].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[43].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[43].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[44].graphtopname = "RMAT_RANGE4";
	_datasets[44].graphname = "RMAT_RANGE4";
	_datasets[44].graph_path = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.mtx";
	_datasets[44].vertices_path = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.vertices";
	_datasets[44].edges_path = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.edges";	
	_datasets[44].vertices_path_bin = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.vertices";
	_datasets[44].edges_path_bin = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.edges";
	_datasets[44].min_vertex = 0;
	_datasets[44].max_vertex = (1 << 24);
	_datasets[44].num_vertices = (1 << 24);
	_datasets[44].num_edges = (1 << 26);
	_datasets[44].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[44].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[44].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[45].graphtopname = "RMAT_RANGE5";
	_datasets[45].graphname = "RMAT_RANGE5";
	_datasets[45].graph_path = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.mtx";
	_datasets[45].vertices_path = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.vertices";
	_datasets[45].edges_path = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.edges";	
	_datasets[45].vertices_path_bin = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.vertices";
	_datasets[45].edges_path_bin = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.edges";
	_datasets[45].min_vertex = 0;
	_datasets[45].max_vertex = (1 << 25);
	_datasets[45].num_vertices = (1 << 25);
	_datasets[45].num_edges = (1 << 26);
	_datasets[45].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[45].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[45].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[46].graphtopname = "RMAT_RANGE6";
	_datasets[46].graphname = "RMAT_RANGE6";
	_datasets[46].graph_path = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.mtx";
	_datasets[46].vertices_path = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.vertices";
	_datasets[46].edges_path = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.edges";	
	_datasets[46].vertices_path_bin = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.vertices";
	_datasets[46].edges_path_bin = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.edges";
	_datasets[46].min_vertex = 0;
	_datasets[46].max_vertex = (1 << 26);
	_datasets[46].num_vertices = (1 << 26);
	_datasets[46].num_edges = (1 << 26);
	_datasets[46].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[46].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[46].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[47].graphtopname = "RMAT_RANGE7";
	_datasets[47].graphname = "RMAT_RANGE7";
	_datasets[47].graph_path = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.mtx";
	_datasets[47].vertices_path = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.vertices";
	_datasets[47].edges_path = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.edges";	
	_datasets[47].vertices_path_bin = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.vertices";
	_datasets[47].edges_path_bin = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.edges";
	_datasets[47].min_vertex = 0;
	_datasets[47].max_vertex = (1 << 27);
	_datasets[47].num_vertices = (1 << 27);
	_datasets[47].num_edges = (1 << 26);
	_datasets[47].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[47].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[47].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	cout<<"graph::loadalldatasets finished. "<<endl;
	return;
}
void graph::load_large_datasets(){
	cout<<"graph::loadalldatasets:: "<<endl;
	
	#ifdef AWS_PLATFORM
	string rootDir = "/oj2zf/";
	#else
	string rootDir = "/home/oj2zf/"; // /home/oj2zf
	#endif

	_datasets[20].graphtopname = "twitter";
	_datasets[20].graphname = "twitter";
	_datasets[20].graph_path = rootDir + "dataset/twitter/twitter.mtx";
	_datasets[20].vertices_path = rootDir + "dataset/twitter/twitter.vertices";
	_datasets[20].edges_path = rootDir + "dataset/twitter/twitter.edges";	
	_datasets[20].vertices_path_bin = rootDir + "dataset/twitter/twitter_bin.vertices"; 
	_datasets[20].edges_path_bin = rootDir + "dataset/twitter/twitter_bin.edges";
	_datasets[20].min_vertex = 0;	
	_datasets[20].max_vertex = 41652230; 
	_datasets[20].num_vertices = 41652230; 
	_datasets[20].num_edges = 1468365184; 
	_datasets[20].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[20].graphorder = DST_SRC;
	_datasets[20].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[21].graphtopname = "MOLIERE_2016";
	_datasets[21].graphname = "MOLIERE_2016";
	_datasets[21].graph_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.mtx";
	_datasets[21].vertices_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.vertices";
	_datasets[21].edges_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.edges";	
	_datasets[21].vertices_path_bin = rootDir + "dataset/MOLIERE_2016/MOLIERE2016_bin.vertices"; 
	_datasets[21].edges_path_bin = rootDir + "dataset/MOLIERE_2016/MOLIERE2016_bin.edges";
	_datasets[21].min_vertex = 0;	
	_datasets[21].max_vertex = 30239687; 
	_datasets[21].num_vertices = 30239687; 
	_datasets[21].num_edges = 3338650683; 
	_datasets[21].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[21].graphorder = DST_SRC_EDGEW;
	_datasets[21].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[22].graphtopname = "kronecker26";
	_datasets[22].graphname = "kronecker26";
	_datasets[22].graph_path = rootDir + "dataset/kronecker26/kronecker26.mtx";
	_datasets[22].vertices_path = rootDir + "dataset/kronecker26/kronecker26.vertices";
	_datasets[22].edges_path = rootDir + "dataset/kronecker26/kronecker26.edges";	
	_datasets[22].vertices_path_bin = rootDir + "dataset/kronecker26/kronecker26_bin.vertices";
	_datasets[22].edges_path_bin = rootDir + "dataset/kronecker26/kronecker26_bin.edges";
	_datasets[22].min_vertex = 0;
	_datasets[22].max_vertex = 67108864; 
	_datasets[22].num_vertices = 67108864;  // 67108864, 67108816(prev)*
	_datasets[22].num_edges = 1073741824; // 1073741824
	_datasets[22].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[22].graphorder = SRC_DST;
	_datasets[22].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[23].graphtopname = "kronecker28";
	_datasets[23].graphname = "kronecker28";
	_datasets[23].graph_path = rootDir + "dataset/kronecker28/kronecker28.mtx";
	_datasets[23].vertices_path = rootDir + "dataset/kronecker28/kronecker28.vertices";
	_datasets[23].edges_path = rootDir + "dataset/kronecker28/kronecker28.edges";	
	_datasets[23].vertices_path_bin = rootDir + "dataset/kronecker28/kronecker28_bin.vertices";
	_datasets[23].edges_path_bin = rootDir + "dataset/kronecker28/kronecker28_bin.edges";
	_datasets[23].min_vertex = 0;
	_datasets[23].max_vertex = 268435456; // 268435456 // 268431360(prev)
	_datasets[23].num_vertices = 268435456; 
	_datasets[23].num_edges = 4294967290;
	_datasets[23].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[23].graphorder = SRC_DST;
	_datasets[23].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[24].graphtopname = "kronecker30";
	_datasets[24].graphname = "kronecker30";
	_datasets[24].graph_path = rootDir + "dataset/kronecker30/kronecker30.mtx";
	_datasets[24].vertices_path = rootDir + "dataset/kronecker30/kronecker30.vertices";
	_datasets[24].edges_path = rootDir + "dataset/kronecker30/kronecker30.edges";	
	_datasets[24].vertices_path_bin = rootDir + "dataset/kronecker30/kronecker30_bin.vertices"; 
	_datasets[24].edges_path_bin = rootDir + "dataset/kronecker30/kronecker30_bin.edges"; 
	_datasets[24].min_vertex = 0;
	_datasets[24].max_vertex = 1073741824; // 1073741824 // 1041451012(prev)
	_datasets[24].num_vertices = 1073741824;
	_datasets[24].num_edges = 17179869184;
	_datasets[24].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[24].graphorder = SRC_DST;
	_datasets[24].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[25].graphtopname = "kronecker32";
	_datasets[25].graphname = "kronecker32";
	_datasets[25].graph_path = rootDir + "dataset/kronecker32/kronecker32.mtx";
	_datasets[25].vertices_path = rootDir + "dataset/kronecker32/kronecker32.vertices";
	_datasets[25].edges_path = rootDir + "dataset/kronecker32/kronecker32.edges";	
	_datasets[25].vertices_path_bin = rootDir + "dataset/kronecker32/kronecker32_bin.vertices"; 
	_datasets[25].edges_path_bin = rootDir + "dataset/kronecker32/kronecker32_bin.edges"; 
	_datasets[25].min_vertex = 0;
	_datasets[25].max_vertex = 4294967296; 
	_datasets[25].num_vertices = 4294967296;
	_datasets[25].num_edges = 68719476736;
	_datasets[25].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[25].graphorder = SRC_DST;
	_datasets[25].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[26].graphtopname = "wdc";
	_datasets[26].graphname = "wdc";
	_datasets[26].graph_path = rootDir + "dataset/wdc/wdc.mtx";
	_datasets[26].vertices_path = rootDir + "dataset/wdc/wdc.vertices";
	_datasets[26].edges_path = rootDir + "dataset/wdc/wdc.edges";	
	_datasets[26].vertices_path_bin = "/projects/DPE/wole/wdc/wdc_bin.vertices"; 
	_datasets[26].edges_path_bin = "/projects/DPE/wole/wdc/wdc_bin.edges";
	_datasets[26].min_vertex = 0;
	_datasets[26].max_vertex = 3315452338;
	_datasets[26].num_vertices = 3315452338;
	_datasets[26].num_edges = 0;
	_datasets[26].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[26].graphorder = SRC_DST;
	_datasets[26].graphgroup = SNAP; // SKEWRATIO;

	cout<<"graph::loadalldatasets finished. "<<endl;
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
	
	#ifdef GRAPHISUNDIRECTED
	thisdataset.graphname = thisdataset.graphname + "_dup";
	thisdataset.num_edges = 2 * thisdataset.num_edges;
	#endif
	
	printdataset();
	return;
}
dataset_t graph::getdataset(){
	return thisdataset;
}
void graph::printdataset(){
	cout<<"graph::printdataset: printing dataset parameters..."<<endl;
	cout<<">>> graphtopname: "<<thisdataset.graphtopname<<endl;
	cout<<">>> graphname: "<<thisdataset.graphname<<endl;
	cout<<">>> graph_path: "<<thisdataset.graph_path<<endl;
	cout<<">>> vertices_path: "<<thisdataset.vertices_path<<endl;
	cout<<">>> edges_path: "<<thisdataset.edges_path<<endl;	
	cout<<">>> vertices_path_bin: "<<thisdataset.vertices_path_bin<<endl;
	cout<<">>> edges_path_bin: "<<thisdataset.edges_path_bin<<endl;
	cout<<">>> min_vertex: "<<thisdataset.min_vertex<<endl;
	cout<<">>> max_vertex: "<<thisdataset.max_vertex<<endl;
	cout<<">>> num_vertices: "<<thisdataset.num_vertices<<endl; 
	cout<<">>> num_edges: "<<thisdataset.num_edges<<endl;
	cout<<">>> graphdirectiontype: "<<thisdataset.graphdirectiontype<<endl;
	cout<<">>> graphorder: "<<thisdataset.graphorder<<endl;
	cout<<">>> graphgroup: "<<thisdataset.graphgroup<<endl;
	return;
}

vertexprop_t * graph::getvertexpropertybuffer(){
	return vertexpropertybuffer;
}
value_t * graph::getvertexdatabuffer(){
	return vertexdatabuffer;
}
value_t * graph::gettempvertexdatabuffer(){
	return tempvertexdatabuffer;
}
edge_t * graph::getvertexptrbuffer(){ 
	return vertexptrbuffer;
}
unsigned int * graph::getvertexisactivebuffer(){
	return vertexisactivebitbuffer;
}

unsigned long graph::gettotalkeyvaluesread(unsigned int col){
	return totalkeyvaluesread[col][0];
}
void graph::appendkeyvaluesread(unsigned int col, unsigned int value){
	totalkeyvaluesread[col][0] += value;
}








