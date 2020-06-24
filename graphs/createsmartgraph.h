#ifndef CREATE_SMARTGRAPH_H
#define CREATE_SMARTGRAPH_H
#include <mutex>
#include "graph.h"
#include "../src/common.h"

class createsmartgraph {
public:
	createsmartgraph(graph * _graphobj);
	~createsmartgraph();
	
	void create();
	void place(unsigned int vid, unsigned int hvid);
	void getdescendants(unsigned vid, vector<smartedge_t> & _1stdescendants, vector<smartedge_t> & _2nddescendants, vector<smartedge_t> & _3rddescendants);
	void placedescendants(unsigned vid, vector<smartedge_t> & _1stdescendants, vector<smartedge_t> & _2nddescendants, vector<smartedge_t> & _3rddescendants);
	vertex_t gethvid(unsigned int vid);
	void sethvid(unsigned int vid);
	void printhvids(unsigned int offset, unsigned int size);
	void printplacedvids(unsigned int offset, unsigned int size);
	void printdescendants(vector<smartedge_t> & descendants, unsigned int offset, unsigned int size);
	
private:
	graph * graphobj;	
	int * hvids;
	block_t * blocks;
	unsigned int * blockids;
};
#endif








