#include "app.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

app::app(unsigned int algorithmid, unsigned int datasetid, std::string _binaryFile1, std::string _binaryFile2){
	algorithm * thisalgorithmobj = new algorithm();
	graphobj = new graph(thisalgorithmobj, datasetid, 1, true, true, true);
	statsobj = new stats(graphobj);
	algorithmobj = new algorithm();
	utilityobj = new utility();
	loadgraphobj = new loadgraph(graphobj, statsobj);
	#if defined(CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS)
	loadedgesobj = new loadedges_sequential(graphobj, statsobj);
	#elif defined(CONFIG_PREPROCESS_LOADEDGES_RANDOMSRCVIDS)
	loadedgesobj = new loadedges_random(graphobj, statsobj);
	#else
	NOT DEFINED.	
	#endif	
	setupkernelobj = new setupkernel(graphobj, thisalgorithmobj, statsobj); 
	swkernelobj = new swkernel(graphobj, thisalgorithmobj, statsobj);

	#ifndef SW_IMPL 
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = (uint512_vec_dt *) aligned_alloc(4096, (TOTALDRAMCAPACITY_KVS * sizeof(uint512_vec_dt))); }				
	vdram = (uint512_vec_dt *) aligned_alloc(4096, (TOTALDRAMCAPACITY_KVS * sizeof(uint512_vec_dt)));
	#else
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvbuffer[i] = new uint512_vec_dt[TOTALDRAMCAPACITY_KVS]; }
	vdram = new uint512_vec_dt[TOTALDRAMCAPACITY_KVS];
	#endif
	#endif 
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	#ifdef FPGA_IMPL
	for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ edges[j] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }	// 2 because edge_type consist 4 bytes not 8 bytes			
	#else
	for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ edges[j] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
	#endif
	#endif 
	
	edgedatabuffer = new edge2_type[graphobj->getedgessize(0)];

	binaryFile[0] = _binaryFile1;
	binaryFile[1] = _binaryFile2;
	
	#ifdef GRAFBOOST_SETUP 
	setupkernelobj->loadSRstructures();
	#endif
}
app::~app(){
	cout<<"app::~app:: finish destroying memory structures... "<<endl;
	delete [] kvbuffer;
}
void app::finish(){}

runsummary_t app::run(){
	#ifdef SW_IMPL 
	run_sw();
	#else 
	run_hw();
	#endif
}
runsummary_t app::run_hw(){
	cout<<"app::run_hw:: app algorithm started. "<<endl;
	#ifdef TESTKERNEL
	cout<< TIMINGRESULTSCOLOR <<"================================================================== APP: THIS IS A TEST RUN ==================================================================" << RESET<< endl;
	#endif 
	
	unsigned int Algo;
	#if defined(PR_ALGORITHM)
	Algo = PAGERANK;
	#elif defined(CF_ALGORITHM)
	Algo = CF;
	#elif defined(CC_ALGORITHM)
	Algo = CC;
	#elif defined(BFS_ALGORITHM)
	Algo = BFS;
	#elif defined(SSSP_ALGORITHM)
	Algo = SSSP;
	#else 
	NOT DEFINED
	#endif
	
	long double totaltime_ms = 0;
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();
	vertexdatabuffer = graphobj->generateverticesdata(Algo);
	graphobj->openfilesforreading(0);
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	vector<edge3_type> edges_temp[NUMSUBCPUTHREADS];
	
	long double edgesprocessed_totals[128];
	long double timeelapsed_totals[128][8];
	
	// set root vid
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	unsigned int NumGraphIters = 1;
	#else 
	unsigned int NumGraphIters = 8;//8; // 32; // 3,12,32
	#endif 
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	// if(NumGraphIters > 2){ cout<<"app: WARNING. NumGraphIters("<<NumGraphIters<<") > 2 TOO MUCH LOG DATA MAY BE GENERATED. EXITING..."<<endl; exit(EXIT_FAILURE); }
	// #endif 
	
	container_t container;
	vector<value_t> actvvs;
	globalparams_TWOt globalparams;

	actvvs.push_back(1);

	// load workload information
	globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsK.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsK.SIZE_EDGES = 0;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsK.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsK.SIZE_VERTICESDATAMASK = 0;
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAM = 0;
	globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = 0;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsE.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsE.SIZE_EDGES = 0;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsE.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsE.SIZE_VERTICESDATAMASK = 0;
	globalparams.globalparamsE.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsE.SIZE_KVDRAM = 0;
	globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	#endif 
	
	globalparams.globalparamsV.BASEOFFSETKVS_MESSAGESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_STATSDRAM = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_KVDRAM = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_KVDRAMWORKSPACE = 0;
	
	globalparams.globalparamsV.SIZE_MESSAGESDRAM = MESSAGESDRAMSZ;
	globalparams.globalparamsV.SIZE_EDGES = 0;
	globalparams.globalparamsV.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsV.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.SIZE_ACTIVEVERTICES = 0;
	globalparams.globalparamsV.SIZE_VERTICESDATAMASK = 0;
	globalparams.globalparamsV.SIZE_KVSTATSDRAM = 0;
	globalparams.globalparamsV.SIZE_KVDRAM = 0;
	globalparams.globalparamsV.SIZE_KVDRAMWORKSPACE = 0;
	
	// edges
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams = loadedgesobj->loadedges(0, graphobj, vertexptrbuffer, edgedatabuffer, (vptr_type **)edges, (edge_type **)edges, edges_temp, &container, globalparams);
	#else
	globalparams = loadedgesobj->loadedges(0, graphobj, vertexptrbuffer, edgedatabuffer, (vptr_type **)kvbuffer, (edge_type **)kvbuffer, edges_temp, &container, globalparams);
	#endif 
	globalparams.globalparamsV.SIZE_EDGES = 0; 
	globalparams.globalparamsV.SIZE_VERTEXPTRS = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA = MESSAGES_BASEOFFSETKVS_MESSAGESDATA + MESSAGESDRAMSZ;
	globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsV.BASEOFFSETKVS_EDGESDATA + globalparams.globalparamsV.SIZE_EDGES;
	// exit(EXIT_SUCCESS);
	
	// src vertices data 
	cout<<"app::loadvertexdata:: loading source vertex datas... "<<endl;
	globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR + ((globalparams.globalparamsK.SIZE_VERTEXPTRS/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	globalparams.globalparamsK.SIZE_SRCVERTICESDATA = 
			#ifdef CONFIG_UNIFYSRCV
			0;
			#else 
			KVDATA_RANGE;	
			#endif 
	globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR + ((globalparams.globalparamsE.SIZE_VERTEXPTRS/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_SRCVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_VERTEXPTR + ((globalparams.globalparamsV.SIZE_VERTEXPTRS/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	// globalparams.globalparamsV.SIZE_SRCVERTICESDATA = NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs * VECTOR2_SIZE;
	globalparams.globalparamsV.SIZE_SRCVERTICESDATA = utilityobj->allignhigherto16_KV(graphobj->get_num_vertices() + 1000000); // NB: '1000000' is padding
	
	loadgraphobj->loadvertexdata(Algo, vertexdatabuffer, (keyvalue_t *)vdram, globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsV.SIZE_SRCVERTICESDATA, globalparams.globalparamsV, 0, SOURCE);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->loadvertexdata(Algo, vertexdatabuffer, (keyvalue_t *)kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsK.SIZE_SRCVERTICESDATA, globalparams.globalparamsK, 0, SOURCE); }					
	// exit(EXIT_SUCCESS);
	
	// dest vertices data 
	cout<<"app::loadvertexdata:: loading dest vertex datas... "<<endl;
	globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + ((globalparams.globalparamsK.SIZE_SRCVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_DESTVERTICESDATA = BATCH_RANGE;
	globalparams.globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsE.BASEOFFSETKVS_SRCVERTICESDATA + ((globalparams.globalparamsE.SIZE_SRCVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_DESTVERTICESDATA = 0;
	globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA = globalparams.globalparamsV.BASEOFFSETKVS_SRCVERTICESDATA + ((globalparams.globalparamsV.SIZE_SRCVERTICESDATA/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsV.SIZE_DESTVERTICESDATA = 0; // KVDATA_RANGE * 2; // FIXME.
	
	loadgraphobj->loadvertexdata(Algo, vertexdatabuffer, (keyvalue_t *)vdram, globalparams.globalparamsV.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsV.SIZE_DESTVERTICESDATA, globalparams.globalparamsV, 0, DEST);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->loadvertexdata(Algo, vertexdatabuffer, (keyvalue_t *)kvbuffer[i], globalparams.globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA * VECTOR_SIZE, globalparams.globalparamsK.SIZE_DESTVERTICESDATA, globalparams.globalparamsK, 0, DEST); }

	// active vertices & masks
	cout<<"app::loadactvvertices:: loading active vertices... "<<endl;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ globalparams = loadgraphobj->loadactvvertices(actvvs, (keyy_t *)&kvbuffer[i], &container, globalparams); }
	cout<<"app::generatevmaskdata:: generating vmask... "<<endl;
	globalparams = loadgraphobj->generatevmaskdata(actvvs, kvbuffer, vdram, globalparams);
	// exit(EXIT_SUCCESS); //
	
	// setting root vid
	cout<<"app::setrootvid:: setting root vid(s)... "<<endl;
	loadgraphobj->setrootvid(Algo, (uint512_vec_dt *)vdram, actvvs, globalparams.globalparamsV);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ loadgraphobj->setrootvid(Algo, (uint512_vec_dt *)kvbuffer[i], actvvs, globalparams.globalparamsK); }
	// exit(EXIT_SUCCESS); //
	
	// stats info 
	cout<<"app::loadoffsetmarkers:: loading offset markers... "<<endl;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams = loadgraphobj->loadoffsetmarkers((vptr_type **)edges, (edge_type **)edges, (keyvalue_t **)edges, edges_temp, &container, globalparams); 
	loadgraphobj->accumstats(kvbuffer, edges, globalparams); // NEWCHANGE.
	#else
	globalparams = loadgraphobj->loadoffsetmarkers((vptr_type **)kvbuffer, (edge_type **)kvbuffer, (keyvalue_t **)kvbuffer, edges_temp, &container, globalparams); 
	#endif
	// exit(EXIT_SUCCESS); //
	
	// messages
	globalparams.globalparamsV = loadgraphobj->finishglobaparams(globalparams.globalparamsV);
	cout<<"app::loadmessages:: loading messages... "<<endl;
	globalparams = loadgraphobj->loadmessages(vdram, edges, kvbuffer, &container, NumGraphIters, Algo, globalparams);
	// exit(EXIT_SUCCESS); //
	
	// others
	cout<<"app::appendkeyvaluecount:: appending value count... "<<endl;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ statsobj->appendkeyvaluecount(0, container.edgessize[i]); }
	
	cout<<"app::experiements: resetting kvdram & kvdram workspaces..."<<endl;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM], globalparams.globalparamsK.SIZE_KVDRAM);
		utilityobj->resetkeyvalues((keyvalue_t *)&kvbuffer[i][globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE], globalparams.globalparamsK.SIZE_KVDRAM);
	}
	
	cout<<"app::experiements: running sw version..."<<endl;
	unsigned int numValidIters = 0;
	#ifdef ALLVERTEXISACTIVE_ALGORITHM
	numValidIters = 1;
	unsigned int total_edges_processed = graphobj->get_num_edges();
	for(unsigned int k=0; k<128; k++){ edgesprocessed_totals[k] = total_edges_processed; }
	#else
	unsigned int total_edges_processed = utilityobj->runsssp_sw(actvvs, vertexptrbuffer, edgedatabuffer, NumGraphIters, edgesprocessed_totals, &numValidIters);
	#endif
	// exit(EXIT_SUCCESS); //
	
	// run_hw
	cout<<endl<< TIMINGRESULTSCOLOR <<">>> app::run_hw: app started. ("<<actvvs.size()<<" active vertices)"<< RESET <<endl;
	long double total_time_elapsed = setupkernelobj->runapp(binaryFile, (uint512_vec_dt *)vdram, (uint512_vec_dt **)edges, (uint512_vec_dt **)kvbuffer, timeelapsed_totals, numValidIters);
	// exit(EXIT_SUCCESS); //
	
	// output
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<NumGraphIters; i++){
		for(unsigned int analysis_i=0; analysis_i<3; analysis_i++){
			cout <<">>> app::run_hw: throughput["<<i<<"]["<<analysis_i<<"]("<<edgesprocessed_totals[i]/1000000<<"M edges,"<<timeelapsed_totals[i][analysis_i]<<" ms): "<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) * (1000))<<" edges/sec ("<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) / (1000))<<" million edges/sec)" <<endl;								
		}
	}
	#endif
	cout<<endl<< TIMINGRESULTSCOLOR << ">>> app::run_hw: THROUGHPUT SUMMARY DETAILS: "<< RESET <<endl;
	for(unsigned int i=0; i<NumGraphIters; i++){
		unsigned int total_edges = 0;
		long double total_time = 0;
		
		unsigned int analysis_begincount;
		unsigned int analysis_icount = 3;
		#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 0;
		#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 0;
		#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 2;
		#else 
		analysis_begincount = 0;
		#endif
		
		for(unsigned int analysis_i=analysis_begincount; analysis_i<analysis_icount; analysis_i++){ total_time += timeelapsed_totals[i][analysis_i]; }
		
		if(edgesprocessed_totals[i] > 0){
			for(unsigned int analysis_i=analysis_begincount; analysis_i<analysis_icount; analysis_i++){ cout<<""<<((edgesprocessed_totals[i] / timeelapsed_totals[i][analysis_i]) / (1000))<<", "; }
			cout<<"[Iter "<<i<<" throughput: "<<((edgesprocessed_totals[i] / total_time) / (1000))<<"]";
			cout<<"[Iter "<<i<<" edges processed: "<<edgesprocessed_totals[i]<<"]";
			cout<<endl;
		}
	}
	cout<<endl<<">>> app::run_hw: total_edges_processed: "<<total_edges_processed<<" edges ("<<total_edges_processed/1000000<<" million edges)"<<endl;
	cout<<">>> app::run_hw: total_time_elapsed: "<<total_time_elapsed<<" ms ("<<total_time_elapsed/1000<<" s)"<<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput: "<<((total_edges_processed / total_time_elapsed) * (1000))<<" edges/sec ("<<((total_edges_processed / total_time_elapsed) / (1000))<<" million edges/sec)"<< RESET <<endl;			
	cout<< TIMINGRESULTSCOLOR <<">>> app::run_hw: throughput projection for 32 workers: ("<<((((total_edges_processed / total_time_elapsed) / (1000)) * 32) / NUM_PEs)<<" million edges/sec)"<< RESET <<endl;

	utilityobj->runsssp_sw(actvvs, vertexptrbuffer, edgedatabuffer, NumGraphIters, edgesprocessed_totals, &numValidIters);
		#if defined(CONFIG_ENABLECLASS_TOPNUSRCV_NUDSTV)
			verifyresults_splitdstvtxs(kvbuffer, globalparams.globalparamsV);
		#elif defined(CONFIG_ENABLECLASS_TOP_USRCV_UDSTV)
			verifyresults_splitdstvtxs(vdram, globalparams.globalparamsV);
		#else
			NOT DEFINED.
		#endif
	
	finish();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	graphobj->closefilesforreading();
	
	#ifdef TESTKERNEL
	cout<< TIMINGRESULTSCOLOR <<"================================================================== APP: THIS WAS A TEST RUN ==================================================================" << RESET << endl;
	#endif 
	return statsobj->timingandsummary(NAp, totaltime_ms);
}

void app::verifyresults(uint512_vec_dt * kvbuffer, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"app::verifyresults: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64];
	for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	
	uint512_vec_dt buff[REDUCEBUFFERSZ];
	for(unsigned int offset_kvs=0; offset_kvs<SRCVERTICESDATASZ_KVS; offset_kvs+=REDUCEBUFFERSZ){
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			buff[i] = kvbuffer[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + offset_kvs + i];
		}
		
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				unsigned int pos = i*VECTOR_SIZE + v;
				unsigned int vid1 = offset_kvs*REDUCESZ*VECTOR_SIZE + v*REDUCESZ + i*2;
				unsigned int vid2 = vid1 + 1;
				unsigned int vdata1 = buff[i].data[v].key;
				unsigned int vdata2 = buff[i].data[v].value;
				
				if(vdata1 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS3
					cout<<"app:verifyresults: vid1: "<<vid1<<",vdata1: "<<vdata1<<endl;
					#endif 
					vdatas[vdata1] += 1; 
				}
				if(vdata2 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS3
					cout<<"app:verifyresults: vid2: "<<vid2<<",vdata2: "<<vdata2<<endl;
					#endif
					vdatas[vdata2] += 1; 
				}
			}
		}
	}
	utilityobj->printvalues("app::verifyresults.vdatas: verifying results after kernel run", vdatas, 16);
	return;
}
void app::verifyresults_splitdstvtxs(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<endl<<"app::verifyresults_splitdstvtxs: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64]; for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int _BASEOFFSETKVS_DESTVERTICESDATA = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key;
		for(unsigned int k=0; k<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				unsigned int vdata1 = kvbuffer[i][_BASEOFFSETKVS_DESTVERTICESDATA + k].data[v].key;
				unsigned int vdata2 = kvbuffer[i][_BASEOFFSETKVS_DESTVERTICESDATA + k].data[v].value;
				
				unsigned int vdata1_tmp = utilityobj->READFROM_UINT(vdata1, 0, SIZEOF_VDATA0);
				unsigned int vdata2_tmp = utilityobj->READFROM_UINT(vdata2, 0, SIZEOF_VDATA1);
				
				if(vdata1_tmp < 64){
					vdatas[vdata1_tmp] += 1; 
				}
				if(vdata2_tmp < 64){
					vdatas[vdata2_tmp] += 1; 
				}
			}
		}
	}
	utilityobj->printvalues("app::verifyresults.vdatas: verifying results after kernel run", vdatas, 16);
	return;
}
void app::verifyresults_splitdstvtxs(uint512_vec_dt * vbuffer, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<endl<<"app::verifyresults_splitdstvtxs: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64]; for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	// for(unsigned int i=0; i<NUM_PEs; i++){
	unsigned int _BASEOFFSETKVS_DESTVERTICESDATA = vbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key;
	for(unsigned int k=0; k<NUM_PEs * NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2; k++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			unsigned int vdata1 = vbuffer[_BASEOFFSETKVS_DESTVERTICESDATA + k].data[v].key;
			unsigned int vdata2 = vbuffer[_BASEOFFSETKVS_DESTVERTICESDATA + k].data[v].value;
			
			unsigned int vdata1_tmp = utilityobj->READFROM_UINT(vdata1, 0, SIZEOF_VDATA0);
			unsigned int vdata2_tmp = utilityobj->READFROM_UINT(vdata2, 0, SIZEOF_VDATA1);
			
			if(vdata1_tmp < 64){
				vdatas[vdata1_tmp] += 1; 
			}
			if(vdata2_tmp < 64){
				vdatas[vdata2_tmp] += 1; 
			}
		}
	}
	// }
	utilityobj->printvalues("app::verifyresults.vdatas: verifying results after kernel run", vdatas, 16);
	return;
}
















