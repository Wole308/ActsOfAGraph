#include <unistd.h>
#include <vector>
#include "../src/host_common.h" 
#include "../debugger/host_debugger.h"
#include "../datasets/datasets.h"
#include "../preprocess/preprocess.h"
#include "host_functions.h"
#include "EdgeProcess.h" // Grafboost header
#ifdef FPGA_IMPL
// #include "xcl2.hpp"
#include "../xcl.h"
#endif

float JP_RESET_PROB = 0.15;
float JP_TOLERANCE = 1.0E-2;

// Helper Functions
int hallignup_KV(int val){
	int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}

int halligndown_KV(int val){
	int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}

int hmin(size_t val1, size_t val2){
	if(val1 < 0){ val1 = 0; }
	if(val2 < 0){ val2 = 0; }
	if(val1 < val2){ return val1; }
	else { return val2; }
}

size_t hmax(size_t val1, size_t val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}

size_t hmax_array(unsigned int kvs_weofs[NUMSSDPARTITIONS], size_t begin, size_t size){
	size_t currmax = 0;
	for(size_t i=begin; i<(begin + size); i++){
		if(kvs_weofs[i] > currmax){ currmax = kvs_weofs[i]; }
	}
	return currmax;
}

size_t hsub(size_t val1, size_t val2){
	if(val1 < val2){ return 0; }
	else { return val1 - val2; }
}

size_t hceildiv(size_t val1, size_t val2){
	return (val1 + (val2 - 1)) / val2;
}

vertex_t GETKVDATA_RANGE_FORSSDPARTITION_(unsigned int ssdpartitonid){
	vertex_t range = 0;
	
if(ssdpartitonid == 0){
		range = KVDATA_RANGE_FORSSDPARTITION0;
	}
else if(ssdpartitonid == 1){
		range = KVDATA_RANGE_FORSSDPARTITION1;
	}
else if(ssdpartitonid == 2){
		range = KVDATA_RANGE_FORSSDPARTITION2;
	}
else if(ssdpartitonid == 3){
		range = KVDATA_RANGE_FORSSDPARTITION3;
	}
else if(ssdpartitonid == 4){
		range = KVDATA_RANGE_FORSSDPARTITION4;
	}
else if(ssdpartitonid == 5){
		range = KVDATA_RANGE_FORSSDPARTITION5;
	}
else if(ssdpartitonid == 6){
		range = KVDATA_RANGE_FORSSDPARTITION6;
	}
else if(ssdpartitonid == 7){
		range = KVDATA_RANGE_FORSSDPARTITION7;
	}
else if(ssdpartitonid == 8){
		range = KVDATA_RANGE_FORSSDPARTITION8;
	}
else if(ssdpartitonid == 9){
		range = KVDATA_RANGE_FORSSDPARTITION9;
	}
else if(ssdpartitonid == 10){
		range = KVDATA_RANGE_FORSSDPARTITION10;
	}
else if(ssdpartitonid == 11){
		range = KVDATA_RANGE_FORSSDPARTITION11;
	}
else if(ssdpartitonid == 12){
		range = KVDATA_RANGE_FORSSDPARTITION12;
	}
else if(ssdpartitonid == 13){
		range = KVDATA_RANGE_FORSSDPARTITION13;
	}
else if(ssdpartitonid == 14){
		range = KVDATA_RANGE_FORSSDPARTITION14;
	}
else if(ssdpartitonid == 15){
		range = KVDATA_RANGE_FORSSDPARTITION15;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"Kernel::GETRANGE should never get here. GETRANGE 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	return range;
}	

vertex_t GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid){
	vertex_t rangeoffset = 0;
	
if(ssdpartitonid == 0){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION0;
	}
else if(ssdpartitonid == 1){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION1;
	}
else if(ssdpartitonid == 2){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION2;
	}
else if(ssdpartitonid == 3){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION3;
	}
else if(ssdpartitonid == 4){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION4;
	}
else if(ssdpartitonid == 5){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION5;
	}
else if(ssdpartitonid == 6){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION6;
	}
else if(ssdpartitonid == 7){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION7;
	}
else if(ssdpartitonid == 8){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION8;
	}
else if(ssdpartitonid == 9){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION9;
	}
else if(ssdpartitonid == 10){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION10;
	}
else if(ssdpartitonid == 11){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION11;
	}
else if(ssdpartitonid == 12){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION12;
	}
else if(ssdpartitonid == 13){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION13;
	}
else if(ssdpartitonid == 14){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION14;
	}
else if(ssdpartitonid == 15){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION15;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"host_functions::GETRANGE should never get here. GETRANGE 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	return rangeoffset;
}

vertex_t GETAPPLYVERTEXBUFFERSZ_FORSSDPARTITION_(unsigned int ssdpartitonid){
	vertex_t size = 0;
	
if(ssdpartitonid == 0){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION0;
	}
else if(ssdpartitonid == 1){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION1;
	}
else if(ssdpartitonid == 2){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION2;
	}
else if(ssdpartitonid == 3){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION3;
	}
else if(ssdpartitonid == 4){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION4;
	}
else if(ssdpartitonid == 5){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION5;
	}
else if(ssdpartitonid == 6){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION6;
	}
else if(ssdpartitonid == 7){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION7;
	}
else if(ssdpartitonid == 8){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION8;
	}
else if(ssdpartitonid == 9){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION9;
	}
else if(ssdpartitonid == 10){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION10;
	}
else if(ssdpartitonid == 11){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION11;
	}
else if(ssdpartitonid == 12){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION12;
	}
else if(ssdpartitonid == 13){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION13;
	}
else if(ssdpartitonid == 14){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION14;
	}
else if(ssdpartitonid == 15){
		size = APPLYVERTEXBUFFERSZ_FORSSDPARTITION15;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"Kernel:: GETAPPLYVERTEXBUFFERSZ_FORSSDPARTITION_ should never get here.  GETAPPLYVERTEXBUFFERSZ_FORSSDPARTITION_ 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	return size;
}

unsigned int GETTREEDEPTH_(unsigned int ssdpartitonid){
	unsigned int depth = 0;
	
if(ssdpartitonid == 0){
		depth = TREEDEPTH_FORSSDPARTITION0;
	}
else if(ssdpartitonid == 1){
		depth = TREEDEPTH_FORSSDPARTITION1;
	}
else if(ssdpartitonid == 2){
		depth = TREEDEPTH_FORSSDPARTITION2;
	}
else if(ssdpartitonid == 3){
		depth = TREEDEPTH_FORSSDPARTITION3;
	}
else if(ssdpartitonid == 4){
		depth = TREEDEPTH_FORSSDPARTITION4;
	}
else if(ssdpartitonid == 5){
		depth = TREEDEPTH_FORSSDPARTITION5;
	}
else if(ssdpartitonid == 6){
		depth = TREEDEPTH_FORSSDPARTITION6;
	}
else if(ssdpartitonid == 7){
		depth = TREEDEPTH_FORSSDPARTITION7;
	}
else if(ssdpartitonid == 8){
		depth = TREEDEPTH_FORSSDPARTITION8;
	}
else if(ssdpartitonid == 9){
		depth = TREEDEPTH_FORSSDPARTITION9;
	}
else if(ssdpartitonid == 10){
		depth = TREEDEPTH_FORSSDPARTITION10;
	}
else if(ssdpartitonid == 11){
		depth = TREEDEPTH_FORSSDPARTITION11;
	}
else if(ssdpartitonid == 12){
		depth = TREEDEPTH_FORSSDPARTITION12;
	}
else if(ssdpartitonid == 13){
		depth = TREEDEPTH_FORSSDPARTITION13;
	}
else if(ssdpartitonid == 14){
		depth = TREEDEPTH_FORSSDPARTITION14;
	}
else if(ssdpartitonid == 15){
		depth = TREEDEPTH_FORSSDPARTITION15;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"Kernel:: GETTREEDEPTH_ should never get here.  GETTREEDEPTH_ 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	return depth;
}

float pr(vertexprop_t srcp, vertex_t outdegree){
	#ifdef _DEBUGMODE_CHECKS2
	checkforzeroornegative(outdegree, "Host::pr:: checking 'outdegree' for zero or negative...");
	#endif
	return ((1.0 - JP_RESET_PROB) / outdegree) * srcp.data;
}

void getVs(uint512_vec_dt * cData_kvdramV, vertex_t vbatch_kvs, vertex_t * firstv, vertex_t * secondv, vertex_t * lastv){
	*firstv = cData_kvdramV[0].data[0].key;
	*secondv = cData_kvdramV[0].data[1].key;
	*lastv = cData_kvdramV[vbatch_kvs-1].data[0].key;
	std::cout <<"Host::getVs: *firstv: "<<*firstv<<endl;
	std::cout <<"Host::getVs: *secondv: "<<*secondv<<endl;
	std::cout <<"Host::getVs: *lastv: "<<*lastv<<endl;
	return;
}

// File Access Methods
void loadverticesfromfile(unsigned int iteration_idx, FILE * nvmeFd_vertices_r, vertexprop_t * verticesbuffer, vertex_t batchsize){
	std::cout << "Host::loadverticesfromfile. loading vertices from file... vertexprop offset: "<<(iteration_idx * VERTICESDATA_BATCHSIZE)<<", batchsize: "<<batchsize<<endl;

	if(fseek (nvmeFd_vertices_r, (size_t)(iteration_idx * VERTICESDATA_BATCHSIZE * sizeof(vertexprop_t)), SEEK_SET) != 0){ cout<<"ERROR 34"<<endl; exit(EXIT_FAILURE); };
	if(fread (verticesbuffer, (size_t)(batchsize * sizeof(vertexprop_t)), 1, nvmeFd_vertices_r) != 1){ cout<<"loadverticesfromfile::ERROR 35"<<endl; exit(EXIT_FAILURE); };

	std::cout <<"Host::loadverticesfromfile. verticesbuffer[0].offset: "<<verticesbuffer[0].offset<<endl;
	std::cout <<"Host::loadverticesfromfile. verticesbuffer[1].offset: "<<verticesbuffer[1].offset<<endl;
	std::cout <<"Host::loadverticesfromfile. verticesbuffer["<<(batchsize-1)<<"].offset: "<<verticesbuffer[batchsize-1].offset<<endl;
	return;
}

void loadverticesfromfileII(unsigned int iteration_idx, FILE * nvmeFd_vertices_r, uint512_vec_dt * kvdram, vertex_t kvoffset, vertex_t kvbatchsize){
	std::cout << "Host::loadverticesfromfileII. loading vertices from file... vertexprop offset: "<<(iteration_idx * VERTICESDATA_BATCHSIZE)<<", kvoffset: "<<kvoffset<<", kvbatchsize: "<<kvbatchsize<<endl;
	unsigned int kvoffset_kvs = kvoffset / VECTOR_SIZE;
	unsigned int kvbatchsize_kvs = kvbatchsize / VECTOR_SIZE;
	std::cout <<"Host::loadverticesfromfileII: kvoffset: "<<kvoffset<<endl;
	std::cout <<"Host::loadverticesfromfileII: kvbatchsize: "<<kvbatchsize<<endl;
	
	vertexpropset_t * kvdramTMP = new vertexpropset_t[kvbatchsize_kvs];
	if(kvbatchsize != 0){ if(fread (kvdramTMP, (size_t)(kvbatchsize * sizeof(vertexprop_t)), 1, nvmeFd_vertices_r) != 1){ cout<<"loadverticesfromfileII::ERROR 35"<<endl; exit(EXIT_FAILURE); } }
	
	// transfer into kv mode
	edge_t firstvoffset = kvdramTMP[0].data[0].offset;
	std::cout <<"Host::loadverticesfromfileII: firstvoffset: "<<firstvoffset<<endl;
	for(unsigned int i_kvs=0; i_kvs<kvbatchsize_kvs; i_kvs++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			kvdram[kvoffset_kvs + i_kvs].data[j].key = kvdramTMP[i_kvs].data[j].offset - firstvoffset;
			#ifdef PR_ALGORITHM
			kvdram[kvoffset_kvs + i_kvs].data[j].value = kvdramTMP[i_kvs].data[j].data;
			#elif defined(BFS_ALGORITHM)
			kvdram[kvoffset_kvs + i_kvs].data[j].value = ACTSINFINITY; // should be set to infinity
			#else 
			cout<<"Host::loadverticesfromfileII: Algotihm not set"<<endl; exit(EXIT_FAILURE);
			#endif 
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout <<"Host::loadverticesfromfileII. kvdramTMP["<<0<<"].data[0].offset: "<<kvdramTMP[0].data[0].offset<<", kvdramTMP["<<0<<"].data[0].data: "<<kvdramTMP[0].data[0].data<<endl;			
	std::cout <<"Host::loadverticesfromfileII. kvdramTMP["<<0<<"].data[1].offset: "<<kvdramTMP[0].data[1].offset<<", kvdramTMP["<<0<<"].data[1].data: "<<kvdramTMP[0].data[1].data<<endl;
	std::cout <<"Host::loadverticesfromfileII. kvdramTMP["<<0<<"].data[2].offset: "<<kvdramTMP[0].data[2].offset<<", kvdramTMP["<<0<<"].data[2].data: "<<kvdramTMP[0].data[2].data<<endl;
	
	std::cout <<"Host::loadverticesfromfileII. kvdram["<<kvoffset_kvs<<"].data[0].key: "<<kvdram[kvoffset_kvs].data[0].key<<", kvdram["<<kvoffset_kvs<<"].data[0].value: "<<kvdram[kvoffset_kvs].data[0].value<<endl;			
	std::cout <<"Host::loadverticesfromfileII. kvdram["<<kvoffset_kvs<<"].data[1].key: "<<kvdram[kvoffset_kvs].data[1].key<<", kvdram["<<kvoffset_kvs<<"].data[1].value: "<<kvdram[kvoffset_kvs].data[1].value<<endl;
	std::cout <<"Host::loadverticesfromfileII. kvdram["<<kvoffset_kvs<<"].data[2].key: "<<kvdram[kvoffset_kvs].data[2].key<<", kvdram["<<kvoffset_kvs<<"].data[2].value: "<<kvdram[kvoffset_kvs].data[2].value<<endl;
	std::cout <<"Host::loadverticesfromfileII. kvdram["<<(kvoffset_kvs + kvbatchsize_kvs - 1)<<"].data[7].key: "<<kvdram[(kvoffset_kvs + kvbatchsize_kvs - 1)].data[7].key<<", kvdram["<<(kvoffset_kvs + kvbatchsize_kvs - 1)<<"].data[7].value: "<<kvdram[(kvoffset_kvs + kvbatchsize_kvs - 1)].data[7].value<<endl;
	std::cout << endl;
	#endif 
	return;
}

void writeverticestofileII(unsigned int iteration_idx, FILE * nvmeFd_vertices_w, uint512_vec_dt * kvdram, vertex_t kvoffset, vertex_t kvbatchsize){
	std::cout << "Host::writeverticestofileII. loading vertices from file... vertexprop offset: "<<(iteration_idx * VERTICESDATA_BATCHSIZE)<<", kvoffset: "<<kvoffset<<", kvbatchsize: "<<kvbatchsize<<endl;
	unsigned int kvoffset_kvs = kvoffset / VECTOR_SIZE;
	unsigned int kvbatchsize_kvs = kvbatchsize / VECTOR_SIZE;
	
	if(fwrite (&kvdram[kvoffset_kvs], (size_t)(kvbatchsize * sizeof(keyvalue_t)), 1, nvmeFd_vertices_w) != 1){ cout<<"writeverticestofileII::ERROR 35"<<endl; exit(EXIT_FAILURE); };
	std::cout <<"Host::writeverticestofileII. kvdram["<<kvoffset_kvs<<"].data[0].key: "<<kvdram[kvoffset_kvs].data[0].key<<", kvdram["<<kvoffset_kvs<<"].data[0].value: "<<kvdram[kvoffset_kvs].data[0].value<<endl;			
	std::cout <<"Host::writeverticestofileII. kvdram["<<kvoffset_kvs<<"].data[1].key: "<<kvdram[kvoffset_kvs].data[1].key<<", kvdram["<<kvoffset_kvs<<"].data[1].value: "<<kvdram[kvoffset_kvs].data[1].value<<endl;
	std::cout <<"Host::writeverticestofileII. kvdram["<<kvoffset_kvs<<"].data[2].key: "<<kvdram[kvoffset_kvs].data[2].key<<", kvdram["<<kvoffset_kvs<<"].data[2].value: "<<kvdram[kvoffset_kvs].data[2].value<<endl;
	std::cout <<"Host::writeverticestofileII. kvdram["<<(kvoffset_kvs + kvbatchsize_kvs - 1)<<"].data[7].key: "<<kvdram[(kvoffset_kvs + kvbatchsize_kvs - 1)].data[7].key<<", kvdram["<<(kvoffset_kvs + kvbatchsize_kvs - 1)<<"].data[7].value: "<<kvdram[(kvoffset_kvs + kvbatchsize_kvs - 1)].data[7].value<<endl;		
	std::cout << endl;
	return;
}

void loadedgesfromfile(unsigned int iteration_idx, FILE * nvmeFd_edges_r, unsigned int beginoffset, unsigned int endoffset, edgeprop_t * edgesbuffer){
	std::cout <<endl<< "Host::loadedgesfromfile. loading edges from file... edgeprop offset: "<<beginoffset<<endl;
	if(fseek (nvmeFd_edges_r, (size_t)(beginoffset * sizeof(edgeprop_t)), SEEK_SET) != 0){ cout<<"loadedgesfromfile::ERROR 36"<<endl; exit(EXIT_FAILURE); };
	if(fread (edgesbuffer, (size_t)((endoffset - beginoffset) * sizeof(edgeprop_t)), 1, nvmeFd_edges_r) != 1){ cout<<"loadedgesfromfile::ERROR 37"<<endl; exit(EXIT_FAILURE); };
	return;
}

void loadedgesfromfileII(unsigned int iteration_idx, FILE * nvmeFd_edges_r, uint512_vec_dt * kvdram, edge_t kvoffset, vertex_t kvbatchsize){
	std::cout <<endl<< "Host::loadedgesfromfileII. loading edges from file... kvoffset: "<<kvoffset<<", kvbatchsize: "<<kvbatchsize<<endl;
	unsigned int kvoffset_kvs = kvoffset / VECTOR_SIZE;
	
	if(kvbatchsize != 0){ if(fread (&kvdram[kvoffset_kvs], (size_t)(kvbatchsize * sizeof(keyvalue_t)), 1, nvmeFd_edges_r) != 1){ cout<<"loadedgesfromfileII::ERROR 37"<<endl; exit(EXIT_FAILURE); } }
	std::cout <<"Host::loadedgesfromfileII. kvdram["<<kvoffset_kvs<<"].data[0].key: "<<kvdram[kvoffset_kvs].data[0].key<<", kvdram["<<kvoffset_kvs<<"].data[0].value: "<<kvdram[kvoffset_kvs].data[0].value<<endl;
	std::cout <<"Host::loadedgesfromfileII. kvdram["<<kvoffset_kvs<<"].data[1].key: "<<kvdram[kvoffset_kvs].data[1].key<<", kvdram["<<kvoffset_kvs<<"].data[1].value: "<<kvdram[kvoffset_kvs].data[1].value<<endl;
	std::cout <<"Host::loadedgesfromfileII. kvdram["<<kvoffset_kvs<<"].data[2].key: "<<kvdram[kvoffset_kvs].data[2].key<<", kvdram["<<kvoffset_kvs<<"].data[2].value: "<<kvdram[kvoffset_kvs].data[2].value<<endl;
	std::cout << endl;
	return;
}

void storevertexupdatestofileIII(uint512_vec_dt * kvdram, unsigned int kvoffset, metadata_t * kvstats, FILE * nvmeFd_kvs_w[MAXNUMSSDPARTITIONS]){
	std::cout << endl<<"Host::storevertexupdatestofileIII. saving vertex updates to file..." << std::endl;
	unsigned int totalnumsaved = 0;
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){
		#ifdef _DEBUGMODE_CHECKS
		std::cout << "Host::storevertexupdatestofileIII. first entry: kvdram["<<((kvoffset + kvstats[i].offset) / VECTOR_SIZE)<<"].data[0].key: " << kvdram[((kvoffset + kvstats[i].offset) / VECTOR_SIZE)].data[0].key << std::endl;
		#endif 
		if(kvstats[i].size > 0){ if(fwrite (&kvdram[((kvoffset + kvstats[i].offset) / VECTOR_SIZE)], (size_t)(kvstats[i].size * sizeof(keyvalue_t)), 1, nvmeFd_kvs_w[i]) != 1){ cout<<"storevertexupdatestofileIII::ERROR seen 56. kvstats[i].offset: "<<kvstats[i].offset<<", kvstats[i].size: "<<kvstats[i].size<<endl; exit(EXIT_FAILURE); } }
		totalnumsaved += kvstats[i].size;
	}
	std::cout << "Host::storevertexupdatestofileIII. "<<totalnumsaved<<" key-value pairs saved to "<<NUMSSDPARTITIONS<<" partitions in file"<<endl;
	return;
}

void storevertexupdatestofileIV(vector<keyvalue_t> & kvs_updates, FILE * nvmeFd_vus_w){
	std::cout << endl<<"Host::processedgesbatch. saving vertex updates to file..." << std::endl;
	unsigned long offsets_bytes;
	
	fseek (nvmeFd_vus_w, 0, SEEK_END);
	offsets_bytes=ftell (nvmeFd_vus_w);
	cout<<"[BEFORE] Host::storevertexupdatestofile:: key-value pairs: "<<(offsets_bytes / sizeof(keyvalue_t))<<" ("<<offsets_bytes<<" bytes)"<<endl;

	std::cout<<"HOST:: logging vertex updates from kvs_updates into SSD (key-value pairs)... "<<endl;
	fseek (nvmeFd_vus_w, (size_t)offsets_bytes, SEEK_SET);
	if(fwrite (kvs_updates.data(), (size_t)(kvs_updates.size() * sizeof(keyvalue_t)), 1, nvmeFd_vus_w) != 1){ cout<<"storevertexupdatestofile::ERROR seen 54"<<endl; exit(EXIT_FAILURE); };
	kvs_updates.clear();

	unsigned long totaloffsets_bytes = 0;
	fseek (nvmeFd_vus_w, 0, SEEK_END);
	offsets_bytes=ftell (nvmeFd_vus_w);
	totaloffsets_bytes += offsets_bytes / sizeof(keyvalue_t);
	cout<<"[AFTER] Host::storevertexupdatestofile:: key-value pairs: "<<(offsets_bytes / sizeof(keyvalue_t))<<" ("<<offsets_bytes<<" bytes)"<<endl;
		
	cout<<"[AFTER] Host::storevertexupdatestofile:: total key-value pairs: "<<totaloffsets_bytes<<endl;
	return;
}

void storestatstofile(FILE * nvmeFd_vus_w, FILE * nvmeFd_vus_stats_w){
	std::cout << endl<<"Host::storestatstofile. saving vertex updates to file..." << std::endl;
	size_t offsets_bytes;
	
	fseek (nvmeFd_vus_w, 0, SEEK_END);
	offsets_bytes=ftell (nvmeFd_vus_w);
	edge_t numvus = offsets_bytes / sizeof(keyvalue_t);
	cout<<"[BEFORE] Host::storestatstofile:: key-value pairs: "<<(offsets_bytes / sizeof(keyvalue_t))<<" ("<<offsets_bytes<<" bytes)"<<endl;

	if(fwrite (&numvus, (size_t)(sizeof(edge_t)), 1, nvmeFd_vus_stats_w) != 1){ cout<<"storestatstofile::ERROR seen 54"<<endl; exit(EXIT_FAILURE); };

	fseek (nvmeFd_vus_w, 0, SEEK_END);
	offsets_bytes=ftell (nvmeFd_vus_w);
	cout<<"[AFTER] Host::storestatstofile:: key-value pairs: "<<(offsets_bytes / sizeof(keyvalue_t))<<" ("<<offsets_bytes<<" bytes)"<<endl;
	return;
}

// DRAM Access Methods
void buffervertexupdates(uint512_vec_dt * kvdram, unsigned int kvdrambeginoffset, metadata_t * kvstats, vector<uint512_vec_dt> (&buffered_nvmeFd_r)[MAXNUMSSDPARTITIONS]){
	std::cout << endl<<"Host::buffervertexupdates. saving vertex updates to memory..." << std::endl;
	for(unsigned int p=0; p<NUMSSDPARTITIONS; p++){
		// std::cout<<"HOST:: logging vertex updates from kvs_partitions["<<p<<"] into MEMORY. partition "<<p<<" (key-value pairs)... "<<endl;
		unsigned int kvdram_beginoffset_kvs = (kvdrambeginoffset + kvstats[p].offset) / VECTOR_SIZE;
		unsigned int kvdram_size_kvs = kvstats[p].size / VECTOR_SIZE;
		for(unsigned int j=0; j<kvdram_size_kvs; j++){
			buffered_nvmeFd_r[p].push_back(kvdram[kvdram_beginoffset_kvs + j]);
		}
	}

	size_t totalkvsstored_kvs = 0;
	for(unsigned int p=0; p<NUMSSDPARTITIONS; p++){
		totalkvsstored_kvs += buffered_nvmeFd_r[p].size();
		// cout<<"[AFTER] Host::buffervertexupdates:: buffer partition: "<<p<<", key-value pairs: "<<(buffered_nvmeFd_r[p].size() * VECTOR_SIZE)<<endl;
	}
	cout<<"[AFTER] Host::buffervertexupdates:: total key-value pairs buffered: "<<(size_t)(totalkvsstored_kvs * VECTOR_SIZE)<<endl;
	return;
}

// Generate KV Updates
void generatekvupdatesbatch(vertexprop_t * verticesbuffer, vertex_t verticesbuffersz, edgeprop_t * edgesbuffer, edge_t edgesbuffersz, vector<keyvalue_t> & kvs_partitions, edge_t ebase){
	std::cout << endl<< "Host::processedgesbatch. processing edges... *** ebase: "<<ebase << " ***"<< std::endl;
	unsigned int num_edges_processed = 0;
	for(unsigned int i = 0; i<verticesbuffersz ; i++){
		if((i % 10000000) == 0){ std::cout <<i <<" edges processed"<<std::endl; }
		vertexprop_t vp = verticesbuffer[i];
		vertexprop_t vp_plus1 = verticesbuffer[(i + 1)];
		for(edge_t j = vp.offset; j<vp_plus1.offset; j++){
			#ifdef _DEBUGMODE_CHECKS2
			if((edge_t)(j - ebase) >= edgesbuffersz){ std::cout<<"Host::processedgesbatch:: out-of-bounds 45. (j - ebase): "<<(j - ebase)<<", edgesbuffersz: "<<edgesbuffersz<<", i: "<<i<<", j: "<<j<<", ebase: "<<ebase<<std::endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int lvpoffset = j - ebase;
			edgeprop_t edge = edgesbuffer[lvpoffset];
			
			#ifdef PR_ALGORITHM
			float res = 0.1;
			#elif defined(BFS_ALGORITHM)
			float res = 0;
			#else 
			cout<<"generatekvupdatesbatch: algorithm undefined"<<endl; exit(EXIT_FALURE);
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			num_edges_processed += 1;
			#endif

			keyvalue_t data;
			data.key = edge.dstvid;
			data.value = res;//0*,res; // FIXME

			kvs_partitions.push_back(data);
		}
	}
	std::cout << "Host::processedgesbatch. finished processing edges. "<< num_edges_processed <<" edges processed" << std::endl;
	std::cout << "Host::processedgesbatch. kvs_partitions.size(): "<< kvs_partitions.size() << std::endl; 
	return;
}

void generatekvupdates(vertex_t num_vertices, FILE * nvmeFd_vertices_r, FILE * nvmeFd_edges_r, FILE * nvmeFd_vus_w){
	cout<<endl<<"Host::generatekvupdates started... num_vertices: "<<num_vertices<<endl;
	
	vertexprop_t * verticesbuffer = new vertexprop_t[VERTICESDATA_BATCHSIZE_PLUS1];
	edgeprop_t * edgesbuffer;
	
	std::vector<keyvalue_t> kvs_updates;
	kvs_updates.clear();
	unsigned int kvs_partitions_weof;
	kvs_partitions_weof = 0; 
	
	#ifdef RUNALLITERATIONS
	unsigned int it_size = (num_vertices + (VERTICESDATA_BATCHSIZE - 1)) / VERTICESDATA_BATCHSIZE;
	#else 
	unsigned int it_size = 1;//5;
	#endif
	cout<<endl<<"Host::generatekvupdates it_size: "<<it_size<<endl;
	for (unsigned int iteration_idx = 0; iteration_idx < it_size; iteration_idx++) {
		cout<<endl<<">>> Host::generatekvupdates:: Iteration "<<iteration_idx<<" ("<<iteration_idx<<" out of "<<it_size<<")"<<endl;
		
		// Get vertices batch size
		vertex_t verticesdramsz = hmin(VERTICESDATA_BATCHSIZE, (num_vertices - (iteration_idx * VERTICESDATA_BATCHSIZE)));
		vertex_t paddedverticesdramsz = verticesdramsz + 1;
		if(((iteration_idx * VERTICESDATA_BATCHSIZE) + paddedverticesdramsz) > num_vertices){ paddedverticesdramsz -= 1; } // edge conditions (end-of-file)
		cout<<"Host::generatekvupdates:: verticesdramsz: "<<verticesdramsz<<endl;
		cout<<"Host::generatekvupdates:: paddedverticesdramsz: "<<paddedverticesdramsz<<endl;

		// load vertices
		loadverticesfromfile(iteration_idx, nvmeFd_vertices_r, verticesbuffer, paddedverticesdramsz);
		cout<<"Host::generatekvupdates:: FRONT (VERTICES) verticesbuffer[0].offset: "<<verticesbuffer[0].offset<<endl;
		cout<<"Host::generatekvupdates:: BACK (VERTICES) verticesbuffer["<<(verticesdramsz-1)<<"].offset: "<<verticesbuffer[verticesdramsz-1].offset<<endl;
		cout<<"Host::generatekvupdates:: BACK (VERTICES) verticesbuffer["<<(paddedverticesdramsz-1)<<"].offset: "<<verticesbuffer[paddedverticesdramsz-1].offset<<endl;

		// load edges
		edge_t beginoffset = verticesbuffer[0].offset;
		edge_t endoffset = verticesbuffer[paddedverticesdramsz-1].offset;
		edge_t edges_batchsz = endoffset - beginoffset;
		edgesbuffer = new edgeprop_t[edges_batchsz];
		loadedgesfromfile(iteration_idx, nvmeFd_edges_r, verticesbuffer[0].offset, verticesbuffer[paddedverticesdramsz-1].offset, edgesbuffer);
		cout<<"Host::generatekvupdates:: FRONT (EDGES) "<<edgesbuffer[0].dstvid<<endl;
		cout<<"Host::generatekvupdates:: BACK (EDGES) "<<edgesbuffer[edges_batchsz-1].dstvid<<endl;
		cout<<"Host::generatekvupdates:: edges_batchsz "<<edges_batchsz<<endl;
		cout<<"Host::generatekvupdates:: beginoffset "<<beginoffset<<endl;
		cout<<"Host::generatekvupdates:: endoffset "<<endoffset<<endl;
		if(paddedverticesdramsz > 3){
		std::cout <<"verticesbuffer[0].offset: "<<verticesbuffer[0].offset<<std::endl; 
		std::cout <<"verticesbuffer[1].offset: "<<verticesbuffer[1].offset<<std::endl; 
		std::cout <<"verticesbuffer[2].offset: "<<verticesbuffer[2].offset<<std::endl; 
		std::cout <<"verticesbuffer[3].offset: "<<verticesbuffer[3].offset<<std::endl;
		} 
		if(edges_batchsz > 2){
		std::cout <<"edgesbuffer[0].dstvid: "<<edgesbuffer[0].dstvid<<", edgesbuffer[0].ew: "<<edgesbuffer[0].ew<<std::endl; 
		std::cout <<"edgesbuffer[1].dstvid: "<<edgesbuffer[1].dstvid<<", edgesbuffer[1].ew: "<<edgesbuffer[1].ew<<std::endl; }
		// exit(EXIT_SUCCESS);

		// process edges
		generatekvupdatesbatch(verticesbuffer, verticesdramsz, edgesbuffer, edges_batchsz, kvs_updates, beginoffset);
		
		// save vertex updates
		storevertexupdatestofileIV(kvs_updates, nvmeFd_vus_w);
	}
	verticesbuffer = NULL;
	edgesbuffer = NULL;
	kvs_updates.clear();
	return;
}

// Collect SSD partition stats
void collectssdpartitionstats(unsigned int kvs_weofs[NUMSSDPARTITIONS], FILE * nvmeFd_kvs_w[MAXNUMSSDPARTITIONS]){
	std::cout << endl<<"Host::collectssdpartitionstats. collecting ssd partition stats..." << std::endl;
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){
		fseek (nvmeFd_kvs_w[i], 0, SEEK_END);
		kvs_weofs[i]=ftell(nvmeFd_kvs_w[i]) / sizeof(keyvalue_t);
		cout<<"[BEFORE] Host::collectssdpartitionstats:: kvs_weofs["<<i<<"]: "<<kvs_weofs[i]<<" key value pairs"<<endl;
	}
	return;
}

void collectssdpartitionstats2(edge_t * vus_weof, FILE * nvmeFd_vus_w){
	std::cout << endl<<"Host::collectssdpartitionstats2. collecting ssd partition stats..." << std::endl;
	
	fseek (nvmeFd_vus_w, 0, SEEK_END);
	*vus_weof=ftell(nvmeFd_vus_w) / sizeof(keyvalue_t);
	cout<<"[BEFORE] Host::collectssdpartitionstats2:: vus_weof: "<<(edge_t)*vus_weof<<" key value pairs"<<endl;
	return;
}

// Populate & Initialize KvDrams
void populatekvdrams(unsigned int iteration_idx, FILE * nvmeFd_r, vector<uint512_vec_dt> & buffered_nvmeFd_r, bool bufferkvupdates, uint512_vec_dt * kvdram, vertex_t kvbeginoffset, vertex_t kvbatchsz){
	std::cout <<endl<< "Host:: populatekvdrams. " << std::endl;
	edge_t kvbeginoffset_kvs = kvbeginoffset / VECTOR_SIZE;
	// vertex_t kvbatchsz_kvs = (kvbatchsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE; // handles edge conditions
	vertex_t kvbatchsz_kvs = kvbatchsz / VECTOR_SIZE;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"Host::populatekvdrams:: kvbeginoffset "<<kvbeginoffset<<endl;
	cout<<"Host::populatekvdrams:: kvbeginoffset_kvs "<<kvbeginoffset_kvs<<endl;
	cout<<"Host::populatekvdrams:: kvbatchsz "<<kvbatchsz<<endl;
	cout<<"Host::populatekvdrams:: kvbatchsz_kvs "<<kvbatchsz_kvs<<endl;
	cout<<"Host::populatekvdrams:: (ftell(nvmeFd_r) / sizeof(keyvalue_t)) "<<(ftell(nvmeFd_r) / sizeof(keyvalue_t))<<endl;
	#endif 

	if(kvbatchsz > 0){
		if(bufferkvupdates == true){
			// memcpy(&kvdram[kvbeginoffset_kvs], buffered_nvmeFd_r.data(), (size_t)(kvbatchsz * sizeof(keyvalue_t)));
			memcpy(&kvdram[kvbeginoffset_kvs], &buffered_nvmeFd_r[buffered_nvmeFd_r.size()-1], (size_t)(kvbatchsz * sizeof(keyvalue_t)));
		} else {
			if(fread (&kvdram[kvbeginoffset_kvs], (size_t)(kvbatchsz * sizeof(keyvalue_t)), 1, nvmeFd_r) != 1){ cout<<"populatekvdrams:: ERROR 56"<<endl; exit(EXIT_FAILURE); };
		}
	}
	
	// for(unsigned int j=kvbatchsz; j<hallignup_KV(kvbatchsz); j++){ kvdram[(kvbeginoffset_kvs + (kvbatchsz_kvs-1))].data[(j%VECTOR_SIZE)].key = 0; } // handles edge conditions
	#ifdef _DEBUGMODE_HOSTPRINTS
	if(kvbatchsz > 0){ printfirstkvs("populatekvdrams", kvdram[kvbeginoffset_kvs], 1); }
	#endif 
	return;
}

vertex_t populatekvdramsII(EdgeProcess<uint32_t,uint32_t>* edge_process, SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader, uint512_vec_dt * kvdram, unsigned int * status){
	cout<<endl<<"Host::populatekvdramsII started..."<<endl;
	
	unsigned int kvthreshold = (PADDEDKVDATA_BATCHSIZE / 10) * 9; //  (1 << 25);
	unsigned int kvdram_weof = 0;
	
	std::tuple<uint32_t,uint32_t,bool> res = reader->Next();
	while ( std::get<2>(res) ) {
		uint32_t key = std::get<0>(res);
		uint32_t val = std::get<1>(res);
		res = reader->Next();
		
		edge_process->LoadKVs(key, val, (keyvalue_t *)kvdram, 0, &kvdram_weof);
		if(kvdram_weof > kvthreshold){  
			cout<<endl<<"Host::populatekvdramsII Finished. kvdram_weof: "<<kvdram_weof<<", status: "<<*status<<endl;
			return kvdram_weof; 
		}
		// if(kvdram_weof > (PADDEDKVDATA_BATCHSIZE - allowance)){ break; }
		// cout<<"populatekvdramsII:: key: "<<key<<", val: "<<val<<", kvdram_weof: "<<kvdram_weof<<endl;
	}
	*status = FINISHED;
	cout<<endl<<"Host::populatekvdramsII Finished. No more Active vertices to process. kvdram_weof: "<<kvdram_weof<<", status: "<<*status<<endl;
	return kvdram_weof;
}

void initializekvdram(unsigned int iteration_idx, uint512_vec_dt * kvdram, vertex_t kvoffset, vertex_t kvsize, value_t init_val, unsigned int rootvid){
	unsigned int kvoffset_kvs = kvoffset / VECTOR_SIZE;
	unsigned int kvsize_kvs = (kvsize + (VECTOR_SIZE - 1)) / VECTOR_SIZE; // kvsize / VECTOR_SIZE;
	cout<<"Host::initializekvdram:: kvoffset "<<kvoffset<<endl;
	cout<<"Host::initializekvdram:: kvoffset_kvs "<<kvoffset_kvs<<endl;
	cout<<"Host::initializekvdram:: kvsize "<<kvsize<<endl;
	cout<<"Host::initializekvdram:: kvsize_kvs "<<kvsize_kvs<<endl;
	
	for(unsigned int i=0; i<kvsize_kvs; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			kvdram[kvoffset_kvs + i].data[j].value = init_val;
		}
	}
	// if(iteration_idx == 0){ kvdram[kvoffset_kvs + 0].data[0].value = rootvid; } // FIXME. just for test
	cout<<"Host::initializekvdram:: kvdram["<<(kvoffset_kvs + 0)<<"].data[0].key: "<<kvdram[(kvoffset_kvs + 0)].data[0].key<<", kvdram["<<(kvoffset_kvs + 0)<<"].data[0].value: "<<kvdram[(kvoffset_kvs + 0)].data[0].value<<endl;
	cout<<"Host::initializekvdram:: kvdram["<<(kvoffset_kvs + 0)<<"].data[1].key: "<<kvdram[(kvoffset_kvs + 0)].data[1].key<<", kvdram["<<(kvoffset_kvs + 0)<<"].data[1].value: "<<kvdram[(kvoffset_kvs + 0)].data[1].value<<endl; 
	return;
}

// Collect stats
void collectstats(uint512_vec_dt * kvdram, metadata_t * kvstats, vertex_t kvoffset, vertex_t kvbatchsz, vertex_t kvrangeoffset, unsigned int finalnumpartitionsfordram_partition, unsigned int finalrangefordram_partition){
	// preprocessing on CPU to collect stats
	#ifdef _DEBUGMODE_HOSTPRINTS
	std::cout << endl << "Collect stats of KvDRAM. "<< std::endl;
	cout<<"HOST::collectstats:: kvoffset: "<<kvoffset<<endl;
	cout<<"HOST::collectstats:: kvbatchsz: "<<kvbatchsz<<endl;
	cout<<"HOST::collectstats:: kvrangeoffset: "<<kvrangeoffset<<endl;
	cout<<"HOST::collectstats:: finalnumpartitionsfordram_partition: "<<finalnumpartitionsfordram_partition<<", KVSTATS_SIZE: "<<KVSTATS_SIZE<<endl;
	cout<<"HOST::collectstats:: finalrangefordram_partition: "<<finalrangefordram_partition<<endl;
	#endif 
	vertex_t kvoffset_kvs = kvoffset / VECTOR_SIZE;
	vertex_t kvbatchsz_kvs = kvbatchsz / VECTOR_SIZE;
	unsigned int numerrors = 0;
	
	for(int i = 0 ; i<KVSTATS_SIZE; i++){ kvstats[i].offset = 0; kvstats[i].size = 0; }
	
	unsigned int partition;
	for(unsigned int i=0; i<kvbatchsz_kvs; i++){
		for(int j=0; j<VECTOR_SIZE; j++){
			partition = (kvdram[(kvoffset_kvs + i)].data[j].key - kvrangeoffset) / finalrangefordram_partition;
			// #ifdef _DEBUGMODE_CHECKS2
			if(partition >= finalnumpartitionsfordram_partition){
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"collectstats:: partition is out-of-range: i:"<<i<<", j:"<<j<<", partition: "<<partition<<", finalnump..: "<<finalnumpartitionsfordram_partition<<", kvdram["<<i<<"].data["<<j<<"].key: "<<kvdram[i].data[j].key<<", kvrangeoffset: "<<kvrangeoffset<<endl; 
				numerrors += 1; if(numerrors > (1000)){ cout<<"host::collectstats: too many errors. possible causes are: (1) improper multithreading implementation..."<<endl; exit(EXIT_FAILURE); }
				#endif
				kvdram[(kvoffset_kvs + i)].data[j].key = kvrangeoffset; // FIXME. this should be corrected
				partition = 0;
			}
			// #endif
			kvstats[partition].size += 1;
		}
	}

	// calculate offsets
	kvstats[0].offset = 0;
	for (unsigned int p = 1; p<finalnumpartitionsfordram_partition; p++){
		kvstats[p].offset = hallignup_KV((kvstats[p-1].offset + kvstats[p-1].size));
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int p=0; p<2; p++){ cout<<"HOST::collectstats:: kvstats["<<p<<"].offset: "<<kvstats[p].offset<<", kvstats["<<p<<"].size: "<<kvstats[p].size<<" key value pairs"<<endl; }
	#endif 
	
	// assign initial workload sizes
	for(int i = 0 ; i<KVSTATS_SIZE; i++){ kvstats[i].size = 0; }
	kvstats[0].size = kvbatchsz; // IMPORTANT	

	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"HOST::DEBUGGER:: checking last offset of collected stats (sanity test). "<<finalnumpartitionsfordram_partition<<", KVSTATS_SIZE: "<<KVSTATS_SIZE<<endl;
	cout<<"HOST::collectstats:: kvstats["<<(finalnumpartitionsfordram_partition - 1)<<"].offset: "<<kvstats[(finalnumpartitionsfordram_partition - 1)].offset<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"HOST::collectstats:: kvstats[0].size: "<<kvstats[0].size<<" key value pairs"<<", numerrors: "<<numerrors<<endl;
	#endif 
	return;
}

// Prepare datasets
void preparedataset(dataset_t dataset){
	std::cout<<"host:: dataset.graph_path: "<<dataset.graph_path<<std::endl;
	std::cout<<"host:: dataset.num_vertices: "<<dataset.num_vertices<<std::endl;
	std::cout<<"host:: dataset.num_edges: "<<dataset.num_edges<<std::endl;
	#ifdef _PREPAREDATASET
	std::ofstream ofs;
	#ifdef _PREPAREVERTICES
	ofs.open(dataset.vertices_path_bin, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open(dataset.vertices_path, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	#endif 
	#ifdef _PREPAREEDGES
	ofs.open(dataset.edges_path_bin, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open(dataset.edges_path, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	#endif 
	print_graph(dataset.graph_path, 0, 250);
	// exit(EXIT_SUCCESS); //
	#ifdef _PREPAREVERTICES
	extract_vertices(dataset.graph_path, dataset.vertices_path_bin, dataset.vertices_path, dataset.graphorder);
	extractv_readvertices(dataset.vertices_path_bin, 0, 2);
	extractv_countvertices(dataset.vertices_path_bin);
	// exit(EXIT_SUCCESS); //
	#endif 
	#ifdef _PREPAREEDGES
	extract_edges(dataset.graph_path, dataset.edges_path_bin, dataset.edges_path, dataset.graphorder);
	extracte_readedges(dataset.edges_path_bin, 0, 2);
	extracte_countedges(dataset.edges_path_bin);
	#endif 
	exit(EXIT_SUCCESS);
	#endif	
	#ifdef _DEBUGMODE_HOSTPRINTS
	print_graph(dataset.graph_path, 0, 240);
	#endif 
	return;
}



