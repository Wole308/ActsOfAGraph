#ifndef ACTGRAPH_PR_FPGA_H
#define ACTGRAPH_PR_FPGA_H
#include <mutex>
#include "kernelprocess.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#include "actgraph_pr_sw.h"
#include "common.h"

#ifdef FPGA_IMPL
class actgraph_pr_fpga {
public:
	actgraph_pr_fpga(unsigned int _graph_iterationidx, graph * graphobj, std::string binaryFile);
	~actgraph_pr_fpga();
	
	void run();
	void Start();
	void finish();
	void summary();
	float totalkerneltime();
	float totalpopulateKvDRAMtime();
	
	void WorkerThread(int threadidx, int gbankoffset);
	void workerthread_generatekvs(int threadidx, unsigned int subthreadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t edgepropertyfilesize, unsigned int iteration_idx, unsigned int flag);
	
	size_t hceildiv(size_t val1, size_t val2);	
	void printkvdrams(uint512_vec_dt * kvdram);
	void printstats(unsigned int size, metadata_t * kvstats);
	void printstructures(unsigned int threadidx, unsigned int flag);
	
private:
	cl_int err;	
	size_t global; 
	size_t local;
	
	unsigned int kvdramsz_kvs; 
	size_t kv_size_bytes;
	size_t kvstats_size_bytes;
	size_t messages_size_bytes;
	
	size_t kvV1_size_bytes;
	size_t kvV2_size_bytes;
	size_t kvV3_size_bytes;
	
	size_t kvV1_offset_bytes;
	size_t kvV2_offset_bytes;
	size_t kvV3_offset_bytes;
	
	// FPGA events
	cl_event kernel_events[2];
	cl_event read_event[8][2];

	// Mapped Memory
	uint512_vec_dt * cData_kvdramAV[2];
	metadata_t * cData_kvstatsA[2];
	int * cData_messagesA[2];
	uint512_vec_dt * cData_kvdramBV[2];
	metadata_t * cData_kvstatsB[2];
	int * cData_messagesB[2];
	uint512_vec_dt * cData_kvdramCV[2];
	metadata_t * cData_kvstatsC[2];
	int * cData_messagesC[2];
	uint512_vec_dt * cData_kvdramDV[2];
	metadata_t * cData_kvstatsD[2];
	int * cData_messagesD[2];
	
	xcl_world world;
	cl_program program;
	cl_kernel kernel;
	
	cl_mem buffer_kvdramA[2];
	cl_mem buffer_kvdramB[2];
	cl_mem buffer_kvdramC[2];
	cl_mem buffer_kvdramD[2];
	cl_mem buffer_kvstatsA[2];
	cl_mem buffer_kvstatsB[2];
	cl_mem buffer_kvstatsC[2];
	cl_mem buffer_kvstatsD[2];
	cl_mem buffer_messagesA[2];
	cl_mem buffer_messagesB[2];
	cl_mem buffer_messagesC[2];
	cl_mem buffer_messagesD[2];

	// Others 
	unsigned int graph_iterationidx;
	
	float totaltime_topkernel_ms;
	float totaltime_populatekvdram_ms;
	
	int nvmeFd_vertexoffsets_r2[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	int nvmeFd_verticesdata_r2[MAXNUMVERTEXBANKS];
	int nvmeFd_verticesdata_w2[MAXNUMVERTEXBANKS];
	int nvmeFd_vertexproperties_r2[MAXNUMVERTEXBANKS];
	int nvmeFd_edgeproperties_r2[MAXNUMEDGEBANKS][MAXNUMEDGEBANKS];
	
	actgraph_pr_sw * actgraph_pr_sw_obj;
	edge_process * edge_process_obj[NUMCPUTHREADS];
	edge_process * edge_process_obj2[NUMDRAMBANKS];
	utility * utilityobj[NUMCPUTHREADS];
	
	vector<uint512_vec_dt> unused_vectorbuffer;
	edge_t totalnumkvsread[NUMCPUTHREADS];
	std::thread panas_thread[NUMCPUTHREADS]; 
	std::thread genw_thread[NUMDRAMBANKS]; // NEW
	
	vertexprop_t * vertexpropertiesbuffer[MAXNUMVERTEXBANKS];
	edgeprop2_t * edgesbuffer[NUMDRAMBANKS];
	prvertexoffset_t * vertexoffsetsbuffer[NUMDRAMBANKS];
	
	unsigned int numvertexbanks;
	unsigned int numedgebanks;
	unsigned int numverticespervertexbank;
};
#endif 
#endif








