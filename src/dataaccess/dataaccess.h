#ifndef DATAACCESS_H
#define DATAACCESS_H
#include "../../src/utility/utility.h"
#include "../../include/common.h"

class dataaccess {
public:
	dataaccess();
	~dataaccess();
	
	// void loaddatafromfile(int fd, size_t fileoffset_bytes, edgeprop1_t * buffer, vertex_t bufferoffset, vertex_t size_bytes);
	// void loaddatafromfile(int fd, size_t fileoffset_bytes, prvertexoffset_t * buffer, vertex_t bufferoffset, vertex_t size_bytes);
	// void loaddatafromfilE(int fd, edge_t offset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size);
	
	// void savedatatofile(int fd, size_t fileoffset_bytes, value_t * buffer, vertex_t bufferoffset, vertex_t size_bytes);
	// void savedatatofilE(int fd, edge_t offset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t size);

private:
	utility * utilityobj;
};
#endif







