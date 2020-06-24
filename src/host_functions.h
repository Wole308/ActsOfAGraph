#ifndef HOST_FUNCTIONS_H
#define HOST_FUNCTIONS_H
#include "../src/host_common.h"
#include "EdgeProcess.h" // Grafboost header
#ifdef FPGA_IMPL
// #include "xcl2.hpp"
#include "../xcl.h"
#endif

// Helper Functions
int hallignup_KV(int val);
int halligndown_KV(int val);
int hmin(size_t val1, size_t val2);
size_t hmax_array(unsigned int nvmeFd_kvs_weofs[NUMSSDPARTITIONS], size_t begin, size_t size);
size_t hsub(size_t val1, size_t val2);
size_t hceildiv(size_t val1, size_t val2);
vertex_t GETKVDATA_RANGE_FORSSDPARTITION_(unsigned int ssdpartitonid);
vertex_t GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid);
vertex_t GETAPPLYVERTEXBUFFERSZ_FORSSDPARTITION_(unsigned int ssdpartitonid);
unsigned int GETTREEDEPTH_(unsigned int ssdpartitonid);
unsigned int getpartition(vertex_t val);
float pr(vertexprop_t srcp, vertex_t outdegree);
void getVs(uint512_vec_dt * cData_kvdramV3, vertex_t vbatch_kvs, vertex_t * firstv, vertex_t * secondv, vertex_t * lastv);

// DRAM Access Methods
void buffervertexupdates(uint512_vec_dt * kvdram, unsigned int kvdrambeginoffset, metadata_t * kvstats, vector<uint512_vec_dt> (&buffered_nvmeFd_r)[MAXNUMSSDPARTITIONS]);

// File Access Methods
void loadverticesfromfile(unsigned int iteration_idx, FILE * nvmeFd_vertices_r, vertexprop_t * verticesbuffer, vertex_t batchsize);
void loadverticesfromfileII(unsigned int iteration_idx, FILE * nvmeFd_vertices_r, uint512_vec_dt * kvdramA, vertex_t kvoffset, vertex_t kvbatchsize);
void writeverticestofileII(unsigned int iteration_idx, FILE * nvmeFd_vertices_w, uint512_vec_dt * kvdramA, vertex_t kvoffset, vertex_t kvbatchsize);
void loadedgesfromfile(unsigned int iteration_idx, FILE * nvmeFd_edges_r, unsigned int beginoffset, unsigned int endoffset, edgeprop_t * edgesbuffer);
void loadedgesfromfileII(unsigned int iteration_idx, FILE * nvmeFd_edges_r, uint512_vec_dt * kvdramA, vertex_t kvoffset, vertex_t kvbatchsize);
void storevertexupdatestofile(keyvalue_t * kvs_partitions[NUMSSDPARTITIONS], unsigned int kvs_partitions_weof[NUMSSDPARTITIONS], FILE * nvmeFd_kvs_w[MAXNUMSSDPARTITIONS]);
void storevertexupdatestofileII(vector<keyvalue_t> (&kvs_partitions)[NUMSSDPARTITIONS], unsigned int kvs_partitions_weof[NUMSSDPARTITIONS], FILE * nvmeFd_kvs_w[MAXNUMSSDPARTITIONS]);
void storevertexupdatestofileIII(uint512_vec_dt * kvdram, unsigned int kvdrambeginoffset, metadata_t * kvstats, FILE * nvmeFd_kvs_w[MAXNUMSSDPARTITIONS]);
void storevertexupdatestofileIV(vector<keyvalue_t> & kvs_updates, FILE * nvmeFd_vus_w);
void storestatstofile(FILE * nvmeFd_vus_w, FILE * nvmeFd_vus_stats_w);

// Generate KV Updates
void generatekvupdatesbatch(vertexprop_t * verticesbuffer, vertex_t verticesbuffersz, edgeprop_t * edgesbuffer, edge_t edgesbuffersz, vector<keyvalue_t> & kvs_partitions, edge_t ebase);
void generatekvupdates(vertex_t num_vertices, FILE * nvmeFd_vertices_r, FILE * nvmeFd_edges_r, FILE * nvmeFd_vus_w);

// Collect SSD partition stats
void collectssdpartitionstats(unsigned int nvmeFd_kvs_weofs[NUMSSDPARTITIONS], FILE * nvmeFd_kvs_w[MAXNUMSSDPARTITIONS]);
void collectssdpartitionstats2(edge_t * nvmeFd_vus_weofs, FILE * nvmeFd_ALLkvs_w);

// Populate & Initialize KvDrams
void populatekvdrams(unsigned int iteration_idx, FILE * nvmeFd_r, vector<uint512_vec_dt> & buffered_nvmeFd_r, bool bufferkvupdates, uint512_vec_dt * kvdram, vertex_t kvbeginoffset, vertex_t kvbatchsz);
unsigned int populatekvdramsII(EdgeProcess<uint32_t,uint32_t>* edge_process, SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader, uint512_vec_dt * kvdram, unsigned int * status);
void initializekvdram(unsigned int iteration_idx, uint512_vec_dt * kvdram, vertex_t kvoffset, vertex_t kvsize, value_t init_val, unsigned int rootvid);

// Collect stats
void collectstats(uint512_vec_dt * kvdram, metadata_t * kvstats, vertex_t kvoffset, vertex_t kvbatchsz, vertex_t kvrangeoffset, unsigned int finalnumpartitionsfordram_partition, unsigned int finalrangefordram_partition);

// Prepare datasets
void preparedataset(dataset_t dataset);
#endif







