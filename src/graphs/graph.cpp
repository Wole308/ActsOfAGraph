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
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../../src/heuristics/heuristics.h"
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
	
graph::graph(algorithm * _algorithmobj, unsigned int datasetid, unsigned int _numvertexbanks, unsigned int _numedgebanks){
	cout<<"graph::graph:: datasetid: "<<datasetid<<", _numvertexbanks: "<<_numvertexbanks<<", _numedgebanks: "<<_numedgebanks<<endl;
	algorithmobj = _algorithmobj;
	utilityobj = new utility();
	
	loadalldatasets();
	setdataset(datasetid);
	
	numvertexbanks = _numvertexbanks;
	numedgebanks = _numedgebanks;
	numverticespervertexbank = KVDATA_RANGE / numvertexbanks;
	
	nvmeFd_verticesdata_r2 = new int[MAXNUMEDGEBANKS];
	nvmeFd_verticesdata_w2 = new int[MAXNUMEDGEBANKS];
	
	nvmeFd_vertexoutdegrees_r2 = new int[MAXNUMEDGEBANKS];
	nvmeFd_vertexproperties_r2 = new int[MAXNUMEDGEBANKS]; 
	nvmeFd_vertexproperties_w2 = new int[MAXNUMEDGEBANKS];	
	
	nvmeFd_edgeproperties_r2 = new int*[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeproperties_r2[i] = new int[MAXNUMEDGEBANKS]; }
	nvmeFd_edgeproperties_w2 = new int*[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeproperties_w2[i] = new int[MAXNUMEDGEBANKS]; }	
	
	nvmeFd_edgeproperties_r = new FILE**[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeproperties_r[i] = new FILE*[MAXNUMEDGEBANKS]; }
	nvmeFd_edgeproperties_w = new FILE**[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeproperties_w[i] = new FILE*[MAXNUMEDGEBANKS]; }	
	
	nvmeFd_edgeoffsets_r2 = new int*[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeoffsets_r2[i] = new int[MAXNUMEDGEBANKS]; } 
	nvmeFd_edgeoffsets_w2 = new int*[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeoffsets_w2[i] = new int[MAXNUMEDGEBANKS]; }	
	
	nvmeFd_edgeoffsets_r = new FILE**[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeoffsets_r[i] = new FILE*[MAXNUMEDGEBANKS]; }
	nvmeFd_edgeoffsets_w = new FILE**[MAXNUMEDGEBANKS]; for(int i=0; i<MAXNUMEDGEBANKS; i++){ nvmeFd_edgeoffsets_w[i] = new FILE*[MAXNUMEDGEBANKS]; }
	
	// for(unsigned int i=0; i<getnumvertexbanks(); i++){ vertexpropertybuffer[i] = new vertexprop_t[KVDATA_RANGE / getnumvertexbanks()]; }
	// for(unsigned int i=0; i<getnumvertexbanks(); i++){ vertexdatabuffer[i] = new value_t[KVDATA_RANGE / getnumvertexbanks()]; }
	for(unsigned int i=0; i<getnumvertexbanks(); i++){ vertexpropertybuffer[i] = new vertexprop_t[KVDATA_RANGE]; }
	for(unsigned int i=0; i<getnumvertexbanks(); i++){ vertexdatabuffer[i] = new value_t[KVDATA_RANGE]; }
	for(unsigned int i=0; i<getnumvertexbanks(); i++){ for(unsigned int j = 0; j < MAXNUMSSDPARTITIONS; j++){ totalkeyvaluesread[i][j] = new unsigned long[1]; }}
	for(unsigned int i=0; i<getnumvertexbanks(); i++){ for(unsigned int j = 0; j < MAXNUMSSDPARTITIONS; j++){ totalkeyvaluesread[i][j][0] = 0; }}
	unsigned int isactivevertexinfo = (KVDATA_RANGE / getnumvertexbanks()) / NUMBITSINUNSIGNEDINT;
	for(unsigned int i=0; i<getnumvertexbanks(); i++){ 
		vertexisactivebitbuffer[i] = new unsigned int[isactivevertexinfo]; 
		for(unsigned int k=0; k<isactivevertexinfo; k++){ vertexisactivebitbuffer[i][k] = 0; }}
}
graph::graph(unsigned int datasetid){
	cout<<"graph::graph:: datasetid: "<<datasetid<<endl;
	loadalldatasets();
	setdataset(datasetid);
	
	heuristics * heuristicsobj = new heuristics();
	numvertexbanks = heuristicsobj->getdefaultnumvertexbanks();
	numedgebanks = heuristicsobj->getdefaultnumedgebanks();
	numverticespervertexbank = KVDATA_RANGE / numvertexbanks;
	
	for(unsigned int i=0; i<getnumvertexbanks(); i++){ for(unsigned int j = 0; j < MAXNUMSSDPARTITIONS; j++){ totalkeyvaluesread[i][j] = new unsigned long[1]; }}
	for(unsigned int i=0; i<getnumvertexbanks(); i++){ for(unsigned int j = 0; j < MAXNUMSSDPARTITIONS; j++){ totalkeyvaluesread[i][j][0] = 0; }}
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

void graph::configureactivevertexreaders(){
	size_t filelenght = lseek(getnvmeFd_activevertexids_r2(), 0, SEEK_END) / sizeof(keyvalue_t);
	for(unsigned int i=0; i<NUMCPUTHREADS; i++) {
		size_t beginoffset =  (size_t)i * (size_t)(filelenght / NUMCPUTHREADS); 
		size_t endoffset = (size_t)beginoffset + (size_t)(filelenght / NUMCPUTHREADS);
		
		cout<<"&&&&& graph::configureactivevertexreaders:: beginoffset: "<<beginoffset<<", endoffset: "<<endoffset<<", filelenght: "<<filelenght<<endl;
		
		if(i==0){ reader_activevertexids_r2[i] = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(getnvmeFd_activevertexids_r2(), (beginoffset * sizeof(keyvalue_t)), ((endoffset + (filelenght % NUMCPUTHREADS)) * sizeof(keyvalue_t))); }				
		else { reader_activevertexids_r2[i] = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(getnvmeFd_activevertexids_r2(), (beginoffset * sizeof(keyvalue_t)), (endoffset * sizeof(keyvalue_t)));  }
	}
	return;
}

void graph::openfilesforreading(){
	cout<<"graph::openfilesforreading : opening files for reading"<<endl;
	// vertex offsets (pread)
	for(unsigned int i=0; i<numvertexbanks; i++){
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
	
	// vertex offsets (fread)
	for(unsigned int i=0; i<numvertexbanks; i++){
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
	
	// edge properties (pread)
	for(unsigned int i=0; i<numvertexbanks; i++){
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
	
	// edge properties (fread)
	for(unsigned int i=0; i<numvertexbanks; i++){
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
	return;
}
void graph::openfilesforwriting(){
	cout<<"graph::openfilesforreading : opening files for writing"<<endl;
	// vertex offsets (pwrite)
	for(unsigned int i=0; i<numvertexbanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgeoffsets(i, j); 
			std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();				
			nvmeFd_edgeoffsets_w2[i][j] = open(path.c_str(), O_WRONLY);
			if (nvmeFd_edgeoffsets_w2[i][j] < 0) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
	
	// vertex offsets (fwrite)
	for(unsigned int i=0; i<numvertexbanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgeoffsets(i,j); 
			std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
			nvmeFd_edgeoffsets_w[i][j] = fopen(path.c_str(), "w"); 
			if (nvmeFd_edgeoffsets_w[i][j] == NULL) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
	
	// edge properties (pwrite)
	for(unsigned int i=0; i<numvertexbanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgesproperties(i, j);
			std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
			nvmeFd_edgeproperties_w2[i][j] = open(path.c_str(), O_WRONLY); 
			if (nvmeFd_edgeproperties_w2[i][j] < 0) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
	
	// edge properties (fwrite)
	for(unsigned int i=0; i<numvertexbanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){
			string path = getpath_edgesproperties(i,j);
			std::ofstream ofs; ofs.open(path.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	
			nvmeFd_edgeproperties_w[i][j] = fopen(path.c_str(), "w"); 
			if (nvmeFd_edgeproperties_w[i][j] == NULL) {
				cout << "ERR: open " << path << " failed: "
					<< strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			cout << "INFO: Successfully opened " << path << endl;
		}
	}
	return;
}

void graph::closefilesforreading(){
	cout<<"graph::closefilesforreading : close files for reading..."<<endl;
	for(unsigned int i=0; i<numvertexbanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_edgeoffsets_r2[i][j]); }
		for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_edgeproperties_r2[i][j]); }
	}
	return;
}
void graph::closefilesforwriting(){
	cout<<"graph::closefilesforreading : close files for writing..."<<endl;
	for(unsigned int i=0; i<numvertexbanks; i++){
		for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_edgeoffsets_w2[i][j]); }
		for(unsigned int j=0; j<numedgebanks; j++){ close(nvmeFd_edgeproperties_w2[i][j]); }
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
	// exit(EXIT_SUCCESS);
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

string graph::getpath_vertexdata(unsigned int i){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + ".vdata";		
}
string graph::getpath_tempvertexdata(unsigned int i){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + ".tempvdata";		
}
string graph::getpath_vertexproperties(unsigned int i){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + ".vproperties";		
}
string graph::getpath_edgesproperties(unsigned int i, unsigned int j){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + "_" + std::to_string(j) + ".edges";		
}
string graph::getpath_edgeoffsets(unsigned int i, unsigned int j){
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + "_" + std::to_string(j) + ".edgeoffsets";		
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
	return datasetRootDir + "datasets" + "/" + thisdataset.graphname + "/" + thisdataset.graphname + "_" + std::to_string(numvertexbanks) + "by" +  std::to_string(numedgebanks) + "/" + thisdataset.graphname + "_" + std::to_string(i) + ".vupdates";		
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

int * graph::getnvmeFd_verticesdata_r2(){ return nvmeFd_verticesdata_r2; } 
int * graph::getnvmeFd_verticesdata_w2(){ return nvmeFd_verticesdata_w2; }
int * graph::getnvmeFd_tempverticesdata_r2(){ return nvmeFd_tempverticesdata_r2; } 
int * graph::getnvmeFd_tempverticesdata_w2(){ return nvmeFd_tempverticesdata_w2; }
int * graph::getnvmeFd_vertexoutdegrees_r2(){ return nvmeFd_vertexoutdegrees_r2; }
int * graph::getnvmeFd_vertexproperties_r2(){ return nvmeFd_vertexproperties_r2; } 
int * graph::getnvmeFd_vertexproperties_w2(){ return nvmeFd_vertexproperties_w2; }
int ** graph::getnvmeFd_edgeproperties_r2(){ return nvmeFd_edgeproperties_r2; } 
int ** graph::getnvmeFd_edgeproperties_w2(){ return nvmeFd_edgeproperties_w2; }
FILE *** graph::getnvmeFd_edgeproperties_r(){ return nvmeFd_edgeproperties_r; }
FILE *** graph::getnvmeFd_edgeproperties_w(){ return nvmeFd_edgeproperties_w; }
int ** graph::getnvmeFd_edgeoffsets_r2(){ return nvmeFd_edgeoffsets_r2; }
int ** graph::getnvmeFd_edgeoffsets_w2(){ return nvmeFd_edgeoffsets_w2; }
FILE *** graph::getnvmeFd_edgeoffsets_r(){ return nvmeFd_edgeoffsets_r; }
FILE *** graph::getnvmeFd_edgeoffsets_w(){ return nvmeFd_edgeoffsets_w; }
int graph::getnvmeFd_activevertexids_r2(){ return nvmeFd_activevertexids_r2; } 
int graph::getnvmeFd_activevertexids_w2(){ return nvmeFd_activevertexids_w2; }
FILE * graph::getnvmeFd_activevertexids_w(){ return nvmeFd_activevertexids_w; }
int graph::getnvmeFd_vertexisactive_r2(){ return nvmeFd_vertexisactive_r2; } 
int graph::getnvmeFd_vertexisactive_w2(){ return nvmeFd_vertexisactive_w2; }
SortReduceUtils::FileKvReader<uint32_t,uint32_t>* graph::getreader_activevertexids(unsigned int i){ return reader_activevertexids_r2[i]; }

vertexprop_t * graph::getvertexpropertybuffer(unsigned int id){
	return vertexpropertybuffer[id];
}
value_t * graph::getvertexdatabuffer(unsigned int id){
	return vertexdatabuffer[id];
}
unsigned int * graph::getvertexisactivebuffer(unsigned int id){
	return vertexisactivebitbuffer[id];
}

void graph::loadvertexpropertiesfromfile(){
	#ifdef LOCKE
	for(int i = 0; i < numvertexbanks; i++){ workerthread_loadvertexpropertiesfromfile(i, nvmeFd_vertexproperties_r2[i], 0, vertexpropertybuffer[i], 0, KVDATA_RANGE); }
	#else
	for(int i = 0; i < numvertexbanks; i++){ mythread[i] = std::thread(&graph::workerthread_loadvertexpropertiesfromfile, this, i, nvmeFd_vertexproperties_r2[i], 0, vertexpropertybuffer[i], 0, KVDATA_RANGE); }
	for(int i = 0; i < numvertexbanks; i++){ mythread[i].join(); }
	#endif
}
void graph::workerthread_loadvertexpropertiesfromfile(int ithreadidx, int fd, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t size){
	if(size > 0){ if(pread(fd, &buffer[bufferoffset], (size_t)(size * sizeof(vertexprop_t)), (size_t)(offset * sizeof(vertexprop_t))) < 0){ cout<<"graph::loadvertexpropertiesfromfile::ERROR 35"<<endl; exit(EXIT_FAILURE); }}
	return;
}
void graph::loadvertexdatafromfile(){
	#ifndef EXTERNALGRAPHPROCESSING
	return;
	#endif
	#ifdef LOCKE
	for(int i = 0; i < numvertexbanks; i++){ workerthread_loadvertexdatafromfile(i, nvmeFd_verticesdata_r2[i], 0, vertexdatabuffer[i], 0, KVDATA_RANGE); }
	#else
	for(int i = 0; i < numvertexbanks; i++){ mythread[i] = std::thread(&graph::workerthread_loadvertexdatafromfile, this, i, nvmeFd_verticesdata_r2[i], 0, vertexdatabuffer[i], 0, KVDATA_RANGE); }
	for(int i = 0; i < numvertexbanks; i++){ mythread[i].join(); }
	#endif
}
void graph::workerthread_loadvertexdatafromfile(int ithreadidx, int fd, unsigned int offset, value_t * buffer, vertex_t bufferoffset, vertex_t size){
	if(size > 0){ if(pread(fd, &buffer[bufferoffset], (size_t)(size * sizeof(value_t)), (size_t)(offset * sizeof(value_t))) < 0){ cout<<"graph::loadvertexpropertiesfromfile::ERROR 35"<<endl; exit(EXIT_FAILURE); }}
	return;
}
void graph::loadvertexdatafromfile(int bank, vertex_t fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size){
	#ifdef EXTERNALGRAPHPROCESSING
	value_t * tempbufferPtr = new value_t[size];
	if(size > 0){ if(pread(nvmeFd_verticesdata_r2[bank], tempbufferPtr, (size_t)(size * sizeof(value_t)), (size_t)(fdoffset * sizeof(value_t))) <= 0){ utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }}
	for(unsigned int i=0; i<size; i++){ buffer[bufferoffset + i].value = tempbufferPtr[i]; }
	delete [] tempbufferPtr;
	#else 
	for(unsigned int i=0; i<size; i++){ buffer[bufferoffset + i].value = vertexdatabuffer[bank][i]; }
	#endif
	return;
}
void graph::loadvertexdatafromfile(int bank, vertex_t fdoffset, value_t * buffer, vertex_t bufferoffset, vertex_t size){
	#ifdef EXTERNALGRAPHPROCESSING
	if(size > 0){ if(pread(nvmeFd_verticesdata_r2[bank], &buffer[bufferoffset], (size_t)(size * sizeof(value_t)), (size_t)(fdoffset * sizeof(value_t))) <= 0){ utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }}
	#else
	for(unsigned int i=0; i<size; i++){ buffer[bufferoffset + i] = vertexdatabuffer[bank][i]; }
	#endif 
	return;
}
void graph::savevertexdatatofile(int bank, vertex_t fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size){
	#ifndef EXTERNALGRAPHPROCESSING
	return;
	#endif
	value_t * tempbufferPtr = new value_t[size];
	for(unsigned int i=0; i<size; i++){ tempbufferPtr[i] = buffer[bufferoffset + i].value; }
	if(size > 0){ if(pwrite(nvmeFd_verticesdata_w2[bank], tempbufferPtr, (size_t)(size * sizeof(value_t)), (size_t)(fdoffset * sizeof(value_t))) <= 0){ utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }}			
	delete [] tempbufferPtr;
	return;
}
void graph::loadedgepropertyfromfile(int bank, int col, size_t fdoffset, edgeprop1_t * buffer, vertex_t bufferoffset, vertex_t size){
	if(size > 0){ if(pread(nvmeFd_edgeproperties_r2[bank][col], &buffer[bufferoffset], (size * sizeof(edgeprop1_t)), fdoffset * sizeof(edgeprop1_t)) <= 0){ utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }}
	return;
}
void graph::loadvertexpointersfromfile(int bank, int col, size_t fdoffset, prvertexoffset_t * buffer, vertex_t bufferoffset, vertex_t size){
	if(size > 0){ if(pread(nvmeFd_edgeoffsets_r2[bank][col], &buffer[bufferoffset], ((size * sizeof(unsigned int)) / NUMBITSINUNSIGNEDINT), ((fdoffset * sizeof(unsigned int)) / NUMBITSINUNSIGNEDINT)) <= 0){ utilityobj->print4("fdoffset", "bufferoffset", "size", "NAp", fdoffset, bufferoffset, size, NAp); exit(EXIT_FAILURE); }}
	return;
}

void graph::generateverticesdata(){
	cout<<"generating vertices data... "<<endl;
	for(unsigned int i=0; i<numvertexbanks; i++){
		for(unsigned int k=0; k<KVDATA_RANGE; k++){ vertexdatabuffer[i][k] = algorithmobj->vertex_initdata(); } 
		#ifdef EXTERNALGRAPHPROCESSING
		if(pwrite(nvmeFd_verticesdata_w2[i], vertexdatabuffer[i], (size_t)(KVDATA_RANGE * sizeof(value_t)), 0) < 0){ cout<<"hostprocess::generateverticesdata::ERROR 36. numvertexbanks: "<<numvertexbanks<<", numverticespervertexbank: "<<numverticespervertexbank<<endl; exit(EXIT_FAILURE); }			
		#endif 
	}
	cout<<"finished generating vertices data"<<endl;
	return;
}
void graph::generatetempverticesdata(){
	cout<<"generating vertices data... "<<endl;
	for(unsigned int i=0; i<numvertexbanks; i++){
		value_t * tempverticesdata = new value_t[KVDATA_RANGE]();
		for(unsigned int k=0; k<KVDATA_RANGE; k++){ tempverticesdata[k] = algorithmobj->vertex_inittempdata(); } 
		if(pwrite(nvmeFd_tempverticesdata_w2[i], tempverticesdata, (size_t)(KVDATA_RANGE * sizeof(value_t)), 0) < 0){ cout<<"hostprocess::generatetempverticesdata::ERROR 36. numvertexbanks: "<<numvertexbanks<<", numverticespervertexbank: "<<numverticespervertexbank<<endl; exit(EXIT_FAILURE); }			
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
	return numactivevertices;
}
void graph::saveactiveverticestofile(vector<keyvalue_t> & activeverticesbuffer, unsigned int graph_iterationidx){
	fwrite (&activeverticesbuffer[0], sizeof(keyvalue_t), activeverticesbuffer.size(), nvmeFd_activevertexids_w);
	return;
}
void graph::savevertexisactiveinfostofile(unsigned int fdoffset_unit, vertex_t bufferoffset, vertex_t datasize_uint, unsigned int graph_iterationidx){
	if(pwrite(nvmeFd_vertexisactive_w2, &vertexisactivebitbuffer[bufferoffset], ((datasize_uint / NUMBITSINUNSIGNEDINT) * sizeof(unsigned int)), ((fdoffset_unit / NUMBITSINUNSIGNEDINT) * sizeof(unsigned int))) <= 0){ cout<<"graph::savevertexisactiveinfostofile::ERROR (graph iteration "<<graph_iterationidx<<") 35"<<endl; exit(EXIT_FAILURE); }		
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
	return;
}
dataset_t graph::getdataset(){
	return thisdataset;
}

unsigned long graph::gettotalkeyvaluesread(unsigned int bank, unsigned int col){
	return totalkeyvaluesread[bank][col][0];
}
void graph::appendkeyvaluesread(unsigned int bank, unsigned int col, unsigned int value){
	totalkeyvaluesread[bank][col][0] += value;
}








