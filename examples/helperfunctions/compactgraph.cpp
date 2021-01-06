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
#include <string.h>
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

/** 
compacted edge: [street addr:8bits:off56 | #items:4bits:off52 | house addrs(k1,k2,k3,k4):14bitseach:off0] 
longword.data = longword.data | 0xF000000000000000; // 0x[metadata:8]000000000000000;
*/

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

unsigned int compactgraph::getllpartition(unsigned int data){
	return data >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH));
}
mail_t compactgraph::shrink(unsigned int x){
	mail_t mail;
	mail.x = x % (1 << SRAMSZ_POW);
	mail.streetaddr = getllpartition(x);
	if(mail.streetaddr >= 256){ cout<<"compactgraph::shrink: mail.streetaddr >= 256. exiting..."<<endl; exit(EXIT_FAILURE); }
	mail.houseno = SRAMSZ_POW;
	
	#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
	cout<<"x [before shrink]: x: "<<x<<", x [after shrink]: mail.x: "<<mail.x<<", mail.houseno: "<<mail.houseno<<endl;
	#endif 
	return mail;
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
	// for(unsigned int vid=0; vid<3; vid++){
		#if defined(_DEBUGMODE_HOSTPRINTS3) || defined(_DEBUGMODE_COMPACTGRAPH)
		if(vid % 100000 == 0){ cout<<endl<<">>> compactgraph::compact: vid: "<<vid<<endl; }
		#endif 
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		value_t buffersize_kvs = 0;
		uuint64_dt longword;
		longword.data = 0;
		unsigned int currpartition = 0;
		unsigned int currbitoffset = 0;
		unsigned int numitems = 0;
		mail_t lastmail;
		
		unsigned int llpartition = getllpartition(edgedatabuffer[vptr_begin].dstvid); 
		currpartition = llpartition;
		mail_t newx = shrink(edgedatabuffer[vptr_begin].dstvid);
		lastmail = newx;
		
		for(unsigned int k=0; k<edges_size; k++){
			#ifdef _DEBUGMODE_STATS
			totalnumedgesprocessed += 1;
			#endif
			edge2_type edge = edgedatabuffer[vptr_begin + k];
			counts1[vid].value += 1;
			
			unsigned int llpartition = getllpartition(edge.dstvid);
			mail_t newx = shrink(edge.dstvid);
			#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
			cout<<"+++ edge.dstvid: "<<edge.dstvid<<", llpartition: "<<llpartition<<", x[after shrink] "<<newx.x<<", currbitoffset: "<<currbitoffset<<", newx.houseno: "<<newx.houseno<<", numitems: "<<numitems<<endl;
			#endif 
			
			if((llpartition != currpartition || currbitoffset + newx.houseno >= COMPACTPARAM_BITSIZE_TOTALDATA) && (numitems != 0) && (k != 0)){
				
				////////
				if(numitems == 0 || numitems > 3){ cout<<"ERROR. numitems("<<numitems<<") == 0 || numitems > 3. exiting..."<<endl; exit(EXIT_FAILURE); }
				
				// ^^^ commit first before continuing
				#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
				cout<<"..........committing >>> numitems: "<<numitems<<", currbitoffset: "<<currbitoffset<<"... REASON: ";
				if(llpartition != currpartition){ cout<<"change in last level partition. "; commitreasontype2 += 1; }
				if(currbitoffset + SRAMSZ_POW >= COMPACTPARAM_ENDOFFSET_DATA){ cout<<"long word full. "; commitreasontype3 += 1; }
				cout<<""<<endl;
				#endif
				#ifdef _DEBUGMODE_STATS
				if(llpartition != currpartition){ commitreasontype2 += 1; }
				if(currbitoffset + SRAMSZ_POW >= COMPACTPARAM_ENDOFFSET_DATA){ commitreasontype3 += 1; }
				#endif 
				
				// append header 
				unsigned long streetaddr = lastmail.streetaddr;
				longword.data = (streetaddr << COMPACTPARAM_STARTOFFSET_STREETADDR) | longword.data; // append street address 
				unsigned long numelements = numitems;
				longword.data = (numelements << COMPACTPARAM_STARTOFFSET_NUMITEMS) | longword.data; // append street address 
				
				// the actual committing...
				#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
				utilityobj->ULONGTOBINARY(longword.data);
				utilityobj->PARSE("compactgraph:: actual committing...", longword.data);
				#endif 
				// exit(EXIT_SUCCESS);
				#ifdef _DEBUGMODE_CHECKS2
				utilityobj->checkoutofbounds("compactgraph 211", committededgescount, PADDEDEDGES_BATCHSIZE, NAp, NAp, NAp);
				#endif 
				packededgedatabuffer[committededgescount] = longword;
				numitemspacked[committededgescount] = numitems;
				
				// updates and resets
				currbitoffset = 0;
				committededgescount += 1;
				counts2[vid].value += 1;
				#ifdef _DEBUGMODE_STATS
				totalnumcommits += 1;
				#ifdef _DEBUGMODE_CHECKS2
				utilityobj->checkoutofbounds("compactgraph 212", numitems, 16, NAp, NAp, NAp);
				#endif 
				numitemscount[numitems] += 1;
				#endif
				numitems = 0;

				longword.data = 0;
				////////
				// exit(EXIT_SUCCESS);
				
				// ^^^ continue with this item 
				utilityobj->PUSH(&longword, newx.x, SRAMSZ_POW);
				currpartition = llpartition;
				currbitoffset += newx.houseno;
				lastmail = newx;
				numitems += 1;
			} else {
				#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
				cout<<"pushing... >>>"<<endl;
				#endif 
				utilityobj->PUSH(&longword, newx.x, SRAMSZ_POW);
				currpartition = llpartition;
				currbitoffset += newx.houseno;
				lastmail = newx;
				numitems += 1;
			}
		}
		
		////////
		if(numitems > 0){
			if(numitems == 0 || numitems > 3){ cout<<"ERROR. numitems("<<numitems<<") == 0 || numitems > 3. exiting..."<<endl; exit(EXIT_FAILURE); }
			
			// ^^^ commit first before continuing
			#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
			cout<<"..........committing >>> numitems: "<<numitems<<", currbitoffset: "<<currbitoffset<<"... REASON: ";
			if(llpartition != currpartition){ cout<<"change in last level partition. "; commitreasontype2 += 1; }
			if(currbitoffset + SRAMSZ_POW >= COMPACTPARAM_ENDOFFSET_DATA){ cout<<"long word full. "; commitreasontype3 += 1; }
			cout<<""<<endl;
			#endif
			#ifdef _DEBUGMODE_STATS
			if(llpartition != currpartition){ commitreasontype2 += 1; }
			if(currbitoffset + SRAMSZ_POW >= COMPACTPARAM_ENDOFFSET_DATA){ commitreasontype3 += 1; }
			#endif 
			
			// append header 
			unsigned long streetaddr = lastmail.streetaddr;
			longword.data = (streetaddr << COMPACTPARAM_STARTOFFSET_STREETADDR) | longword.data; // append street address 
			unsigned long numelements = numitems;
			longword.data = (numelements << COMPACTPARAM_STARTOFFSET_NUMITEMS) | longword.data; // append street address 
			
			// the actual committing...
			#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
			utilityobj->ULONGTOBINARY(longword.data);
			utilityobj->PARSE("compactgraph:: actual committing...", longword.data);
			#endif
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("compactgraph 213", committededgescount, PADDEDEDGES_BATCHSIZE, NAp, NAp, NAp);
			#endif
			packededgedatabuffer[committededgescount] = longword;
			numitemspacked[committededgescount] = numitems;
			
			// updates and resets
			currbitoffset = 0;
			committededgescount += 1;
			counts2[vid].value += 1;
			#ifdef _DEBUGMODE_STATS
			totalnumcommits += 1;
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("compactgraph 214", numitems, 16, NAp, NAp, NAp);
			#endif
			numitemscount[numitems] += 1;
			#endif
			numitems = 0;

			longword.data = 0;
		}
		////////		
	}
	cout<<"--- utility::compactgraph::compact..."<<endl;
	// exit(EXIT_SUCCESS);
	// utilityobj->calculateunallignedoffsets(counts1, KVDATA_RANGE); // FIXME.
	utilityobj->calculateunallignedoffsets(counts2, KVDATA_RANGE);
	cout<<"--- utility::compactgraph::compacttttt..."<<endl;
	// exit(EXIT_SUCCESS);
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ packedvertexptrbuffer[i] = counts2[i].key; }
	cout<<"--- utility::compactgraph::compactttttnnnnn..."<<endl;
	// exit(EXIT_SUCCESS);
	
	#ifdef _DEBUGMODE_HOSTPRINTS
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
	delete counts1;
	delete counts2;
	delete numitemspacked;
	return;
}
void compactgraph::verify(edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * packedvertexptrbuffer, uuint64_dt * packededgedatabuffer, unsigned int * numitemspacked){
	unsigned int edges_nc_count = 0;
	unsigned int edgesdstv_nc_sum = 0;
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
	// for(unsigned int vid=0; vid<3; vid++){ 
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		for(unsigned int k=0; k<edges_size; k++){
			#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
			cout<<"vid: "<<vid<<", dstvid: "<<edgedatabuffer[vptr_begin + k].dstvid<<endl;
			#endif 
			edges_nc_count += 1;
			edgesdstv_nc_sum += edgedatabuffer[vptr_begin + k].dstvid;
		}
		#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
		cout<<"vid: "<<vid<<", edgesdstv_nc_sum: "<<edgesdstv_nc_sum<<endl;
		#endif
	}
	
	unsigned int edges_c_count = 0;
	unsigned int edgesdstv_c_sum = 0;
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
	// for(unsigned int vid=0; vid<3; vid++){
		#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
		cout<<endl<<">>> compactgraph::verify: vid: "<<vid<<endl;
		#endif 
		edge_t vptr_begin = packedvertexptrbuffer[vid];
		edge_t vptr_end = packedvertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"compactgraph::verify: vid: "<<vid<<endl;
		cout<<"compactgraph::verify: vptr_begin: "<<vptr_begin<<endl;
		cout<<"compactgraph::verify: vptr_end: "<<vptr_end<<endl;
		cout<<"compactgraph::verify: edges_size: "<<edges_size<<endl;
		#endif 
		
		unsigned int items[8];
		for(unsigned int k=0; k<edges_size; k++){
			unsigned long longword = packededgedatabuffer[vptr_begin + k].data;
			
			unsigned int numitems = utilityobj->PARSE(longword, items);
			if(numitems > 3){
				cout<<"numitems > 3. exiting..."<<endl;
				utilityobj->ULONGTOBINARY(longword);
				utilityobj->PARSE("compactgraph::verify actual committing...", longword);
				exit(EXIT_FAILURE);
			}
			#if defined(_DEBUGMODE_HOSTPRINTS) || defined(_DEBUGMODE_COMPACTGRAPH)
			for(unsigned int t=0; t<numitems; t++){ cout<<"- items["<<t<<"]: "<<items[t]<<endl; }
			#endif 
			
			edges_c_count += 1;
			for(unsigned int t=0; t<numitems; t++){ edgesdstv_c_sum += items[t]; }
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"vid: "<<vid<<", edgesdstv_c_sum: "<<edgesdstv_c_sum<<endl;
		#endif 
	}
	
	cout<<"--- compactgraph::verify: edges_nc_count (non compact): "<<edges_nc_count<<endl;
	cout<<"--- compactgraph::verify: edgesdstv_nc_sum (non compact): "<<edgesdstv_nc_sum<<endl;
	
	cout<<"--- compactgraph::verify: edges_c_count (compact): "<<edges_c_count<<endl;
	cout<<"--- compactgraph::verify: edgesdstv_c_sum (compact): "<<edgesdstv_c_sum<<endl;
	if(edgesdstv_c_sum != edgesdstv_nc_sum){ cout<<"compactgraph::verify: ERROR: edgesdstv_c_sum != edgesdstv_nc_sum. exiting..."<<endl; exit(EXIT_FAILURE); }
	cout<<"compactgraph::verify: verify successful."<<endl;
	return;
}







