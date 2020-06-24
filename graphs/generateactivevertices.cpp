#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <map>
#include <tuple>
#include <string>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"
#include "EdgeProcess.h"
#include "VertexValues.h"
#include "../heuristics/heuristics.h"
#include "graph.h"
#include "generateactivevertices.h"
using namespace std;

generateactivevertices::generateactivevertices(graph * _graphobj){
	graphobj = _graphobj;
}
generateactivevertices::~generateactivevertices(){} 

inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	uint32_t ret = a;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	return vid;
}

inline uint32_t finalize_program(uint32_t oldval, uint32_t val) {
	return val;
}

inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	if ( old == 0xffffffff ) return true;
	return false;
}

void generateactivevertices::generate(){
	#ifdef SW
	#if not (defined(GRAFBOOST_SETUP) & defined(BFS_ALGORITHM))
	cout<<"generateactivevertices:: WARNING. must be GRAFBOOST_SETUP & BFS_ALGORITHM settings to generate. exiting..."<<endl; exit(EXIT_FAILURE);
	#endif 
	cout<<"generateactivevertices:: generating active vertices for workload "<<graphobj->getdataset().graphname<<", numvertexbanks: "<<graphobj->getnumvertexbanks()<<", numedgebanks: "<<graphobj->getnumedgebanks()<<"..."<<endl;					
	srand(time(0));

	/* string tmp_dir = graphobj->gettmp_dir().c_str();
	string idx_path = graphobj->getidx_path().c_str();
	string mat_path = graphobj->getmat_path().c_str(); */
	
	string tmp_dir = graphobj->gettmp_dir();
	string idx_path = graphobj->getidx_path();
	string mat_path = graphobj->getmat_path();

	int max_thread_count = 12;
	int max_sr_thread_count = 8;
	int max_vertexval_thread_count = 4;
	int max_edgeproc_thread_count = 8;
		
	std::chrono::high_resolution_clock::time_point start_all;
	start_all = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point now;
	std::chrono::milliseconds duration_milli;

	EdgeProcess<uint32_t,uint32_t>* edge_process = new EdgeProcess<uint32_t,uint32_t>(idx_path, mat_path, &edge_program);
	size_t vertex_count = edge_process->GetVertexCount();
	VertexValues<uint32_t,uint32_t>* vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, 0xffffffff, &is_active, &finalize_program, max_vertexval_thread_count);

	keyvalue_t keyvalue; keyvalue.key = 12; keyvalue.value = 0;
	graphobj->saveactiveverticestofile(0, keyvalue); // save active vertices to file
	
	int iteration = 0;
	while ( true ) {
		//char filename[128];
		//sprintf(filename, "out%04d.sr", iteration);
	
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
			// edge_process->SourceVertex(12,0, true);
			edge_process->SourceVertex(121,0, true);
		} else {
			// TODO Spawn edge process threads

			int fd = vertex_values->OpenActiveFile(iteration-1);
			graphobj->saveactiveverticestofile(iteration, fd); // save active vertices to file
			SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(fd);
			std::tuple<uint32_t,uint32_t,bool> res = reader->Next();
			while ( std::get<2>(res) ) {
				uint32_t key = std::get<0>(res);
				uint32_t val = std::get<1>(res);
				
				edge_process->SourceVertex(key,val, true);
				res = reader->Next();

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

			if ( (size_t)key <= vertex_count ) {
				while ( !vertex_values->Update(key,val) ) ;
			}
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
		// if(iteration == 3){ break; }

		delete sr;
	}
	int fd = vertex_values->OpenActiveFile(iteration-1);
	graphobj->saveactiveverticestofile(iteration, fd); // save active vertices to file
	
	keyvalue_t empty;
	graphobj->saveactiveverticestofile(iteration + 1, empty, false); // save active vertices to file
	
	now = std::chrono::high_resolution_clock::now();
	duration_milli = std::chrono::duration_cast<std::chrono::milliseconds> (now-start_all);
	printf( "\t\t++ All Done! %lu ms\n", duration_milli.count() );
	#endif 
	return;
}


