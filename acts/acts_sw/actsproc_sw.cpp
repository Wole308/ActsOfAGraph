#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "../../acts/actsutility/actsutility.h"
#include "actsproc_sw.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {vertices data}
// actvvs area {active vertices, active vertices mask}
// workspace area {stats, kvdram, kvdram workspace}

actsproc_sw::actsproc_sw(){ actsutilityobj = new actsutility(); }
actsproc_sw::~actsproc_sw(){}

void actsproc_sw::printstats(vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS]){
	for(unsigned int t = 0; t < TOTALNUMPARTITIONS; t++){
		if(t==0 && t<(1) && kvdram[t].size()>0){
			cout<<"topkernelproc:: LOP 1: partition: 1"<<": (t:"<<t<<") ["<<kvdram[t].size()<<"]"<<endl; 
		} else if(t>=(1) && t<(1+16) && kvdram[t].size()>0){
			cout<<"topkernelproc:: LOP 2: partition: "<<t-(1)<<": (t:"<<t<<") ["<<kvdram[t].size()<<"]"<<endl; 
		} else if(t>=(1+16) && t<(1+16+256) && kvdram[t].size()>0){
			cout<<"topkernelproc:: LOP 3: partition: "<<t-(1+16)<<": (t:"<<t<<") ["<<kvdram[t].size()<<"]"<<endl; 
		}
	}
	return;
}

unsigned int actsproc_sw::amin(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}

unsigned int actsproc_sw::get_num_source_partitions(unsigned int currentLOP){
	if(currentLOP == 0){ currentLOP = 1; }
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

unsigned int actsproc_sw::getskipsize(unsigned int currentLOP, bool_type sourceORdest){
	unsigned int result;
	if(currentLOP == 0){ currentLOP = 1; }
	
	unsigned int N = (1 << (NUM_PARTITIONS_POW * TREE_DEPTH));
	if(sourceORdest == SOURCE){ result = NUMLASTLEVELPARTITIONS; }
	else if (sourceORdest == DEST){ result = NUMLASTLEVELPARTITIONS / NUM_PARTITIONS; }
	
	for(unsigned int i=0; i<(currentLOP-1); i++){
		result = result / NUM_PARTITIONS;
	}
	return result;
}

sweepparams_t actsproc_sw::getsweepparams(unsigned int currentLOP, unsigned int source_partition){
	sweepparams_t sweepparams;
	unsigned int sourceskipsize = getskipsize(currentLOP, SOURCE);
	
	unsigned int div =  BATCH_RANGE;
	for(unsigned int i=0; i<TREE_DEPTH; i++){
		div = div / NUM_PARTITIONS;
	}
	
	sweepparams.currentLOP = currentLOP;
	sweepparams.upperlimit = source_partition * sourceskipsize * div; // POW_BATCHRANGE
	sweepparams.source_partition = source_partition;
	return sweepparams;
}

unsigned int actsproc_sw::processfunc(unsigned int udata, unsigned int edgew, unsigned int GraphAlgo){
	unsigned int res = 0;
	#ifdef PR_ALGORITHM
	res = udata;
	#endif
	#ifdef BFS_ALGORITHM
	// res = NAp;  // BFS
	res = udata + edgew;
	#endif 
	#ifdef SSSP_ALGORITHM
	res = udata + edgew;
	#endif
	return res;
}

value_t actsproc_sw::reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	#ifdef PR_ALGORITHM
	temp = vtemp + res;
	#endif 
	#ifdef BFS_ALGORITHM
	temp = amin(vtemp, res);
	#endif 
	#ifdef SSSP_ALGORITHM
	temp = amin(vtemp, res);
	#endif
	return temp;
}

unsigned int actsproc_sw::getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit, unsigned int batch_range_pow){
	unsigned int partition;

	if(keyvalue.value == INVALIDDATA){ partition = keyvalue.key; } 
	else { partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP))); }
	
	#ifdef _DEBUGMODE_CHECKS2
	if(partition >= NUM_PARTITIONS){
		cout<<"actsproc_sw::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", keyvalue.value: "<<keyvalue.value<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", BATCH_RANGE_POW: "<<BATCH_RANGE_POW<<endl; 
		exit(EXIT_FAILURE); 
	}
	#endif
	return partition;
}

void actsproc_sw::processit(edge_type * edges, edge_t * vptrs, value_t * vdatas, vector<vertex_t> &actvvs, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int currentLOP, unsigned int GraphAlgo, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker){				
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"processit called."<<endl;
	#endif
	unsigned int _sourcestatsmarker = *sourcestatsmarker;
	unsigned int _deststatsmarker = *deststatsmarker;
	// cout<<"processit: _sourcestatsmarker: "<<_sourcestatsmarker<<", _deststatsmarker: "<<_deststatsmarker<<endl;

	sweepparams_t sweepparams = getsweepparams(currentLOP, 0);
	
	for(unsigned int i=0; i<actvvs.size(); i++){
		unsigned int vid = actvvs[i];
		edge_t vptr_begin = vptrs[vid];
		edge_t vptr_end = vptrs[vid+1];
		
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ cout<<"topkernelproc: ERROR SEEN."<<endl; continue; }
		
		for(unsigned int k=0; k<edges_size; k++){
			unsigned int dstvid = edges[vptr_begin + k].dstvid;
			unsigned int res = processfunc(vdatas[vid], 1, GraphAlgo);
			
			keyvalue_t keyvalue;
			keyvalue.key = dstvid;
			keyvalue.value = res;
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"processit: udata: "<<res<<", srcvid: "<<vid<<", dstvid: "<<dstvid<<endl;
			#endif 
			
			unsigned int partition = getpartition(keyvalue, currentLOP, sweepparams.upperlimit, BATCH_RANGE_POW);
			kvdram[_deststatsmarker + partition].push_back(keyvalue);
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("actsproc_sw.processit. ERROR 7.", _deststatsmarker + partition, TOTALNUMPARTITIONS, NAp, NAp, NAp); 
			#endif 
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(1);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1);
			#endif 
		}
	}
	
	*sourcestatsmarker = _sourcestatsmarker + 1;
	*deststatsmarker = _deststatsmarker + NUM_PARTITIONS;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

void actsproc_sw::partitionit(vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int currentLOP, unsigned int GraphAlgo, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"partitionit called."<<endl;
	#endif
	unsigned int _sourcestatsmarker = *sourcestatsmarker;
	unsigned int _deststatsmarker = *deststatsmarker;
	// cout<<"partitionit: _sourcestatsmarker: "<<_sourcestatsmarker<<", _deststatsmarker: "<<_deststatsmarker<<endl;

	for(currentLOP=2; currentLOP<3; currentLOP++){
		unsigned int num_source_partitions = get_num_source_partitions(currentLOP);
		// cout<<"partitionit: currentLOP: "<<currentLOP<<", num_source_partitions: "<<num_source_partitions<<endl;
		
		for(unsigned int source_partition=0; source_partition<num_source_partitions; source_partition+=1){
			sweepparams_t sweepparams = getsweepparams(currentLOP, source_partition);
	
			for(unsigned int t=0; t<kvdram[_sourcestatsmarker].size(); t++){
				keyvalue_t keyvalue = kvdram[_sourcestatsmarker][t];
				
				unsigned int partition = getpartition(keyvalue, currentLOP, sweepparams.upperlimit, BATCH_RANGE_POW);
				kvdram[_deststatsmarker + partition].push_back(keyvalue);
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("actsproc_sw.partitionit. ERROR 7.", _deststatsmarker + partition, TOTALNUMPARTITIONS, NAp, NAp, NAp); 
				#endif 
			}
			
			_sourcestatsmarker += 1;
			_deststatsmarker += NUM_PARTITIONS;
		}
	}
	
	*sourcestatsmarker = _sourcestatsmarker;
	*deststatsmarker = _deststatsmarker;
	return;
}

void actsproc_sw::reduceit(vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], value_t * vdatas, vector<vertex_t> &actvvs_nextit, unsigned int currentLOP, unsigned int GraphAlgo, unsigned int GraphIter, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"reduceit called."<<endl;
	#endif
	unsigned int _sourcestatsmarker = *sourcestatsmarker;
	unsigned int _deststatsmarker = *deststatsmarker;
	// cout<<"reduceit: _sourcestatsmarker: "<<_sourcestatsmarker<<", _deststatsmarker: "<<_deststatsmarker<<endl;
	
	unsigned int num_source_partitions = get_num_source_partitions(currentLOP);
	// cout<<"reduceit: currentLOP: "<<currentLOP<<", num_source_partitions: "<<num_source_partitions<<endl;
	
	for(unsigned int source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		sweepparams_t sweepparams = getsweepparams(currentLOP, source_partition);

		for(unsigned int t=0; t<kvdram[_sourcestatsmarker].size(); t++){
			keyvalue_t keyvalue = kvdram[_sourcestatsmarker][t];
			
			unsigned int dstvid = keyvalue.key;
			value_t vprop = vdatas[dstvid];
			value_t vtemp = reducefunc(vprop, keyvalue.value, GraphIter, GraphAlgo);
			vdatas[dstvid] = vtemp;
			if(vtemp != vprop){ actvvs_nextit.push_back(dstvid); } 
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"reduceit: destvid: "<<keyvalue.key<<", vtemp: "<<vtemp<<endl;
			#endif 
		}
		
		_sourcestatsmarker += 1;
	}
	
	*sourcestatsmarker = _sourcestatsmarker;
	*deststatsmarker = _deststatsmarker;
	return;
}

void actsproc_sw::topkernelproc(edge_type * edges, edge_t * vptrs, value_t * vdatas, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int GraphIter){
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 1;
	
	for(unsigned int currentLOP=1; currentLOP<1+3; currentLOP++){
		if(currentLOP == 1){ processit(edges, vptrs, vdatas, actvvs, kvdram, currentLOP, GraphAlgo, &sourcestatsmarker, &deststatsmarker); }
		else if(currentLOP > 1 && currentLOP <= 2) { partitionit(kvdram, currentLOP, GraphAlgo, &sourcestatsmarker, &deststatsmarker); }
		else { reduceit(kvdram, vdatas, actvvs_nextit, currentLOP, GraphAlgo, GraphIter, &sourcestatsmarker, &deststatsmarker); }
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		printstats(kvdram);
		#endif
	}
	return;
}









