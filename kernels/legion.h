#ifndef KERNEL_LEGION_H
#define KERNEL_LEGION_H
#include "../src/common.h"
#ifdef FPGA_IMPL
#include <ap_int.h>
#endif
using namespace std;

#define COLLECTSTATSPROCESS 5
#define SPREADPROCESS 6


#define LSRCKVBUFFERSZ KVBUFFERSZ
#define LSRCKVBUFFERSZ_KVS (LSRCKVBUFFERSZ / VECTOR_SIZE)
#define LDESTKVBUFFERSZ KVBUFFERSZ
#define LDESTKVBUFFERSZ_KVS (LDESTKVBUFFERSZ / VECTOR_SIZE)

#define LKVSTATSBIGBUFFERSZ_KVS KVBUFFERSZ_KVS

typedef unsigned int batch_type;
typedef unsigned int buffer_type;
typedef unsigned int partition_type;
typedef unsigned int code_type;
typedef unsigned int smallint_type;
typedef unsigned int vector_type;
typedef unsigned int code_type;

typedef struct {
	unsigned int coffset;
	unsigned int size;
} coffset_t;

typedef struct {
	unsigned int baseoffset_kvs;
	unsigned int basesize_kvs;
} workoffset_t;

typedef struct {
	unsigned int partitionmodule;
	unsigned int mergemodule;
} runcmd_t;

typedef struct {
	unsigned int baseoffset_kvs;
	unsigned int basesize_kvs;
	unsigned int steps;
	unsigned int finalnumpartitions;
	unsigned int paddedkvdatabatchsz_kvs;
	unsigned int capsulebatchsz_kvs;
	unsigned int vbegin;
} globalparams_t;

typedef struct {
	unsigned int sourcebaseaddr_kvs;
	unsigned int destbaseaddr_kvs;
	unsigned int baseaddr_capsule_kvs;
	unsigned int baseaddr_stats_kvs;
	unsigned int num_source_partitions;
	unsigned int nextsourceoffset_partition;
	unsigned int nextdestoffset_partition;
	unsigned int nextsourceoffset_kv;
	unsigned int nextdestoffset_kv;
	unsigned int sourceskipsize_partition;
	unsigned int destskipsize_partition;
	unsigned int sourceskipsize_kv;
	unsigned int destskipsize_kv;
	unsigned int destpartitionrange;
	unsigned int currentLOP;
	unsigned int upperlimit;
} clopparams_t;

typedef struct {
	unsigned int begin_kvs;
	unsigned int size_kvs;
	unsigned int step_kvs;
} partitionparams_t;

typedef struct {
	unsigned int i_kvs;
	unsigned int iend_kvs;
} travstate_t;

#ifndef FPGA_IMPL
class legion {
public:
	legion();
	~legion();
	#ifdef _LDEBUGMODE_HEADER
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound);
	void checkforequal(string message, unsigned int data1, unsigned int data2);
	void print1(string messagea, unsigned int dataa);
	void print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad);
	void print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size);
	void printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs);
	void getvaluecount(string message, keyvalue_t * keyvalues, unsigned int size);
	void printparameters();
	#endif
	#ifdef _DEBUGMODE_STATS
	void printglobalvars();
	void clearglobalvars();
	void globalstats_countkvstatsread(unsigned int count);
	void globalstats_countkvstatscollected(unsigned int count);
	void globalstats_countkvstatswritten(unsigned int count);
	void globalstats_countkvsread(unsigned int count);
	void globalstats_countkvsspreadwritten(unsigned int count);
	void globalstats_countkvsspread(unsigned int count);
	void globalstats_countkvsreduced(unsigned int count);
	void globalstats_countkvsreducewritten(unsigned int count);
	#endif
	
	vertex_t GETKVDATA_RANGE_PERSSDPARTITION();
	vertex_t GETKVDATA_RANGE_PERSSDPARTITION_POW();
	vertex_t GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION();
	vertex_t GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS();
	unsigned int GETTREEDEPTH();
	unsigned int min(unsigned int A, unsigned int B);
	unsigned int hsub(unsigned int A, unsigned int B);	
	
	buffer_type getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize);
	batch_type getAddr(batch_type addr);
	batch_type getcapsuleAddr(batch_type addr);	
	unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit);
	buffer_type getglobalstatoffset_kvs(unsigned int kvoffset_kvs);
	unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param);
	unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param);
	keyvalue_t getkeyvalue(uint512_dt * buffer, batch_type addr, batch_type maxaddr_kvs);
	void setkeyvalue(uint512_dt * buffer, batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs);
	void setvalue(uint512_dt * buffer, batch_type addr, value_t value, batch_type maxaddr_kvs);
	void loaddeststats(uint512_dt kvstats[KVSTATS_SIZE_KVS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions);
	void storedeststats(uint512_dt kvstats[KVSTATS_SIZE_KVS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions);
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	void addtobuffer(uint512_dt buffer[LDESTKVBUFFERSZ_KVS], buffer_type offset, keyvalue_t kventry);
	void addtobuffer2(uint512_dt buffer0[LDESTKVBUFFERSZ_KVS],uint512_dt buffer1[LDESTKVBUFFERSZ_KVS],uint512_dt buffer2[LDESTKVBUFFERSZ_KVS],uint512_dt buffer3[LDESTKVBUFFERSZ_KVS],uint512_dt buffer4[LDESTKVBUFFERSZ_KVS],uint512_dt buffer5[LDESTKVBUFFERSZ_KVS],uint512_dt buffer6[LDESTKVBUFFERSZ_KVS],uint512_dt buffer7[LDESTKVBUFFERSZ_KVS], buffer_type offset, uint512_dt keyvalueset);
	void calculateoffsets(keyvalue_t buffer[NUM_PARTITIONS]);
	value_t reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount);
	
	void loadstatsbuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[KVSTATS_SIZE_KVS], batch_type baseaddress);
	void loadcapsulestatsS_ddr0(uint512_dt * stats, keyvalue_t buffer0[NUM_PARTITIONS], keyvalue_t buffer1[NUM_PARTITIONS], keyvalue_t buffer2[NUM_PARTITIONS], keyvalue_t buffer3[NUM_PARTITIONS], keyvalue_t buffer4[NUM_PARTITIONS], keyvalue_t buffer5[NUM_PARTITIONS], keyvalue_t buffer6[NUM_PARTITIONS], keyvalue_t buffer7[NUM_PARTITIONS],  batch_type localoffset_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void loadbufferH_ddr0(uint512_dt * kvdram, uint512_dt buffer[LSRCKVBUFFERSZ_KVS], batch_type localoffset_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void loadbufferV_ddr0(uint512_dt * kvdram, keyvalue_t buffer0[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer1[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer2[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer3[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer4[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer5[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer6[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer7[LSRCKVBUFFERSZ_KVS], batch_type localoffset_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	
	void extractcapsules_ddr0(uint512_dt sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], vector_type vecid, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void spreadkvs_ddr0_v0(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void spreadkvs_ddr0_v1(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void spreadkvs_ddr0_v2(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void spreadkvs_ddr0_v3(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void spreadkvs_ddr0_v4(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void spreadkvs_ddr0_v5(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void spreadkvs_ddr0_v6(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void spreadkvs_ddr0_v7(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduce_ddr0_v0(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduce_ddr0_v1(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduce_ddr0_v2(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduce_ddr0_v3(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduce_ddr0_v4(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduce_ddr0_v5(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduce_ddr0_v6(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduce_ddr0_v7(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	
	void savestatsbuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[KVSTATS_SIZE_KVS], batch_type baseaddress);
	void savepartialbuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[LDESTKVBUFFERSZ_KVS], batch_type baseaddress_kvs, batch_type localoffset_kvs, buffer_type size_kvs, code_type callerprocessid, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void savebuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type baseaddress_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void savebuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[LDESTKVBUFFERSZ_KVS], batch_type baseaddress_kvs, batch_type localoffset_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	
	void processpartition_ddr0(uint512_dt * kvdram, keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void processCLOP_ddr0(uint512_dt * kvdram, uint512_dt kvstatsbuffer[KVSTATS_SIZE_KVS], globalparams_t globalparams, clopparams_t llopparams);
	void partitionphase_ddr0(uint512_dt * kvdram, globalparams_t globalparams);
	
	void reducepartition_ddr0(uint512_dt * kvdram_source, uint512_dt * kvdram_dest, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams);
	void reduceLLOP_ddr0(uint512_dt * kvdram_source, uint512_dt * kvdram_dest, uint512_dt kvstatsbuffer[KVSTATS_SIZE_KVS], globalparams_t globalparams, clopparams_t llopparams);
	void reducephase_ddr0(uint512_dt * kvdram_source, uint512_dt * kvdram_dest, globalparams_t globalparams);
	
	void topkernel(
uint512_dt * kvdram_sourceA
		,uint512_dt * kvdram_destA
        );
private:
};
#endif 
#endif 







