#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../include/common.h"
#include "dataaccess.h"
using namespace std;

dataaccess::dataaccess(){
	utilityobj = new utility();
}
dataaccess::~dataaccess(){} 

/* void dataaccess::loaddatafromfile(int fd, size_t fileoffset_bytes, edgeprop1_t * buffer, vertex_t bufferoffset, vertex_t size_bytes){
	if(size_bytes > 0){ if(pread(fd, &buffer[bufferoffset], size_bytes, fileoffset_bytes) <= 0){ utilityobj->print4("fileoffset_bytes", "bufferoffset", "size_bytes", "NAp", fileoffset_bytes, bufferoffset, size_bytes, NAp); exit(EXIT_FAILURE); }}
	return;
}
void dataaccess::loaddatafromfile(int fd, size_t fileoffset_bytes, prvertexoffset_t * buffer, vertex_t bufferoffset, vertex_t size_bytes){
	if(size_bytes > 0){ if(pread(fd, &buffer[bufferoffset], size_bytes, fileoffset_bytes) <= 0){ utilityobj->print4("fileoffset_bytes", "bufferoffset", "size_bytes", "NAp", fileoffset_bytes, bufferoffset, size_bytes, NAp); exit(EXIT_FAILURE); }}
	return;
}
void dataaccess::loaddatafromfilE(int fd, edge_t offset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size){
	value_t * tempbufferPtr = new value_t[size];
	loaddatafromfile(fd, (size_t)(offset * sizeof(value_t)), tempbufferPtr, bufferoffset, (size_t)(size * sizeof(value_t)));
	for(unsigned int i=0; i<size; i++){ buffer[bufferoffset + i].value = tempbufferPtr[i]; }
	delete [] tempbufferPtr;
	return;
}

void dataaccess::savedatatofile(int fd, size_t fileoffset_bytes, value_t * buffer, vertex_t bufferoffset, vertex_t size_bytes){
	if(size_bytes > 0){ if(pwrite(fd, &buffer[bufferoffset], size_bytes, fileoffset_bytes) <= 0){ utilityobj->print4("fileoffset_bytes", "bufferoffset", "size_bytes", "NAp", fileoffset_bytes, bufferoffset, size_bytes, NAp); exit(EXIT_FAILURE); }}			
	return;
}
void dataaccess::savedatatofilE(int fd, edge_t offset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size){
	value_t * tempbufferPtr = new value_t[size];
	for(unsigned int i=0; i<size; i++){ tempbufferPtr[i] = buffer[bufferoffset + i].value; }
	savedatatofile(fd, (size_t)(offset * sizeof(value_t)), tempbufferPtr, bufferoffset, (size_t)(size * sizeof(value_t)));
	delete [] tempbufferPtr;
	return;
} */












