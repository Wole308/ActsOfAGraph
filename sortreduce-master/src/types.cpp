#include "types.h"
#include "../../include/common.h" // ACTGraph header

// #define LARGEMEMORY

#if (defined(FPGA_IMPL) || defined(HW)) // FIXME
template <class K, class V>
SortReduceTypes::Config<K,V>::Config() {
	this->temporary_directory = "/oj2zf/tmpdata_grafboost";
	this->output_filename = "";

	this->maximum_threads = 8;

	this->update = NULL;
	this->quiet = false;
	
	#ifdef LARGEMEMORY
	this->buffer_size = 8*1024*1024;
	this->buffer_count = 256;
	#else 
	// this->buffer_size = 8*1024*1024;
	// this->buffer_count = 64;	
	this->buffer_size = 8*1024*1024;
	this->buffer_count = 256;
	#endif 
	this->max_bytes_inflight = 0;
}
#else 
template <class K, class V>
SortReduceTypes::Config<K,V>::Config(std::string temporary_directory, std::string output_filename, int max_threads) {
	this->temporary_directory = temporary_directory;
	this->output_filename = output_filename;

	if ( max_threads < 0 ) {
		this->maximum_threads = std::thread::hardware_concurrency();
	} else {
		this->maximum_threads = max_threads;
	}

	this->update = NULL;
	this->quiet = false;
	
	#ifdef LARGEMEMORY
	this->buffer_size = 8*1024*1024;
	this->buffer_count = 256;
	#else 
	// this->buffer_size = 8*1024*1024;
	// this->buffer_count = 64;	
	this->buffer_size = 8*1024*1024;
	this->buffer_count = 256;
	#endif 
	this->max_bytes_inflight = 0;
}
#endif 

template <class K, class V>
void 
SortReduceTypes::Config<K,V>::SetManagedBufferSize(size_t buffer_size, int buffer_count) {
	this->buffer_size = buffer_size;
	this->buffer_count = buffer_count;
}

template <class K, class V>
void 
SortReduceTypes::Config<K,V>::SetMaxBytesInFlight(size_t buffer_size) {
	this->max_bytes_inflight = buffer_size;
}

template <class K, class V>
void
SortReduceTypes::Config<K,V>::SetUpdateFunction(V (*update)(V,V)) {
	this->update = update;
}

/*
template <class K, class V>
void
SortReduceTypes::Config<K,V>::SetUpdateFunction(uint64_t (*update64)(uint64_t,uint64_t)) {
	this->update64 = update64;
}
*/

SortReduceTypes::Status::Status() {
	this->done_input = false;
	this->done_inmem = false;
	this->done_external = false;
	this->done_file = NULL;
}

TEMPLATE_EXPLICIT_INSTANTIATION(SortReduceTypes::Config)
TEMPLATE_EXPLICIT_INSTANTIATION(SortReduceTypes::KvPair)
TEMPLATE_EXPLICIT_INSTANTIATION(SortReduceTypes::KvPairSrc)
