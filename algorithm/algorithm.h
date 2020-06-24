#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "../src/common.h"
#include "../src/host_common.h"

class algorithm {
public:
	algorithm();
	~algorithm();
	
	int amin(size_t val1, size_t val2);
	uint32_t vertex_update_add(uint32_t a, uint32_t b);
	uint32_t vertex_update(uint32_t a, uint32_t b);
	static uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout);
	uint32_t finalize_program(uint32_t oldval, uint32_t val);
	bool is_active(uint32_t old, uint32_t newv, bool marked);
	value_t cummulate(value_t data1, value_t data2);
	value_t vertex_initdata();
	value_t vertex_inittempdata();
	uint32_t apply(value_t vtempdata, value_t vdata);
	
private:
};
#endif








