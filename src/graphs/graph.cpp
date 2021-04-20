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
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h"
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
string datasetRootDir = "/localtmp/oj2zf/";
string tmpDatarootDir = "/localtmp/oj2zf/";
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
value_t * graph::generateverticesdata(){ 
	cout<<"generating vertices data... "<<endl;
	for(unsigned int k=0; k<KVDATA_RANGE; k++){
		vertexdatabuffer[k] = algorithmobj->vertex_initdata(); 
	} 
	return vertexdatabuffer;
}
value_t * graph::generatetempverticesdata(){ 
	cout<<"generating temp vertices data... "<<endl;
	for(unsigned int k=0; k<KVDATA_RANGE; k++){ tempvertexdatabuffer[k] = algorithmobj->vertex_initdata(); }					 
	return tempvertexdatabuffer;
}
void graph::loadedgesfromfile(int col, size_t fdoffset, edge2_type * buffer, vertex_t bufferoffset, vertex_t size){
	if(size > 0){ if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset], (size * sizeof(edge2_type)), fdoffset * sizeof(edge2_type)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"]. EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }}					
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
	string rootDir = "/localtmp/oj2zf/";
	#endif
	
	// small dataset
	_datasets[30].graphtopname = "soc-orkut";
	_datasets[30].graphname = "soc-orkut";
	_datasets[30].graph_path = rootDir + "dataset/soc-orkut/soc-orkut.mtx";
	_datasets[30].vertices_path = rootDir + "dataset/soc-orkut/soc-orkut.vertices";
	_datasets[30].edges_path = rootDir + "dataset/soc-orkut/soc-orkut.edges";	
	_datasets[30].vertices_path_bin = rootDir + "dataset/soc-orkut/soc-orkut_bin.vertices";
	_datasets[30].edges_path_bin = rootDir + "dataset/soc-orkut/soc-orkut_bin.edges";
	_datasets[30].min_vertex = 0;
	_datasets[30].max_vertex = 2997166; 
	_datasets[30].num_vertices = 2997166;
	_datasets[30].num_edges = 106349209;
	_datasets[30].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[30].graphorder = DST_SRC;
	_datasets[30].skewratio = NAp; // SKEWRATIO;
	
	_datasets[31].graphtopname = "hollywood-2009";
	_datasets[31].graphname = "hollywood-2009";
	_datasets[31].graph_path = rootDir + "dataset/hollywood-2009/hollywood-2009.mtx";
	_datasets[31].vertices_path = rootDir + "dataset/hollywood-2009/hollywood-2009.vertices";
	_datasets[31].edges_path = rootDir + "dataset/hollywood-2009/hollywood-2009.edges";	
	_datasets[31].vertices_path_bin = rootDir + "dataset/hollywood-2009/hollywood-2009_bin.vertices";
	_datasets[31].edges_path_bin = rootDir + "dataset/hollywood-2009/hollywood-2009_bin.edges";
	_datasets[31].min_vertex = 0;
	_datasets[31].max_vertex = 1139905; 
	_datasets[31].num_vertices = 1139905;
	_datasets[31].num_edges = 57515616;
	_datasets[31].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[31].graphorder = DST_SRC;
	_datasets[31].skewratio = NAp; // SKEWRATIO;
	
	_datasets[32].graphtopname = "indochina-04";
	_datasets[32].graphname = "indochina-04";
	_datasets[32].graph_path = rootDir + "dataset/indochina-04/indochina-2004.mtx";
	_datasets[32].vertices_path = rootDir + "dataset/indochina-04/indochina-2004.vertices";
	_datasets[32].edges_path = rootDir + "dataset/indochina-04/indochina-2004.edges";	
	_datasets[32].vertices_path_bin = rootDir + "dataset/indochina-04/indochina-2004_bin.vertices";
	_datasets[32].edges_path_bin = rootDir + "dataset/indochina-04/indochina-2004_bin.edges";
	_datasets[32].min_vertex = 0;
	_datasets[32].max_vertex = 7414866; 
	_datasets[32].num_vertices = 7414866;
	_datasets[32].num_edges = 194109311;
	_datasets[32].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[32].graphorder = DST_SRC;
	_datasets[32].skewratio = NAp; // SKEWRATIO;
	
	_datasets[33].graphtopname = "kron-g500-logn21";
	_datasets[33].graphname = "kron-g500-logn21";
	_datasets[33].graph_path = rootDir + "dataset/kron-g500-logn21/kron-g500-logn21.mtx";
	_datasets[33].vertices_path = rootDir + "dataset/kron-g500-logn21/kron-g500-logn21.vertices";
	_datasets[33].edges_path = rootDir + "dataset/kron-g500-logn21/kron-g500-logn21.edges";	
	_datasets[33].vertices_path_bin = rootDir + "dataset/kron-g500-logn21/kron-g500-logn21_bin.vertices";
	_datasets[33].edges_path_bin = rootDir + "dataset/kron-g500-logn21/kron-g500-logn21_bin.edges";
	_datasets[33].min_vertex = 0;
	_datasets[33].max_vertex = 2097151; // 2097152, 2097151
	_datasets[33].num_vertices = 2097151; // 2097152, 2097151
	_datasets[33].num_edges = 91042010;
	_datasets[33].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[33].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[33].skewratio = NAp; // SKEWRATIO;
	
	// https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/rgg_n_2_24_s0.tar.gz
	_datasets[34].graphtopname = "rgg_n_2_24_s0";
	_datasets[34].graphname = "rgg_n_2_24_s0";
	_datasets[34].graph_path = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0.mtx";
	_datasets[34].vertices_path = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0.vertices";
	_datasets[34].edges_path = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0.edges";	
	_datasets[34].vertices_path_bin = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0_bin.vertices";
	_datasets[34].edges_path_bin = rootDir + "dataset/rgg_n_2_24_s0/rgg_n_2_24_s0_bin.edges";
	_datasets[34].min_vertex = 0;
	_datasets[34].max_vertex = 16777216; 
	_datasets[34].num_vertices = 16777216;
	_datasets[34].num_edges = 132557200;
	_datasets[34].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[34].graphorder = DST_SRC;
	_datasets[34].skewratio = NAp; // SKEWRATIO;
	
	_datasets[35].graphtopname = "roadNet-CA";
	_datasets[35].graphname = "roadNet-CA";
	_datasets[35].graph_path = rootDir + "dataset/roadNet-CA/roadNet-CA.mtx";
	_datasets[35].vertices_path = rootDir + "dataset/roadNet-CA/roadNet-CA.vertices";
	_datasets[35].edges_path = rootDir + "dataset/roadNet-CA/roadNet-CA.edges";	
	_datasets[35].vertices_path_bin = rootDir + "dataset/roadNet-CA/roadNet-CA_bin.vertices";
	_datasets[35].edges_path_bin = rootDir + "dataset/roadNet-CA/roadNet-CA_bin.edges";
	_datasets[35].min_vertex = 0;
	_datasets[35].max_vertex = 1971281; 
	_datasets[35].num_vertices = 1971281;
	_datasets[35].num_edges = 2766607;
	_datasets[35].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[35].graphorder = DST_SRC;
	_datasets[35].skewratio = NAp; // SKEWRATIO;
	
	_datasets[36].graphtopname = "flickr";
	_datasets[36].graphname = "flickr";
	_datasets[36].graph_path = rootDir + "dataset/flickr/flickr.mtx";
	_datasets[36].vertices_path = rootDir + "dataset/flickr/flickr.vertices";
	_datasets[36].edges_path = rootDir + "dataset/flickr/flickr.edges";
	_datasets[36].vertices_path_bin = rootDir + "dataset/flickr/flickr_bin.vertices";
	_datasets[36].edges_path_bin = rootDir + "dataset/flickr/flickr_bin.edges";
	_datasets[36].min_vertex = 0;
	_datasets[36].max_vertex = 820878;
	_datasets[36].num_vertices = 820879;
	_datasets[36].num_edges = 9837214;
	_datasets[36].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[36].graphorder = DST_SRC;
	_datasets[36].skewratio = NAp; // SKEWRATIO;
	
	_datasets[37].graphtopname = "kron_g500-logn20";
	_datasets[37].graphname = "kron_g500-logn20";
	_datasets[37].graph_path = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20.mtx";
	_datasets[37].vertices_path = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20.vertices";
	_datasets[37].edges_path = rootDir + "dataset/kron_g500-logn21/kron_g500-logn20.edges";	
	_datasets[37].vertices_path_bin = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20_bin.vertices";
	_datasets[37].edges_path_bin = rootDir + "dataset/kron_g500-logn20/kron_g500-logn20_bin.edges";
	_datasets[37].min_vertex = 0;
	_datasets[37].max_vertex = 1048576;
	_datasets[37].num_vertices = 1048576;
	_datasets[37].num_edges = 44620272;
	_datasets[37].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[37].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[37].skewratio = NAp; // SKEWRATIO;
	
	// https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/rgg_n_2_22_s0.tar.gz
	_datasets[38].graphtopname = "rgg_n_2_22_s0";
	_datasets[38].graphname = "rgg_n_2_22_s0";
	_datasets[38].graph_path = rootDir + "dataset/rgg_n_2_22_s0/rgg_n_2_22_s0.mtx";
	_datasets[38].vertices_path = rootDir + "dataset/rgg_n_2_22_s0/rgg_n_2_22_s0.vertices";
	_datasets[38].edges_path = rootDir + "dataset/rgg_n_2_22_s0/rgg_n_2_22_s0.edges";	
	_datasets[38].vertices_path_bin = rootDir + "dataset/rgg_n_2_22_s0/rgg_n_2_22_s0_bin.vertices";
	_datasets[38].edges_path_bin = rootDir + "dataset/rgg_n_2_22_s0/rgg_n_2_22_s0_bin.edges";
	_datasets[38].min_vertex = 0;
	_datasets[38].max_vertex = 4194304; 
	_datasets[38].num_vertices = 4194304;
	_datasets[38].num_edges = 30359198;
	_datasets[38].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[38].graphorder = DST_SRC;
	_datasets[38].skewratio = NAp; // SKEWRATIO;
	
	// large dataset
	_datasets[2].graphtopname = "twitter";
	_datasets[2].graphname = "twitter";
	_datasets[2].graph_path = rootDir + "dataset/twitter/twitter.mtx";
	_datasets[2].vertices_path = rootDir + "dataset/twitter/twitter.vertices";
	_datasets[2].edges_path = rootDir + "dataset/twitter/twitter.edges";	
	_datasets[2].vertices_path_bin = rootDir + "dataset/twitter/twitter_bin.vertices"; 
	_datasets[2].edges_path_bin = rootDir + "dataset/twitter/twitter_bin.edges";
	_datasets[2].min_vertex = 0;	
	_datasets[2].max_vertex = 41652230; 
	_datasets[2].num_vertices = 41652230; 
	_datasets[2].num_edges = 1468365184; 
	_datasets[2].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[2].graphorder = DST_SRC;
	_datasets[2].skewratio = NAp; // SKEWRATIO;
	
	_datasets[3].graphtopname = "MOLIERE_2016";
	_datasets[3].graphname = "MOLIERE_2016";
	_datasets[3].graph_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.mtx";
	_datasets[3].vertices_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.vertices";
	_datasets[3].edges_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.edges";	
	_datasets[3].vertices_path_bin = rootDir + "dataset/MOLIERE_2016/MOLIERE2016_bin.vertices"; 
	_datasets[3].edges_path_bin = rootDir + "dataset/MOLIERE_2016/MOLIERE2016_bin.edges";
	_datasets[3].min_vertex = 0;	
	_datasets[3].max_vertex = 30239687; 
	_datasets[3].num_vertices = 30239687; 
	_datasets[3].num_edges = 3338650683; 
	_datasets[3].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[3].graphorder = DST_SRC_EDGEW;
	_datasets[3].skewratio = NAp; // SKEWRATIO;
	
	_datasets[4].graphtopname = "kronecker26";
	_datasets[4].graphname = "kronecker26";
	_datasets[4].graph_path = rootDir + "dataset/kronecker26/kronecker26.mtx";
	_datasets[4].vertices_path = rootDir + "dataset/kronecker26/kronecker26.vertices";
	_datasets[4].edges_path = rootDir + "dataset/kronecker26/kronecker26.edges";	
	_datasets[4].vertices_path_bin = rootDir + "dataset/kronecker26/kronecker26_bin.vertices";
	_datasets[4].edges_path_bin = rootDir + "dataset/kronecker26/kronecker26_bin.edges";
	_datasets[4].min_vertex = 0;
	_datasets[4].max_vertex = 67108864; 
	_datasets[4].num_vertices = 67108864;  // 67108864, 67108816(prev)*
	_datasets[4].num_edges = 1073741824; // 1073741824
	_datasets[4].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[4].graphorder = SRC_DST;
	_datasets[4].skewratio = NAp; // SKEWRATIO;
	
	_datasets[5].graphtopname = "kronecker28";
	_datasets[5].graphname = "kronecker28";
	_datasets[5].graph_path = rootDir + "dataset/kronecker28/kronecker28.mtx";
	_datasets[5].vertices_path = rootDir + "dataset/kronecker28/kronecker28.vertices";
	_datasets[5].edges_path = rootDir + "dataset/kronecker28/kronecker28.edges";	
	_datasets[5].vertices_path_bin = rootDir + "dataset/kronecker28/kronecker28_bin.vertices";
	_datasets[5].edges_path_bin = rootDir + "dataset/kronecker28/kronecker28_bin.edges";
	_datasets[5].min_vertex = 0;
	_datasets[5].max_vertex = 268435456; // 268435456 // 268431360(prev)
	_datasets[5].num_vertices = 268435456; 
	_datasets[5].num_edges = 4294967290;
	_datasets[5].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[5].graphorder = SRC_DST;
	_datasets[5].skewratio = NAp; // SKEWRATIO;
	
	_datasets[6].graphtopname = "kronecker30";
	_datasets[6].graphname = "kronecker30";
	_datasets[6].graph_path = rootDir + "dataset/kronecker30/kronecker30.mtx";
	_datasets[6].vertices_path = rootDir + "dataset/kronecker30/kronecker30.vertices";
	_datasets[6].edges_path = rootDir + "dataset/kronecker30/kronecker30.edges";	
	_datasets[6].vertices_path_bin = rootDir + "dataset/kronecker30/kronecker30_bin.vertices"; 
	_datasets[6].edges_path_bin = rootDir + "dataset/kronecker30/kronecker30_bin.edges"; 
	_datasets[6].min_vertex = 0;
	_datasets[6].max_vertex = 1073741824; // 1073741824 // 1041451012(prev)
	_datasets[6].num_vertices = 1073741824;
	_datasets[6].num_edges = 17179869184;
	_datasets[6].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[6].graphorder = SRC_DST;
	_datasets[6].skewratio = NAp; // SKEWRATIO;
	
	_datasets[7].graphtopname = "kronecker32";
	_datasets[7].graphname = "kronecker32";
	_datasets[7].graph_path = rootDir + "dataset/kronecker32/kronecker32.mtx";
	_datasets[7].vertices_path = rootDir + "dataset/kronecker32/kronecker32.vertices";
	_datasets[7].edges_path = rootDir + "dataset/kronecker32/kronecker32.edges";	
	_datasets[7].vertices_path_bin = rootDir + "dataset/kronecker32/kronecker32_bin.vertices"; 
	_datasets[7].edges_path_bin = rootDir + "dataset/kronecker32/kronecker32_bin.edges"; 
	_datasets[7].min_vertex = 0;
	_datasets[7].max_vertex = 4294967296; 
	_datasets[7].num_vertices = 4294967296;
	_datasets[7].num_edges = 68719476736;
	_datasets[7].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[7].graphorder = SRC_DST;
	_datasets[7].skewratio = NAp; // SKEWRATIO;
	
	_datasets[10].graphtopname = "wdc";
	_datasets[10].graphname = "wdc";
	_datasets[10].graph_path = rootDir + "dataset/wdc/wdc.mtx";
	_datasets[10].vertices_path = rootDir + "dataset/wdc/wdc.vertices";
	_datasets[10].edges_path = rootDir + "dataset/wdc/wdc.edges";	
	_datasets[10].vertices_path_bin = "/projects/DPE/wole/wdc/wdc_bin.vertices"; 
	_datasets[10].edges_path_bin = "/projects/DPE/wole/wdc/wdc_bin.edges";
	_datasets[10].min_vertex = 0;
	_datasets[10].max_vertex = 3315452338;
	_datasets[10].num_vertices = 3315452338;
	_datasets[10].num_edges = 0;
	_datasets[10].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[10].graphorder = SRC_DST;
	_datasets[10].skewratio = NAp; // SKEWRATIO;

	for(unsigned int i=0; i<32; i++){
		_datasets[i].groupvoffset[0] = 0;
		_datasets[i].groupvoffset[1] = _datasets[i].num_vertices;
	}
	
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
	cout<<">>> skewratio: "<<thisdataset.skewratio<<endl;
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








