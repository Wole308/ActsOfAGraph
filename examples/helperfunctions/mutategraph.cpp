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

arbval_t mutategraph::shrink(unsigned int x){
	arbval_t result;
	cout<<"x [before shrink]: x: "<<x<<endl;
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
	cout<<"x [after shrink]: result.x: "<<result.x<<", result.numbits: "<<result.numbits<<endl;
	return result;
}
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
	
	value_t buffersize_kvs = 0;
	uuint64_dt commitvertexupdate;
	uuint64_dt longword; longword.data = 0;
	unsigned int currpartition = 0;
	unsigned int currbitoffset = 0;
	unsigned int numitems = 0;
	unsigned int header_bitsize = 0;
	bool commit = false;
	
	// for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){
	for(unsigned int vid=1; vid<2; vid++){
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		for(unsigned int k=0; k<edges_size; k++){
			edge2_type edge = edgedatabuffer[vptr_begin + k];
			cout<<"edge.dstvid: "<<edge.dstvid<<endl;
			
			unsigned int llpartition = edge.dstvid >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * TREE_DEPTH));
	
			arbval_t newx = shrink(edge.dstvid);
			
			if(llpartition != currpartition || currbitoffset + newx.numbits > 64){
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
				cout<<"..............................committing..."<<endl; 
				// longword.data = longword.data | 0xF000000000000000; // 0x[metadata:8]000000000000000;
				
				if(header_bitsize == 0){ longword.data = longword.data | numitems << 4; }
				
				if(header_bitsize == 0){ longword.data = longword.data | 0x1000000000000000; }
				else if(header_bitsize == 8){ longword.data = longword.data | 0x3000000000000000; }
				else if(header_bitsize == 16){ longword.data = longword.data | 0x1000000000000000; }
				else if(header_bitsize == 24){ longword.data = longword.data | 0x1000000000000000; }
				else { }
				
				// buffer1[{{v}}][buffersize{{v}}_kvs] = commitvertexupdate{{v}}; buffersize{{v}}_kvs += 1; 
			}
			
			cout<<"longword.data: "<<(unsigned long)longword.data<<" (committed?"<<commit<<")"<<endl;
			utilityobj->ulongtobinary(longword.data);
			utilityobj->printcodedkeyvalue("processactivevertices.longword.data", longword.data, newx.numbits);
			
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
	
	
	
	
	
	
	/** {%for v in context['VECTOR_SIZE_seq']%}
	value_t buffersize{{v}}_kvs = 0; // NEWCHANGE.
	{%endfor%}
	{%for v in context['VECTOR_SIZE_seq']%}
	uuint64_dt commitvertexupdate{{v}};
	uuint64_dt commitvertex2update{{v}};
	uuint64_dt longword{{v}}; longword{{v}}.data = 0; // REMOVEME.
	unsigned int currpartition{{v}} = 0;
	unsigned int currbitoffset{{v}} = 0;
	bool commit{{v}} = false;
	bool en{{v}} = false;
	{%endfor%} */
	
	// #ifdef GGG
	/////////////////////////////////////
	/** {%for v in context['VECTOR_SIZE_seq']%}
	cout<<"vertexupdate{{v}}.key: "<<vertexupdate{{v}}.key<<", vertexupdate{{v}}.value: "<<vertexupdate{{v}}.value<<endl;
	{%endfor%} */
	
	// {%for v in context['VECTOR_SIZE_seq']%}
	// if(((edgeid_kvs == edgesbegin_kvs) && ({{v}} < colstart)) || ((edgeid_kvs == edgesend_kvs-1) && ({{v}} > colend))){ en{{v}} = false; }
	// else { en{{v}} = true; }
	// {%endfor%}
	
	// {%for v in context['VECTOR_SIZE_seq']%}
	// if(en{{v}} == true){ cout<<"vertexupdate{{v}}.key: "<<vertexupdate{{v}}.key<<", vertexupdate{{v}}.value: "<<vertexupdate{{v}}.value<<endl; }
	// {%endfor%}
	
	/** {%for v in context['VECTOR_SIZE_seq']%}
	unsigned int llpartition{{v}} = NAp;
	if(en{{v}} == true){ llpartition{{v}} = (vertexupdate{{v}}.key - 0) >> (globalparams.batch_range_pow - (NUM_PARTITIONS_POW * TREE_DEPTH)); } // FIXME. upperlimit
	{%endfor%} */
	
	/** {%for v in context['VECTOR_SIZE_seq']%}
	arbval_t newx{{v}};
	if(en{{v}} == true){ newx{{v}} = shrink(vertexupdate{{v}}.key); }
	{%endfor%} */
	
	/** {%for v in context['VECTOR_SIZE_seq']%}
	if(en{{v}} == false){
		// do nothing.
	} else if(llpartition{{v}} != currpartition{{v}} || currbitoffset{{v}} + newx{{v}}.numbits >= 64){
		commit{{v}} = true;
		commitvertexupdate{{v}} = longword{{v}};
	} else {
		push(&longword{{v}}, newx{{v}});
		currpartition{{v}} = llpartition{{v}};
		currbitoffset{{v}} += newx{{v}}.numbits;
	}
	{%endfor%} */
	
	/** {%for v in context['VECTOR_SIZE_seq']%}
	if(en{{v}} == true && commit{{v}} == true){ 
		cout<<"committing at v: {{v}}..."<<endl;
		// buffer1[{{v}}][buffersize{{v}}_kvs] = commitvertexupdate{{v}}; buffersize{{v}}_kvs += 1; 
	}
	{%endfor%} */
	
	/** {%for v in context['VECTOR_SIZE_seq']%}
	if(en{{v}} == true && commit{{v}} == true){
		longword{{v}}.data = 0;
		push(&longword{{v}}, newx{{v}});
		currpartition{{v}} = llpartition{{v}};
		currbitoffset{{v}} = 0;
		currbitoffset{{v}} += newx{{v}}.numbits;
	}
	{%endfor%} */
	// {%for v in context['VECTOR_SIZE_seq']%}
	// cout<<"[after possible commit]: commit{{v}}: "<<commit{{v}}<<", currpartition{{v}}: "<<currpartition{{v}}<<", currbitoffset{{v}}: "<<currbitoffset{{v}}<<endl;
	// {%endfor%}
	
	/** {%for v in context['VECTOR_SIZE_seq']%}
	if(en{{v}} == true && commit{{v}} == true){ commit{{v}} = false; }
	{%endfor%} */
	
	/** {%for v in context['VECTOR_SIZE_seq']%}
	cout<<endl<<"printing current state for v: {{v}}"<<endl;
	cout<<"longword{{v}}.data: "<<(unsigned long)longword{{v}}.data<<endl;
	actsutilityobj->ulongtobinary(longword{{v}}.data);
	actsutilityobj->printcodedkeyvalue("processactivevertices.longword{{v}}.data", longword{{v}}.data, newx{{v}}.numbits);
	{%endfor%} */
	
	// exit(EXIT_SUCCESS);
	/////////////////////////////////////
	// #endif
	
	return;
}









