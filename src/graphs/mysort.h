#ifndef MYSORT_H
#define MYSORT_H
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../../include/host_common.h"
#include "../../include/common.h"

class mysort {
public:
	mysort(graph * _graphobj);						
	~mysort();
	
	bool compare(uint32_t a[2], uint32_t b[2]);
	bool compareeq(uint32_t a[2], uint32_t b[2]);

	void swap(uint32_t* buffer, uint32_t a, uint32_t b) ;
	void sort_block(uint32_t* buffer, int count);

	bool check_sorted(uint32_t* buffer, int count);
	void sort_block_q(uint32_t* buffer, int count);
	
	bool copyFile(const char *_src, const char* _dest);

	void start();
	
private:
	graph * graphobj;
};
#endif








