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
	
	#ifdef _DEBUGMODE_HOSTPRINTS
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
	
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
	// for(unsigned int vid=0; vid<4; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<endl<<">>> compactgraph::compact: vid: "<<vid<<endl;
		#endif 
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		value_t buffersize_kvs = 0;
		uuint64_dt commitvertexupdate;
		uuint64_dt longword; longword.data = 0;
		unsigned int currpartition = 0;
		unsigned int currbitoffset = 0;
		#ifdef _DEBUGMODE_STATS
		unsigned int numitems = 0;
		#endif 
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
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"+++ edge.dstvid: "<<edge.dstvid<<", llpartition: "<<llpartition<<", x[after shrink] "<<newx.x<<", newx.houseno: "<<newx.houseno<<", numitems: "<<numitems<<endl;
			#endif 
			
			if(llpartition != currpartition || currbitoffset + newx.houseno > 64){
				commit = true;
				commitvertexupdate = longword;
				#ifdef _DEBUGMODE_STATS
				if(numitems == 0){ cout<<"ERROR. numitems == 0. exiting..."<<endl; exit(EXIT_FAILURE); }
				#endif
			} else {
				commit = false;
				push(&longword, newx);
				currpartition = llpartition;
				currbitoffset += newx.houseno;
				#ifdef _DEBUGMODE_STATS
				numitems += 1;
				#endif 
				lastmail = newx;
			}
			
			if(commit == true || k == edges_size-1){
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"..........committing "<<numitems<<" items... RESON: ";
				if(llpartition != currpartition){ cout<<"change in last level partition. "; commitreasontype2 += 1; }
				if(currbitoffset + newx.houseno > 64){ cout<<"long word full. "; commitreasontype3 += 1; }
				cout<<""<<endl;
				#endif 
				
				#ifdef _DEBUGMODE_STATS
				if(llpartition != currpartition){ commitreasontype2 += 1; }
				if(currbitoffset + newx.houseno > 64){ commitreasontype3 += 1; }
				#endif 
				
				// append header 
				unsigned long streetaddr = lastmail.streetaddr; // 0xFF;
				longword.data = (streetaddr << 56) | longword.data;
				
				// the actual committing...
				packededgedatabuffer[committededgescount] = longword; // FIXME.
				committededgescount += 1;
				counts2[vid].value += 1;
				#ifdef _DEBUGMODE_STATS
				totalnumcommits += 1;
				numitemscount[numitems] += 1;
				numitems = 0;
				#endif 
				
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"longword.data: "<<(unsigned long)longword.data<<" (committed?"<<commit<<")"<<endl;
				utilityobj->ulongtobinary(longword.data);
				#endif 
				#ifdef _DEBUGMODE_HOSTPRINTS
				utilityobj->printcodedkeyvalue("processactivevertices.longword.data", longword.data);
				#endif
			
				// insert current one
				longword.data = 0;
				push(&longword, newx);
				currpartition = llpartition;
				currbitoffset = 0;
				currbitoffset += newx.houseno;
				#ifdef _DEBUGMODE_STATS
				numitems += 1;
				#endif 
				lastmail = newx;
				
				// reset
				commit = false;
			}
		}
		
		// edge conditions
		if(numitems != 0){
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"..........[edge conditions]committing "<<numitems<<" items... RESON: ";
			if(llpartition != currpartition){ cout<<"change in last level partition. "; commitreasontype2 += 1; }
			if(currbitoffset + newx.houseno > 64){ cout<<"long word full. "; commitreasontype3 += 1; }
			cout<<""<<endl;
			#endif 
			
			#ifdef _DEBUGMODE_STATS
			if(llpartition != currpartition){ commitreasontype2 += 1; }
			if(currbitoffset + newx.houseno > 64){ commitreasontype3 += 1; }
			#endif
			
			// append header 
			unsigned long streetaddr = lastmail.streetaddr; // 0xFF;
			longword.data = (streetaddr << 56) | longword.data;
			
			// the actual committing...
			counts2[vid].value += 1;
			#ifdef _DEBUGMODE_STATS
			totalnumcommits += 1;
			numitemscount[numitems] += 1;
			numitems = 0;
			#endif 
			
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"longword.data: "<<(unsigned long)longword.data<<" (committed?"<<commit<<")"<<endl;
			utilityobj->ulongtobinary(longword.data);
			#endif 
			#ifdef _DEBUGMODE_HOSTPRINTS
			utilityobj->printcodedkeyvalue("processactivevertices.longword.data", longword.data);
			#endif
		
			// insert current one
			longword.data = 0;
			push(&longword, newx);
			currpartition = llpartition;
			currbitoffset = 0;
			currbitoffset += newx.houseno;
			#ifdef _DEBUGMODE_STATS
			numitems += 1;
			#endif 
			lastmail = newx;
			
			// reset
			commit = false;
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
	#endif
	return ;
}









