#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/postprocess.h"
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "mutategraph.h"
using namespace std;

/** mutated edge: [setof(8,16,32,64...) | numitems | data(kv,kv,kv,kv...)] */
/** longword.data = longword.data | 0xF000000000000000; // 0x[metadata:8]000000000000000; */

mutategraph::mutategraph(graph * _graphobj, stats * _statsobj){
	parametersobj = new parameters();
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
}
mutategraph::mutategraph(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
mutategraph::~mutategraph(){} 

* arbval_t mutategraph::shrink(unsigned int x){
	arbval_t result;
	if(x > 0 && x <= (1 << 8)){
		result.x = x & 0xFF; 
		result.numbits = 8;
	} else if(x > (1 << 8) && x <= (1 << (8+8))){
		result.x = x & 0xFFFF;
		result.numbits = 16;
	} else if(x > (1 << (8+8)) && x <= (1 << (8+8+8))){
		result.x = x & 0xFFFFFF;
		result.numbits = 24;
	} else {
		result.x = x;
		result.numbits = 32;
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"x [before shrink]: x: "<<x<<", x [after shrink]: result.x: "<<result.x<<", result.numbits: "<<result.numbits<<endl;
	#endif 
	return result;
}
#ifdef YHY
arbval_t mutategraph::shrink(unsigned int x){ // REMOVEME.
	/* arbval_t result;
	if(x >= 0 && x < 256){
		result.x = x & 0xFF; 
		result.numbits = 8;
	} else if(x >= 256 && x < 65536){
		result.x = x & 0xFFFF;
		result.numbits = 16;
	} else if(x >= 65536 && x < 16777216){
		result.x = x & 0xFFFFFF;
		result.numbits = 24;
	} else {
		result.x = x;
		result.numbits = 32;
	} */
	
	/* if(x >= 0 && x < 256){
		result.x = x & 0xFF; 
		result.numbits = 8;
	} else if(x >= 256 && x < 65536){
		result.x = x & 0xFFFF;
		result.numbits = 16;
	} else if(x >= 65536 && x < 16777216){
		result.x = x & 0xFFFFFF;
		result.numbits = 24;
	} else {
		result.x = x;
		result.numbits = 32;
	} */
	
	/* if(x > 0 && x <= (1 << 7)){
		result.x = x & 0xFF; 
		result.numbits = 8;
	} else if(x > (1 << 7) && x <= (1 << (7+7))){
		result.x = x & 0xFFFF;
		result.numbits = 16;
	} else if(x > (1 << (7+7)) && x <= (1 << (7+7+7))){
		result.x = x & 0xFFFFFF;
		result.numbits = 24;
	} else {
		result.x = x;
		result.numbits = 32;
	} */
	
	/* if(x >= 0 && x < (1 << 10)){
		result.x = x & 0xFF; 
		result.numbits = 10;
	} else if(x >= (1 << 10) && x < (1 << 20)){
		result.x = x & 0xFFFF;
		result.numbits = 16;
	} else if(x >= (1 << 16) && x < (1 << 22)){
		result.x = x & 0xFFFFFF;
		result.numbits = 22;
	} else {
		result.x = x;
		result.numbits = 32;
	} */
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"x [before shrink]: x: "<<x<<", x [after shrink]: result.x: "<<result.x<<", result.numbits: "<<result.numbits<<endl;
	#endif 
	return result;
}
#endif 
void mutategraph::push(uuint64_dt * longword, arbval_t kv){
	if(kv.numbits == 8){ 
		longword->data = (longword->data << 8) | kv.x; 
	} else if(kv.numbits == 16){ 
		longword->data = (longword->data << 16) | kv.x; 
	} else if(kv.numbits == 24){ 
		longword->data = (longword->data << 24) | kv.x; 
	} else { 
		longword->data = (longword->data << 32) | kv.x; 
	}
	return;
}

void mutategraph::mutate(edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * mutated_vertexptrbuffer, edge2_type * mutated_edgedatabuffer){
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	unsigned int totalnumedgesprocessed = 0;
	unsigned int totalnumcommits = 0;
	unsigned int commitreasontype1 = 0;
	unsigned int commitreasontype2 = 0;
	unsigned int commitreasontype3 = 0;
	unsigned int header_bitsize8count = 0;
	unsigned int header_bitsize16count = 0;
	unsigned int header_bitsize24count = 0;
	unsigned int header_bitsize32count = 0;
	unsigned int numitemscount[16]; for(unsigned int i=0; i<16; i++){ numitemscount[i] = 0; }
	
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
	// for(unsigned int vid=0; vid<12800; vid++){ // 3
		#ifdef _DEBUGMODE_HOSTPRINTS//3
		cout<<endl<<">>> mutategraph::mutate: vid: "<<vid<<endl;
		#endif 
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		value_t buffersize_kvs = 0;
		uuint64_dt commitvertexupdate;
		uuint64_dt longword; longword.data = 0;
		unsigned int currpartition = 0;
		unsigned int currbitoffset = 0;
		unsigned int numitems = 0;
		unsigned int header_bitsize = 0;
		bool commit = false;
		
		unsigned int llpartition = edgedatabuffer[vptr_begin].dstvid >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH));
		currpartition = llpartition;
		arbval_t newx = shrink(edgedatabuffer[vptr_begin].dstvid);
		header_bitsize = newx.numbits;
		
		for(unsigned int k=0; k<edges_size; k++){
			totalnumedgesprocessed += 1;
			edge2_type edge = edgedatabuffer[vptr_begin + k];
			
			unsigned int llpartition = edge.dstvid >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH));
			arbval_t newx = shrink(edge.dstvid);
			#ifdef _DEBUGMODE_HOSTPRINTS//3
			cout<<"+++ edge.dstvid: "<<edge.dstvid<<", llpartition: "<<llpartition<<", x[after shrink] "<<newx.x<<", newx.numbits: "<<newx.numbits<<", header_bitsize: "<<header_bitsize<<", numitems: "<<numitems<<endl;
			#endif 
			
			if(newx.numbits != header_bitsize || llpartition != currpartition || currbitoffset + newx.numbits > 64){
			// if(newx.numbits != header_bitsize || currbitoffset + newx.numbits > 64){
				commit = true;
				commitvertexupdate = longword;
			} else {
				push(&longword, newx);
				currpartition = llpartition;
				currbitoffset += newx.numbits;
				numitems += 1;
				header_bitsize = newx.numbits;
			}
			
			if(commit == true){
				#ifdef _DEBUGMODE_HOSTPRINTS//3
				cout<<"..........committing... RESON: ";
				if(newx.numbits != header_bitsize){ cout<<"new shrinked bitsize. "; commitreasontype1 += 1; }
				if(llpartition != currpartition){ cout<<"change in last level partition. "; commitreasontype2 += 1; }
				if(currbitoffset + newx.numbits > 64){ cout<<"long word full. "; commitreasontype3 += 1; }
				cout<<""<<endl;
				#endif 
				
				if(newx.numbits != header_bitsize){ commitreasontype1 += 1; }
				if(llpartition != currpartition){ commitreasontype2 += 1; }
				if(currbitoffset + newx.numbits > 64){ commitreasontype3 += 1; }
				
				if(header_bitsize == 8){
					header_bitsize8count += 1;
				} else if(header_bitsize == 16){
					header_bitsize16count += 1;
				} else if(header_bitsize == 24){
					header_bitsize24count += 1;
				} else if(header_bitsize == 32){
					header_bitsize32count += 1;
				} else {
					cout<<"mutategraph:: ERROR. should NEVER get here 2. exiting..."<<endl;
					exit(EXIT_FAILURE);
				}
				/* if(header_bitsize == 10){ // REMOVEME.
					header_bitsize8count += 1;
				} else if(header_bitsize == 16){
					header_bitsize16count += 1;
				} else if(header_bitsize == 22){
					header_bitsize24count += 1;
				} else if(header_bitsize == 32){
					header_bitsize32count += 1;
				} else {
					cout<<"mutategraph:: ERROR. should NEVER get here 2. exiting..."<<endl;
					exit(EXIT_FAILURE);
				} */
				numitemscount[numitems] += 1;
				
				totalnumcommits += 1;
				
				#ifdef KKK
				if(header_bitsize == 8){
					longword.data = longword.data | 0x1000000000000000;
				} else if(header_bitsize == 16){
					longword.data = longword.data | 0x2000000000000000;
				} else if(header_bitsize == 32){
					longword.data = longword.data | 0x3000000000000000;
				} else if(header_bitsize == 64){
					longword.data = longword.data | 0x4000000000000000;
				} else {
					cout<<"mutategraph:: ERROR. should NEVER get here 3. exiting..."<<endl;
					exit(EXIT_FAILURE);
				}
				
				if(numitems == 1){
					longword.data = longword.data | 0x0000000000000000;
				} else if(numitems == 2){
					longword.data = longword.data | 0x0100000000000000;
				} else if(numitems == 3){
					longword.data = longword.data | 0x0200000000000000;
				} else if(numitems == 4){
					longword.data = longword.data | 0x0300000000000000;
				} else if(numitems == 5){
					longword.data = longword.data | 0x0400000000000000;
				} else if(numitems == 6){
					longword.data = longword.data | 0x0500000000000000;
				} else if(numitems == 7){
					longword.data = longword.data | 0x0600000000000000;
				} else if(numitems == 8){
					longword.data = longword.data | 0x0700000000000000;
				} else {
					cout<<"mutategraph:: ERROR. should NEVER get here 4. exiting..."<<endl;
					exit(EXIT_FAILURE);
				}
				#endif 
				
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"longword.data: "<<(unsigned long)longword.data<<" (committed?"<<commit<<")"<<endl;
				utilityobj->ulongtobinary(longword.data);
				#endif 
				#ifdef _DEBUGMODE_HOSTPRINTS//3
				utilityobj->printcodedkeyvalue("processactivevertices.longword.data", longword.data, header_bitsize);
				#endif 
			}
			
			if(commit == true){
				longword.data = 0;
				push(&longword, newx);
				currpartition = llpartition;
				currbitoffset = 0;
				currbitoffset += newx.numbits;
				numitems = 0;
				header_bitsize = newx.numbits;
			}
			
			if(commit== true){ commit = false; }
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"STATS: totalnumedgesprocessed: "<<totalnumedgesprocessed<<", totalnumcommits: "<<totalnumcommits<<", reduction factor: "<<(float)((float)totalnumedgesprocessed / (float)totalnumcommits)<<endl;
	cout<<"STATS: numcommits from: new shrinked bitsize: "<<commitreasontype1<<endl;
	cout<<"STATS: numcommits from: change in last level partition: "<<commitreasontype2<<endl;
	cout<<"STATS: numcommits from: long word full: "<<commitreasontype3<<endl;
	
	cout<<"STATS: numcommits from: header_bitsize8count: "<<header_bitsize8count<<endl;
	cout<<"STATS: numcommits from: header_bitsize16count: "<<header_bitsize16count<<endl;
	cout<<"STATS: numcommits from: header_bitsize24count: "<<header_bitsize24count<<endl;
	cout<<"STATS: numcommits from: header_bitsize32count: "<<header_bitsize32count<<endl;

	for(unsigned int i=0; i<16; i++){ cout<<"STATS: commits with "<<i<<" items: (numitemscount["<<i<<"]): "<<numitemscount[i]<<endl; }
	#endif 
	return;
}








