#include "host_debugger.h"

#ifdef _DEBUGMODE_HEADER
void printallparameters(dataset_t dataset){
	std::cout<<"host:: GRAPH: "<<dataset.graph_path<<std::endl;
	std::cout<<"host:: NUM VERTICES: "<<dataset.num_vertices<<std::endl;
	std::cout<<"host:: NUM EDGES: "<<dataset.num_edges<<std::endl;	
	std::cout<<"host:: NUMDRAMBANKS: "<<NUMDRAMBANKS<<std::endl;
	std::cout<<"host:: NUMWORKERS: "<<NUMWORKERS<<std::endl;
	std::cout<<"host:: NUMSUBWORKERS: "<<NUMSUBWORKERS<<std::endl;
	std::cout<<"host:: NUM_PARTITIONS: "<<NUM_PARTITIONS<<std::endl;
	std::cout<<"host:: NUMCPUTHREADS: "<<NUMCPUTHREADS<<std::endl;
	std::cout<<"host:: DATAWIDTH: "<<DATAWIDTH<<std::endl;
	std::cout<<"host:: VECTOR_SIZE: "<<VECTOR_SIZE<<std::endl;
	std::cout<<"host:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<std::endl;
	std::cout<<"host:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<std::endl;	
	std::cout<<"host:: PADDEDKVDATA_BATCHSIZE: "<<PADDEDKVDATA_BATCHSIZE<<std::endl;
	std::cout<<"host:: PADDEDKVDATA_BATCHSIZE_KVS: "<<PADDEDKVDATA_BATCHSIZE_KVS<<std::endl;
	std::cout<<"host:: _x2PADDEDKVDATA_SIZE: "<<_x2PADDEDKVDATA_SIZE<<std::endl;
	std::cout<<"host:: _x2PADDEDKVDATA_SIZE_KVS: "<<_x2PADDEDKVDATA_SIZE_KVS<<std::endl;
	std::cout<<"host:: _x3PADDEDKVDATA_SIZE: "<<_x3PADDEDKVDATA_SIZE<<std::endl;
	std::cout<<"host:: _x3PADDEDKVDATA_SIZE_KVS: "<<_x3PADDEDKVDATA_SIZE_KVS<<std::endl;	
	std::cout<<"host:: DRAMSZ: "<<DRAMSZ<<std::endl;
	std::cout<<"host:: DRAMSZ_KVS: "<<DRAMSZ_KVS<<std::endl;	
	std::cout<<"host:: KVDATA_RANGE: "<<KVDATA_RANGE<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_POW: "<<KVDATA_RANGE_POW<<std::endl;
	std::cout<<"host:: KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	std::cout<<"host:: KVBUFFERSZ_KVS: "<<KVBUFFERSZ_KVS<<std::endl;
	std::cout<<"host:: KVSRCBUFFERSZ: "<<KVSRCBUFFERSZ<<std::endl;
	std::cout<<"host:: KVSRCBUFFERSZ_KVS: "<<KVSRCBUFFERSZ_KVS<<std::endl;
	std::cout<<"host:: KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	std::cout<<"host:: KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<std::endl;
	std::cout<<"host:: KVDESTBUFFERSZ: "<<KVDESTBUFFERSZ<<std::endl;
	std::cout<<"host:: KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl;
	std::cout<<"host:: OFFSETSCODE: "<<OFFSETSCODE<<std::endl;
	std::cout<<"host:: SIZESCODE: "<<SIZESCODE<<std::endl;
	std::cout<<"host:: SIZEOFKV: "<<SIZEOFKV<<std::endl;
	std::cout<<"host:: SIZEOFVERTEXID: "<<SIZEOFVERTEXID<<std::endl;
	std::cout<<"host:: TREE_DEPTH: "<<TREE_DEPTH<<std::endl;
	std::cout<<"host:: KVSTATS_SIZE: "<<KVSTATS_SIZE<<std::endl;
	std::cout<<"host:: APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ<<std::endl;
	std::cout<<"host:: APPLYVERTEXBUFFERSZ_KVS: "<<APPLYVERTEXBUFFERSZ_KVS<<std::endl;	
	std::cout<<"host:: VERTICESDATA_BATCHSIZE: "<<VERTICESDATA_BATCHSIZE<<std::endl;
	std::cout<<"host:: VERTICESDATA_BUFFERSZ: "<<VERTICESDATA_BUFFERSZ<<std::endl;
	std::cout<<"host:: KVSPARTITIONBUFFERSZ: "<<KVSPARTITIONBUFFERSZ<<std::endl;	
	std::cout<<"host:: NUMSSDPARTITIONS: "<<NUMSSDPARTITIONS<<std::endl;	
	std::cout<<"host:: KVDATA_RANGE_PERSSDPARTITION_POW: "<<KVDATA_RANGE_PERSSDPARTITION_POW<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_PERSSDPARTITION: "<<KVDATA_RANGE_PERSSDPARTITION<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION0: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION0<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION1: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION1<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION2: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION2<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION3: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION3<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION4: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION4<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION5: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION5<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION6: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION6<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION7: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION7<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION8: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION8<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION9: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION9<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION10: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION10<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION11: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION11<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION12: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION12<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION13: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION13<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION14: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION14<<std::endl;
	std::cout<<"host:: KVDATA_RANGEOFFSET_FORSSDPARTITION15: "<<KVDATA_RANGEOFFSET_FORSSDPARTITION15<<std::endl;
	std::cout<<"host:: VERTICESDATASZ_FORDRAMX: "<<VERTICESDATASZ_FORDRAMX<<std::endl;
	std::cout<<"host:: MAXTREEDEPTH: "<<MAXTREEDEPTH<<std::endl;
	std::cout<<"host:: MESSAGES_SIZE: "<<MESSAGES_SIZE<<std::endl;	
	std::cout<<"host:: NUMSSDPARTITIONS: "<<NUMSSDPARTITIONS<<std::endl;
	std::cout<<"host:: MAXNUMSSDPARTITIONS: "<<MAXNUMSSDPARTITIONS<<std::endl;	
	std::cout<<"host:: NM: "<<NM<<std::endl;
	std::cout<<"host:: DESTTMPKVBUFFERSZ: "<<DESTTMPKVBUFFERSZ<<std::endl;	
	std::cout<<"host:: DESTTMPKVBUFFERSZ_KVS: "<<DESTTMPKVBUFFERSZ_KVS<<std::endl;	
	std::cout<<"host:: TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl;	
	std::cout<<"host:: TOTALDESTTMPKVBUFFERSZ_KVS: "<<TOTALDESTTMPKVBUFFERSZ_KVS<<std::endl;
	std::cout<<"host:: NUMWORKERS_APPLYPH: "<<NUMWORKERS_APPLYPH<<std::endl;	
	std::cout<<"host:: NUMSUBWORKERS_APPLYPH: "<<NUMSUBWORKERS_APPLYPH<<std::endl;
	#ifdef LOCKE
	std::cout<<"host:: LOCKE DEFINED"<<std::endl;
	#else 
	std::cout<<"host:: LOCKE NOT DEFINED"<<std::endl;	
	#endif 
	#ifdef STREAMEDGESSETUP
	std::cout<<"host:: STREAMEDGESSETUP DEFINED"<<std::endl;
	#else 
	std::cout<<"host:: STREAMEDGESSETUP NOT DEFINED"<<std::endl;	
	#endif	
	std::cout<<"host:: sizeof(keyvalue_t): "<<sizeof(keyvalue_t)<<" bytes ("<<(sizeof(keyvalue_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"host:: sizeof(vertex_t): "<<sizeof(vertex_t)<<" bytes ("<<(sizeof(vertex_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"host:: sizeof(edge_t): "<<sizeof(edge_t)<<" bytes ("<<(sizeof(edge_t) * 8)<<" bits)"<<std::endl;
	std::cout<<std::endl;
	#ifdef PR_ALGORITHM
	cout<<"printallparameters:: PR_ALGORITHM"<<endl;
	#elif defined(BFS_ALGORITHM)
	cout<<"printallparameters:: BFS_ALGORITHM"<<endl;
	#elif defined(BC_ALGORITHM)
	cout<<"printallparameters:: BC_ALGORITHM"<<endl;
	#else 
	cout<<"printallparameters:: Algotihm not set"<<endl; exit(EXIT_FAILURE);
	#endif 
	#ifdef _LARGEDATASET
	#ifdef _DEBUGMODE_PRINTS_TMP
	std::cout<<"host:: WARNING: _DEBUGMODE_PRINTS_TMP enabled when running with large dataset can cause lots of printing, disable it first"<<std::endl;
	// exit(EXIT_FAILURE);
	#endif 
	#endif
	#ifdef _LARGEDATASET_1B
	if(sizeof(edge_t) == sizeof(unsigned int)){
		std::cout<<"host:: WARNING: WRONG DATA TYPE FOR EDGE_T. EXITING..."<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif 
	#ifdef _LARGEDATASET_4B
	if(sizeof(edge_t) == sizeof(unsigned int)){
		std::cout<<"host:: WARNING: WRONG DATA TYPE FOR EDGE_T. EXITING..."<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif	
	/** #ifdef _GENERATE2DGRAPH
	std::cout<<"host:: WARNING: _GENERATE2DGRAPH is enabled. Are you sure that's what you want?... (y/n)"<<std::endl;
	string response;
	getline(cin, response);
	std::cout<<"response: "<<response<<std::endl;
	if(response == "y"){} else { exit(EXIT_FAILURE); }
	#endif */
	#ifdef PROCESS_PHASE
	std::cout<<"host:: WARNING: PROCESS_PHASE is enabled. Are you sure that's what you want?... (y/n)"<<std::endl;
	string response;
	getline(cin, response);
	std::cout<<"response: "<<response<<std::endl;
	if(response == "y"){} else { exit(EXIT_FAILURE); }
	#endif
	/** #if (defined(ACTGRAPH_SETUP) & defined(PR_ALGORITHM))
	if(MAXNUMSSDPARTITIONS != NUMWORKLOADBANKS){
	std::cout<<"host:: WARNING: MAXNUMSSDPARTITIONS must be equal to NUMWORKLOADBANKS . EXITING..."<<std::endl;
	exit(EXIT_FAILURE);
	}
	#endif */
	std::cout<<"host:: KVDRAMSZ: "<<KVDRAMSZ<<std::endl;
	std::cout<<"host:: KVDRAMSZ_KVS: "<<KVDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: KVDRAMWORKSPACESZ: "<<KVDRAMWORKSPACESZ<<std::endl;
	std::cout<<"host:: KVDRAMWORKSPACESZ_KVS: "<<KVDRAMWORKSPACESZ_KVS<<std::endl;	
	// std::cout<<"host:: CAPSULEDRAMSZ: "<<CAPSULEDRAMSZ<<std::endl;
	// std::cout<<"host:: CAPSULEDRAMSZ_KVS: "<<CAPSULEDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: STATSDRAMSZ: "<<STATSDRAMSZ<<std::endl;
	// std::cout<<"host:: STATSDRAMSZ_KVS: "<<STATSDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAM: "<<KVDRAMSZ<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAM_KVS: "<<KVDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAMWORKSPACE: "<<KVDRAMWORKSPACESZ<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAMWORKSPACE_KVS: "<<KVDRAMWORKSPACESZ_KVS<<std::endl;	
	// std::cout<<"host:: BASEOFFSET_CAPSULEDRAM: "<<CAPSULEDRAMSZ<<std::endl;
	// std::cout<<"host:: BASEOFFSET_CAPSULEDRAM_KVS: "<<CAPSULEDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_STATSDRAM: "<<STATSDRAMSZ<<std::endl;
	// std::cout<<"host:: BASEOFFSET_STATSDRAM_KVS: "<<STATSDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<std::endl;	
	std::cout<<"host:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<std::endl;	
	std::cout<<"host:: KVSTATSDRAMSZ: "<<KVSTATSDRAMSZ<<std::endl;
	return;
}

void printkvdrams(uint512_vec_dt * kvdram){
	// Print results after Kernel run
	for(unsigned int i = 0 ; i<(16/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<i<<"].data["<<j<<"].key: "<<kvdram[i].data[j].key<<std::endl;
		}
    }
	for(unsigned int i = 0 ; i<(16/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<(PADDEDKVDATA_BATCHSIZE_KVS + i)<<"].data["<<j<<"].key: "<<kvdram[(PADDEDKVDATA_BATCHSIZE_KVS + i)].data[j].key<<std::endl;
		}
    }
	std::cout<<std::endl;
	return;
}

void printkvdramsII(uint512_vec_dt * kvdram, metadata_t * kvstats){
	#ifdef _DEBUGMODE_CHECKS
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){
	cout<<">>> partition: "<<i<<endl;
		for(unsigned int k = ((PADDEDKVDATA_BATCHSIZE + kvstats[i].offset) / VECTOR_SIZE) ; k<(((PADDEDKVDATA_BATCHSIZE + kvstats[i].offset) / VECTOR_SIZE) + 2) ; k++){
			for(unsigned int j=0; j<VECTOR_SIZE; j++){
				std::cout<<"kvdram["<<k<<"].data["<<j<<"].key: "<<kvdram[k].data[j].key<<std::endl;
			}
		}
	}
	#endif 
	return;
}

void printkvdramsIII(uint512_vec_dt * kvdram){
	// Print results after Kernel run
	for(unsigned int i = 0 ; i<(10/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<i<<"].data["<<j<<"].key: "<<kvdram[i].data[j].key<<std::endl;
		}
    }
	std::cout<<std::endl;
	return;
}

void printstatsIII(unsigned int size, metadata_t * kvstats){
	unsigned int totalsize = 0;
	for(unsigned int p = 0 ; p<size; p++){ std::cout<<"kvstats["<<p<<"]: [offset: "<<kvstats[p].offset<<", size:"<<kvstats[p].size<<"]"<<std::endl; }
	for(unsigned int p = 0 ; p<KVSTATS_SIZE; p++){ totalsize += kvstats[p].size; } 
	cout<<"printstatsIII: total number of kvs in kvstats: "<<totalsize<<endl;
	std::cout<<std::endl;
	return;
}

void print_graph(string path_graph, edge_t begin, edge_t size){
	cout<<"print_graph FUNC. path_graph: "<<path_graph<<endl;
	std::ifstream file(path_graph);
	edge_t linecnt = 0;
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			if(linecnt >= (begin)){
				printf("%s \n", line.c_str());
			} else {}
			linecnt += 1; 
			if(linecnt >= ((begin) + size)){ break; }
		}
		file.close();
	}
	return;
}

void check_kvdram2(uint512_vec_dt * kvdramX, metadata_t * kvstats, vertex_t vbegin, vertex_t applyvertexbuffersz, unsigned int treedepth, string message){
	cout<<endl<<"DEBUGGER::check_kvdram2:: checking kvdram for errors... vbegin: "<<vbegin<<", applyvertexbuffersz: "<<applyvertexbuffersz<<", message: "<<message<<endl;
	
	unsigned int finalnum_partitions = pow(NUM_PARTITIONS, treedepth);
	
	cout<<"DEBUGGER::check_kvdram2:: kvstats["<<(finalnum_partitions - 1)<<"].offset: "<<kvstats[finalnum_partitions - 1].offset<<endl;
	cout<<"DEBUGGER::check_kvdram2:: kvstats["<<(finalnum_partitions - 1)<<"].size: "<<kvstats[finalnum_partitions - 1].size<<endl;
	cout<<"DEBUGGER::check_kvdram2:: kvstats["<<(finalnum_partitions - 1)<<"].offset + kvstats["<<(finalnum_partitions - 1)<<"].size: "<<(kvstats[finalnum_partitions - 1].offset + kvstats[finalnum_partitions - 1].size)<<endl;		
	
	unsigned int baseaddr_kvs = 0;
	if((treedepth % 2) == 1){
		baseaddr_kvs = PADDEDKVDATA_BATCHSIZE_KVS; // FIXME
	} else {
		baseaddr_kvs = 0;
	}
	unsigned int items_counted = 0;
	
	for(unsigned int i=0; i<finalnum_partitions; i++){
		for(unsigned int j=(kvstats[i].offset / VECTOR_SIZE); j<((kvstats[i].offset + kvstats[i].size) / VECTOR_SIZE); j++){
			for(unsigned int k=0; k<VECTOR_SIZE; k++){
				if(((kvdramX[baseaddr_kvs + j].data[k].key - vbegin) - (i * applyvertexbuffersz)) > applyvertexbuffersz){ 
					cout<<"!!! DEBUGGER::check_kvdram2:: ERROR: out of bounds seen in kvdram:"<<endl;
					cout<<"kvdramX["<<(baseaddr_kvs + j)<<"].data["<<k<<"].key: "<<kvdramX[baseaddr_kvs + j].data[k].key<<endl;
					cout<<"kvitemoffset: "<<(j * VECTOR_SIZE + k)<<endl;
					cout<<endl;
					exit(EXIT_FAILURE);
				}
				items_counted += 1;
			}
		}
	}
	cout<<"DEBUGGER::check_kvdram2::"<<message<<" CHECK COMPLETED SUCCESSFULLY. "<<items_counted<<" key-values counted"<<endl;
	return;
}

void checkforzeroornegative(int val, string message){
	if(val <= 0) {
		cout<<""<<message<<": ERROR. zero or negative value. val: "<<val<<endl;
		exit (EXIT_FAILURE);
	}
	return;
}

void printfirstkvs(string message, uint512_vec_dt kvs, unsigned int count){
	for(unsigned int j=0; j<count; j++){
		cout<<"Host::"<<message<<":: kvs.data["<<j<<"].key "<<kvs.data[j].key<<", kvs.data["<<j<<"].value: "<<kvs.data[j].value<<endl;
	}	
	return;
}

void printERROR(string message){
	cout<<"printERROR: "<<message<<""<<endl;
	exit(EXIT_FAILURE);
}
#endif


