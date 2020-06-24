#ifndef KERNELPROCESS_H
#define KERNELPROCESS_H
#include "../src/common.h"

class kernelprocess {
public:
	kernelprocess();
	~kernelprocess();
	
	void topkernel(
uint512_dt * kvdramA
		,metadata_t * kvstatsA
		,int * messagesA
        );
	
	void printstate();
	unsigned int ceildiv(unsigned int val1, unsigned int val2);
	unsigned int min8(unsigned int val0, unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4, unsigned int val5, unsigned int val6, unsigned int val7);
	unsigned int min(unsigned int val1, unsigned int val2);
	unsigned int max(unsigned int val1, unsigned int val2);
	unsigned int minorzero(int val1);
	unsigned int allignup_KV(unsigned int val);
	unsigned int alligndown_KV(unsigned int val);
	vertex_t GETKVDATA_RANGE_PERSSDPARTITION(unsigned int ssdpartitonid);
	vertex_t GETKVDATA_RANGE_PERSSDPARTITION_POW(unsigned int ssdpartitonid);
	vertex_t GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(unsigned int ssdpartitonid);
	vertex_t GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(unsigned int ssdpartitonid);
	unsigned int GETTREEDEPTH(unsigned int ssdpartitonid);
	unsigned int hashfunc(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, vertex_t range);
	unsigned int hashfunc2(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid);
	unsigned int hashfunc3(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid);
	unsigned int hashfunc4(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid);
	unsigned int hashfunc5(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int range);
	keyvalue_t getkventry(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	keyvalue_t getkventryII(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	keyvalue_t getkventryIII(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	keyvalue_t getkventryIV(int addr, uint512_dt kvbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void setkventry(uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS], unsigned int addr, keyvalue_t kventry);
	void setkventryII(uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS], unsigned int addr, keyvalue_t kventry);
	void setkventryIII(
		keyvalue_t kvbuffer_vec0[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec1[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec2[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec3[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec4[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec5[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec6[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec7[KVDESTBUFFERSZ_KVS], 
		unsigned int addr, keyvalue_t kventry);
	void setkventry4(
		keyvalue_t kvbuffer_vec0[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec1[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec2[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec3[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec4[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec5[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec6[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec7[APPLYVERTEXBUFFERSZ_KVS], 
		unsigned int addr, keyvalue_t kventry);
	keyvalue_t getkventry3(uint512_dt tmpV1, unsigned int idx);
	keyvalue_t getkventry4(
		keyvalue_t kvbuffer_vec0[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec1[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec2[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec3[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec4[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec5[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec6[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec7[KVSRCBUFFERSZ_APPLY_KVS], 
		unsigned int addr);
	unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param);
	unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param);
	void loaddeststats(metadata_t * kvstats, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions);
	void storedeststats(metadata_t * kvstats, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions);
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	unsigned int reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount);
	
	/// Main Functions
	// READ	
	void read_ddr0_w0_sw0(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	void read_ddr0_w0_sw1(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	void read_ddr0_w0_sw2(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	void read_ddr0_w0_sw3(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	void read_ddr0_w0_sw4(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	void read_ddr0_w0_sw5(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	void read_ddr0_w0_sw6(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);
	void read_ddr0_w0_sw7(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]);


	void readAU_VU_ddr0_w0_sw0(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);
	void readAU_VU_ddr0_w0_sw1(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);
	void readAU_VU_ddr0_w0_sw2(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);
	void readAU_VU_ddr0_w0_sw3(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);
	void readAU_VU_ddr0_w0_sw4(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);
	void readAU_VU_ddr0_w0_sw5(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);
	void readAU_VU_ddr0_w0_sw6(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);
	void readAU_VU_ddr0_w0_sw7(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);
		

	void readAU_V_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void readAU_V_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void readAU_V_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void readAU_V_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void readAU_V_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void readAU_V_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void readAU_V_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void readAU_V_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);


	// PARTITION
	void partition_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
			#ifdef _PARTITIONED_KVBUFFERSTATS
			,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
			#else 
			,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
			#endif 
			,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	void partition_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
			#ifdef _PARTITIONED_KVBUFFERSTATS
			,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
			#else 
			,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
			#endif 
			,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	void partition_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
			#ifdef _PARTITIONED_KVBUFFERSTATS
			,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
			#else 
			,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
			#endif 
			,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	void partition_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
			#ifdef _PARTITIONED_KVBUFFERSTATS
			,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
			#else 
			,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
			#endif 
			,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	void partition_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
			#ifdef _PARTITIONED_KVBUFFERSTATS
			,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
			#else 
			,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
			#endif 
			,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	void partition_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
			#ifdef _PARTITIONED_KVBUFFERSTATS
			,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
			#else 
			,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
			#endif 
			,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	void partition_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
			#ifdef _PARTITIONED_KVBUFFERSTATS
			,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
			#else 
			,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
			#endif 
			,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	void partition_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
			#ifdef _PARTITIONED_KVBUFFERSTATS
			,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
			#else 
			,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
			#endif 
			,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);

	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractstats2_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractstats2_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractstats2_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractstats2_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractstats2_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractstats2_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractstats2_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractstats2_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 

	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partition2_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partition2_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partition2_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partition2_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partition2_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partition2_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partition2_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 
	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partition2_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
			uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]);
	#endif 



	// APPLY
	void reduce_ddr0_w0_sw0(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);				
	void reduce_ddr0_w0_sw1(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);				
	void reduce_ddr0_w0_sw2(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);				
	void reduce_ddr0_w0_sw3(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);				
	void reduce_ddr0_w0_sw4(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);				
	void reduce_ddr0_w0_sw5(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);				
	void reduce_ddr0_w0_sw6(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);				
	void reduce_ddr0_w0_sw7(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]);				


	// WRITE
	void write_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void write_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void write_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void write_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void write_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void write_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void write_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void write_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);

	void writeremainingovs_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void writeremainingovs_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void writeremainingovs_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void writeremainingovs_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void writeremainingovs_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void writeremainingovs_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void writeremainingovs_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	void writeremainingovs_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs);

	// BRAM 2 TMP TRANSFERS
	#ifdef _BUFFERB4WRITE
	void mergeoffsetsandsizes_ddr0_w0(unsigned int ssdpartitonid, unsigned int workerid
			,smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]
			,smalluint_t kvbufferstats_sw0_size[NUM_PARTITIONS]
			,smalluint_t kvbufferstats_sw1_size[NUM_PARTITIONS]
			,smalluint_t kvbufferstats_sw2_size[NUM_PARTITIONS]
			,smalluint_t kvbufferstats_sw3_size[NUM_PARTITIONS]
			,smalluint_t kvbufferstats_sw4_size[NUM_PARTITIONS]
			,smalluint_t kvbufferstats_sw5_size[NUM_PARTITIONS]
			,smalluint_t kvbufferstats_sw6_size[NUM_PARTITIONS]
			,smalluint_t kvbufferstats_sw7_size[NUM_PARTITIONS]
			);
	#endif

	#ifdef _BUFFERB4WRITE
	void write_bram2tmp_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2tmp_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2tmp_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2tmp_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2tmp_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2tmp_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2tmp_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2tmp_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 



	// BRAM 2 DRAM TRANSFERS
	#ifdef _BUFFERB4WRITE
	void write_bram2dram_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2dram_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2dram_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2dram_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2dram_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2dram_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2dram_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 
	#ifdef _BUFFERB4WRITE
	void write_bram2dram_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, 
			uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs);
	#endif 

	// WRITE VERTICES TO DRAM (APPLY PHASE)
	void writeAU_Vs_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void writeAU_Vs_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void writeAU_Vs_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void writeAU_Vs_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void writeAU_Vs_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void writeAU_Vs_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void writeAU_Vs_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);
	void writeAU_Vs_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]);


	// READ MANY
	void readmany_ddr0_w0(unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
		,uint512_dt kvbuffer0[KVSRCBUFFERSZ_KVS]
		,uint512_dt kvbuffer1[KVSRCBUFFERSZ_KVS]
		,uint512_dt kvbuffer2[KVSRCBUFFERSZ_KVS]
		,uint512_dt kvbuffer3[KVSRCBUFFERSZ_KVS]
		,uint512_dt kvbuffer4[KVSRCBUFFERSZ_KVS]
		,uint512_dt kvbuffer5[KVSRCBUFFERSZ_KVS]
		,uint512_dt kvbuffer6[KVSRCBUFFERSZ_KVS]
		,uint512_dt kvbuffer7[KVSRCBUFFERSZ_KVS]
		);


	void readmanyAU_Vs_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
		,uint512_dt verticesbuffer0[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer1[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer2[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer3[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer4[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer5[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer6[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer7[APPLYVERTEXBUFFERSZ_KVS]
		);


	void readmanyAU_VUs_ddr0_w0(unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, metadata_t sourcestatbuffer[NUMSUBWORKERS], unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
		,uint512_dt kvbuffer0[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer1[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer2[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer3[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer4[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer5[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer6[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer7[KVSRCBUFFERSZ_APPLY_KVS]
		);


	// PARTITION MANY
	void partitionmany_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange
		,uint512_dt kvbuffer01[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer02[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS], smallmargin_t ov0[NUM_PARTITIONS], uint512_dt ovkvs_tmp0[NUM_PARTITIONS]
		,uint512_dt kvbuffer11[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer12[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS], smallmargin_t ov1[NUM_PARTITIONS], uint512_dt ovkvs_tmp1[NUM_PARTITIONS]
		,uint512_dt kvbuffer21[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer22[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS], smallmargin_t ov2[NUM_PARTITIONS], uint512_dt ovkvs_tmp2[NUM_PARTITIONS]
		,uint512_dt kvbuffer31[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer32[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS], smallmargin_t ov3[NUM_PARTITIONS], uint512_dt ovkvs_tmp3[NUM_PARTITIONS]
		,uint512_dt kvbuffer41[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer42[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS], smallmargin_t ov4[NUM_PARTITIONS], uint512_dt ovkvs_tmp4[NUM_PARTITIONS]
		,uint512_dt kvbuffer51[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer52[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS], smallmargin_t ov5[NUM_PARTITIONS], uint512_dt ovkvs_tmp5[NUM_PARTITIONS]
		,uint512_dt kvbuffer61[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer62[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS], smallmargin_t ov6[NUM_PARTITIONS], uint512_dt ovkvs_tmp6[NUM_PARTITIONS]
		,uint512_dt kvbuffer71[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer72[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS], smallmargin_t ov7[NUM_PARTITIONS], uint512_dt ovkvs_tmp7[NUM_PARTITIONS]
		);

	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void extractmanystats2_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange
		,uint512_dt kvbuffer01[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer02[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS], smallmargin_t ov0[NUM_PARTITIONS], uint512_dt ovkvs_tmp0[NUM_PARTITIONS]
		,uint512_dt kvbuffer11[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer12[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS], smallmargin_t ov1[NUM_PARTITIONS], uint512_dt ovkvs_tmp1[NUM_PARTITIONS]
		,uint512_dt kvbuffer21[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer22[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS], smallmargin_t ov2[NUM_PARTITIONS], uint512_dt ovkvs_tmp2[NUM_PARTITIONS]
		,uint512_dt kvbuffer31[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer32[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS], smallmargin_t ov3[NUM_PARTITIONS], uint512_dt ovkvs_tmp3[NUM_PARTITIONS]
		,uint512_dt kvbuffer41[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer42[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS], smallmargin_t ov4[NUM_PARTITIONS], uint512_dt ovkvs_tmp4[NUM_PARTITIONS]
		,uint512_dt kvbuffer51[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer52[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS], smallmargin_t ov5[NUM_PARTITIONS], uint512_dt ovkvs_tmp5[NUM_PARTITIONS]
		,uint512_dt kvbuffer61[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer62[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS], smallmargin_t ov6[NUM_PARTITIONS], uint512_dt ovkvs_tmp6[NUM_PARTITIONS]
		,uint512_dt kvbuffer71[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer72[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS], smallmargin_t ov7[NUM_PARTITIONS], uint512_dt ovkvs_tmp7[NUM_PARTITIONS]
		);
	#endif 

	#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
	void partitionmany2_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange
		,uint512_dt kvbuffer01[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer02[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS], smallmargin_t ov0[NUM_PARTITIONS], uint512_dt ovkvs_tmp0[NUM_PARTITIONS]
		,uint512_dt kvbuffer11[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer12[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS], smallmargin_t ov1[NUM_PARTITIONS], uint512_dt ovkvs_tmp1[NUM_PARTITIONS]
		,uint512_dt kvbuffer21[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer22[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS], smallmargin_t ov2[NUM_PARTITIONS], uint512_dt ovkvs_tmp2[NUM_PARTITIONS]
		,uint512_dt kvbuffer31[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer32[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS], smallmargin_t ov3[NUM_PARTITIONS], uint512_dt ovkvs_tmp3[NUM_PARTITIONS]
		,uint512_dt kvbuffer41[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer42[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS], smallmargin_t ov4[NUM_PARTITIONS], uint512_dt ovkvs_tmp4[NUM_PARTITIONS]
		,uint512_dt kvbuffer51[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer52[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS], smallmargin_t ov5[NUM_PARTITIONS], uint512_dt ovkvs_tmp5[NUM_PARTITIONS]
		,uint512_dt kvbuffer61[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer62[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS], smallmargin_t ov6[NUM_PARTITIONS], uint512_dt ovkvs_tmp6[NUM_PARTITIONS]
		,uint512_dt kvbuffer71[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer72[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS], smallmargin_t ov7[NUM_PARTITIONS], uint512_dt ovkvs_tmp7[NUM_PARTITIONS]
		);
	#endif 


	// APPLY MANY
	void reducemany_ddr0_w0(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int verticesreadoffset_vs, metadata_t sourcestatbuffer[NUMSUBWORKERS], unsigned int kvreadoffset_kvs
		,uint512_dt verticesbuffer0[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer1[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer2[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer3[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer4[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer5[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer6[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer7[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt kvbuffer0[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer1[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer2[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer3[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer4[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer5[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer6[KVSRCBUFFERSZ_APPLY_KVS]
		,uint512_dt kvbuffer7[KVSRCBUFFERSZ_APPLY_KVS]
	
		);


	// WRITE MANY
	void writemany_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs, unsigned int currentLOP, unsigned int destpartitionoffset	
		,uint512_dt kvbuffer0[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS]
		,uint512_dt kvbuffer1[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS]
		,uint512_dt kvbuffer2[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS]
		,uint512_dt kvbuffer3[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS]
		,uint512_dt kvbuffer4[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS]
		,uint512_dt kvbuffer5[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS]
		,uint512_dt kvbuffer6[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS]
		,uint512_dt kvbuffer7[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS]
		);

	void writemanyAU_Vs_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_vs, unsigned int kvreadoffset_kvs
		,uint512_dt verticesbuffer0[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer1[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer2[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer3[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer4[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer5[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer6[APPLYVERTEXBUFFERSZ_KVS]
		,uint512_dt verticesbuffer7[APPLYVERTEXBUFFERSZ_KVS]
		);


	#ifdef _BUFFERB4WRITE
	void writemany_bram2tmp_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], unsigned int baseaddr_kvs, unsigned int currentLOP, unsigned int destpartitionoffset
		,uint512_dt kvbuffer0[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS]
		,uint512_dt kvbuffer1[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS]
		,uint512_dt kvbuffer2[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS]
		,uint512_dt kvbuffer3[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS]
		,uint512_dt kvbuffer4[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS]
		,uint512_dt kvbuffer5[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS]
		,uint512_dt kvbuffer6[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS]
		,uint512_dt kvbuffer7[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS]
		,unsigned int ibegin_kvs, unsigned int iend_kvs
		);
	#endif 



	#ifdef _BUFFERB4WRITE
	void writemany_bram2dram_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], unsigned int baseaddr_kvs, unsigned int currentLOP, unsigned int destpartitionoffset
		,uint512_dt kvbuffer0[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS]
		,uint512_dt kvbuffer1[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS]
		,uint512_dt kvbuffer2[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS]
		,uint512_dt kvbuffer3[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS]
		,uint512_dt kvbuffer4[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS]
		,uint512_dt kvbuffer5[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS]
		,uint512_dt kvbuffer6[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS]
		,uint512_dt kvbuffer7[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS]
		,unsigned int ibegin_kvs, unsigned int iend_kvs
		);
	#endif 

	// WRITE TMP 2 DRAM
	#ifdef _BUFFERB4WRITE
	void write_tmp2dram_ddr0_w0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset, 
			uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], unsigned int baseaddr_kvs, unsigned int ibegin_kvs, unsigned int iend_kvs);
	#endif 



	#ifdef PROCESSEDGES_DUMMYSTREAMINGMODE // DummyStreaming
	
	void topkernel_instancePE0(
			unsigned int command,
			unsigned int ssdpartitonid,
			uint512_dt * kvdram_src,
			uint512_dt * kvdram_dest,
			unsigned int paddedkvdatabatchsz_kvs,
			unsigned int vsize,
			unsigned int dramID
			);
	#endif 
	#ifdef PROCESSEDGES_STREAMINGMODE // Streaming
	
	void topkernel_instancePE0(
			unsigned int command,
			unsigned int IterCount,
			unsigned int ssdpartitonid,
			uint512_dt * kvdram_src,
			uint512_dt * kvdram_dest,
			unsigned int paddedkvdatabatchsz_kvs,
			unsigned int vsize,
			unsigned int dramID
			);
	#endif 
	#ifdef PROCESSEDGES_CONTIGUOUSMODE // Contiguous
	
	void topkernel_instancePE0(
			unsigned int command,
			unsigned int IterCount,
			unsigned int ssdpartitonid,
			uint512_dt * kvdram_src,
			uint512_dt * kvdram_dest,
			unsigned int paddedkvdatabatchsz_kvs,
			unsigned int vsize,
			unsigned int dramID
			);
	#endif
	#ifdef PROCESSEDGES_RANDOMMODE // Random
	
	void topkernel_instancePE0(
			unsigned int command,
			unsigned int IterCount,
			unsigned int ssdpartitonid,
			uint512_dt * kvdram_src,
			uint512_dt * kvdram_dest,
			unsigned int paddedkvdatabatchsz_kvs,
			unsigned int vsize,
			unsigned int dramID
			);
	#endif

	void topkernel_instancePU0(
			unsigned int command,
			uint512_dt * kvdram,
			metadata_t * kvstats,
			unsigned int treedepth,
			unsigned int finalnumpartitions,
			unsigned int vbegin,
			unsigned int ssdpartitonid,
			unsigned int paddedkvdatabatchsz_kvs
			
			
			);

	void topkernel_instanceAU0(
			unsigned int command,
			unsigned int IterCount,
			uint512_dt * kvdram,
			metadata_t * kvstats,
			unsigned int treedepth,
			unsigned int finalnumpartitions,
			unsigned int vbegin,
			unsigned int ssdpartitonid,
			unsigned int paddedkvdatabatchsz_kvs
			
			
			);
	
private:
	uint512_dt * _kvdramA;
	metadata_t * _kvstatsA;
	int * _messagesA;
};
#endif 







