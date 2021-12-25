#include "top_nonunifiedvts.h"
using namespace std;

#ifdef SW
top_nonunifiedvts::top_nonunifiedvts(){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(); 
	processedges_unslicedgraphobj = new processedges_unslicedgraph();
	processedges_slicedgraphobj = new processedges_slicedgraph();
	partitionupdatesobj = new partitionupdates();
	reduceupdatesobj = new reduceupdates();
	mem_accessobj = new mem_access();
	actsobj = new acts();
}
top_nonunifiedvts::~top_nonunifiedvts(){}
#endif

void
	#ifdef SW
	top_nonunifiedvts::
	#endif 
processit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,								
			unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	#pragma HLS INLINE 
	analysis_type analysis_loop1 = 1;
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	
	keyvalue_t globalstatsbuffer[NUM_PARTITIONS]; 
	
	batch_type sourcestatsmarker = 0;
	batch_type deststatsmarker = 1;
	sweepparams_t sweepparams;
	
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	buffer_type vmaskbuffersz_kvs = (globalparamsK.SIZE_REDUCE * VDATA_PACKINGSIZE) / 512;
	
	step_type currentLOP=globalparamsK.ACTSPARAMS_BEGINLOP;
	acts_utilobj->UTIL_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	sweepparams = acts_utilobj->UTIL_getsweepparams(globalparamsK, currentLOP, 0);
	travstate_t avtravstate;
	batch_type vmaskoffset_kvs = v_chunkids[v_chunkid] * (vmaskbuffersz_kvs); 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type tempnum_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);
	#else 
	batch_type tempnum_source_partitions = NUM_PARTITIONS;	
	#endif 
	
	globalparams_t globalparamsVPTRS = globalparamsE;
	globalparams_t globalparamsVDATA = globalparamsK;
	
	avtravstate.begin_kvs = 0;
	avtravstate.end_kvs = avtravstate.begin_kvs + (globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE); avtravstate.size_kvs = globalparamsK.ACTSPARAMS_SRCVSIZE / VECTOR2_SIZE;
	mem_accessobj->MEMACCESS_readglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK); 
	acts_utilobj->UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
	
	batch_type vptrbaseoffset_kvs = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR + (globalparamsVPTRS.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE. NOTE: no need to include v_chunkids[32] because source_partition handles it
	batch_type vdatabaseoffset_kvs = globalparamsVDATA.BASEOFFSETKVS_SRCVERTICESDATA + (globalparamsVDATA.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE); // NEWCHANGE.
	
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int SKIP_KVS = reducebuffersz * FETFACTOR;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->print7("### processit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.end_kvs * VECTOR_SIZE, (avtravstate.end_kvs - avtravstate.begin_kvs) * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); 							
	#endif
	PROCESSIT_MAINLOOP: for(batch_type source_partition=v_chunkids[v_chunkid]; source_partition<v_chunkids[v_chunkid+1]; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		// cout<<"--- source_partition "<<source_partition<<" ... "<<endl; // REMOVEME.

		batch_type voffset_kvs = source_partition * reducebuffersz * FETFACTOR;
		if(voffset_kvs >= avtravstate.end_kvs){ continue; }
		if(GraphAlgo != PAGERANK) { if(vmask_p[source_partition] == 0){ vmaskoffset_kvs += vmaskbuffersz_kvs; continue; }}
		
		sweepparams.source_partition = source_partition;
		avtravstate.i_kvs = voffset_kvs;
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print5("### processit:: source_partition", "voffset", "vbegin", "vend", "vskip", source_partition, voffset_kvs * VECTOR_SIZE, avtravstate.begin_kvs * VECTOR_SIZE, avtravstate.size_kvs * VECTOR_SIZE, SRCBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		mem_accessobj->MEMACCESS_loadvmasks(ON, kvdram, vmask, vbuffer, globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK + vmaskoffset_kvs, vmaskbuffersz_kvs, globalparamsK); // NOTE: this should come before loadvdata because vbuffer is used as a temp buffer
		mem_accessobj->MEMACCESS_readvdata(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs, vbuffer, 0, 0, reducebuffersz, globalparamsK);
		mem_accessobj->MEMACCESS_readvdata(ON, kvdram, vdatabaseoffset_kvs + voffset_kvs + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparamsK);
		vmaskoffset_kvs += vmaskbuffersz_kvs;
	
		vertex_t srcvlocaloffset = (voffset_kvs * VECTOR2_SIZE);
		vertex_t beginsrcvid = globalparamsK.ACTSPARAMS_SRCVOFFSET + (voffset_kvs * VECTOR2_SIZE);
		vertex_t endsrcvid = beginsrcvid + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR);
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endsrcvid = beginsrcvid; }
		if((srcvlocaloffset < globalparamsK.ACTSPARAMS_SRCVSIZE) && (srcvlocaloffset + ((reducebuffersz * VECTOR2_SIZE) * FETFACTOR) >= globalparamsK.ACTSPARAMS_SRCVSIZE)){ endsrcvid = beginsrcvid + globalparamsK.ACTSPARAMS_SRCVSIZE - srcvlocaloffset; }
		
		tuple_t tup = mem_accessobj->MEMACCESS_getvptrs_opt( kvdram, vptrbaseoffset_kvs, voffset_kvs * VECTOR2_SIZE, (voffset_kvs + SKIP_KVS) * VECTOR2_SIZE, edgebankID); // CRITICAL NEWCHANGE.
		keyy_t beginvptr = tup.A;
		keyy_t endvptr = tup.B; 
	
		if(srcvlocaloffset >= globalparamsK.ACTSPARAMS_SRCVSIZE){ endvptr = beginvptr; }
		
		keyy_t localbeginvptr = beginvptr;
		keyy_t localendvptr = endvptr;
		keyy_t numedges = localendvptr - localbeginvptr + 2*VECTOR_SIZE;
		if(localbeginvptr == localendvptr){ numedges = 0; }
		keyy_t localbeginvptr_kvs = localbeginvptr / VECTOR_SIZE; // NOTE: this should be in KVS(8) terms
		keyy_t numedges_kvs = numedges / VECTOR_SIZE; // NB: this is correct.
		#ifdef _DEBUGMODE_CHECKS2
		if(localendvptr < localbeginvptr){ cout<<"processit::ERROR: localendvptr < localbeginvptr. localbeginvptr: "<<localbeginvptr<<", localendvptr: "<<localendvptr<<endl; exit(EXIT_FAILURE); }
		if(localendvptr < globalparamsE.SIZE_EDGES){ actsutilityobj->checkptr("processit", beginsrcvid, endsrcvid, localbeginvptr, localendvptr, (keyvalue_t *)&kvdram[globalparamsE.BASEOFFSETKVS_EDGESDATA]); }
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"[index: "<<source_partition<<"][beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<"][beginvptr: "<<localbeginvptr<<", endvptr: "<<localendvptr<<", edges size: "<<numedges<<"][voffset: "<<voffset_kvs * VECTOR_SIZE<<"]"<<endl;
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
		actsobj->ACTS_actit
		#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
		actsobj->ACTS_priorit	
		#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
		actsobj->ACTS_tradit
		#else 
		actsobj->ACTS_tradit
		#endif 
		(
			ON, PROCESSMODE,
 kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalstatsbuffer, 
			globalparamsK, sweepparams, etravstate, globalparamsE.BASEOFFSETKVS_EDGESDATA, globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE,
			resetenv, flush, edgebankID);
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
		
	mem_accessobj->MEMACCESS_saveglobalstats(ON, kvdram, globalstatsbuffer, globalparamsK.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

void
	#ifdef SW 
	top_nonunifiedvts::
	#endif 
partitionit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], globalparams_t globalparams, unsigned int edgebankID){
	#pragma HLS INLINE
	analysis_type analysis_numllops = 1;
	analysis_type analysis_numsourcepartitions = 1;
	#ifdef TRAD_PARTITION_AND_REDUCE_STRETEGY
	return; // no partitionit for TRAD_PARTITION_AND_REDUCE_STRETEGY
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->printglobalparameters("top_nonunifiedvts::acts_utilobj->UTIL_getglobalparams:: printing global parameters", globalparams);
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
	
		batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(currentLOP);
		bool_type enreduce = ON;
		
		PARTITIONIT_LOOP1B: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_numsourcepartitions avg=analysis_numsourcepartitions

			acts_utilobj->UTIL_resetkeysandvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
		
			sweepparams = acts_utilobj->UTIL_getsweepparams(globalparams, currentLOP, source_partition);
			travstate_t ptravstate = acts_utilobj->UTIL_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);
			
			// collect stats
			mem_accessobj->MEMACCESS_readglobalstats(ON, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams);
			acts_utilobj->UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			
			// partition
			if(ptravstate.size_kvs > 0){ config.enablepartition = ON; } 
			else { ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablepartition = OFF; }
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if((config.enablepartition == ON) && (currentLOP >= 1) && (currentLOP <= globalparams.ACTSPARAMS_TREEDEPTH)){ actsutilityobj->print7("### partitionit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
			#endif
			acts_utilobj->UTIL_resetvalues(globalstatsbuffer, NUM_PARTITIONS, 0);
			bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
			
			#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
			actsobj->ACTS_actit
			#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
			actsobj->ACTS_priorit	
			#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
			actsobj->ACTS_tradit
			#else 
			actsobj->ACTS_tradit
			#endif
			(config.enablepartition, PARTITIONMODE,
 kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalstatsbuffer, // CRITICAL FIXME.
					globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
					ON, ON, NAp);
					
			mem_accessobj->MEMACCESS_saveglobalstats(config.enablepartition, kvdram, globalstatsbuffer, globalparams.BASEOFFSETKVS_STATSDRAM + deststatsmarker, globalparams); 
			
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

void
	#ifdef SW 
	top_nonunifiedvts::
	#endif 
reduceit( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparams, unsigned int edgebankID){	
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
	
	sweepparams = acts_utilobj->UTIL_getsweepparams(globalparams, currentLOP, source_partition);
	travstate_t ptravstate = acts_utilobj->UTIL_gettravstate(ON, kvdram, globalparams, currentLOP, sourcestatsmarker);

	if(ptravstate.size_kvs == 0){ ptravstate.begin_kvs = 0; ptravstate.end_kvs = 0; config.enablereduce = OFF; }
	else { config.enablereduce = ON; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(ptravstate.size_kvs > 0){ actsutilityobj->print7("### reduceit:: source_p", "upperlimit", "begin", "end", "size", "dest range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, ptravstate.begin_kvs * VECTOR_SIZE, ptravstate.end_kvs * VECTOR_SIZE, ptravstate.size_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP); }	
	#endif
	bool_type resetenv; if(source_partition==0){ resetenv = ON; } else { resetenv = OFF; }
	
	#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
	actsobj->ACTS_actit
	#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
	actsobj->ACTS_priorit	
	#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
	actsobj->ACTS_tradit
	#else 
	actsobj->ACTS_tradit
	#endif
	(config.enablereduce, REDUCEMODE,
 kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalstatsbuffer, // CRITICAL FIXME.
			globalparams, sweepparams, ptravstate, sweepparams.worksourcebaseaddress_kvs, sweepparams.workdestbaseaddress_kvs,
			ON, ON, NAp); // REMOVEME.
	return;
}

void
	#ifdef SW 
	top_nonunifiedvts::
	#endif 
dispatch(bool_type en_process, bool_type en_partition, bool_type en_reduce,  uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE],
			batch_type sourcestatsmarker, batch_type source_partition, globalparams_t globalparamsE, globalparams_t globalparamsK,
				unsigned int v_chunkids[EDGESSTATSDRAMSZ], unsigned int v_chunkid, unsigned int edgebankID){
	if(en_process == ON){ processit( kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, globalparamsE, globalparamsK, v_chunkids, v_chunkid, edgebankID); } 
	if(en_partition == ON){ partitionit( kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, globalparamsK, NAp); } 
	if(en_reduce == ON){ reduceit( kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, sourcestatsmarker, source_partition, globalparamsK, NAp); } 
	return;
}

void
	#ifdef SW 
	top_nonunifiedvts::
	#endif 
dispatch_reduce( uint512_dt * kvdram, keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], uint32_type vmask_p[BLOCKRAM_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK,	
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
	
	unsigned int vreadoffset_kvs = 0;
	buffer_type reducebuffersz = globalparamsK.SIZE_REDUCE / 2;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH;
	#else 
	step_type currentLOP = globalparamsK.ACTSPARAMS_TREEDEPTH + 1; // NEWCHANGE.	
	#endif 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(currentLOP); // NEWCHANGE.
	#else 
	batch_type num_source_partitions = NUM_PARTITIONS;	
	#endif

	// for(unsigned int k=0; k<num_source_partitions; k++){ vmask_p[k] = 0; } // vmask_subp[k] = 0; // CRITICAL NEWCHANGE.
	bool_type enablereduce = ON;
	
	DISPATCHREDUCE_MAINLOOP: for(batch_type source_partition=0; source_partition<num_source_partitions; source_partition+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
		#ifdef _DEBUGMODE_KERNELPRINTS
		actsutilityobj->print3("### dispatch_reduce:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		enablereduce = ON;
		travstate_t rtravstate = acts_utilobj->UTIL_gettravstate(ON, kvdram, globalparamsK, currentLOP, sourcestatsmarker);
		if(rtravstate.size_kvs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		batch_type voffset_kvs = source_partition * reducebuffersz * FETFACTOR;
		
		// read vertices
		mem_accessobj->MEMACCESS_readvdata(enablereduce, kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer, 0, 0, reducebuffersz, globalparamsK);
		mem_accessobj->MEMACCESS_readvdata(enablereduce, kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparamsK);
		
		// reduce
		dispatch(OFF, OFF, enablereduce,  kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, sourcestatsmarker, source_partition, globalparamsE, globalparamsK, v_chunkids, v_chunkid, NAp);
		
		// writeback vertices
		mem_accessobj->MEMACCESS_savevdata(enablereduce, kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer, 0, 0, reducebuffersz, globalparamsK);
		mem_accessobj->MEMACCESS_savevdata(enablereduce, kvdram, globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer, 8, 0, reducebuffersz, globalparamsK);
		
		sourcestatsmarker += 1;
		vreadoffset_kvs += reducebuffersz * 2;
	}
	return;
} 

// top
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelproc_embedded(
	
	uint512_dt * kvdram){
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->printparameters();
	#endif
	#if defined(_DEBUGMODE_KERNELPRINTS) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> Light weight ACTS (PR: 1 ACTS IN 1 COMPUTEUNITS) Launched... size: "<<acts_utilobj->UTIL_GETKEYENTRY(kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	keyvalue_buffer_t sourcebuffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE];
	#pragma HLS array_partition variable = sourcebuffer
	keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer
	unitBRAMwidth_type vmask[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask
	unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE];
	#pragma HLS DATA_PACK variable = vmask_subp
	uint32_type vmask_p[BLOCKRAM_SIZE];
	
	globalparams_t globalparamsK = acts_utilobj->UTIL_getglobalparams(kvdram);
	globalparams_t globalparamsE[MAX_NUM_EDGE_BANKS];
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	
	#else 
	globalparamsE[0] = globalparamsK;
	#endif 
	unsigned int GraphAlgo = globalparamsK.ALGORITHMINFO_GRAPHALGORITHMID;
	
	if(GraphAlgo != PAGERANK){ mem_accessobj->MEMACCESS_loadvmask_p(kvdram, vmask_p, globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK, BLOCKRAM_SIZE); }
	if(GraphAlgo != PAGERANK){ acts_utilobj->UTIL_resetkvstatvalues(kvdram, globalparamsK); }	
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	value_t buffer[DOUBLE_BLOCKRAM_SIZE]; // CRITICAL AUTOMATEME.
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	#endif 
	
	#ifdef ENABLERECURSIVEPARTITIONING
	batch_type num_vchunks = acts_utilobj->UTIL_get_num_source_partitions(globalparamsK.ACTSPARAMS_TREEDEPTH);
	#else 
	batch_type num_vchunks = NUM_PARTITIONS;	
	#endif 
	
	unsigned int PARTITION_CHKPT[MAX_NUM_EDGE_BANKS][EDGESSTATSDRAMSZ];
	for(unsigned int u=0; u<EDGESSTATSDRAMSZ; u++){
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			#ifdef _WIDEWORD
	
			#else
	
			#endif
		#else 
			#ifdef _WIDEWORD
			PARTITION_CHKPT[0][u] = kvdram[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].range(31, 0); 
			#else
			PARTITION_CHKPT[0][u] = kvdram[globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + u].data[0].key; 
			#endif
		#endif 
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int u=0; u<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER+1; u++){ cout<<">>> globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER: "<<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER<<", PARTITION_CHKPT["<<u<<"]: "<<PARTITION_CHKPT[u]<<endl; }			
	#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	copyvs(kvdram, vbuffer, globalparamsK);
	#endif
	unsigned int FIRST_BASEOFFSETKVS_STATSDRAM = globalparamsK.BASEOFFSETKVS_STATSDRAM;
	
	unsigned int it_size = 0;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	it_size = NUM_EDGE_BANKS;
	#else 
	it_size = 1;	
	#endif 
	
	for(unsigned int edgebankID=0; edgebankID<it_size; edgebankID++){
		globalparamsK.VARS_WORKBATCH = 0;
		globalparams_t _globalparamsE = globalparamsE[edgebankID];

		for(unsigned int v_chunkid=0; v_chunkid<globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER; v_chunkid++){
			// process & partition
			#ifdef PROCESSMODULE
			if(globalparamsK.ENABLE_PROCESSCOMMAND == ON){
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelproc: processing instance ... "<<endl;
				#endif
				dispatch(ON, OFF, OFF,  kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, _globalparamsE, globalparamsK, PARTITION_CHKPT[edgebankID], v_chunkid, edgebankID);
			}
			#endif
			
			// partition
			#ifdef PARTITIONMODULE
			if(globalparamsK.ENABLE_PARTITIONCOMMAND == ON){
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelproc: partitioning instance ... "<<endl;
				#endif
				dispatch(OFF, ON, OFF,  kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, NAp, NAp, _globalparamsE, globalparamsK, PARTITION_CHKPT[edgebankID], v_chunkid, NAp);
			}
			#endif
			
			// reduce & partition
			#if defined(REDUCEMODULE)
			if(globalparamsK.ENABLE_APPLYUPDATESCOMMAND == ON){ 
				#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
				cout<<"topkernelproc: reducing instance ... "<<endl;
				#endif
				dispatch_reduce( kvdram, sourcebuffer, vbuffer, vmask, vmask_subp, vmask_p, _globalparamsE, globalparamsK, PARTITION_CHKPT[edgebankID], v_chunkid, NAp);
			}
			#endif
			
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			accumkvstats(kvdram, buffer, globalparamsK);
			globalparamsK.VARS_WORKBATCH += 1;
			#endif 
			#if defined(_DEBUGMODE_KERNELPRINTS2) || defined(_DEBUGMODE_CHECKS2)
			actsutilityobj->clearglobalvars();
			#endif
		}
		
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		globalparamsK.BASEOFFSETKVS_STATSDRAM += (globalparamsK.SIZE_KVSTATSDRAM / VECTOR_SIZE) / NUM_EDGE_BANKS; 
		#endif
	}
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparamsK.BASEOFFSETKVS_STATSDRAM = FIRST_BASEOFFSETKVS_STATSDRAM;
	commitkvstats(kvdram, buffer, globalparamsK);
	#endif
	acts_utilobj->UTIL_increment_graphiteration(kvdram, globalparamsK);
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS3) || defined(_DEBUGMODE_CHECKS2)
	actsutilityobj->clearglobalvars();
	#endif
	return;
}

extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP1(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP1: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL1_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL1_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 1; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge1andsavevdata(enablereduce, vdram, vbuffer0, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge1andsavevdata(enablereduce, vdram, vbuffer0, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP2(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem2
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP2: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP2: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL2_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL2_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 2; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge2andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge2andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP3(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP3: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP3: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP3: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL3_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL3_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 3; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge3andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge3andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP4(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem8
#else 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem4
#endif 

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP4: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP4: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP4: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP4: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL4_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL4_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 4; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge4andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge4andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP5(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
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

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP5: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP5: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP5: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP5: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	cout<<"--------------***************------------------------------------ topkernelP5: processing instance 4"<<endl;
	topkernelproc_embedded(kvdram4);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL5_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL5_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 5; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge5andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge5andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP6(
	uint512_dt * kvdram0,
	uint512_dt * kvdram1,
	uint512_dt * kvdram2,
	uint512_dt * kvdram3,
	uint512_dt * kvdram4,
	uint512_dt * kvdram5,
	uint512_dt * vdram
	){
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
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

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
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
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP6: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP6: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP6: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP6: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	cout<<"--------------***************------------------------------------ topkernelP6: processing instance 4"<<endl;
	topkernelproc_embedded(kvdram4);
	cout<<"--------------***************------------------------------------ topkernelP6: processing instance 5"<<endl;
	topkernelproc_embedded(kvdram5);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL6_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL6_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 6; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer5, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge6andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge6andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP7(
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
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
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

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
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
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP7: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP7: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP7: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP7: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	cout<<"--------------***************------------------------------------ topkernelP7: processing instance 4"<<endl;
	topkernelproc_embedded(kvdram4);
	cout<<"--------------***************------------------------------------ topkernelP7: processing instance 5"<<endl;
	topkernelproc_embedded(kvdram5);
	cout<<"--------------***************------------------------------------ topkernelP7: processing instance 6"<<endl;
	topkernelproc_embedded(kvdram6);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL7_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL7_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 7; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer5, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer6, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge7andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge7andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP8(
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
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
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

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
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
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
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

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP8: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP8: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP8: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP8: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	cout<<"--------------***************------------------------------------ topkernelP8: processing instance 4"<<endl;
	topkernelproc_embedded(kvdram4);
	cout<<"--------------***************------------------------------------ topkernelP8: processing instance 5"<<endl;
	topkernelproc_embedded(kvdram5);
	cout<<"--------------***************------------------------------------ topkernelP8: processing instance 6"<<endl;
	topkernelproc_embedded(kvdram6);
	cout<<"--------------***************------------------------------------ topkernelP8: processing instance 7"<<endl;
	topkernelproc_embedded(kvdram7);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL8_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL8_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 8; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer5, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer6, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer7, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge8andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge8andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP9(
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
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = edges8 offset = slave bundle = gmem16
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

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges8 bundle = control
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
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#pragma HLS DATA_PACK variable = edges8
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

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 4"<<endl;
	topkernelproc_embedded(kvdram4);
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 5"<<endl;
	topkernelproc_embedded(kvdram5);
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 6"<<endl;
	topkernelproc_embedded(kvdram6);
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 7"<<endl;
	topkernelproc_embedded(kvdram7);
	cout<<"--------------***************------------------------------------ topkernelP9: processing instance 8"<<endl;
	topkernelproc_embedded(kvdram8);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL9_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL9_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[8] = acts_utilobj->UTIL_gettravstate(ON, kvdram8, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 9; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer5, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer6, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer7, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer8, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer8, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge9andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge9andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP10(
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
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = edges8 offset = slave bundle = gmem16
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
#pragma HLS INTERFACE m_axi port = edges9 offset = slave bundle = gmem18
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

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges9 bundle = control
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
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#pragma HLS DATA_PACK variable = edges8
#pragma HLS DATA_PACK variable = edges9
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

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 4"<<endl;
	topkernelproc_embedded(kvdram4);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 5"<<endl;
	topkernelproc_embedded(kvdram5);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 6"<<endl;
	topkernelproc_embedded(kvdram6);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 7"<<endl;
	topkernelproc_embedded(kvdram7);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 8"<<endl;
	topkernelproc_embedded(kvdram8);
	cout<<"--------------***************------------------------------------ topkernelP10: processing instance 9"<<endl;
	topkernelproc_embedded(kvdram9);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL10_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL10_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[8] = acts_utilobj->UTIL_gettravstate(ON, kvdram8, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[9] = acts_utilobj->UTIL_gettravstate(ON, kvdram9, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 10; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer5, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer6, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer7, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer8, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer8, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer9, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer9, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge10andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge10andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP11(
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
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = edges8 offset = slave bundle = gmem16
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
#pragma HLS INTERFACE m_axi port = edges9 offset = slave bundle = gmem18
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem19
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem20
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

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges9 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
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
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#pragma HLS DATA_PACK variable = edges8
#pragma HLS DATA_PACK variable = edges9
#pragma HLS DATA_PACK variable = edges10
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

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 4"<<endl;
	topkernelproc_embedded(kvdram4);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 5"<<endl;
	topkernelproc_embedded(kvdram5);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 6"<<endl;
	topkernelproc_embedded(kvdram6);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 7"<<endl;
	topkernelproc_embedded(kvdram7);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 8"<<endl;
	topkernelproc_embedded(kvdram8);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 9"<<endl;
	topkernelproc_embedded(kvdram9);
	cout<<"--------------***************------------------------------------ topkernelP11: processing instance 10"<<endl;
	topkernelproc_embedded(kvdram10);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL11_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL11_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[8] = acts_utilobj->UTIL_gettravstate(ON, kvdram8, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[9] = acts_utilobj->UTIL_gettravstate(ON, kvdram9, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[10] = acts_utilobj->UTIL_gettravstate(ON, kvdram10, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 11; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer5, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer6, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer7, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer8, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer8, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer9, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer9, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer10, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer10, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge11andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge11andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}
extern "C" {
void 
	#ifdef SW 
	top_nonunifiedvts:: 
	#endif
topkernelP12(
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
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = edges3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = edges4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = edges5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = edges6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = edges7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = edges8 offset = slave bundle = gmem16
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem17
#pragma HLS INTERFACE m_axi port = edges9 offset = slave bundle = gmem18
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem19
#pragma HLS INTERFACE m_axi port = edges10 offset = slave bundle = gmem20
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem21
#pragma HLS INTERFACE m_axi port = edges11 offset = slave bundle = gmem22
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

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#pragma HLS INTERFACE s_axilite port = edges3 bundle = control
#pragma HLS INTERFACE s_axilite port = edges4 bundle = control
#pragma HLS INTERFACE s_axilite port = edges5 bundle = control
#pragma HLS INTERFACE s_axilite port = edges6 bundle = control
#pragma HLS INTERFACE s_axilite port = edges7 bundle = control
#pragma HLS INTERFACE s_axilite port = edges8 bundle = control
#pragma HLS INTERFACE s_axilite port = edges9 bundle = control
#pragma HLS INTERFACE s_axilite port = edges10 bundle = control
#pragma HLS INTERFACE s_axilite port = edges11 bundle = control
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
#pragma HLS DATA_PACK variable = edges0
#pragma HLS DATA_PACK variable = edges1
#pragma HLS DATA_PACK variable = edges2
#pragma HLS DATA_PACK variable = edges3
#pragma HLS DATA_PACK variable = edges4
#pragma HLS DATA_PACK variable = edges5
#pragma HLS DATA_PACK variable = edges6
#pragma HLS DATA_PACK variable = edges7
#pragma HLS DATA_PACK variable = edges8
#pragma HLS DATA_PACK variable = edges9
#pragma HLS DATA_PACK variable = edges10
#pragma HLS DATA_PACK variable = edges11
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

	#if defined(_DEBUGMODE_KERNELPRINTS2) & defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int GraphIter = acts_utilobj->UTIL_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
	#endif
	
	// run acts
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 0"<<endl;
	topkernelproc_embedded(kvdram0);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 1"<<endl;
	topkernelproc_embedded(kvdram1);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 2"<<endl;
	topkernelproc_embedded(kvdram2);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 3"<<endl;
	topkernelproc_embedded(kvdram3);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 4"<<endl;
	topkernelproc_embedded(kvdram4);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 5"<<endl;
	topkernelproc_embedded(kvdram5);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 6"<<endl;
	topkernelproc_embedded(kvdram6);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 7"<<endl;
	topkernelproc_embedded(kvdram7);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 8"<<endl;
	topkernelproc_embedded(kvdram8);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 9"<<endl;
	topkernelproc_embedded(kvdram9);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 10"<<endl;
	topkernelproc_embedded(kvdram10);
	cout<<"--------------***************------------------------------------ topkernelP12: processing instance 11"<<endl;
	topkernelproc_embedded(kvdram11);
	exit(EXIT_SUCCESS); //
	
	/** TOPKERNEL12_BASELOOP1: for(batch_type source_partition=0; source_partition<256; source_partition+=1){ // AUTOMATEME.
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->print3("### TOPKERNEL12_BASELOOP1:: source_partition", "currentLOP", "NAp", source_partition, currentLOP, NAp); 							
		#endif
		
		bool_type enablereduce = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[8] = acts_utilobj->UTIL_gettravstate(ON, kvdram8, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[9] = acts_utilobj->UTIL_gettravstate(ON, kvdram9, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[10] = acts_utilobj->UTIL_gettravstate(ON, kvdram10, globalparamsK, currentLOP, sourcestatsmarker);
		rtravstates[11] = acts_utilobj->UTIL_gettravstate(ON, kvdram11, globalparamsK, currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < 12; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablereduce = ON; } else { enablereduce = OFF; }
		
		// read vertices
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer0, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer1, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer2, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer3, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer4, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer5, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer6, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer7, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer8, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer8, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer9, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer9, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer10, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer10, 8, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer11, 0, 0, reducebuffersz, globalparamsV); 
		readvdata(enablereduce, vdram, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz, vbuffer11, 8, 0, reducebuffersz, globalparamsV); 
		
		// merge 
		merge12andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, 0, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs); 
		merge12andsavevdata(enablereduce, vdram, vbuffer0,vbuffer1,vbuffer2,vbuffer3,vbuffer4,vbuffer5,vbuffer6,vbuffer7,vbuffer8,vbuffer9,vbuffer10,vbuffer11, 8, 0, globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs + reducebuffersz); 
		
		// update stats
		if(enablereduce == ON){ buffer[source_partition] += 64; } 
		else { buffer[source_partition] += 0; }
	}	 */		
	return;
}
}


