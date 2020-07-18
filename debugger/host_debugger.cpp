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
	std::cout<<"host:: KVDATA_RANGE: "<<KVDATA_RANGE<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_POW: "<<KVDATA_RANGE_POW<<std::endl;
	std::cout<<"host:: TREE_DEPTH: "<<TREE_DEPTH<<std::endl;
	std::cout<<"host:: KVSTATS_SIZE: "<<KVSTATS_SIZE<<std::endl;
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
	std::cout<<"host:: MESSAGES_SIZE: "<<MESSAGES_SIZE<<std::endl;	
	std::cout<<"host:: NUMSSDPARTITIONS: "<<NUMSSDPARTITIONS<<std::endl;
	std::cout<<"host:: MAXNUMSSDPARTITIONS: "<<MAXNUMSSDPARTITIONS<<std::endl;
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
	std::cout<<"host:: CAPSULESZ: "<<CAPSULESZ<<std::endl;
	std::cout<<"host:: CAPSULESZ_KVS: "<<CAPSULESZ_KVS<<std::endl;
	std::cout<<"host:: STATSDRAMSZ: "<<STATSDRAMSZ<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAM: "<<KVDRAMSZ<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAM_KVS: "<<KVDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAMWORKSPACE: "<<KVDRAMWORKSPACESZ<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAMWORKSPACE_KVS: "<<KVDRAMWORKSPACESZ_KVS<<std::endl;	
	std::cout<<"host:: BASEOFFSET_CAPSULES: "<<BASEOFFSET_CAPSULES<<std::endl;
	std::cout<<"host:: BASEOFFSET_CAPSULES_KVS: "<<BASEOFFSET_CAPSULES_KVS<<std::endl;	
	std::cout<<"host:: BASEOFFSET_STATSDRAM: "<<BASEOFFSET_STATSDRAM<<std::endl;
	std::cout<<"host:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<std::endl;	
	std::cout<<"host:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<std::endl;	
	std::cout<<"host:: PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<std::endl;	
	std::cout<<"host:: PADDEDKVSOURCEDRAMSZ_KVS: "<<PADDEDKVSOURCEDRAMSZ_KVS<<std::endl;
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
			std::cout<<"kvdram["<<(KVDATA_BATCHSIZE_KVS + i)<<"].data["<<j<<"].key: "<<kvdram[(KVDATA_BATCHSIZE_KVS + i)].data[j].key<<std::endl;
		}
    }
	std::cout<<std::endl;
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


