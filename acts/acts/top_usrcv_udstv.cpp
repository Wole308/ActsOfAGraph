#include "top_usrcv_udstv.h"
using namespace std;

#ifdef SW
top_usrcv_udstv::top_usrcv_udstv(mydebug * _mydebugobj){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(_mydebugobj); 
	processedges_splitdstvxsobj = new processedges_splitdstvxs(_mydebugobj);
	partitionupdatesobj = new partitionupdates(_mydebugobj);
	reduceupdatesobj = new reduceupdates(_mydebugobj);
	mem_access_splitdstvxsobj = new mem_access_splitdstvxs(_mydebugobj);
	actsobj = new acts(_mydebugobj);
	mydebugobj = _mydebugobj;
}
top_usrcv_udstv::~top_usrcv_udstv(){}
#endif

void acts_all::processit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,							
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif

	sweepparams_t sweepparams;
	
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2; // globalparamsK.SIZEKVS2_REDUCEPARTITION
	buffer_type vmaskbuffersz_kvs = (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512; // globalparamsK.SIZEKVS_VMASKBUFFER
	
	if(globalposition.source_partition == globalposition.first_source_partition){ UTIL_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0); } // CRITICAL NEWCHANGE.
	sweepparams = UTIL_getsweepparams(globalparamsK, globalposition.currentLOP, 0);
	travstate_t avtravstate;
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	if(globalposition.source_partition == globalposition.first_source_partition){ MEMACCESS_SPL_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); } // CRITICAL NEWCHANGE.
	if(globalposition.source_partition == globalposition.first_source_partition){ UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0); } // CRITICAL NEWCHANGE.

	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	
	batch_type voffset_kvs = globalposition.source_partition * reducebuffersz * FETFACTOR;
	if(voffset_kvs >= avtravstate.end_kvs){ return; } // continue; }
	if(GraphAlgo != PAGERANK) { if(vmask_p[globalposition.source_partition] == 0){ return; }} // vmaskoffset_kvs += vmaskbuffersz_kvs; continue; }} // CRITICAL CHECKME.
	
	sweepparams.source_partition = globalposition.source_partition;
	avtravstate.i_kvs = voffset_kvs;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print5("### processit:: source_partition", "voffset", "vbegin", "vend", "vskip", globalposition.source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
	#endif
	
	vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
	vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
	vertex_t endsrcvid = beginsrcvid + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR);
	if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
	if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
	unsigned int SKIP_KVS = reducebuffersz * FETFACTOR; // globalparamsK.SIZEKVS2_REDUCEPARTITION
	
	tuple_t tup = MEMACCESS_SPL_getvptrs_opt( kvdram, vptrbaseoffset_kvs, voffset_kvs * VECTOR2_SIZE, (voffset_kvs + SKIP_KVS) * VECTOR2_SIZE, edgebankID); // CRITICAL NEWCHANGE.
	keyy_t beginvptr = tup.A;
	keyy_t endvptr = tup.B; 
	
	if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
	
	keyy_t localbeginvptr = beginvptr;
	keyy_t localendvptr = endvptr;
	keyy_t numedges = localendvptr - localbeginvptr + 1*VECTOR_SIZE; // CRITICAL NEWCHANGE.
	if(localbeginvptr == localendvptr){ numedges = 0; }
	keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
	keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
	
	#ifdef _DEBUGMODE_CHECKS2
	if(localendvptr < localbeginvptr){ cout<<"processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<", voffset_kvs: "<<voffset_kvs<<endl; exit(EXIT_FAILURE); }
	if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"[index: "<<globalposition.source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"]"<<endl;
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	if(localendvptr < localbeginvptr){ cout<<"ERROR: localendvptr < localbeginvptr. EXITING..."<<endl; exit(EXIT_FAILURE); }
	actsutilityobj->clearallstats();
	#endif
	
	travstate_t etravstate;
	etravstate.begin_kvs = localbeginvptr_kvs;
	etravstate.size_kvs = numedges_kvs;
	etravstate.end_kvs = etravstate.begin_kvs + etravstate.size_kvs;
	etravstate.skip_kvs = SRCBUFFER_SIZE;
	etravstate.i_kvs = etravstate.begin_kvs;
	etravstate.i2 = voffset_kvs * VECTOR2_SIZE;
	etravstate.i2_kvs = voffset_kvs;
	bool_type resetenv = ON;
	bool_type flush = ON;
	if(voffset_kvs == avtravstate.begin_kvs){ resetenv = ON; } else { resetenv = OFF; }
	if((voffset_kvs + (reducebuffersz * FETFACTOR)) >= avtravstate.end_kvs){ flush = ON; } else { flush = OFF; }
	if(GraphAlgo != PAGERANK){ resetenv = ON; flush = ON; } // CRITICAL NEWCHANGE.

	#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
ACTS_actit
	#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
ACTS_priorit	
	#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
ACTS_tradit
	#else 
ACTS_tradit
	#endif 
	(
		ON, PROCESSMODE,
 kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, globalstatsbuffer, 
		globalparamsK, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE,
		resetenv, flush, edgebankID);
	
MEMACCESS_SPL_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + globalposition.deststatsmarker, globalparamsK); // CRITICAL OPTIMIZEME. should be called only once
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

void acts_all::partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], globalparams_t globalparams, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no partitionit for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nusrcv_nudstv::UTIL_getglobalparams:: printing global parameters", globalparams);
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	#ifdef _DEBUGMODE_STATS
	unsigned int edges_count = 0;
	unsigned int edgesdstv_sum = 0;
	#endif
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	batch_type sourcestatsmarker = 1;
	batch_type deststatsmarker = 1 + NUM_PARTITIONS;
	config_t config;
	sweepparams_t sweepparams;
	travstate_t actvvstravstate; actvvstravstate.i=0; actvvstravstate.i_kvs=0;
	
	PARTITIONIT_MAINLOOP1: for(step_type currentLOP=globalparams.ACTSPARAMS_BEGINLOP + 1; currentLOP<globalparams.ACTSPARAMS_BEGINLOP + 1 + (globalparams.ACTSPARAMS_NUMLOPS-2); currentLOP++){ // REMOVEME.
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numllops avg=analysis_numllops	
	
		batch_type num_source_partitions = UTIL_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions
			// cout<<"partitionit: source_partition "<<source_partition<<" ... "<<endl;

UTIL_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = UTIL_getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t ptravstate = UTIL_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
			
			// collect stats
MEMACCESS_SPL_readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY) // CRITICAL REMOVEME.
ACTS_actit
			#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
ACTS_priorit	
			#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
ACTS_tradit
			#else 
ACTS_tradit
			#endif
			(config.enablepartition, PARTITIONMODE,
 kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, globalstatsbuffer, // CRITICAL FIXME.
					globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp);
					
MEMACCESS_SPL_saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
			
			if(currentLOP > 0){
				sourcestatsmarker += 1;
				deststatsmarker += NUM_PARTITIONS;
			}
			
			#ifdef _DEBUGMODE_CHECKS
			if(config.enablereduce == ON){ actsutilityobj->printpartitionresult2(ON, kvdram, globalstatsbuffer, sweepparams); }
			#endif
			#ifdef _DEBUGMODE_STATS
			if(config.enablereduce == ON){ edges_count = actsutilityobj->globalstats_getcountvalidkvsreduced(); edgesdstv_sum = actsutilityobj->globalstats_getreducevar1(); }
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printglobalvars();
			actsutilityobj->clearglobalvars();
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		#endif 
		#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
		actsutilityobj->clearglobalvars();
		#endif
	}
	return;
}

void acts_all::reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID){	
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	config_t config;
	sweepparams_t sweepparams;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH;
	#else 
	step_type currentLOP = globalparams.ACTSPARAMS_TREEDEPTH + 1;
	#endif 
	
	sweepparams = UTIL_getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = UTIL_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
ACTS_actit
	#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
ACTS_priorit	
	#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
ACTS_tradit
	#else 
ACTS_tradit
	#endif
	(config.enablereduce, REDUCEMODE,
 kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, globalstatsbuffer, // CRITICAL FIXME.
			globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp); // REMOVEME.
	return;
}

void acts_all::dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	if(en_process == ON){ processit( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, globalstatsbuffer, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, edgebankID); } 
	if(en_partition == ON){ partitionit( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE,  vmask_subp, globalparamsK, NAp); } 
	if(en_reduce == ON){ reduceit( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, sourcestatsmarker, source_partition, globalparamsK, NAp); } 
	return;
}

void acts_all::dispatch_reduce( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unit1_type vmaskREAD[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmask_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,	
					unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_loop1 = 1;
	analysis_type analysis_treedepth = TREE_DEPTH;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no dispatch_reduce for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->clearglobalvars();
	#endif
	
	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS]; // UNUSED.
	globalposition_t globalposition; // UNUSED.
	
	unsigned int vreadoffset_kvs2 = 0;
	unsigned int vmask_offset_kvs = 0;
	unsigned int vmaskp_offset_kvs = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	#else 
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH + 1;
	#endif 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type num_source_partitions = UTIL_get_num_source_partitions(currentLOP);
	#else 
	batch_type num_source_partitions = NUM_PARTITIONS;	
	#endif

	bool_type enablereduce = ON;
	
	DISPATCHREDUCE_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### dispatch_reduce:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, num_source_partitions); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = UTIL_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
MEMACCESS_SPL_readvdata(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK);
		#ifdef CONFIG_SEPERATEVMASKFROMVDATA
UTIL_reset(vmaskWRITE);
		#endif 
		
		// reduce
		dispatch(OFF, OFF, enablereduce,  kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, globalstatsbuffer, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, globalposition, v_chunkids, v_chunkid, NAp);
		
		// writeback vertices
MEMACCESS_SPL_savevdataandmasks(enablereduce, kvdram, vbuffer, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, vreadoffset_kvs2, 0, globalparamsK.SIZEKVS2_REDUCEPARTITION, globalparamsK);
		#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_savemasks(enablereduce, kvdram, vmaskWRITE, globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK, vmask_offset_kvs, globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + vmaskp_offset_kvs, globalparamsK); // NEW
		#endif 
		
		sourcestatsmarker += 1;
		vreadoffset_kvs2 += globalparamsK.SIZEKVS2_REDUCEPARTITION;
		vmask_offset_kvs += globalparamsK.SIZEKVS_VMASKBUFFER;
		#ifdef CONFIG_SPLIT_DESTVTXS
		vmaskp_offset_kvs += NUM_PEs;
		#else 
		vmaskp_offset_kvs += 1;	
		#endif
	}
	return;
} 

// top
void acts_all::topkernelproc_embedded(unsigned int en_process, unsigned int en_partition, unsigned int en_reduce,  uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], unit1_type vmask_subp[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskREAD[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmaskWRITE[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], globalposition_t globalposition){

	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR: 1 ACTS IN 1 COMPUTEUNITS) Launched... size: "<<GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ]; // NOT USED. CALLED IN UPPER-FUNCTION
	globalparams_t globalparamsK;
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	globalparams_t _globalparamsE;
	
	globalparamsK = UTIL_getglobalparams(kvdram); // CRITICAL OPTIMIZEME. MOVETOBASE?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	_globalparamsE = globalparamsEs[globalposition.edgebankID];
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid;
	for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // OPTIMIZEME? IS THIS OPTIMIZED
	#endif 
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	if(GraphAlgo != PAGERANK){ if(globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==globalposition.first_source_partition){ UTIL_resetkvstatvalues(kvdram, globalparamsK); }}	// CRITICAL FIXME. NEWCHANGE. DO FOR ALL.
	
	// process & partition
	#ifdef CONFIG_ENABLEPROCESSMODULE
	if(globalparamsK.ENABLE_PROCESSCOMMAND == ON && en_process == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: processing instance ... "<<endl;
		#endif
		dispatch(globalposition.EN_PROCESS, OFF, OFF,  kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITION_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, globalposition.edgebankID); // PARTITION_CHKPT[0], 0, 0);
	}
	#endif
	
	// partition
	#ifdef CONFIG_ENABLEPARTITIONMODULE
	if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON && en_partition == ON){
		#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: partitioning instance ... "<<endl;
		#endif
		dispatch(OFF, globalposition.EN_PARTITION, OFF,  kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, globalstatsbuffer, NAp, NAp, _globalparamsE, globalparamsK, globalposition, PARTITION_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, NAp); // PARTITION_CHKPT[0], 0, NAp);
	}
	#endif
	
	// reduce & partition
	#if defined(CONFIG_ENABLEREDUCEMODULE)
	if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON && en_reduce == ON){ 
		#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
		cout<<"topkernelproc: reducing instance ... "<<endl;
		#endif
		dispatch_reduce( kvdram, sourcebuffer, vbuffer, vmaskREAD, vmaskWRITE, vmask_subp, vmask_p, _globalparamsE, globalparamsK, PARTITION_CHKPT[globalposition.edgebankID], globalposition.v_chunkid, NAp);
	}
	#endif
	
	if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){
UTIL_increment_graphiteration(kvdram, globalparamsK);
		// #ifdef _WIDEWORD
		// kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsK.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
		// #else
		// kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsK.ALGORITHMINFO_GRAPHITERATIONID + 1;
		// #endif 
	}
	// UTIL_increment_graphiteration(kvdram, globalparamsK);
	
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void acts_all::topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[1];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[1];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask1(vdram, vmask0_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL1_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL1_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL1_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL1_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL1_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL1_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 1; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks1(enable_loadmasks, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask1(enable_loadmasks, vmask0, vmask0_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate1vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate1vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
	
						/*  */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge1andsavevdata(ON, vdram, vbuffer0, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge1andsavevdata(ON, vdram, vbuffer0, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs1(kvdram0, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[2];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[2];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask2(vdram, vmask0_p,vmask1_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL2_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL2_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL2_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL2_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL2_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL2_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 2; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks2(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask2(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate2vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate2vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
	
						/*  */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge2andsavevdata(ON, vdram, vbuffer0,vbuffer1, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge2andsavevdata(ON, vdram, vbuffer0,vbuffer1, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs2(kvdram0,kvdram1, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[3];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[3];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask3(vdram, vmask0_p,vmask1_p,vmask2_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL3_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL3_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL3_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL3_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL3_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL3_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 3; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks3(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask3(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate3vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate3vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge3andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge3andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs3(kvdram0,kvdram1,kvdram2, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem8
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[4];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[4];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask4(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL4_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL4_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL4_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL4_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL4_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL4_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 4; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks4(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask4(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate4vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate4vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge4andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge4andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs4(kvdram0,kvdram1,kvdram2,kvdram3, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem10
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem5
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	unit1_type vmaskREAD4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD4
	unit1_type vmaskWRITE4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	unit1_type vmask4_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask4_subp
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[5];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[5];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3);globalparamsKs[4] = UTIL_getglobalparams(kvdram4); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask5(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL5_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL5_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL5_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL5_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL5_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL5_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[4] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram4, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 5; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer4[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD4[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks5(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3,vmaskREAD4, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask5(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp,vmask4_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate5vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate5vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram4, vbuffer4, vmask4_p, vmask4_subp, vmaskREAD4, vmaskWRITE4, globalstatsbuffer4, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge5andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge5andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs5(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem12
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	unit1_type vmaskREAD4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD4
	unit1_type vmaskWRITE4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	unit1_type vmask4_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask4_subp
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	unit1_type vmaskREAD5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD5
	unit1_type vmaskWRITE5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	unit1_type vmask5_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask5_subp
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[6];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[6];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3);globalparamsKs[4] = UTIL_getglobalparams(kvdram4);globalparamsKs[5] = UTIL_getglobalparams(kvdram5); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask6(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL6_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL6_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL6_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL6_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL6_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL6_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[4] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram4, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[5] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram5, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 6; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer4[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer5[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD4[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD5[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks6(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3,vmaskREAD4,vmaskREAD5, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask6(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp,vmask4_subp,vmask5_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate6vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate6vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram4, vbuffer4, vmask4_p, vmask4_subp, vmaskREAD4, vmaskWRITE4, globalstatsbuffer4, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram5, vbuffer5, vmask5_p, vmask5_subp, vmaskREAD5, vmaskWRITE5, globalstatsbuffer5, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge6andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge6andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs6(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP7(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem12
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem14
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem7
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	unit1_type vmaskREAD4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD4
	unit1_type vmaskWRITE4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	unit1_type vmask4_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask4_subp
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	unit1_type vmaskREAD5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD5
	unit1_type vmaskWRITE5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	unit1_type vmask5_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask5_subp
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	unit1_type vmaskREAD6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD6
	unit1_type vmaskWRITE6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	unit1_type vmask6_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask6_subp
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[7];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[7];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3);globalparamsKs[4] = UTIL_getglobalparams(kvdram4);globalparamsKs[5] = UTIL_getglobalparams(kvdram5);globalparamsKs[6] = UTIL_getglobalparams(kvdram6); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask7(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL7_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL7_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL7_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL7_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL7_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL7_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[4] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram4, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[5] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram5, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[6] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram6, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 7; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer4[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer5[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer6[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD4[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD5[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD6[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks7(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3,vmaskREAD4,vmaskREAD5,vmaskREAD6, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask7(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp,vmask4_subp,vmask5_subp,vmask6_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate7vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate7vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram4, vbuffer4, vmask4_p, vmask4_subp, vmaskREAD4, vmaskWRITE4, globalstatsbuffer4, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram5, vbuffer5, vmask5_p, vmask5_subp, vmaskREAD5, vmaskWRITE5, globalstatsbuffer5, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram6, vbuffer6, vmask6_p, vmask6_subp, vmaskREAD6, vmaskWRITE6, globalstatsbuffer6, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge7andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge7andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs7(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP8(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem12
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
	#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem14
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem16
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem8
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	unit1_type vmaskREAD4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD4
	unit1_type vmaskWRITE4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	unit1_type vmask4_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask4_subp
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	unit1_type vmaskREAD5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD5
	unit1_type vmaskWRITE5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	unit1_type vmask5_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask5_subp
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	unit1_type vmaskREAD6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD6
	unit1_type vmaskWRITE6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	unit1_type vmask6_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask6_subp
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	unit1_type vmaskREAD7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD7
	unit1_type vmaskWRITE7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	unit1_type vmask7_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask7_subp
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[8];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[8];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3);globalparamsKs[4] = UTIL_getglobalparams(kvdram4);globalparamsKs[5] = UTIL_getglobalparams(kvdram5);globalparamsKs[6] = UTIL_getglobalparams(kvdram6);globalparamsKs[7] = UTIL_getglobalparams(kvdram7); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask8(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL8_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL8_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL8_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL8_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL8_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL8_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[4] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram4, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[5] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram5, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[6] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram6, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[7] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram7, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 8; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer4[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer5[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer6[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer7[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD4[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD5[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD6[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD7[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks8(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3,vmaskREAD4,vmaskREAD5,vmaskREAD6,vmaskREAD7, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask8(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp,vmask4_subp,vmask5_subp,vmask6_subp,vmask7_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate8vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate8vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram4, vbuffer4, vmask4_p, vmask4_subp, vmaskREAD4, vmaskWRITE4, globalstatsbuffer4, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram5, vbuffer5, vmask5_p, vmask5_subp, vmaskREAD5, vmaskWRITE5, globalstatsbuffer5, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram6, vbuffer6, vmask6_p, vmask6_subp, vmaskREAD6, vmaskWRITE6, globalstatsbuffer6, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram7, vbuffer7, vmask7_p, vmask7_subp, vmaskREAD7, vmaskWRITE7, globalstatsbuffer7, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge8andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge8andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs8(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP9(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem12
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
	#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem14
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
	#pragma HLS INTERFACE m_axi port = edges80 offset = slave bundle = gmem16
	#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem18
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem9
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = edges80
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	unit1_type vmaskREAD4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD4
	unit1_type vmaskWRITE4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	unit1_type vmask4_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask4_subp
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	unit1_type vmaskREAD5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD5
	unit1_type vmaskWRITE5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	unit1_type vmask5_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask5_subp
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	unit1_type vmaskREAD6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD6
	unit1_type vmaskWRITE6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	unit1_type vmask6_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask6_subp
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	unit1_type vmaskREAD7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD7
	unit1_type vmaskWRITE7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	unit1_type vmask7_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask7_subp
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	unit1_type vmaskREAD8[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD8
	unit1_type vmaskWRITE8[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE8
	uint32_type vmask8_p[BLOCKRAM_SIZE];
	unit1_type vmask8_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask8_subp
	keyvalue_t globalstatsbuffer8[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[9];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[9];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3);globalparamsKs[4] = UTIL_getglobalparams(kvdram4);globalparamsKs[5] = UTIL_getglobalparams(kvdram5);globalparamsKs[6] = UTIL_getglobalparams(kvdram6);globalparamsKs[7] = UTIL_getglobalparams(kvdram7);globalparamsKs[8] = UTIL_getglobalparams(kvdram8); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask9(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p,vmask8_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL9_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL9_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL9_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL9_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL9_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL9_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[4] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram4, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[5] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram5, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[6] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram6, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[7] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram7, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[8] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram8, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 9; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer4[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer5[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer6[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer7[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer8[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD4[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD5[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD6[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD7[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD8[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks9(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3,vmaskREAD4,vmaskREAD5,vmaskREAD6,vmaskREAD7,vmaskREAD8, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask9(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp,vmask4_subp,vmask5_subp,vmask6_subp,vmask7_subp,vmask8_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate9vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate9vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram4, vbuffer4, vmask4_p, vmask4_subp, vmaskREAD4, vmaskWRITE4, globalstatsbuffer4, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram5, vbuffer5, vmask5_p, vmask5_subp, vmaskREAD5, vmaskWRITE5, globalstatsbuffer5, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram6, vbuffer6, vmask6_p, vmask6_subp, vmaskREAD6, vmaskWRITE6, globalstatsbuffer6, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram7, vbuffer7, vmask7_p, vmask7_subp, vmaskREAD7, vmaskWRITE7, globalstatsbuffer7, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram8, vbuffer8, vmask8_p, vmask8_subp, vmaskREAD8, vmaskWRITE8, globalstatsbuffer8, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge9andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge9andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs9(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP10(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem12
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
	#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem14
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
	#pragma HLS INTERFACE m_axi port = edges80 offset = slave bundle = gmem16
	#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
	#pragma HLS INTERFACE m_axi port = edges90 offset = slave bundle = gmem18
	#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem19
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem20
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem10
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = edges90
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	unit1_type vmaskREAD4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD4
	unit1_type vmaskWRITE4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	unit1_type vmask4_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask4_subp
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	unit1_type vmaskREAD5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD5
	unit1_type vmaskWRITE5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	unit1_type vmask5_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask5_subp
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	unit1_type vmaskREAD6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD6
	unit1_type vmaskWRITE6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	unit1_type vmask6_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask6_subp
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	unit1_type vmaskREAD7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD7
	unit1_type vmaskWRITE7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	unit1_type vmask7_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask7_subp
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	unit1_type vmaskREAD8[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD8
	unit1_type vmaskWRITE8[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE8
	uint32_type vmask8_p[BLOCKRAM_SIZE];
	unit1_type vmask8_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask8_subp
	keyvalue_t globalstatsbuffer8[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	unit1_type vmaskREAD9[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD9
	unit1_type vmaskWRITE9[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE9
	uint32_type vmask9_p[BLOCKRAM_SIZE];
	unit1_type vmask9_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask9_subp
	keyvalue_t globalstatsbuffer9[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[10];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[10];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3);globalparamsKs[4] = UTIL_getglobalparams(kvdram4);globalparamsKs[5] = UTIL_getglobalparams(kvdram5);globalparamsKs[6] = UTIL_getglobalparams(kvdram6);globalparamsKs[7] = UTIL_getglobalparams(kvdram7);globalparamsKs[8] = UTIL_getglobalparams(kvdram8);globalparamsKs[9] = UTIL_getglobalparams(kvdram9); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask10(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p,vmask8_p,vmask9_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL10_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL10_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL10_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL10_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL10_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL10_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[4] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram4, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[5] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram5, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[6] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram6, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[7] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram7, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[8] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram8, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[9] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram9, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 10; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer4[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer5[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer6[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer7[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer8[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer9[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD4[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD5[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD6[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD7[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD8[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD9[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks10(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3,vmaskREAD4,vmaskREAD5,vmaskREAD6,vmaskREAD7,vmaskREAD8,vmaskREAD9, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask10(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp,vmask4_subp,vmask5_subp,vmask6_subp,vmask7_subp,vmask8_subp,vmask9_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate10vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate10vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram4, vbuffer4, vmask4_p, vmask4_subp, vmaskREAD4, vmaskWRITE4, globalstatsbuffer4, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram5, vbuffer5, vmask5_p, vmask5_subp, vmaskREAD5, vmaskWRITE5, globalstatsbuffer5, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram6, vbuffer6, vmask6_p, vmask6_subp, vmaskREAD6, vmaskWRITE6, globalstatsbuffer6, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram7, vbuffer7, vmask7_p, vmask7_subp, vmaskREAD7, vmaskWRITE7, globalstatsbuffer7, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram8, vbuffer8, vmask8_p, vmask8_subp, vmaskREAD8, vmaskWRITE8, globalstatsbuffer8, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram9, vbuffer9, vmask9_p, vmask9_subp, vmaskREAD9, vmaskWRITE9, globalstatsbuffer9, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge10andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge10andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs10(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP11(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * kvdram10,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem12
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
	#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem14
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
	#pragma HLS INTERFACE m_axi port = edges80 offset = slave bundle = gmem16
	#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
	#pragma HLS INTERFACE m_axi port = edges90 offset = slave bundle = gmem18
	#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem19
	#pragma HLS INTERFACE m_axi port = edges100 offset = slave bundle = gmem20
	#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem21
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem22
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem11
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
#pragma HLS INTERFACE s_axilite port = edges100 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = edges90
#pragma HLS DATA_PACK variable = edges100
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	unit1_type vmaskREAD4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD4
	unit1_type vmaskWRITE4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	unit1_type vmask4_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask4_subp
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	unit1_type vmaskREAD5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD5
	unit1_type vmaskWRITE5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	unit1_type vmask5_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask5_subp
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	unit1_type vmaskREAD6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD6
	unit1_type vmaskWRITE6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	unit1_type vmask6_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask6_subp
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	unit1_type vmaskREAD7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD7
	unit1_type vmaskWRITE7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	unit1_type vmask7_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask7_subp
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	unit1_type vmaskREAD8[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD8
	unit1_type vmaskWRITE8[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE8
	uint32_type vmask8_p[BLOCKRAM_SIZE];
	unit1_type vmask8_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask8_subp
	keyvalue_t globalstatsbuffer8[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	unit1_type vmaskREAD9[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD9
	unit1_type vmaskWRITE9[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE9
	uint32_type vmask9_p[BLOCKRAM_SIZE];
	unit1_type vmask9_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask9_subp
	keyvalue_t globalstatsbuffer9[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	unit1_type vmaskREAD10[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD10
	unit1_type vmaskWRITE10[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE10
	uint32_type vmask10_p[BLOCKRAM_SIZE];
	unit1_type vmask10_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask10_subp
	keyvalue_t globalstatsbuffer10[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[11];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[11];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3);globalparamsKs[4] = UTIL_getglobalparams(kvdram4);globalparamsKs[5] = UTIL_getglobalparams(kvdram5);globalparamsKs[6] = UTIL_getglobalparams(kvdram6);globalparamsKs[7] = UTIL_getglobalparams(kvdram7);globalparamsKs[8] = UTIL_getglobalparams(kvdram8);globalparamsKs[9] = UTIL_getglobalparams(kvdram9);globalparamsKs[10] = UTIL_getglobalparams(kvdram10); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask11(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p,vmask8_p,vmask9_p,vmask10_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL11_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL11_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL11_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL11_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL11_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL11_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[4] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram4, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[5] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram5, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[6] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram6, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[7] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram7, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[8] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram8, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[9] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram9, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[10] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram10, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// 							// 							// for(unsigned int i = 0; i < 11; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer4[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer5[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer6[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer7[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer8[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer9[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer10[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD4[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD5[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD6[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD7[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD8[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD9[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD10[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks11(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3,vmaskREAD4,vmaskREAD5,vmaskREAD6,vmaskREAD7,vmaskREAD8,vmaskREAD9,vmaskREAD10, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask11(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp,vmask4_subp,vmask5_subp,vmask6_subp,vmask7_subp,vmask8_subp,vmask9_subp,vmask10_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate11vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate11vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram4, vbuffer4, vmask4_p, vmask4_subp, vmaskREAD4, vmaskWRITE4, globalstatsbuffer4, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram5, vbuffer5, vmask5_p, vmask5_subp, vmaskREAD5, vmaskWRITE5, globalstatsbuffer5, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram6, vbuffer6, vmask6_p, vmask6_subp, vmaskREAD6, vmaskWRITE6, globalstatsbuffer6, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram7, vbuffer7, vmask7_p, vmask7_subp, vmaskREAD7, vmaskWRITE7, globalstatsbuffer7, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram8, vbuffer8, vmask8_p, vmask8_subp, vmaskREAD8, vmaskWRITE8, globalstatsbuffer8, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram9, vbuffer9, vmask9_p, vmask9_subp, vmaskREAD9, vmaskWRITE9, globalstatsbuffer9, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram10, vbuffer10, vmask10_p, vmask10_subp, vmaskREAD10, vmaskWRITE10, globalstatsbuffer10, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge11andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge11andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs11(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
extern "C" {
void acts_all::topkernelP12(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * kvdram6,
	uint512_dt * kvdram7,
	uint512_dt * kvdram8,
	uint512_dt * kvdram9,
	uint512_dt * kvdram10,
	uint512_dt * kvdram11,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#pragma HLS INTERFACE m_axi port = edges00 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = edges20 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = edges30 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = edges40 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = edges50 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = edges60 offset = slave bundle = gmem12
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
	#pragma HLS INTERFACE m_axi port = edges70 offset = slave bundle = gmem14
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
	#pragma HLS INTERFACE m_axi port = edges80 offset = slave bundle = gmem16
	#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
	#pragma HLS INTERFACE m_axi port = edges90 offset = slave bundle = gmem18
	#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem19
	#pragma HLS INTERFACE m_axi port = edges100 offset = slave bundle = gmem20
	#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem21
	#pragma HLS INTERFACE m_axi port = edges110 offset = slave bundle = gmem22
	#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem23
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem24
#else 
	#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem12
#endif 
//

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges00 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges20 bundle = control
#pragma HLS INTERFACE s_axilite port = edges30 bundle = control
#pragma HLS INTERFACE s_axilite port = edges40 bundle = control
#pragma HLS INTERFACE s_axilite port = edges50 bundle = control
#pragma HLS INTERFACE s_axilite port = edges60 bundle = control
#pragma HLS INTERFACE s_axilite port = edges70 bundle = control
#pragma HLS INTERFACE s_axilite port = edges80 bundle = control
#pragma HLS INTERFACE s_axilite port = edges90 bundle = control
#pragma HLS INTERFACE s_axilite port = edges100 bundle = control
#pragma HLS INTERFACE s_axilite port = edges110 bundle = control
	
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram11 bundle = control
	

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges00
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges20
#pragma HLS DATA_PACK variable = edges30
#pragma HLS DATA_PACK variable = edges40
#pragma HLS DATA_PACK variable = edges50
#pragma HLS DATA_PACK variable = edges60
#pragma HLS DATA_PACK variable = edges70
#pragma HLS DATA_PACK variable = edges80
#pragma HLS DATA_PACK variable = edges90
#pragma HLS DATA_PACK variable = edges100
#pragma HLS DATA_PACK variable = edges110
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = kvdram11
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	unit1_type vmaskREAD0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD0
	unit1_type vmaskWRITE0[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE0
	uint32_type vmask0_p[BLOCKRAM_SIZE];
	unit1_type vmask0_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask0_subp
	keyvalue_t globalstatsbuffer0[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	unit1_type vmaskREAD1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD1
	unit1_type vmaskWRITE1[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE1
	uint32_type vmask1_p[BLOCKRAM_SIZE];
	unit1_type vmask1_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask1_subp
	keyvalue_t globalstatsbuffer1[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	unit1_type vmaskREAD2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD2
	unit1_type vmaskWRITE2[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE2
	uint32_type vmask2_p[BLOCKRAM_SIZE];
	unit1_type vmask2_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask2_subp
	keyvalue_t globalstatsbuffer2[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	unit1_type vmaskREAD3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD3
	unit1_type vmaskWRITE3[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE3
	uint32_type vmask3_p[BLOCKRAM_SIZE];
	unit1_type vmask3_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask3_subp
	keyvalue_t globalstatsbuffer3[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	unit1_type vmaskREAD4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD4
	unit1_type vmaskWRITE4[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE4
	uint32_type vmask4_p[BLOCKRAM_SIZE];
	unit1_type vmask4_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask4_subp
	keyvalue_t globalstatsbuffer4[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	unit1_type vmaskREAD5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD5
	unit1_type vmaskWRITE5[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE5
	uint32_type vmask5_p[BLOCKRAM_SIZE];
	unit1_type vmask5_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask5_subp
	keyvalue_t globalstatsbuffer5[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	unit1_type vmaskREAD6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD6
	unit1_type vmaskWRITE6[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE6
	uint32_type vmask6_p[BLOCKRAM_SIZE];
	unit1_type vmask6_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask6_subp
	keyvalue_t globalstatsbuffer6[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	unit1_type vmaskREAD7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD7
	unit1_type vmaskWRITE7[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE7
	uint32_type vmask7_p[BLOCKRAM_SIZE];
	unit1_type vmask7_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask7_subp
	keyvalue_t globalstatsbuffer7[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	unit1_type vmaskREAD8[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD8
	unit1_type vmaskWRITE8[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE8
	uint32_type vmask8_p[BLOCKRAM_SIZE];
	unit1_type vmask8_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask8_subp
	keyvalue_t globalstatsbuffer8[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	unit1_type vmaskREAD9[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD9
	unit1_type vmaskWRITE9[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE9
	uint32_type vmask9_p[BLOCKRAM_SIZE];
	unit1_type vmask9_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask9_subp
	keyvalue_t globalstatsbuffer9[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	unit1_type vmaskREAD10[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD10
	unit1_type vmaskWRITE10[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE10
	uint32_type vmask10_p[BLOCKRAM_SIZE];
	unit1_type vmask10_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask10_subp
	keyvalue_t globalstatsbuffer10[MAX_NUM_PARTITIONS];
	keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11
	unit1_type vmaskREAD11[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskREAD11
	unit1_type vmaskWRITE11[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmaskWRITE11
	uint32_type vmask11_p[BLOCKRAM_SIZE];
	unit1_type vmask11_subp[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vmask11_subp
	keyvalue_t globalstatsbuffer11[MAX_NUM_PARTITIONS];
	travstate_t rtravstates[12];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	globalparams_t globalparamsKs[12];
	globalparams_t globalparamsEs[MAX_NUM_EDGE_BANKS];
	
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 0;
	unsigned int reducesourcestatsmarker = 0;
	unsigned int EN_PROCESS = OFF; unsigned int EN_PARTITION = OFF; unsigned int EN_REDUCE = OFF; unsigned int EN_PROCESSANDREDUCE = OFF; unsigned int EN_PROCESSANDPARTITION = OFF;
	globalposition_t globalposition;
	
globalparamsKs[0] = UTIL_getglobalparams(kvdram0);globalparamsKs[1] = UTIL_getglobalparams(kvdram1);globalparamsKs[2] = UTIL_getglobalparams(kvdram2);globalparamsKs[3] = UTIL_getglobalparams(kvdram3);globalparamsKs[4] = UTIL_getglobalparams(kvdram4);globalparamsKs[5] = UTIL_getglobalparams(kvdram5);globalparamsKs[6] = UTIL_getglobalparams(kvdram6);globalparamsKs[7] = UTIL_getglobalparams(kvdram7);globalparamsKs[8] = UTIL_getglobalparams(kvdram8);globalparamsKs[9] = UTIL_getglobalparams(kvdram9);globalparamsKs[10] = UTIL_getglobalparams(kvdram10);globalparamsKs[11] = UTIL_getglobalparams(kvdram11); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	globalparams_t globalparamsK = globalparamsKs[0]; // UTIL_getglobalparams(kvdram0); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsEs[0] = globalparamsK;
	#endif 
	globalparams_t globalparamsV = UTIL_getglobalparams(vdram);
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){ // CRITICAL NEWCHANGE.
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram0[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	buffer_type vmaskbuffersz_kvs = globalparamsK.SIZEKVS_VMASKBUFFER; // (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	batch_type vmaskoffset_kvs = 0; 
	unsigned int vreadoffset_kvs2 = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK.ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
		reducesourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}
	unsigned int FIRSTLOP = 0;
	unsigned int NUMLOPs = 0;
	unsigned int FIRSTSOURCEPARTITION = 0;
	unsigned int LASTSOURCEPARTITIONS = 0;
	batch_type num_source_partitions = 0; 

	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	if(GraphAlgo != PAGERANK){ MEMACCESS_SPL_readmanypmask12(vdram, vmask0_p,vmask1_p,vmask2_p,vmask3_p,vmask4_p,vmask5_p,vmask6_p,vmask7_p,vmask8_p,vmask9_p,vmask10_p,vmask11_p, globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ UTIL_resetkvstatvalues(vdram, globalparamsV); }
	unsigned int num_edge_banks = NUM_EDGE_BANKS;
	unsigned int it_size; if(num_edge_banks==0){ it_size = 1; } else { it_size = NUM_EDGE_BANKS; }
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(vdram, vbuffer0, globalparamsV);
	#endif
	
	unsigned int num_stages = 3;
	if(globalparamsV.ENABLE_PROCESSCOMMAND == ON){ num_stages = 1; }
	if(globalparamsV.ENABLE_PARTITIONCOMMAND == ON){ num_stages = 2; }
	if(globalparamsV.ENABLE_APPLYUPDATESCOMMAND == ON){ num_stages = 3; }
	
	TOPKERNEL12_BASELOOP1: for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalposition.edgebankID = edgebankID;
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		for(unsigned int i=0; i<globalposition.edgebankID; i++){ globalparamsK.BASEOFFSETKVS_STATSDRAM += ((globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS); } // SHIFT.
		#endif 
		TOPKERNEL12_BASELOOP1B: for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			globalposition.v_chunkid = v_chunkid;
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			globalparamsK.VARS_WORKBATCH = globalposition.v_chunkid; // SHIFT.
			#endif 
			TOPKERNEL12_BASELOOP1C: for(step_type stage=0; stage<num_stages; stage++){
			
				EN_PROCESS = OFF; EN_PARTITION = OFF; EN_REDUCE = OFF; EN_PROCESSANDREDUCE = OFF; EN_PROCESSANDPARTITION = OFF;
				if(stage==0){ EN_PROCESS = ON; EN_PROCESSANDREDUCE = ON; EN_PROCESSANDPARTITION = ON;  } 
				else if(stage==1){ EN_PARTITION = ON; EN_PROCESSANDPARTITION = ON;  } 
				else { EN_REDUCE = ON; EN_PROCESSANDREDUCE = ON; }
				globalposition.EN_PROCESS = EN_PROCESS; globalposition.EN_PARTITION = EN_PARTITION; globalposition.EN_REDUCE = EN_REDUCE; globalposition.EN_PROCESSANDPARTITION = EN_PROCESSANDPARTITION; globalposition.EN_PROCESSANDREDUCE = EN_PROCESSANDREDUCE;
				
				if(stage==0){ sourcestatsmarker = 0; deststatsmarker = 1; } 
				else if(stage==1){ sourcestatsmarker = 1; deststatsmarker = 1 + NUM_PARTITIONS; } 
				else { sourcestatsmarker = reducesourcestatsmarker; deststatsmarker = NAp; }
				
				if(stage==0){ FIRSTLOP = 1; NUMLOPs = 1; } // FIXME. CHANGE FOR ~ENABLERECURSIVEPARTITIONING
				else if(stage==1){ FIRSTLOP = globalparamsK.ACTSPARAMS_BEGINLOP + 1; NUMLOPs = globalparamsK.ACTSPARAMS_NUMLOPS-2;  } 
				else{ FIRSTLOP = globalparamsK.ACTSPARAMS_TREEDEPTH; NUMLOPs = 1; }
				
				vmaskoffset_kvs = 0; 
				vreadoffset_kvs2 = 0;
				unsigned int vreadskipsz_kvs2 = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs; // TOO EXPENSIVE.
				
				TOPKERNEL12_BASELOOP1D: for(step_type currentLOP=FIRSTLOP; currentLOP<(FIRSTLOP + NUMLOPs); currentLOP+=1){
					#ifdef ENABLERECURSIVEPARTITIONING
					if(stage==0){ num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH); }
					else if(stage==1){ num_source_partitions = UTIL_get_num_source_partitions(currentLOP);  }
					else { num_source_partitions = UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);  }
					#else
					NOT IMPLEMENTED.
					#endif
					
					if(stage==0){ FIRSTSOURCEPARTITION = PARTITION_CHKPT[edgebankID][v_chunkid]; LASTSOURCEPARTITIONS = PARTITION_CHKPT[edgebankID][v_chunkid+1]; } 
					else if(stage==1){ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions;  } 
					else{ FIRSTSOURCEPARTITION = 0; LASTSOURCEPARTITIONS = num_source_partitions; }
					
					if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs = PARTITION_CHKPT[edgebankID][v_chunkid] * (vmaskbuffersz_kvs); } // NEWCHANGE.
					batch_type vreadoffset_kvs2 = 0;
					batch_type vdatabaseoffset_kvs = globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsV.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
					
					TOPKERNEL12_BASELOOP1E: for(batch_type source_partition=FIRSTSOURCEPARTITION; source_partition<LASTSOURCEPARTITIONS; source_partition+=1){
						#ifdef _DEBUGMODE_KERNELPRINTS
						actsutilityobj->print3("### TOPKERNEL12_BASELOOP1C:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
						#endif
						
						globalposition.stage = stage; 
						globalposition.laststage = 2; 
						globalposition.currentLOP = currentLOP; 
						globalposition.lastLOP = FIRSTLOP + NUMLOPs - 1; 
						globalposition.source_partition = source_partition;
						globalposition.first_source_partition = FIRSTSOURCEPARTITION;
						globalposition.last_source_partition = LASTSOURCEPARTITIONS - 1;
						globalposition.num_source_partitions = LASTSOURCEPARTITIONS - FIRSTSOURCEPARTITION; // FIXME.
						globalposition.sourcestatsmarker = sourcestatsmarker; 
						globalposition.deststatsmarker = deststatsmarker; 
						if(globalposition.v_chunkid==0 && globalposition.stage==0 && globalposition.currentLOP==1 && globalposition.source_partition==0){ globalposition.first=ON; } else { globalposition.first=OFF; }
						if(globalposition.v_chunkid==globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER-1 && globalposition.stage==globalposition.laststage && globalposition.currentLOP==globalposition.lastLOP && globalposition.source_partition==globalposition.last_source_partition){ globalposition.last=ON; } else { globalposition.last=OFF; }
						
						bool_type enableprocess = ON;
						if(globalposition.EN_PROCESS == ON){
							if(GraphAlgo != PAGERANK){ if(vmask0_p[source_partition] > 0){ enableprocess = ON; } else { enableprocess = OFF; }}
						}
						// exit(EXIT_SUCCESS);
						// bool_type enablereduce = OFF; 
						// unsigned int ntravszs = 0;
						// if(globalposition.EN_REDUCE == ON){
							// 							// 							// rtravstates[0] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram0, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[1] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram1, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[2] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram2, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[3] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram3, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[4] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram4, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[5] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram5, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[6] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram6, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[7] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram7, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[8] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram8, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[9] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram9, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[10] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram10, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// rtravstates[11] = UTIL_gettravstate(globalposition.EN_REDUCE, kvdram11, globalparamsK, currentLOP, sourcestatsmarker); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
							// 							// 							// 							// 							// for(unsigned int i = 0; i < 12; i++){ ntravszs += rtravstates[i].size_kvs; }
							// if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
						// }
						
						// bool_type enable_loadmasks = OFF; 
						// if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_loadmasks = ON; } else { enable_loadmasks = OFF; }
						
						bool_type enable_readandreplicatevdata = OFF; 
						// if((globalposition.EN_PROCESS == ON && enableprocess == ON) || (globalposition.EN_REDUCE == ON && enablereduce == ON)){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						if(globalposition.EN_PROCESS == ON && enableprocess == ON){ enable_readandreplicatevdata = ON; } else { enable_readandreplicatevdata = OFF; }
						
						// read vertices & vmasks
						if(enable_readandreplicatevdata == ON){
							#ifdef CONFIG_SEPERATEVMASKFROMVDATA
MEMACCESS_SPL_readvmaskschunks(ON, vdram, vmaskREAD0, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							#endif 
MEMACCESS_SPL_readvdatachunks(ON, vdram, vbuffer0, vdatabaseoffset_kvs, vreadoffset_kvs2, vreadskipsz_kvs2, globalparamsV);
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer0[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer1[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer2[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer3[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer4[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer5[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer6[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer7[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer8[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer9[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer10[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vbuffer11[v][t] = vbuffer0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD0[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD1[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD2[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD3[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD4[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD5[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD6[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD7[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD8[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD9[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD10[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
							for(unsigned int t=0; t<DOUBLE_BLOCKRAM_SIZE; t++){ // replicate
								for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
									vmaskREAD11[v][t] = vmaskREAD0[v][t]; // CRITICAL FIXME.
								}
							}
	
						}	
						// MEMACCESS_SPL_readmanyvmasks12(enable_loadmasks, vdram, vmaskREAD0,vmaskREAD1,vmaskREAD2,vmaskREAD3,vmaskREAD4,vmaskREAD5,vmaskREAD6,vmaskREAD7,vmaskREAD8,vmaskREAD9,vmaskREAD10,vmaskREAD11, vbuffer0, globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsV); 
						// #ifdef ENABLE_SUBVMASKING
						// MEMACCESS_SPL_readmanyspmask12(enable_loadmasks, vmask0, vmask0_subp,vmask1_subp,vmask2_subp,vmask3_subp,vmask4_subp,vmask5_subp,vmask6_subp,vmask7_subp,vmask8_subp,vmask9_subp,vmask10_subp,vmask11_subp, vmaskbuffersz_kvs); 
						// #endif
						// mergeobj->MERGE_readandreplicate12vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, 0, 0, reducebuffersz, globalparamsV); 
						// mergeobj->MERGE_readandreplicate12vdata(enable_readandreplicatevdata, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, 8, 0, reducebuffersz, globalparamsV); 
						
						// proc 
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram0, vbuffer0, vmask0_p, vmask0_subp, vmaskREAD0, vmaskWRITE0, globalstatsbuffer0, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram1, vbuffer1, vmask1_p, vmask1_subp, vmaskREAD1, vmaskWRITE1, globalstatsbuffer1, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram3, vbuffer3, vmask3_p, vmask3_subp, vmaskREAD3, vmaskWRITE3, globalstatsbuffer3, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram4, vbuffer4, vmask4_p, vmask4_subp, vmaskREAD4, vmaskWRITE4, globalstatsbuffer4, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram5, vbuffer5, vmask5_p, vmask5_subp, vmaskREAD5, vmaskWRITE5, globalstatsbuffer5, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram6, vbuffer6, vmask6_p, vmask6_subp, vmaskREAD6, vmaskWRITE6, globalstatsbuffer6, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram7, vbuffer7, vmask7_p, vmask7_subp, vmaskREAD7, vmaskWRITE7, globalstatsbuffer7, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram8, vbuffer8, vmask8_p, vmask8_subp, vmaskREAD8, vmaskWRITE8, globalstatsbuffer8, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram9, vbuffer9, vmask9_p, vmask9_subp, vmaskREAD9, vmaskWRITE9, globalstatsbuffer9, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram10, vbuffer10, vmask10_p, vmask10_subp, vmaskREAD10, vmaskWRITE10, globalstatsbuffer10, globalposition);	
						topkernelproc_embedded(enableprocess, ON, ON,  kvdram11, vbuffer11, vmask11_p, vmask11_subp, vmaskREAD11, vmaskWRITE11, globalstatsbuffer11, globalposition);	
	
						/* 						topkernelproc_embedded(enableprocess, ON, ON,  kvdram2, vbuffer2, vmask2_p, vmask2_subp, vmaskREAD2, vmaskWRITE2, globalstatsbuffer2, globalposition);	
 */
						
						// merge 
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge12andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2); }
						// if(globalposition.EN_REDUCE == ON && enablereduce == ON){ mergeobj->MERGE_merge12andsavevdata(ON, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs2 + reducebuffersz); }
						
						// update stats
						// if(globalposition.EN_REDUCE == ON){ 
							// if(enablereduce == ON){ buffer[source_partition] += 64; } 
							// else { buffer[source_partition] += 0; }
						// }
						
						// increments
						// if(globalposition.EN_PARTITION == ON || globalposition.EN_REDUCE == ON){ sourcestatsmarker += 1; }
						// if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						// if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						// if(globalposition.EN_PROCESS == ON || globalposition.EN_REDUCE == ON){ vreadoffset_kvs2 += reducebuffersz * 2; }
						if(globalposition.EN_PARTITION == ON){ sourcestatsmarker += 1; }
						if(globalposition.EN_PARTITION == ON){ deststatsmarker += NUM_PARTITIONS; }
						if(globalposition.EN_PROCESS == ON){ vmaskoffset_kvs += vmaskbuffersz_kvs; }
						if(globalposition.EN_PROCESS == ON){ vreadoffset_kvs2 += vreadskipsz_kvs2; }
					}
				}
			} // stage
		} // v_chunkid
	} // edgebankID
	
MEMACCESS_SPL_commitkvstats(vdram, buffer, globalparamsV, reducesourcestatsmarker);
	// UTIL_increment_graphiteration(vdram, globalparamsV);
	
MERGE_SPLIT_mergeVs12(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11, vdram);
	
	// #ifdef _WIDEWORD
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1; // CRITICAL NOTEME: Graph Iteration is incremented here
	// #else
	// vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = globalparamsV.ALGORITHMINFO_GRAPHITERATIONID + 1;
	// #endif
	// exit(EXIT_SUCCESS);
	return;
}
}
