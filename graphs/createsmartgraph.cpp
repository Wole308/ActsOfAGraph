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
#include <stack>
#include "../heuristics/heuristics.h"
#include "createsmartgraph.h"
#include "graph.h"
using namespace std;

#define TAKENTHRESHOLD 8
#define OUTERCOURTRANGE (1 << 12)
#define INNERCOURTRANGE (1 << 10) 
#define INNERMOSTCOURTRANGE (1 << 8)
#define NUMBLOCKS (1 << 12)
#define BLOCKSZ (KVDATA_RANGE / NUMBLOCKS)

unsigned int prevblockid;
unsigned int numblockchanges;

createsmartgraph::createsmartgraph(graph * _graphobj){
	cout<<"createsmartgraph::createsmartgraph:: started"<<endl;
	graphobj = _graphobj;
	hvids = new int[KVDATA_RANGE];
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ hvids[i] = -1; }
	blocks = new block_t[NUMBLOCKS];	
	for(unsigned int i=0; i<NUMBLOCKS; i++){
		block_t block;
		block.id = i;
		block.offset = i * BLOCKSZ;
		block.size = BLOCKSZ;
		blocks[i] = block;
	}
	blockids = new unsigned int[KVDATA_RANGE];
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ blockids[i] = i / BLOCKSZ; } // FIXME.
	prevblockid = 0;
	numblockchanges = 0;
	cout<<"createsmartgraph::createsmartgraph:: finished"<<endl;
}
createsmartgraph::~createsmartgraph(){
	hvids = NULL;
	blocks = NULL;
	blockids = NULL;
}

void createsmartgraph::create(){
	place(1, 213);	
	printplacedvids(0, 16);	
	cout<<"createsmartgraph::create:: number of block changes seen: "<<numblockchanges<<endl;
	return;
}

void createsmartgraph::place(unsigned int vid, unsigned int hvid){
	cout<<"createsmartgraph::place::"<<endl;
	vector<smartedge_t> _1stdescendants;
	vector<smartedge_t> _2nddescendants; 
	vector<smartedge_t> _3rddescendants;
	unsigned int iterationidx = 0;
	
	smartedge_t sedge;
	std::stack<smartedge_t> mystack;
	sedge.srcvid = vid; sedge.dstvid = NAp; sedge.ew = 0;
	mystack.push(sedge);
	
	while (!mystack.empty()){
		cout<<"createsmartgraph::place:: iteration: "<<iterationidx<<endl;		
		smartedge_t sedge = mystack.top();
		// exit(EXIT_SUCCESS);
		getdescendants(sedge.srcvid, _1stdescendants, _2nddescendants, _3rddescendants);
		placedescendants(sedge.srcvid, _1stdescendants, _2nddescendants, _3rddescendants);
		
		for (int i=0; i<_1stdescendants.size(); ++i) { mystack.push(_1stdescendants[i]); }
		for (int i=0; i<_2nddescendants.size(); ++i) { mystack.push(_2nddescendants[i]); }
		for (int i=0; i<_3rddescendants.size(); ++i) { mystack.push(_3rddescendants[i]); }

		#ifdef _DEBUGMODE_HOSTPRINTS
		printdescendants(_1stdescendants, 0, 8);
		printdescendants(_2nddescendants, 0, 8);
		printdescendants(_3rddescendants, 0, 8);
		#endif
		
		_1stdescendants.clear();
		_2nddescendants.clear();
		_3rddescendants.clear();
		
		iterationidx += 1;
		// exit(EXIT_SUCCESS);
		// break;
		if(iterationidx >= 200){ break; }
	}
	cout<<"createsmartgraph::place:: total number of iterations: "<<iterationidx<<endl;	
	return;
}
void createsmartgraph::getdescendants(unsigned vid, vector<smartedge_t> & _1stdescendants, vector<smartedge_t> & _2nddescendants, vector<smartedge_t> & _3rddescendants){
	vector<edgeprop2_t> _1stdescendantedges; 
	vector<edgeprop2_t> _2nddescendantedges; 
	vector<edgeprop2_t> _3rddescendantedges;
	
	// 1st descendants
	graphobj->addedges(vid, _1stdescendantedges);
	for(unsigned int i=0; i<_1stdescendantedges.size(); i++){ smartedge_t sedge; sedge.srcvid = vid; sedge.dstvid = _1stdescendantedges[i].dstvid; sedge.ew = 0; _1stdescendants.push_back(sedge); }
	
	// 2nd descendants
	for(vertex_t v=0; v<_1stdescendantedges.size(); v++){		
		graphobj->addedges(_1stdescendantedges[v].dstvid, _2nddescendantedges);
	}
	for(unsigned int i=0; i<_2nddescendantedges.size(); i++){ smartedge_t sedge; sedge.srcvid = vid; sedge.dstvid = _2nddescendantedges[i].dstvid; sedge.ew = 0; _2nddescendants.push_back(sedge); }
	
	// 3rd descendants
	for(vertex_t v=0; v<_2nddescendantedges.size(); v++){
		graphobj->addedges(_2nddescendantedges[v].dstvid, _3rddescendantedges);
	}
	for(unsigned int i=0; i<_3rddescendantedges.size(); i++){ smartedge_t sedge; sedge.srcvid = vid; sedge.dstvid = _3rddescendantedges[i].dstvid; sedge.ew = 0; _3rddescendants.push_back(sedge); }
	return;
}
void createsmartgraph::placedescendants(unsigned vid, vector<smartedge_t> & _1stdescendants, vector<smartedge_t> & _2nddescendants, vector<smartedge_t> & _3rddescendants){	
	// 1st descendants
	for(unsigned int i=0; i<_1stdescendants.size(); i++){
		sethvid(_1stdescendants[i].dstvid);
	}
	
	// 2nd descendants
	for(unsigned int i=0; i<_2nddescendants.size(); i++){
		sethvid(_2nddescendants[i].dstvid);
	}
	
	// 3rd descendants
	for(unsigned int i=0; i<_3rddescendants.size(); i++){
		sethvid(_3rddescendants[i].dstvid);
	}
	return;
}
vertex_t createsmartgraph::gethvid(unsigned int vid){
	return hvids[vid];
}
void createsmartgraph::sethvid(unsigned int vid){
	unsigned int blockid = blockids[vid];
	if(blockid != prevblockid){ 
		// cout<<endl<<"createsmartgraph::sethvid:: new block chosen. blockid: "<<blockid<<endl; 
		prevblockid = blockid; numblockchanges += 1; 
	}
	block_t block = blocks[blockid];
	unsigned int hvid = block.offset + (rand() % block.size);
	hvids[vid] = hvid;
	return;
}
void createsmartgraph::printhvids(unsigned int offset, unsigned int size){
	cout<<endl<<"createsmartgraph::printhvids::"<<endl;
	for(unsigned int vid=offset; vid<(offset + size); vid++){
		cout<<"createsmartgraph::printhvids:: hvids["<<vid<<"]: "<<hvids[vid]<<endl;
	}
	return;
}
void createsmartgraph::printplacedvids(unsigned int offset, unsigned int size){
	cout<<endl<<"createsmartgraph::printplacedvids::"<<endl;
	unsigned int count=0, totalcount=0;
	for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){
		if((hvids[vid] != -1) && (vid >= offset) && (count < size)){ cout<<"createsmartgraph::printplacedvids:: hvids["<<vid<<"]: "<<hvids[vid]<<endl; }
		if(hvids[vid] != -1){ totalcount += 1; }
		count += 1;
	}
	cout<<endl<<"createsmartgraph::printplacedvids:: finished. total number of vids placed: "<<totalcount<<endl;
	return;
}
void createsmartgraph::printdescendants(vector<smartedge_t> & descendants, unsigned int offset, unsigned int size){
	if(size > descendants.size()){ size = descendants.size(); }
	cout<<endl<<"createsmartgraph::printdescendants:: printing "<<size<<" of "<<descendants.size()<<" descendants"<<endl;
	for(unsigned int vid=offset; vid<(offset + size); vid++){
		cout<<"createsmartgraph::printdescendants:: descendants["<<vid<<"].srcvid: "<<descendants[vid].srcvid<<", descendants["<<vid<<"].dstvid: "<<descendants[vid].dstvid<<", descendants["<<vid<<"].ew: "<<descendants[vid].ew<<endl;
	}
	return;
}







