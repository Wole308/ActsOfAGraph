#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <map>
#include <tuple>

#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"

#include "EdgeProcess.h"
#include "VertexValues.h"

// utils/obj/encode flickr.txt N Y
// utils/obj/encode /home/wole.j/Documents/graphs/flickr.graph N Y
// utils/obj/encode /home/wole.j/Documents/graphs/twitter7.graph N Y

/** 
sudo utils/obj/encode /mnt/data1/twitter7/twitter7.graph Y Y
sudo mv matrix.dat /mnt/data1/twitter7/matrix.dat
sudo mv ridx.dat /mnt/data1/twitter7/ridx.dat

sudo utils/obj/encode /mnt/data1/it_2004/it_2004.graph Y Y
sudo mv matrix.dat /mnt/data1/it_2004/matrix.dat
sudo mv ridx.dat /mnt/data1/it_2004/ridx.dat

sudo utils/obj/encode /mnt/data1/sk_2005/sk_2005.graph Y Y
sudo mv matrix.dat /mnt/data1/sk_2005/matrix.dat
sudo mv ridx.dat /mnt/data1/sk_2005/ridx.dat

sudo utils/obj/encode /mnt/data1/mycielskian20/mycielskian20.graph Y Y
sudo mv matrix.dat /mnt/data1/mycielskian20/matrix.dat
sudo mv ridx.dat /mnt/data1/mycielskian20/ridx.dat

sudo utils/obj/encode /mnt/data1/friendster/friendster.graph Y Y
sudo mv matrix.dat /mnt/data1/friendster/matrix.dat
sudo mv ridx.dat /mnt/data1/friendster/ridx.dat

sudo utils/obj/encode /mnt/data1/wdc/wdc.graph N Y
sudo mv matrix.dat /mnt/data1/wdc/matrix.dat
sudo mv ridx.dat /mnt/data1/wdc/ridx.dat

sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/twitter7/ridx.dat /mnt/data1/twitter7/matrix.dat > res_twitter7.out
sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/it_2004/ridx.dat /mnt/data1/it_2004/matrix.dat > res_it_2004.out
sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/sk_2005/ridx.dat /mnt/data1/sk_2005/matrix.dat > res_sk_2005.out
sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/mycielskian20/ridx.dat /mnt/data1/mycielskian20/matrix.dat > res_mycielskian20.out
sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/friendster/ridx.dat /mnt/data1/friendster/matrix.dat > res_friendster.out 
sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/wdc/ridx.dat /mnt/data1/wdc/matrix.dat > res_wdc.out 

sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/twitter7/ridx.dat /mnt/data1/twitter7/matrix.dat
sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/kronecker28/ridx.dat /mnt/data1/kronecker28/matrix.dat
sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/kronecker30/ridx.dat /mnt/data1/kronecker30/matrix.dat
nohup sudo obj/pr /mnt/data1/grafboost_ssd/ /mnt/data1/kronecker30_32/ridx.dat /mnt/data1/kronecker30_32/matrix.dat

nohup obj/pr /home/oj2zf/Documents/tmpdata_grafboost /localtmp/oj2zf/datasets/kronecker26/ridx.dat /localtmp/oj2zf/datasets/kronecker26/matrix.dat

nohup obj/pr /localtmp/oj2zf/tmpdata_grafboost /localtmp/oj2zf/datasets/kronecker26/ridx.dat /localtmp/oj2zf/datasets/kronecker26/matrix.dat
nohup obj/pr /localtmp/oj2zf/tmpdata_grafboost /localtmp/oj2zf/datasets/kronecker28/ridx.dat /localtmp/oj2zf/datasets/kronecker28/matrix.dat
nohup obj/pr /localtmp/oj2zf/tmpdata_grafboost /localtmp/oj2zf/datasets/kronecker30/ridx.dat /localtmp/oj2zf/datasets/kronecker30/matrix.dat
nohup obj/pr /localtmp/oj2zf/tmpdata_grafboost /localtmp/oj2zf/datasets/kronecker32/ridx.dat /localtmp/oj2zf/datasets/kronecker32/matrix.dat


*/

inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	float af = *(float*)&a;
	float bf = *(float*)&b;
	float tf = af+bf;
	
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	float af = *(float*)&value;
	float tf = af/(float)fanout;

	uint32_t ret = *(uint32_t*)&tf;

	return ret;
}

size_t g_vertex_count = 1;
inline uint32_t finalize_program(uint32_t oldval, uint32_t val) {
	float damp = 0.15/(float)g_vertex_count;
	float af = *(float*)&val;
	float tf = damp + af*0.85;
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}

inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	if ( !marked ) return false;

	float af = *(float*)&old;
	float bf = *(float*)&newv;

	if ( af > bf ) {
		if (af-bf < 0.0001 ) return false;
		else return true;
	} else {
		if (bf-af < 0.0001 ) return false;
		else return true;
	}
}

int main(int argc, char** argv) {
	srand(time(0));

	if ( argc < 4 ) {
		fprintf(stderr, "usage: %s ridx matrix directory \n", argv[0] );
		exit(1);
	}

	char* tmp_dir = argv[1];
	char* idx_path = argv[2];
	char* mat_path = argv[3];

	int max_thread_count = 12;
	int max_sr_thread_count = 8;
	int max_vertexval_thread_count = 4;
	int max_edgeproc_thread_count = 8;
	if ( argc > 4 ) {
		max_thread_count = atoi(argv[4]);
	}
	if ( max_thread_count >= 32 ) {
		max_sr_thread_count = 28;
		max_vertexval_thread_count = 8;
		max_edgeproc_thread_count = 8;
	} else if ( max_thread_count >= 24 ) {
		max_sr_thread_count = 20;
		max_vertexval_thread_count = 8;
		max_edgeproc_thread_count = 8;
	}
		
	std::chrono::high_resolution_clock::time_point start_all;
	start_all = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point now;
	std::chrono::milliseconds duration_milli;

	//conf->SetManagedBufferSize(1024*1024*4, 256); // 4 GiB


	EdgeProcess<uint32_t,uint32_t>* edge_process = new EdgeProcess<uint32_t,uint32_t>(idx_path, mat_path, &edge_program);
	size_t vertex_count = edge_process->GetVertexCount();
	g_vertex_count = vertex_count;
	float init_val = 1.0/(float)vertex_count;
	VertexValues<uint32_t,uint32_t>* vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, *(uint32_t*)&init_val, &is_active, &finalize_program, max_vertexval_thread_count);

	int iteration = 0;
	while ( true ) {

		SortReduceTypes::Config<uint32_t,uint32_t>* conf =
			new SortReduceTypes::Config<uint32_t,uint32_t>(tmp_dir, "", max_sr_thread_count);
		conf->quiet = true;
		conf->SetUpdateFunction(&vertex_update);
		
		SortReduce<uint32_t,uint32_t>* sr = new SortReduce<uint32_t,uint32_t>(conf);
		SortReduce<uint32_t,uint32_t>::IoEndpoint* ep = sr->GetEndpoint(true);
		edge_process->SetSortReduceEndpoint(ep);
		for ( int i = 0; i < max_edgeproc_thread_count; i++ ) {
			//FIXME this is inefficient...
			edge_process->AddSortReduceEndpoint(sr->GetEndpoint(true));
		}

		std::chrono::high_resolution_clock::time_point start, iteration_start;
		start = std::chrono::high_resolution_clock::now();
		iteration_start = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds iteration_duration_milli;

		printf( "\t\t++ Starting iteration %d\n", iteration ); fflush(stdout);
		edge_process->Start();

		if ( iteration == 0 ) {
			for ( size_t i = 0; i < vertex_count; i++ ) {
				edge_process->SourceVertex((uint32_t)i,*(uint32_t*)&init_val, true);
			}
		} else {
			// TODO subgraph selection needs row-compresed file.
			SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>("vertex_data.dat", conf);
			std::tuple<uint32_t,uint32_t,bool> res = reader->Next();
			uint32_t idx = 0;
			while ( std::get<2>(res) ) {
				//uint32_t iteration = std::get<0>(res);
				uint32_t val = std::get<1>(res);
				edge_process->SourceVertex(idx,val, true);
				res = reader->Next();
				idx++;

				//printf( "Vertex %lx %lx\n", key, val );
			}
			delete reader;
		}
		edge_process->Finish();
		sr->Finish();
		now = std::chrono::high_resolution_clock::now();
		duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start);
		printf( "\t\t++ Input done : %lu ms\n", duration_milli.count() );

		start = std::chrono::high_resolution_clock::now();
		SortReduceTypes::Status status = sr->CheckStatus();
		while ( status.done_external == false ) {
			usleep(1000);
			status = sr->CheckStatus();
		}

		now = std::chrono::high_resolution_clock::now();
		duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start);
		printf( "\t\t++ Sort-Reduce done : %lu ms\n", duration_milli.count() );
		fflush(stdout);

		
		start = std::chrono::high_resolution_clock::now();
		std::tuple<uint32_t,uint32_t,bool> res = sr->Next();
		vertex_values->Start();
		while ( std::get<2>(res) ) {
			uint32_t key = std::get<0>(res);
			uint32_t val = std::get<1>(res);

			//printf( "\t\t++ SRR %x %x\n", key, val );
			while ( !vertex_values->Update(key,val) ) ;

			res = sr->Next();
		}


		vertex_values->Finish();
		size_t active_cnt = vertex_values->GetActiveCount();

		now = std::chrono::high_resolution_clock::now();
		duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start);
		iteration_duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-iteration_start);
		
		printf( "\t\t++ Iteration done : %lu ms / %lu ms, Active %ld\n", duration_milli.count(), iteration_duration_milli.count(), active_cnt );
		if ( active_cnt == 0 ) break;
		vertex_values->NextIteration();
		iteration++;

		delete sr;

		//if ( iteration > 20 ) break;
		if ( iteration >= 1 ) break;
	}
	now = std::chrono::high_resolution_clock::now();
	duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start_all);
	printf( "\t\t++ All Done! %lu ms\n", duration_milli.count() );

	exit(0);
}
