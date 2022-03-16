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
	
	// unsigned int szA = size/2;
	// unsigned int szB = size - szA;
	
	// if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset], (szA * sizeof(edge2_type)), fdoffset * sizeof(edge2_type)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](A). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	// if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset + szA], (szB * sizeof(edge2_type)), (fdoffset + szA) * sizeof(edge2_type)) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](B). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	
	size_t szA = size/4;
	size_t szB = size/4;
	size_t szC = size/4;
	size_t szD = size - (szA + szB + szC);
	
	if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset], (size_t)(szA * sizeof(edge2_type)), (size_t)(fdoffset * sizeof(edge2_type))) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](A). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset + szA], (size_t)(szB * sizeof(edge2_type)), (size_t)((fdoffset + szA) * sizeof(edge2_type))) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](B). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset + szA + szB], (size_t)(szB * sizeof(edge2_type)), (size_t)((fdoffset + szA + szB) * sizeof(edge2_type))) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](A). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
	if(pread(nvmeFd_edges_r2[col], &buffer[bufferoffset + szA + szB + szC], (size_t)(szC * sizeof(edge2_type)), (size_t)((fdoffset + szA + szB + szC) * sizeof(edge2_type))) <= 0){ cout<<"graph::loadedgesfromfile:: ERROR. insufficient edges at col["<<col<<"](B). EXITING..."<<endl; utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }
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

	for(unsigned int i=0; i<1024; i++){
		_datasets[i].groupvoffset[0] = 0;
		_datasets[i].groupvoffset[1] = _datasets[i].num_vertices;
		_datasets[i].max_vertex = _datasets[i].num_vertices - 1; // NEWCHANGE.
		// _datasets[i].rawgraph_path = rootDir + "dataset/"+_datasets[i].graphtopname+"/"+_datasets[i].graphtopname+".dat";
		_datasets[i].rawgraph_path = _datasets[i].graph_path;
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
	_datasets[6].num_edges = 132557200;
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
	_datasets[7].num_edges = 54054660;
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
	_datasets[8].num_edges = 58608800;
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
	_datasets[9].num_edges = 31790179;
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
	_datasets[10].num_edges = 69389281;
	_datasets[10].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[10].graphorder = DST_SRC;
	_datasets[10].graphgroup = SNAP;
	
	_datasets[11].graphtopname = "indochina-2004";
	_datasets[11].graphname = "indochina-2004";
	_datasets[11].graph_path = rootDir + "dataset/indochina-2004/indochina-2004.mtx";
	_datasets[11].vertices_path = rootDir + "dataset/indochina-2004/indochina-2004.vertices";
	_datasets[11].edges_path = rootDir + "dataset/indochina-2004/indochina-2004.edges";	
	_datasets[11].vertices_path_bin = rootDir + "dataset/indochina-2004/indochina-2004_bin.vertices";
	_datasets[11].edges_path_bin = rootDir + "dataset/indochina-2004/indochina-2004_bin.edges";
	_datasets[11].min_vertex = 0;
	_datasets[11].max_vertex = 7414866; 
	_datasets[11].num_vertices = 7414866;
	_datasets[11].num_edges = 194109311;
	_datasets[11].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[11].graphorder = DST_SRC;
	_datasets[11].graphgroup = SNAP;
	
	_datasets[12].graphtopname = "uk-2002";
	_datasets[12].graphname = "uk-2002";
	_datasets[12].graph_path = rootDir + "dataset/uk-2002/uk-2002.mtx";
	_datasets[12].vertices_path = rootDir + "dataset/uk-2002/uk-2002.vertices";
	_datasets[12].edges_path = rootDir + "dataset/uk-2002/uk-2002.edges";	
	_datasets[12].vertices_path_bin = rootDir + "dataset/uk-2002/uk-2002_bin.vertices";
	_datasets[12].edges_path_bin = rootDir + "dataset/uk-2002/uk-2002_bin.edges";
	_datasets[12].min_vertex = 0;
	_datasets[12].max_vertex = 18520486; 
	_datasets[12].num_vertices = 18520486;
	_datasets[12].num_edges = 298113762;
	_datasets[12].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[12].graphorder = DST_SRC;
	_datasets[12].graphgroup = SNAP;
	
	_datasets[13].graphtopname = "ljournal-2008";
	_datasets[13].graphname = "ljournal-2008";
	_datasets[13].graph_path = rootDir + "dataset/ljournal-2008/ljournal-2008.mtx";
	_datasets[13].vertices_path = rootDir + "dataset/ljournal-2008/ljournal-2008.vertices";
	_datasets[13].edges_path = rootDir + "dataset/ljournal-2008/ljournal-2008.edges";	
	_datasets[13].vertices_path_bin = rootDir + "dataset/ljournal-2008/ljournal-2008_bin.vertices";
	_datasets[13].edges_path_bin = rootDir + "dataset/ljournal-2008/ljournal-2008_bin.edges";
	_datasets[13].min_vertex = 0;
	_datasets[13].max_vertex = 5363260; 
	_datasets[13].num_vertices = 5363260;
	_datasets[13].num_edges = 79023142;
	_datasets[13].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[13].graphorder = DST_SRC;
	_datasets[13].graphgroup = SNAP;
	
	_datasets[14].graphtopname = "mawi_201512020030";
	_datasets[14].graphname = "mawi_201512020030";
	_datasets[14].graph_path = rootDir + "dataset/mawi_201512020030/mawi_201512020030.mtx";
	_datasets[14].vertices_path = rootDir + "dataset/mawi_201512020030/mawi_201512020030.vertices";
	_datasets[14].edges_path = rootDir + "dataset/mawi_201512020030/mawi_201512020030.edges";	
	_datasets[14].vertices_path_bin = rootDir + "dataset/mawi_201512020030/mawi_201512020030_bin.vertices";
	_datasets[14].edges_path_bin = rootDir + "dataset/mawi_201512020030/mawi_201512020030_bin.edges";
	_datasets[14].min_vertex = 0;
	_datasets[14].max_vertex = 68863315; 
	_datasets[14].num_vertices = 68863315;
	_datasets[14].num_edges = 71707480;
	_datasets[14].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[14].graphorder = DST_SRC;
	_datasets[14].graphgroup = SNAP;
	
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
	
	_datasets[100].graphtopname = "RMAT22_SPARSE0";
	_datasets[100].graphname = "RMAT22_SPARSE0";
	_datasets[100].graph_path = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.mtx";
	_datasets[100].vertices_path = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.vertices";
	_datasets[100].edges_path = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.edges";	
	_datasets[100].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.vertices";
	_datasets[100].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE0/RMAT22_SPARSE0.edges";
	_datasets[100].min_vertex = 0;
	_datasets[100].max_vertex = (1 << 22);
	_datasets[100].num_vertices = (1 << 22);
	_datasets[100].num_edges = ((1 << 22) * 30);
	_datasets[100].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[100].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[100].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[101].graphtopname = "RMAT22_SPARSE1";
	_datasets[101].graphname = "RMAT22_SPARSE1";
	_datasets[101].graph_path = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.mtx";
	_datasets[101].vertices_path = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.vertices";
	_datasets[101].edges_path = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.edges";	
	_datasets[101].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.vertices";
	_datasets[101].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE1/RMAT22_SPARSE1.edges";
	_datasets[101].min_vertex = 0;
	_datasets[101].max_vertex = (1 << 22);
	_datasets[101].num_vertices = (1 << 22);
	_datasets[101].num_edges = ((1 << 22) * 30);
	_datasets[101].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[101].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[101].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[102].graphtopname = "RMAT22_SPARSE2";
	_datasets[102].graphname = "RMAT22_SPARSE2";
	_datasets[102].graph_path = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.mtx";
	_datasets[102].vertices_path = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.vertices";
	_datasets[102].edges_path = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.edges";	
	_datasets[102].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.vertices";
	_datasets[102].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE2/RMAT22_SPARSE2.edges";
	_datasets[102].min_vertex = 0;
	_datasets[102].max_vertex = (1 << 22);
	_datasets[102].num_vertices = (1 << 22);
	_datasets[102].num_edges = ((1 << 22) * 30);
	_datasets[102].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[102].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[102].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[103].graphtopname = "RMAT22_SPARSE3";
	_datasets[103].graphname = "RMAT22_SPARSE3";
	_datasets[103].graph_path = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.mtx";
	_datasets[103].vertices_path = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.vertices";
	_datasets[103].edges_path = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.edges";	
	_datasets[103].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.vertices";
	_datasets[103].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE3/RMAT22_SPARSE3.edges";
	_datasets[103].min_vertex = 0;
	_datasets[103].max_vertex = (1 << 22);
	_datasets[103].num_vertices = (1 << 22);
	_datasets[103].num_edges = ((1 << 22) * 30);
	_datasets[103].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[103].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[103].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[104].graphtopname = "RMAT22_SPARSE4";
	_datasets[104].graphname = "RMAT22_SPARSE4";
	_datasets[104].graph_path = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.mtx";
	_datasets[104].vertices_path = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.vertices";
	_datasets[104].edges_path = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.edges";	
	_datasets[104].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.vertices";
	_datasets[104].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE4/RMAT22_SPARSE4.edges";
	_datasets[104].min_vertex = 0;
	_datasets[104].max_vertex = (1 << 22);
	_datasets[104].num_vertices = (1 << 22);
	_datasets[104].num_edges = ((1 << 22) * 30);
	_datasets[104].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[104].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[104].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[105].graphtopname = "RMAT22_SPARSE5";
	_datasets[105].graphname = "RMAT22_SPARSE5";
	_datasets[105].graph_path = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.mtx";
	_datasets[105].vertices_path = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.vertices";
	_datasets[105].edges_path = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.edges";	
	_datasets[105].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.vertices";
	_datasets[105].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE5/RMAT22_SPARSE5.edges";
	_datasets[105].min_vertex = 0;
	_datasets[105].max_vertex = (1 << 22);
	_datasets[105].num_vertices = (1 << 22);
	_datasets[105].num_edges = ((1 << 22) * 30);
	_datasets[105].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[105].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[105].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[106].graphtopname = "RMAT22_SPARSE6";
	_datasets[106].graphname = "RMAT22_SPARSE6";
	_datasets[106].graph_path = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.mtx";
	_datasets[106].vertices_path = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.vertices";
	_datasets[106].edges_path = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.edges";	
	_datasets[106].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.vertices";
	_datasets[106].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE6/RMAT22_SPARSE6.edges";
	_datasets[106].min_vertex = 0;
	_datasets[106].max_vertex = (1 << 22);
	_datasets[106].num_vertices = (1 << 22);
	_datasets[106].num_edges = ((1 << 22) * 30);
	_datasets[106].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[106].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[106].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[107].graphtopname = "RMAT22_SPARSE7";
	_datasets[107].graphname = "RMAT22_SPARSE7";
	_datasets[107].graph_path = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.mtx";
	_datasets[107].vertices_path = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.vertices";
	_datasets[107].edges_path = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.edges";	
	_datasets[107].vertices_path_bin = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.vertices";
	_datasets[107].edges_path_bin = rootDir + "dataset/RMAT22_SPARSE7/RMAT22_SPARSE7.edges";
	_datasets[107].min_vertex = 0;
	_datasets[107].max_vertex = (1 << 22);
	_datasets[107].num_vertices = (1 << 22);
	_datasets[107].num_edges = ((1 << 22) * 30);
	_datasets[107].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[107].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[107].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	/////////////////////////////////////////////////////////////////////
	
	_datasets[110].graphtopname = "RMAT21_SPARSE0";
	_datasets[110].graphname = "RMAT21_SPARSE0";
	_datasets[110].graph_path = rootDir + "dataset/RMAT21_SPARSE0/RMAT21_SPARSE0.mtx";
	_datasets[110].vertices_path = rootDir + "dataset/RMAT21_SPARSE0/RMAT21_SPARSE0.vertices";
	_datasets[110].edges_path = rootDir + "dataset/RMAT21_SPARSE0/RMAT21_SPARSE0.edges";	
	_datasets[110].vertices_path_bin = rootDir + "dataset/RMAT21_SPARSE0/RMAT21_SPARSE0.vertices";
	_datasets[110].edges_path_bin = rootDir + "dataset/RMAT21_SPARSE0/RMAT21_SPARSE0.edges";
	_datasets[110].min_vertex = 0;
	_datasets[110].max_vertex = (1 << 21);
	_datasets[110].num_vertices = (1 << 21);
	_datasets[110].num_edges = ((1 << 22) * 30);
	_datasets[110].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[110].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[110].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[112].graphtopname = "RMAT21_SPARSE2";
	_datasets[112].graphname = "RMAT21_SPARSE2";
	_datasets[112].graph_path = rootDir + "dataset/RMAT21_SPARSE2/RMAT21_SPARSE2.mtx";
	_datasets[112].vertices_path = rootDir + "dataset/RMAT21_SPARSE2/RMAT21_SPARSE2.vertices";
	_datasets[112].edges_path = rootDir + "dataset/RMAT21_SPARSE2/RMAT21_SPARSE2.edges";	
	_datasets[112].vertices_path_bin = rootDir + "dataset/RMAT21_SPARSE2/RMAT21_SPARSE2.vertices";
	_datasets[112].edges_path_bin = rootDir + "dataset/RMAT21_SPARSE2/RMAT21_SPARSE2.edges";
	_datasets[112].min_vertex = 0;
	_datasets[112].max_vertex = (1 << 21);
	_datasets[112].num_vertices = (1 << 21);
	_datasets[112].num_edges = ((1 << 22) * 30);
	_datasets[112].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[112].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[112].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[114].graphtopname = "RMAT21_SPARSE4";
	_datasets[114].graphname = "RMAT21_SPARSE4";
	_datasets[114].graph_path = rootDir + "dataset/RMAT21_SPARSE4/RMAT21_SPARSE4.mtx";
	_datasets[114].vertices_path = rootDir + "dataset/RMAT21_SPARSE4/RMAT21_SPARSE4.vertices";
	_datasets[114].edges_path = rootDir + "dataset/RMAT21_SPARSE4/RMAT21_SPARSE4.edges";	
	_datasets[114].vertices_path_bin = rootDir + "dataset/RMAT21_SPARSE4/RMAT21_SPARSE4.vertices";
	_datasets[114].edges_path_bin = rootDir + "dataset/RMAT21_SPARSE4/RMAT21_SPARSE4.edges";
	_datasets[114].min_vertex = 0;
	_datasets[114].max_vertex = (1 << 21);
	_datasets[114].num_vertices = (1 << 21);
	_datasets[114].num_edges = ((1 << 22) * 30);
	_datasets[114].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[114].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[114].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	///////////////////////////////////////////////////////////////////// synthetic datasets (II)
	
	_datasets[120].graphtopname = "RMAT_RANGE0";
	_datasets[120].graphname = "RMAT_RANGE0";
	_datasets[120].graph_path = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.mtx";
	_datasets[120].vertices_path = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.vertices";
	_datasets[120].edges_path = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.edges";	
	_datasets[120].vertices_path_bin = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.vertices";
	_datasets[120].edges_path_bin = rootDir + "dataset/RMAT_RANGE0/RMAT_RANGE0.edges";
	_datasets[120].min_vertex = 0;
	_datasets[120].max_vertex = (1 << 20);
	_datasets[120].num_vertices = (1 << 20);
	_datasets[120].num_edges = (1 << 26);
	_datasets[120].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[120].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[120].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[121].graphtopname = "RMAT_RANGE1";
	_datasets[121].graphname = "RMAT_RANGE1";
	_datasets[121].graph_path = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.mtx";
	_datasets[121].vertices_path = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.vertices";
	_datasets[121].edges_path = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.edges";	
	_datasets[121].vertices_path_bin = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.vertices";
	_datasets[121].edges_path_bin = rootDir + "dataset/RMAT_RANGE1/RMAT_RANGE1.edges";
	_datasets[121].min_vertex = 0;
	_datasets[121].max_vertex = (1 << 21);
	_datasets[121].num_vertices = (1 << 21);
	_datasets[121].num_edges = (1 << 26);
	_datasets[121].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[121].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[121].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[122].graphtopname = "RMAT_RANGE2";
	_datasets[122].graphname = "RMAT_RANGE2";
	_datasets[122].graph_path = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.mtx";
	_datasets[122].vertices_path = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.vertices";
	_datasets[122].edges_path = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.edges";	
	_datasets[122].vertices_path_bin = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.vertices";
	_datasets[122].edges_path_bin = rootDir + "dataset/RMAT_RANGE2/RMAT_RANGE2.edges";
	_datasets[122].min_vertex = 0;
	_datasets[122].max_vertex = (1 << 22);
	_datasets[122].num_vertices = (1 << 22);
	_datasets[122].num_edges = (1 << 26);
	_datasets[122].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[122].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[122].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[123].graphtopname = "RMAT_RANGE3";
	_datasets[123].graphname = "RMAT_RANGE3";
	_datasets[123].graph_path = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.mtx";
	_datasets[123].vertices_path = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.vertices";
	_datasets[123].edges_path = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.edges";	
	_datasets[123].vertices_path_bin = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.vertices";
	_datasets[123].edges_path_bin = rootDir + "dataset/RMAT_RANGE3/RMAT_RANGE3.edges";
	_datasets[123].min_vertex = 0;
	_datasets[123].max_vertex = (1 << 23);
	_datasets[123].num_vertices = (1 << 23);
	_datasets[123].num_edges = (1 << 26);
	_datasets[123].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[123].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[123].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[124].graphtopname = "RMAT_RANGE4";
	_datasets[124].graphname = "RMAT_RANGE4";
	_datasets[124].graph_path = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.mtx";
	_datasets[124].vertices_path = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.vertices";
	_datasets[124].edges_path = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.edges";	
	_datasets[124].vertices_path_bin = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.vertices";
	_datasets[124].edges_path_bin = rootDir + "dataset/RMAT_RANGE4/RMAT_RANGE4.edges";
	_datasets[124].min_vertex = 0;
	_datasets[124].max_vertex = (1 << 24);
	_datasets[124].num_vertices = (1 << 24);
	_datasets[124].num_edges = (1 << 26);
	_datasets[124].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[124].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[124].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[125].graphtopname = "RMAT_RANGE5";
	_datasets[125].graphname = "RMAT_RANGE5";
	_datasets[125].graph_path = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.mtx";
	_datasets[125].vertices_path = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.vertices";
	_datasets[125].edges_path = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.edges";	
	_datasets[125].vertices_path_bin = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.vertices";
	_datasets[125].edges_path_bin = rootDir + "dataset/RMAT_RANGE5/RMAT_RANGE5.edges";
	_datasets[125].min_vertex = 0;
	_datasets[125].max_vertex = (1 << 25);
	_datasets[125].num_vertices = (1 << 25);
	_datasets[125].num_edges = (1 << 26);
	_datasets[125].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[125].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[125].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[126].graphtopname = "RMAT_RANGE6";
	_datasets[126].graphname = "RMAT_RANGE6";
	_datasets[126].graph_path = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.mtx";
	_datasets[126].vertices_path = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.vertices";
	_datasets[126].edges_path = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.edges";	
	_datasets[126].vertices_path_bin = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.vertices";
	_datasets[126].edges_path_bin = rootDir + "dataset/RMAT_RANGE6/RMAT_RANGE6.edges";
	_datasets[126].min_vertex = 0;
	_datasets[126].max_vertex = (1 << 26);
	_datasets[126].num_vertices = (1 << 26);
	_datasets[126].num_edges = (1 << 26);
	_datasets[126].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[126].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[126].graphgroup = SYNTHETIC; // SKEWRATIO;
	
	_datasets[127].graphtopname = "RMAT_RANGE7";
	_datasets[127].graphname = "RMAT_RANGE7";
	_datasets[127].graph_path = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.mtx";
	_datasets[127].vertices_path = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.vertices";
	_datasets[127].edges_path = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.edges";	
	_datasets[127].vertices_path_bin = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.vertices";
	_datasets[127].edges_path_bin = rootDir + "dataset/RMAT_RANGE7/RMAT_RANGE7.edges";
	_datasets[127].min_vertex = 0;
	_datasets[127].max_vertex = (1 << 27);
	_datasets[127].num_vertices = (1 << 27);
	_datasets[127].num_edges = (1 << 26);
	_datasets[127].graphdirectiontype = DIRECTEDGRAPH;
	_datasets[127].graphorder = DST_SRC; // DST_SRC, SRC_DST;
	_datasets[127].graphgroup = SYNTHETIC; // SKEWRATIO;
	
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
	
	_datasets[200].graphtopname = "RMAT24_268M";
	_datasets[200].graphname = "RMAT24_268M";
	_datasets[200].graph_path = rootDir + "dataset/RMAT24_268M/RMAT24_268M.mtx";
	_datasets[200].vertices_path = rootDir + "dataset/RMAT24_268M/RMAT24_268M.vertices";
	_datasets[200].edges_path = rootDir + "dataset/RMAT24_268M/RMAT24_268M.edges";	
	_datasets[200].vertices_path_bin = rootDir + "dataset/RMAT24_268M/RMAT24_268M_bin.vertices"; 
	_datasets[200].edges_path_bin = rootDir + "dataset/RMAT24_268M/RMAT24_268M_bin.edges";
	_datasets[200].min_vertex = 0;	
	_datasets[200].max_vertex = 16777216; 
	_datasets[200].num_vertices = 16777216; 
	_datasets[200].num_edges = 268435456; 
	_datasets[200].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[200].graphorder = DST_SRC;
	_datasets[200].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[201].graphtopname = "RMAT25_268M";
	_datasets[201].graphname = "RMAT25_268M";
	_datasets[201].graph_path = rootDir + "dataset/RMAT25_268M/RMAT25_268M.mtx";
	_datasets[201].vertices_path = rootDir + "dataset/RMAT25_268M/RMAT25_268M.vertices";
	_datasets[201].edges_path = rootDir + "dataset/RMAT25_268M/RMAT25_268M.edges";	
	_datasets[201].vertices_path_bin = rootDir + "dataset/RMAT25_268M/RMAT25_268M_bin.vertices"; 
	_datasets[201].edges_path_bin = rootDir + "dataset/RMAT25_268M/RMAT25_268M_bin.edges";
	_datasets[201].min_vertex = 0;	
	_datasets[201].max_vertex = 33554432; 
	_datasets[201].num_vertices = 33554432; 
	_datasets[201].num_edges = 268435456; 
	_datasets[201].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[201].graphorder = DST_SRC;
	_datasets[201].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[202].graphtopname = "RMAT26_268M";
	_datasets[202].graphname = "RMAT26_268M";
	_datasets[202].graph_path = rootDir + "dataset/RMAT26_268M/RMAT26_268M.mtx";
	_datasets[202].vertices_path = rootDir + "dataset/RMAT26_268M/RMAT26_268M.vertices";
	_datasets[202].edges_path = rootDir + "dataset/RMAT26_268M/RMAT26_268M.edges";	
	_datasets[202].vertices_path_bin = rootDir + "dataset/RMAT26_268M/RMAT26_268M_bin.vertices"; 
	_datasets[202].edges_path_bin = rootDir + "dataset/RMAT26_268M/RMAT26_268M_bin.edges";
	_datasets[202].min_vertex = 0;	
	_datasets[202].max_vertex = 60000000; 
	_datasets[202].num_vertices = 60000000; 
	_datasets[202].num_edges = 268435456; 
	_datasets[202].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[202].graphorder = DST_SRC;
	_datasets[202].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[203].graphtopname = "RMAT24_380M";
	_datasets[203].graphname = "RMAT24_380M";
	_datasets[203].graph_path = rootDir + "dataset/RMAT24_380M/RMAT24_380M.mtx";
	_datasets[203].vertices_path = rootDir + "dataset/RMAT24_380M/RMAT24_380M.vertices";
	_datasets[203].edges_path = rootDir + "dataset/RMAT24_380M/RMAT24_380M.edges";	
	_datasets[203].vertices_path_bin = rootDir + "dataset/RMAT24_380M/RMAT24_380M_bin.vertices"; 
	_datasets[203].edges_path_bin = rootDir + "dataset/RMAT24_380M/RMAT24_380M_bin.edges";
	_datasets[203].min_vertex = 0;	
	_datasets[203].max_vertex = 16777216; 
	_datasets[203].num_vertices = 16777216; 
	_datasets[203].num_edges = 380000000; 
	_datasets[203].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[203].graphorder = DST_SRC;
	_datasets[203].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[204].graphtopname = "RMAT25_380M";
	_datasets[204].graphname = "RMAT25_380M";
	_datasets[204].graph_path = rootDir + "dataset/RMAT25_380M/RMAT25_380M.mtx";
	_datasets[204].vertices_path = rootDir + "dataset/RMAT25_380M/RMAT25_380M.vertices";
	_datasets[204].edges_path = rootDir + "dataset/RMAT25_380M/RMAT25_380M.edges";	
	_datasets[204].vertices_path_bin = rootDir + "dataset/RMAT25_380M/RMAT25_380M_bin.vertices"; 
	_datasets[204].edges_path_bin = rootDir + "dataset/RMAT25_380M/RMAT25_380M_bin.edges";
	_datasets[204].min_vertex = 0;	
	_datasets[204].max_vertex = 33554432; 
	_datasets[204].num_vertices = 33554432; 
	_datasets[204].num_edges = 380000000; 
	_datasets[204].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[204].graphorder = DST_SRC;
	_datasets[204].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[205].graphtopname = "RMAT26_380M";
	_datasets[205].graphname = "RMAT26_380M";
	_datasets[205].graph_path = rootDir + "dataset/RMAT26_380M/RMAT26_380M.mtx";
	_datasets[205].vertices_path = rootDir + "dataset/RMAT26_380M/RMAT26_380M.vertices";
	_datasets[205].edges_path = rootDir + "dataset/RMAT26_380M/RMAT26_380M.edges";	
	_datasets[205].vertices_path_bin = rootDir + "dataset/RMAT26_380M/RMAT26_380M_bin.vertices"; 
	_datasets[205].edges_path_bin = rootDir + "dataset/RMAT26_380M/RMAT26_380M_bin.edges";
	_datasets[205].min_vertex = 0;	
	_datasets[205].max_vertex = 60000000; 
	_datasets[205].num_vertices = 60000000; 
	_datasets[205].num_edges = 380000000; 
	_datasets[205].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[205].graphorder = DST_SRC;
	_datasets[205].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[220].graphtopname = "twitter";
	_datasets[220].graphname = "twitter";
	_datasets[220].graph_path = rootDir + "dataset/twitter/twitter.mtx";
	_datasets[220].vertices_path = rootDir + "dataset/twitter/twitter.vertices";
	_datasets[220].edges_path = rootDir + "dataset/twitter/twitter.edges";	
	_datasets[220].vertices_path_bin = rootDir + "dataset/twitter/twitter_bin.vertices"; 
	_datasets[220].edges_path_bin = rootDir + "dataset/twitter/twitter_bin.edges";
	_datasets[220].min_vertex = 0;	
	_datasets[220].max_vertex = 41652230; 
	_datasets[220].num_vertices = 41652230; 
	_datasets[220].num_edges = 1468365184; 
	_datasets[220].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[220].graphorder = DST_SRC;
	_datasets[220].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[221].graphtopname = "MOLIERE_2016";
	_datasets[221].graphname = "MOLIERE_2016";
	_datasets[221].graph_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.mtx";
	_datasets[221].vertices_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.vertices";
	_datasets[221].edges_path = rootDir + "dataset/MOLIERE_2016/MOLIERE_2016.edges";	
	_datasets[221].vertices_path_bin = rootDir + "dataset/MOLIERE_2016/MOLIERE2016_bin.vertices"; 
	_datasets[221].edges_path_bin = rootDir + "dataset/MOLIERE_2016/MOLIERE2016_bin.edges";
	_datasets[221].min_vertex = 0;	
	_datasets[221].max_vertex = 30239687; 
	_datasets[221].num_vertices = 30239687; 
	_datasets[221].num_edges = 3338650683; 
	_datasets[221].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[221].graphorder = DST_SRC_EDGEW;
	_datasets[221].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[222].graphtopname = "kronecker26";
	_datasets[222].graphname = "kronecker26";
	_datasets[222].graph_path = rootDir + "dataset/kronecker26/kronecker26.mtx";
	_datasets[222].vertices_path = rootDir + "dataset/kronecker26/kronecker26.vertices";
	_datasets[222].edges_path = rootDir + "dataset/kronecker26/kronecker26.edges";	
	_datasets[222].vertices_path_bin = rootDir + "dataset/kronecker26/kronecker26_bin.vertices";
	_datasets[222].edges_path_bin = rootDir + "dataset/kronecker26/kronecker26_bin.edges";
	_datasets[222].min_vertex = 0;
	_datasets[222].max_vertex = 67108864; 
	_datasets[222].num_vertices = 67108864;  // 67108864, 67108816(prev)*
	_datasets[222].num_edges = 1073741824; // 1073741824
	_datasets[222].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[222].graphorder = SRC_DST;
	_datasets[222].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[223].graphtopname = "kronecker28";
	_datasets[223].graphname = "kronecker28";
	_datasets[223].graph_path = rootDir + "dataset/kronecker28/kronecker28.mtx";
	_datasets[223].vertices_path = rootDir + "dataset/kronecker28/kronecker28.vertices";
	_datasets[223].edges_path = rootDir + "dataset/kronecker28/kronecker28.edges";	
	_datasets[223].vertices_path_bin = rootDir + "dataset/kronecker28/kronecker28_bin.vertices";
	_datasets[223].edges_path_bin = rootDir + "dataset/kronecker28/kronecker28_bin.edges";
	_datasets[223].min_vertex = 0;
	_datasets[223].max_vertex = 268435456; // 268435456 // 268431360(prev)
	_datasets[223].num_vertices = 268435456; 
	_datasets[223].num_edges = 4294967290;
	_datasets[223].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[223].graphorder = SRC_DST;
	_datasets[223].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[224].graphtopname = "kronecker30";
	_datasets[224].graphname = "kronecker30";
	_datasets[224].graph_path = rootDir + "dataset/kronecker30/kronecker30.mtx";
	_datasets[224].vertices_path = rootDir + "dataset/kronecker30/kronecker30.vertices";
	_datasets[224].edges_path = rootDir + "dataset/kronecker30/kronecker30.edges";	
	_datasets[224].vertices_path_bin = rootDir + "dataset/kronecker30/kronecker30_bin.vertices"; 
	_datasets[224].edges_path_bin = rootDir + "dataset/kronecker30/kronecker30_bin.edges"; 
	_datasets[224].min_vertex = 0;
	_datasets[224].max_vertex = 1073741824; // 1073741824 // 1041451012(prev)
	_datasets[224].num_vertices = 1073741824;
	_datasets[224].num_edges = 17179869184;
	_datasets[224].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[224].graphorder = SRC_DST;
	_datasets[224].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[225].graphtopname = "kronecker32";
	_datasets[225].graphname = "kronecker32";
	_datasets[225].graph_path = rootDir + "dataset/kronecker32/kronecker32.mtx";
	_datasets[225].vertices_path = rootDir + "dataset/kronecker32/kronecker32.vertices";
	_datasets[225].edges_path = rootDir + "dataset/kronecker32/kronecker32.edges";	
	_datasets[225].vertices_path_bin = rootDir + "dataset/kronecker32/kronecker32_bin.vertices"; 
	_datasets[225].edges_path_bin = rootDir + "dataset/kronecker32/kronecker32_bin.edges"; 
	_datasets[225].min_vertex = 0;
	_datasets[225].max_vertex = 4294967296; 
	_datasets[225].num_vertices = 4294967296;
	_datasets[225].num_edges = 68719476736;
	_datasets[225].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[225].graphorder = SRC_DST;
	_datasets[225].graphgroup = SNAP; // SKEWRATIO;
	
	_datasets[226].graphtopname = "wdc";
	_datasets[226].graphname = "wdc";
	_datasets[226].graph_path = rootDir + "dataset/wdc/wdc.mtx";
	_datasets[226].vertices_path = rootDir + "dataset/wdc/wdc.vertices";
	_datasets[226].edges_path = rootDir + "dataset/wdc/wdc.edges";	
	_datasets[226].vertices_path_bin = "/projects/DPE/wole/wdc/wdc_bin.vertices"; 
	_datasets[226].edges_path_bin = "/projects/DPE/wole/wdc/wdc_bin.edges";
	_datasets[226].min_vertex = 0;
	_datasets[226].max_vertex = 3315452338;
	_datasets[226].num_vertices = 3315452338;
	_datasets[226].num_edges = 0;
	_datasets[226].graphdirectiontype = UNDIRECTEDGRAPH;
	_datasets[226].graphorder = SRC_DST;
	_datasets[226].graphgroup = SNAP; // SKEWRATIO;

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








