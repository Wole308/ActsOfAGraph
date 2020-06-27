#ifndef EDGE_PROCESS_H
#define EDGE_PROCESS_H
#include <mutex>
#include "../utility/utility.h"
#include "../algorithm/algorithm.h"
#include "../graphs/graph.h"
#include "common.h"

class edge_process {
public:
	edge_process(graph * graphobj);
	~edge_process();
	
	unsigned int generateupdates_random(SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader, int nvmeFd_edgeoffsets_r2, int nvmeFd_edgeproperties_r2, uint512_vec_dt * kvdram, unsigned int kvoffset, unsigned int * status);
	unsigned int generateupdates2_random(SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader, keyvalue_t * kvdram, unsigned int kvoffset, unsigned int * status);
	void generatekeyvalues_random(vertex_t key, value_t val, int nvmeFd_edgeoffsets_r2, int nvmeFd_edgeproperties_r2, keyvalue_t * kvdram, unsigned int kvoffset, unsigned int * kvsize);
	
	unsigned int generateupdates_stream(unsigned int edgesoffset, unsigned int basevertexoffset, unsigned int * runningvertexid, vertex_t edgespropsz, int nvmeFd_edgeproperties_r2, int nvmeFd_edgeoffsets_r2, vertexprop_t * vertexpropertiesbuffer, unsigned int * isactivevertexinfobuffer, uint512_vec_dt * kvdram, keyvalue_t * kvstats, unsigned int threadidx);
	unsigned int generatekeyvalues_stream(unsigned int edgesoffset, unsigned int basevertexoffset, unsigned int * runningvertexid, vertexprop_t * vertexpropertiesbuffer, unsigned int * isactivevertexinfobuffer, vertex_t edgesbuffersz, keyvalue_t * kvdram, vertex_t kvdramoffset);
	
	void generateupdates_contiguous(size_t wordoffset, vertex_t edgespropsz, int nvmeFd_edgeproperties_r2, int nvmeFd_edgeoffsets_r2, vertexprop_t * vertexpropertiesbuffer, uint512_vec_dt * kvdram, keyvalue_t * kvstats, unsigned int threadidx);
	void generatekeyvalues_contiguous(vertexprop_t * vertexpropertiesbuffer, keyvalue_t * kvdram, vertex_t vertexpropsz, vertex_t edgespropsz, vertex_t kvdramoffset);
	
	template <class T>
	void loadedgepropertiesfromfile(int nvmeFd_edgeproperties_r2, size_t fileoffset, T * buffer, edge_t bufferoffset, vertex_t numbytestoread);
	template <class T>
	void loadedgeoffsetsfromfile(int nvmeFd_edgeoffsets_r2, size_t fileoffset, T * buffer, vertex_t bufferoffset, size_t numbytestoread);
	void collectstats(uint512_vec_dt * kvdram, keyvalue_t * kvstats, vertex_t kvdramoffset, vertex_t kvstatsoffset, vertex_t kvsize, vertex_t kvrangeoffset, unsigned int LLOPnumpartitions, unsigned int LLOPrangepartitions);
	void calculateoffsets(keyvalue_t * kvstats, vertex_t kvstatsoffset, unsigned int LLOPnumpartitions);

private:	
	unsigned int numvertexbanks;
	unsigned int numedgebanks;
	unsigned int numverticespervertexbank;
	vertex_t numvertices;
	
	edgeprop1_t * edgesbuffer_stream;
	edgeprop2_t * edgesbuffer_contiguous;
	prvertexoffset_t * edgeoffsetsbuffer;
	
	size_t m_idx_buffer_offset;
	size_t m_idx_buffer_bytes;
	void * mp_idx_buffer;
	// size_t m_edge_buffer_offset; // FIXME
	uint64_t m_edge_buffer_offset;
	size_t m_edge_buffer_bytes;
	void * mp_edge_buffer;
	// static const size_t m_buffer_alloc_bytes = (1024*32); // original*	
	// static const size_t m_buffer_alloc_bytes = (1024*2);
	static const size_t m_buffer_alloc_bytes = (1024*2);
	size_t m_edgebuffer_alloc_bytes; // = (1024*2); // CHANGED
	std::atomic<size_t> m_index_blocks_read;
	std::atomic<size_t> m_edge_blocks_read;
	
	algorithm * algorithmobj;
	utility * utilityobj;
};
#endif






