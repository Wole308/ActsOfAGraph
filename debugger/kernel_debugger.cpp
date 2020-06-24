#include "/home/oj2zf/Documents/ActsOfAGraph/debugger/kernel_debugger.h"

#ifdef _DEBUGMODE_HEADER
void printstats_kernel(metadata_t * stats, unsigned int size, std::string message, unsigned int val1, unsigned int val2, unsigned int val3){
	std::cout<< std::endl << "printer:: printstats_kernel:: printing stats {offset, size}. size: "<<size<<", message"<<message<<", val1: "<<val1<<", val2: "<<val2<<", val3: "<<val3<<endl;
	for(unsigned int i=0; i<size; i++){
		std::cout<< stats[i].offset << ", " << stats[i].size << endl;
	}
	std::cout << std::endl;
	return;
}

void printstats2_kernel(smallmetadata_t * stats, unsigned int size, unsigned int val1, unsigned int val2, unsigned int val3){
	std::cout<< std::endl << "printer:: printstats2_kernel:: printing stats. size: "<<size<<", val1: "<<val1<<", val2: "<<val2<<", val3: "<<val3<<endl;
	for(unsigned int i=0; i<size; i++){
		std::cout<< stats[i].offset << ", " << stats[i].size << endl;
	}
	std::cout << std::endl;
	return;
}

void printstats3_kernel(smalluint_t * stats, unsigned int size, string message, unsigned int val1, unsigned int val2, unsigned int val3){
	std::cout<< std::endl << "printer:: printstats3_kernel:: printing stats. size: "<<size<<", val1: "<<val1<<", val2: "<<val2<<", val3: "<<val3<<endl;
	for(unsigned int i=0; i<size; i++){
		std::cout<< "stats["<<i<<"].size: " << stats[i] << endl;
	}
	std::cout << std::endl;
	return;
}

void printkvbuffer_kernel(uint512_dt * kvbuffer, unsigned int size, string message){
	std::cout<< std::endl << "printer:: printkvbuffer_kernel:: printing kvbuffer. size: "<<size<<", message: "<<message<<std::endl;
	for(unsigned int i=0; i<(size / VECTOR_SIZE); i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			#ifndef _WIDEWORD
			std::cout<< kvbuffer[i].data[j].key << ", " << 0 << endl;
			#else
			// unsigned int ihi = ((VECTOR_SIZE - j) * 32) - 1;
			// unsigned int ilo = ihi - 31;
			// std::cout<< kvbuffer[i].range(ihi, ilo) << ", " << 0 << endl;
			#endif
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}

void printkvs2_kernel(keyvalue_t * kvs, string message){
	std::cout<< std::endl << "printer:: printkvbuffer_kernel:: printing kvbuffer. message: "<<message<<std::endl;
	for(unsigned int j=0; j<VECTOR_SIZE; j++){
		std::cout<< kvs[j].key << ", " << 0 << endl;
	}
	std::cout << std::endl;
	return;
}

void print1int_kernel(string context, string message1, unsigned int val1){
	cout<<"debugger::print1int_kernel::"<<context<<":: ";
	cout<<message1<<": "<<val1;
	cout<<endl;
	return;
}

void print2ints_kernel(string context, string message1, unsigned int val1, string message2, unsigned int val2){
	cout<<"debugger::print2ints_kernel::"<<context<<":: ";
	cout<<message1<<": "<<val1<<", ";
	cout<<message2<<": "<<val2<<", ";
	cout<<endl;
	return;
}

void print4ints_kernel(string context, string message1, unsigned int val1, string message2, unsigned int val2, string message3, unsigned int val3, string message4, unsigned int val4){
	cout<<"debugger::print4ints_kernel::"<<context<<":: ";
	cout<<message1<<": "<<val1<<", ";
	cout<<message2<<": "<<val2<<", ";
	cout<<message3<<": "<<val3<<", ";
	cout<<message4<<": "<<val4;
	cout<<endl;
	return;
}

void printovs2_kernel(margin_t * ov, unsigned int p, std::string message, unsigned int val1, unsigned int val2, unsigned int val3){
	cout<<"DEBUGGER::PRINTOVS::"<<message<<":: ov["<<p<<"].begin: "<<ov[p].begin<<", ov["<<p<<"].size: "<<ov[p].size<<", val1: "<<val1<<", val2: "<<val2<<", val3: "<<val3<<endl;
	return;
}

unsigned int getdestskipsize_kernel(int currentLOP, int param){
	unsigned int pow = NUM_PARTITIONS;
	unsigned int _finalnumpartitions = param / NUM_PARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){
		_finalnumpartitions = _finalnumpartitions / NUM_PARTITIONS;
	}
	return _finalnumpartitions;
}

void checkforoutofbounds_kernel(unsigned int val, unsigned int compsz, std::string message, unsigned int val1, unsigned int val2, unsigned int val3){
	// std::cout<< std::endl << "========================================printer:: checkforoutofbounds_kernel:: val: "<<val<<std::endl;
	if(val >= compsz){
		cout<<""<<message<<": ERROR. out-of-bounds. val: "<<val<<", compsz: "<<compsz<<", val1: "<<val1<<", val2: "<<val2<<", val3: "<<val3<<endl;
		exit (EXIT_FAILURE);
	} else {
		// cout<<""<<message<<": SUCCESS. not out-of-bounds. val: "<<val<<", compsz: "<<compsz<<", val1: "<<val1<<", val2: "<<val2<<", val3: "<<val3<<endl;
	}
	return;
}

void check_kvdram_kernel(uint512_dt * kvdramA, unsigned int baseaddr_kvs, metadata_t * kvstats_tmp, unsigned int partitionoffset, unsigned int currentLOP, unsigned int treedepth, unsigned int finalnumpartitions, unsigned int kvdata_range){		
	std::cout<< std::endl << "printer:: check_kvdram_kernel:: checking kvdram. baseaddr_kvs: "<<baseaddr_kvs<<", partitionoffset: "<<partitionoffset<<", currentLOP: "<<currentLOP<<", finalnumpartitions: "<<finalnumpartitions<<std::endl;
	cout<<"check_kvdram_kernel:: kvstats_tmp[0].size: "<<kvstats_tmp[0].size<<endl;
	cout<<"check_kvdram_kernel:: kvstats_tmp[1].size: "<<kvstats_tmp[1].size<<endl;

	unsigned int num_destpartitions_in_lastLOP = pow(NUM_PARTITIONS, treedepth);
	unsigned int range_per_destpartition = kvdata_range / num_destpartitions_in_lastLOP; // KVDATA_RANGE
	unsigned int destskipsize = getdestskipsize_kernel(currentLOP, num_destpartitions_in_lastLOP);

	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int lower_bound = (partitionoffset + (p * destskipsize)) * range_per_destpartition;
		unsigned int upper_bound = (partitionoffset + ((p + 1) * destskipsize)) * range_per_destpartition;
		unsigned int size = kvstats_tmp[p].size / VECTOR_SIZE;
		unsigned int offset = kvstats_tmp[p].offset / VECTOR_SIZE;
		unsigned int count = 0;
		unsigned int rowcount = 0;
		#ifdef xxx
		std::cout<<"partition: "<<(partitionoffset + p)<<std::endl;
		std::cout<<"upper_bound: "<<upper_bound<<std::endl;
		std::cout<<"lower_bound: "<<lower_bound<<std::endl;
		std::cout<<"kvstats_tmp["<<p<<"].offset: "<<kvstats_tmp[p].offset<<std::endl;
		std::cout<<"kvstats_tmp["<<p<<"].size: "<<kvstats_tmp[p].size<<std::endl;
		std::cout<<"offset: "<<offset<<std::endl;
		std::cout<<"size: "<<size<<std::endl;
		std::cout<<std::endl<<std::endl;
		#endif

		for(unsigned int i=offset; i<(offset + size); i++){
			for(int j=0; j<VECTOR_SIZE; j++){
				unsigned int data;
				#ifdef _WIDEWORD
				data = kvdramA[(baseaddr_kvs + i)].range((((j+1)*32)-1), (j*32));
				#else
				data = kvdramA[(baseaddr_kvs + i)].data[j].key;
				#endif
				count += 1;

				if(data > upper_bound || data < lower_bound){
					cout<<"DEBUGGER::check_kvdram_kernel:: ERROR: out of bounds seen in kvdram. i: "<<i<<", j: "<<j<<", data: "<<data<<", lower_bound: "<<lower_bound<<", upper_bound: "<<upper_bound<<", offset: "<<offset<<", (offset + size): "<<(offset + size)<<endl;
					exit(EXIT_FAILURE);
				} else {
					// cout<<"DEBUGGER::check_kvdram_kernel:: SUCCESS: within bounds seen in kvdram. i: "<<i<<", j: "<<j<<", data: "<<data<<", lower_bound: "<<lower_bound<<", upper_bound: "<<upper_bound<<endl;
				}
			}
			rowcount += 1;
		}
		std::cout<<"status: partition check successful. "<<count<<" kvs seem, "<<rowcount<<" rows traversed"<<std::endl;
	}
	cout<<"DEBUGGER::check_kvdram_kernel:: ALL CHECKS COMPLETED SUCCESSFULLY"<<endl;
	return;
}

void checkentryforoutofbounds_kernel(unsigned int partitionoffset, unsigned int partition, unsigned int currentLOP, unsigned int treedepth, unsigned int kvdata_range, unsigned int data, string message, unsigned int val1, unsigned int val2, unsigned int val3){
	unsigned int num_destpartitions_in_lastLOP = pow(NUM_PARTITIONS, treedepth);
	unsigned int kvrange_for_each_partition_in_lastLOP = kvdata_range / num_destpartitions_in_lastLOP; // KVDATA_RANGE
	unsigned int destskipsize = getdestskipsize_kernel(currentLOP, num_destpartitions_in_lastLOP);
	// NB: unit of destskipsize is wrt. *lastLOP*
	unsigned int lower_bound = (partitionoffset + (partition * destskipsize)) * kvrange_for_each_partition_in_lastLOP;
	unsigned int upper_bound = (partitionoffset + ((partition + 1) * destskipsize)) * kvrange_for_each_partition_in_lastLOP;

	if(data > upper_bound || data < lower_bound){
		cout<<"DEBUGGER::checkentryforoutofbounds_kernel:: ERROR: out of bounds seen 67. data: "<<data
		<<", lower_bound: "<<lower_bound
		<<", upper_bound: "<<upper_bound
		<<", partition: "<<partition
		<<", partitionoffset: "<<partitionoffset
		<<", kvrange_for_each_partition_in_lastLOP: "<<kvrange_for_each_partition_in_lastLOP
		<<", num_destpartitions_in_lastLOP: "<<num_destpartitions_in_lastLOP
		<<", destskipsize: "<<destskipsize
		<<", message: "<<message
		<<", val1: "<<val1
		<<", val2: "<<val2
		<<", val3: "<<val3
		<<endl;
		exit(EXIT_FAILURE);
	} else {
		// cout<<"DEBUGGER::checkentryforoutofbounds_kernel:: SUCCESS: within bounds seen. data: "<<data<<", lower_bound: "<<lower_bound<<", upper_bound: "<<upper_bound<<", partition: "<<partition<<", message: "<<message<<", val1: "<<val1<<", val2: "<<val2<<", val3: "<<val3<<endl;
	}
	// cout<<"DEBUGGER::checkentryforoutofbounds_kernel:: ALL CHECKS COMPLETED SUCCESSFULLY"<<endl;
	return;
}

void checkforzeroornegative_kernel(int val, string message){
	if(val <= 0) {
		cout<<""<<message<<": ERROR. zero or negative value. val: "<<val<<endl;
		exit (EXIT_FAILURE);
	}
	return;
}
#endif


