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
#include "compactgraph.h"
using namespace std;

/** compactd edge: [setof(8,16,32,64...) | numitems | data(kv,kv,kv,kv...)] */
/** longword.data = longword.data | 0xF000000000000000; // 0x[metadata:8]000000000000000; */

compactgraph::compactgraph(graph * _graphobj, stats * _statsobj){
	parametersobj = new parameters();
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
}
compactgraph::compactgraph(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
compactgraph::~compactgraph(){} 

mail_t compactgraph::shrink(unsigned int x){
	mail_t mail;
	mail.x = x % (1 << SRAMSZ_POW);
	mail.streetaddr = x >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH));
	mail.houseno = SRAMSZ_POW;
	
	#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
	cout<<"x [before shrink]: x: "<<x<<", x [after shrink]: mail.x: "<<mail.x<<", mail.houseno: "<<mail.houseno<<endl;
	#endif 
	return mail;
}
void compactgraph::push(uuint64_dt * longword, mail_t kv){
	longword->data = (longword->data << SRAMSZ_POW) | kv.x;
	return;
}

void compactgraph::compact(edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * packedvertexptrbuffer, uuint64_dt * packededgedatabuffer){				
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	
	#ifdef _DEBUGMODE_STATS
	unsigned int totalnumedgesprocessed = 0;
	unsigned int totalnumcommits = 0;
	unsigned int commitreasontype1 = 0;
	unsigned int commitreasontype2 = 0;
	unsigned int commitreasontype3 = 0;
	unsigned int numitemscount[16]; for(unsigned int i=0; i<16; i++){ numitemscount[i] = 0; }
	#endif 
	
	keyvalue_t * counts1 = new keyvalue_t[KVDATA_RANGE];
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ counts1[i].value = 0; }
	keyvalue_t * counts2 = new keyvalue_t[KVDATA_RANGE];
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ counts2[i].key = 0; counts2[i].value = 0; }
	unsigned int committededgescount = 0;
	unsigned int * numitemspacked = new unsigned int[PADDEDEDGES_BATCHSIZE];
	for(unsigned int i=0; i<PADDEDEDGES_BATCHSIZE; i++){ numitemspacked[i] = 0; }
	
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){ // FIXME.
	// for(unsigned int vid=0; vid<24; vid++){
		#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
		cout<<endl<<">>> compactgraph::compact: vid: "<<vid<<endl;
		#endif 
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		value_t buffersize_kvs = 0;
		uuint64_dt longword;
		longword.data = 0;
		// longword.data = 0xFFFFFFFFFFFFFFFF;
		unsigned int currpartition = 0;
		unsigned int currbitoffset = 0;
		unsigned int numitems = 0;
		bool commit = false;
		mail_t lastmail;
		
		unsigned int llpartition = edgedatabuffer[vptr_begin].dstvid >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH));
		currpartition = llpartition;
		mail_t newx = shrink(edgedatabuffer[vptr_begin].dstvid);
		lastmail = newx;
		
		for(unsigned int k=0; k<edges_size; k++){
			#ifdef _DEBUGMODE_STATS
			totalnumedgesprocessed += 1;
			#endif
			edge2_type edge = edgedatabuffer[vptr_begin + k];
			counts1[vid].value += 1;
			
			unsigned int llpartition = edge.dstvid >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH));
			mail_t newx = shrink(edge.dstvid);
			#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
			cout<<"+++ edge.dstvid: "<<edge.dstvid<<", llpartition: "<<llpartition<<", x[after shrink] "<<newx.x<<", newx.houseno: "<<newx.houseno<<", numitems: "<<numitems<<endl;
			#endif 
			
			if((llpartition == currpartition && currbitoffset + newx.houseno < 56) || (numitems == 0)){
				push(&longword, newx);
				commit = false;
				currpartition = llpartition;
				currbitoffset += newx.houseno;
				lastmail = newx;
				numitems += 1;
			} else {
				if(llpartition == currpartition){ push(&longword, newx); numitems += 1; }
				commit = true;
				if(numitems == 0){ cout<<"ERROR. numitems == 0. exiting..."<<endl; exit(EXIT_FAILURE); }
			}
			
			if(commit == true || k == edges_size-1){
				#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
				cout<<"..........committing numitems: "<<numitems<<", currbitoffset: "<<currbitoffset<<"... REASON: ";
				if(llpartition != currpartition){ cout<<"change in last level partition. "; commitreasontype2 += 1; }
				if(currbitoffset == 42){ cout<<"long word full. "; commitreasontype3 += 1; }
				cout<<""<<endl;
				#endif
				#ifdef _DEBUGMODE_STATS
				if(llpartition != currpartition){ commitreasontype2 += 1; }
				if(currbitoffset == 42){ commitreasontype3 += 1; }
				#endif 
				
				// append header 
				unsigned long streetaddr = lastmail.streetaddr;
				longword.data = (streetaddr << 56) | longword.data;
				// longword.data = (streetaddr << 56) | (longword.data & 0x00FFFFFFFFFFFFFF);
				
				// the actual committing...
				packededgedatabuffer[committededgescount] = longword;
				numitemspacked[committededgescount] = numitems;
				
				// updates and resets
				currbitoffset = 0;
				committededgescount += 1;
				counts2[vid].value += 1;
				#ifdef _DEBUGMODE_STATS
				totalnumcommits += 1;
				numitemscount[numitems] += 1;
				#endif
				numitems = 0;
				
				#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
				utilityobj->ulongtobinary(longword.data);
				#endif 
				#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
				utilityobj->printcodedkeyvalue("processactivevertices.longword.data", longword.data);
				#endif

				longword.data = 0;
				// longword.data = 0xFFFFFFFFFFFFFFFF;
				// exit(EXIT_SUCCESS);
			}
			
			if(llpartition != currpartition){
				push(&longword, newx);
				commit = false;
				currpartition = llpartition;
				currbitoffset += newx.houseno;
				lastmail = newx;
				numitems += 1;
			}
		}
		
		// edge conditions
		if(numitems > 0){
			#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
			cout<<"..........[edge conditions]committing "<<numitems<<" items... REASON: ";
			if(llpartition != currpartition){ cout<<"change in last level partition. "; commitreasontype2 += 1; }
			if(currbitoffset == 42){ cout<<"long word full. "; commitreasontype3 += 1; }
			cout<<""<<endl;
			#endif 
			#ifdef _DEBUGMODE_STATS
			if(llpartition != currpartition){ commitreasontype2 += 1; }
			if(currbitoffset == 42){ commitreasontype3 += 1; }
			#endif
			
			// append header 
			unsigned long streetaddr = lastmail.streetaddr;
			longword.data = (streetaddr << 56) | longword.data;

			// the actual committing...
			packededgedatabuffer[committededgescount] = longword;
			numitemspacked[committededgescount] = numitems;
			
			// updates and resets
			currbitoffset = 0;
			committededgescount += 1;
			counts2[vid].value += 1;
			#ifdef _DEBUGMODE_STATS
			totalnumcommits += 1;
			numitemscount[numitems] += 1;
			#endif
			numitems = 0;
			
			#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
			utilityobj->ulongtobinary(longword.data);
			#endif 
			#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
			utilityobj->printcodedkeyvalue("processactivevertices.longword.data", longword.data);
			#endif
		
			longword.data = 0;
			// longword.data = 0xFFFFFFFFFFFFFFFF;
		}
	}
	utilityobj->calculateunallignedoffsets(counts1, KVDATA_RANGE);
	utilityobj->calculateunallignedoffsets(counts2, KVDATA_RANGE);
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ packedvertexptrbuffer[i] = counts2[i].key; }
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"STATS: totalnumedgesprocessed: "<<totalnumedgesprocessed<<", totalnumcommits: "<<totalnumcommits<<", reduction factor: "<<(float)((float)totalnumedgesprocessed / (float)totalnumcommits)<<endl;
	cout<<"STATS: numcommits from: new shrinked bitsize: "<<commitreasontype1<<endl;
	cout<<"STATS: numcommits from: change in last level partition: "<<commitreasontype2<<endl;
	cout<<"STATS: numcommits from: long word full: "<<commitreasontype3<<endl;
	for(unsigned int i=0; i<16; i++){ cout<<"STATS: commits with "<<i<<" items: (numitemscount["<<i<<"]): "<<numitemscount[i]<<endl; }
	 
	utilityobj->printvalues("compactgraph::compact:: vertexptrbuffer", vertexptrbuffer, 16);
	
	utilityobj->printkeyvalues("compactgraph::compact:: counts1", counts1, 16);
	utilityobj->countvalues("compactgraph::compact:: counts1", counts1, KVDATA_RANGE);
	
	utilityobj->printkeyvalues("compactgraph::compact:: counts2", counts2, 16);
	utilityobj->countvalues("compactgraph::compact:: counts2", counts2, KVDATA_RANGE);
	
	utilityobj->printpackededges("compactgraph::compact:: packededgedatabuffer", packededgedatabuffer, 16);
	utilityobj->printvalues("compactgraph::compact:: packedvertexptrbuffer", packedvertexptrbuffer, 16);
	
	// graphobj->printdataset();
	for(unsigned int i=0; i<16; i++){ cout<<"STATS: numitemspacked["<<i<<"]: "<<numitemspacked[i]<<endl; }
	
	verify(vertexptrbuffer, edgedatabuffer, packedvertexptrbuffer, packededgedatabuffer, numitemspacked);
	#endif
	// exit(EXIT_SUCCESS);
	return ;
}
void compactgraph::verify(edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * packedvertexptrbuffer, uuint64_dt * packededgedatabuffer, unsigned int * numitemspacked){
	unsigned int edges_nc_count = 0;
	unsigned int edgesdstv_nc_sum = 0;
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
	// for(unsigned int vid=0; vid<24; vid++){ // 1024
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		for(unsigned int k=0; k<edges_size; k++){
			#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
			cout<<"vid: "<<vid<<", dstvid: "<<edgedatabuffer[vptr_begin + k].dstvid<<endl;
			#endif 
			edges_nc_count += 1;
			edgesdstv_nc_sum += edgedatabuffer[vptr_begin + k].dstvid;
		}
		#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
		cout<<"vid: "<<vid<<", edgesdstv_nc_sum: "<<edgesdstv_nc_sum<<endl;
		#endif
	}
	
	unsigned int edges_c_count = 0;
	unsigned int edgesdstv_c_sum = 0;
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
	// for(unsigned int vid=0; vid<24; vid++){
		#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
		cout<<endl<<">>> compactgraph::compact: vid: "<<vid<<endl;
		#endif 
		edge_t vptr_begin = packedvertexptrbuffer[vid];
		edge_t vptr_end = packedvertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"compactgraph::compact: vid: "<<vid<<endl;
		cout<<"compactgraph::compact: vptr_begin: "<<vptr_begin<<endl;
		cout<<"compactgraph::compact: vptr_end: "<<vptr_end<<endl;
		cout<<"compactgraph::compact: edges_size: "<<edges_size<<endl;
		#endif 
		
		for(unsigned int k=0; k<edges_size; k++){
			unsigned long longword = packededgedatabuffer[vptr_begin + k].data;
			
			unsigned int h = utilityobj->bitExtracted(longword, 8, 56);
			unsigned int d = utilityobj->bitExtracted(longword, 14, 42);
			unsigned int c = utilityobj->bitExtracted(longword, 14, 28);
			unsigned int b = utilityobj->bitExtracted(longword, 14, 14);
			unsigned int a = utilityobj->bitExtracted(longword, 14, 0);
			#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
			cout<<"- h: "<<h<<", d: "<<d<<", c: "<<c<<", b: "<<b<<", a: "<<a<<endl;
			#endif 
			
			unsigned int ax = 0;
			unsigned int bx = 0;
			unsigned int cx = 0;
			unsigned int dx = 0;
			
			// if(a != 0){ ax = (h * (1 << SRAMSZ_POW)) + a; }
			// if(b != 0){ bx = (h * (1 << SRAMSZ_POW)) + b; }
			// if(c != 0){ cx = (h * (1 << SRAMSZ_POW)) + c; }
			// if(d != 0){ dx = (h * (1 << SRAMSZ_POW)) + d; }
			
			if(numitemspacked[vptr_begin + k] >= 1){ ax = (h * (1 << SRAMSZ_POW)) + a; }
			if(numitemspacked[vptr_begin + k] >= 2){ bx = (h * (1 << SRAMSZ_POW)) + b; }
			if(numitemspacked[vptr_begin + k] >= 3){ cx = (h * (1 << SRAMSZ_POW)) + c; }
			if(numitemspacked[vptr_begin + k] >= 4){ dx = (h * (1 << SRAMSZ_POW)) + d; }
			
			// ax = (h * (1 << SRAMSZ_POW)) + a; 
			// bx = (h * (1 << SRAMSZ_POW)) + b; 
			// cx = (h * (1 << SRAMSZ_POW)) + c; 
			// dx = (h * (1 << SRAMSZ_POW)) + d; 
			
			#if defined(_DEBUGMODE_HOSTPRINTS) && defined(_DEBUGMODE_COMPACTGRAPH)
			cout<<"numitemspacked["<<vptr_begin + k<<"]: "<<numitemspacked[vptr_begin + k]<<endl;
			utilityobj->printcodedkeyvalue("processactivevertices.verify.longword", longword);
			cout<<"ax: "<<ax<<endl;
			cout<<"bx: "<<bx<<endl;
			cout<<"cx: "<<cx<<endl;
			cout<<"dx: "<<dx<<endl;
			#endif 
			
			edges_c_count += 1;
			// edges_c_count += numitemspacked[vptr_begin + k];
			edgesdstv_c_sum += ax;
			edgesdstv_c_sum += bx;
			edgesdstv_c_sum += cx;
			edgesdstv_c_sum += dx;
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"vid: "<<vid<<", edgesdstv_c_sum: "<<edgesdstv_c_sum<<endl;
		#endif 
	}
	
	cout<<"--- compactgraph:: edges_nc_count (non compact): "<<edges_nc_count<<endl;
	cout<<"--- compactgraph:: edgesdstv_nc_sum (non compact): "<<edgesdstv_nc_sum<<endl;
	
	cout<<"--- compactgraph:: edges_c_count (compact): "<<edges_c_count<<endl;
	cout<<"--- compactgraph:: edgesdstv_c_sum (compact): "<<edgesdstv_c_sum<<endl;
	return;
}







