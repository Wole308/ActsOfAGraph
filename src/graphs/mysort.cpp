#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <fstream>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../include/common.h"
#include "../../include/host_common.h"
#include "mysort.h"

#ifdef AWS_PLATFORM
string datasetRootDir_4sort = "/oj2zf/";
#else
string datasetRootDir_4sort = "/localtmp/oj2zf/";
#endif

// #define SORT_BLOCK_SIZE (1024*1024)
#define SORT_BLOCK_SIZE (1024*128) // CRITICAL REMOVEME....................................................

// kron28 
// nohup ./obj/mysort 0 /localtmp/oj2zf/dataset/kronecker28/kronecker28.dat [4B]
// nohup ./obj/mysort 0 /localtmp/oj2zf/dataset/kronecker26/kronecker26.dat [1B]
// nohup ./obj/mysort 0 /localtmp/oj2zf/dataset/kronecker30/kronecker30.dat
// nohup ./obj/mysort 0 /localtmp/oj2zf/dataset/roadNet-CA/roadNet-CA_1by1/roadNet-CA_dup_0_0.edges

mysort::mysort(graph * _graphobj){
	graphobj = _graphobj;
	
	// string edgesoutpath = "/localtmp/oj2zf/dataset/flickr/tmpfiles/";
		// std::ofstream ofs; ofs.open(edgesoutpath.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	//////////
		// exit(EXIT_SUCCESS);
}
mysort::~mysort() {}

// returns true if a <= b
bool mysort::compare(uint32_t a[2], uint32_t b[2]) {
	if ( a[0] > b[0] ) return false;
	if ( a[0] < b[0] ) return true;
	if ( a[1] > b[1] ) return false;
	return true;
}
bool mysort::compareeq(uint32_t a[2], uint32_t b[2]) {
	if ( a[0] == b[0] && a[1] == b[1] ) return true;
	return false;
}

void mysort::swap(uint32_t* buffer, uint32_t a, uint32_t b) {
	uint32_t temp[2] = {buffer[a*2], buffer[a*2+1]};
	buffer[a*2] = buffer[b*2];
	buffer[a*2+1] = buffer[b*2+1];
	
	buffer[b*2] = temp[0];
	buffer[b*2+1] = temp[1];
}
void mysort::sort_block(uint32_t* buffer, int count) {
	for ( int i = 0; i < count; i++ ) {
		for ( int j = i; j < count; j++ ) {
			if ( buffer[j*2] < buffer[i*2] ||
				(buffer[j*2] == buffer[i*2] && buffer[j*2+1] < buffer[i*2+1])) {

				uint32_t temp[2] = {buffer[i*2], buffer[i*2+1]};
				buffer[i*2] = buffer[j*2];
				buffer[i*2+1] = buffer[j*2+1];
				
				buffer[j*2] = temp[0];
				buffer[j*2+1] = temp[1];
			}
		}
	}
	uint32_t last[2] = {buffer[0], buffer[1]};
	for ( int i = 0; i < count; i++ ) {
		uint32_t cur[2] = {buffer[i*2],buffer[i*2+1]};
		if ( !compare(last, cur) ) {
			printf( "Block mysort has errors!\n" );
		}
		last[0] = cur[0];
		last[1] = cur[1];
	}
}

bool mysort::check_sorted(uint32_t* buffer, int count) {
	uint32_t last[2] = {buffer[0], buffer[1]};
	for ( int i = 0; i < count; i++ ) {
		uint32_t cur[2] = {buffer[i*2],buffer[i*2+1]};
		if ( !compare(last, cur) ) {
			return false;
		}
		last[0] = cur[0];
		last[1] = cur[1];
	}
	return true;
}
void mysort::sort_block_q(uint32_t* buffer, int count) {
	if ( count <= 1 ) return;

	if ( count <= 32 ) {
		sort_block(buffer,count);
		return;
	};

	uint32_t z[2] = {buffer[0], buffer[1]};

	int nel = -1;
	bool alleq = true;
	for ( int i = 1; i < count; i++ ) {
		uint32_t a[2] = {buffer[i*2], buffer[i*2+1]};
		if ( !compareeq (a,z) ) {
			alleq = false;
			nel = i;
			break;
		}
	}
	if ( alleq ) return;

	if ( nel >= 0 ) {
		swap(buffer,0,nel);
	}
	z[0] = buffer[0];
	z[1] = buffer[1];

	int lc = 1;
	for ( int i = 1; i < count; i++ ) {
		uint32_t a[2] = {buffer[i*2], buffer[i*2+1]};
		if ( compare (a,z) ) {
			swap(buffer, i,lc);
			lc++;
		}
	}
	swap(buffer,0,lc-1);

	sort_block_q(buffer, lc);
	sort_block_q(buffer+(lc*2), count-lc);

	if ( count < SORT_BLOCK_SIZE ) return;
}

bool mysort::copyFile(const char *_src, const char* _dest){
    std::ifstream src(_src, std::ios::binary);
    std::ofstream dest(_dest, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

void mysort::start() {
	int slev = 0;
	
	// check before sorting
	#ifdef KOOOKOOOO
	string icur_in_filename_str = datasetRootDir_4sort + "dataset/" + graphtopname + "/" + graphtopname + "_1by1/" + graphtopname + "_dup_0_0.edges"; /////////
	printf( "[before sorting] Checking final file, %s\n", icur_in_filename_str );
	FILE* ifchin = fopen(icur_in_filename_str.c_str(), "rb");
	uint32_t ilast[2] = {0,0};
	uint32_t icount = 0;
	while (!feof(ifchin)) {
		uint32_t cur[2] = {0,0};
		if ( fread(cur, sizeof(uint32_t)*2, 1, ifchin) == 0 ) break;
		
		if(icount < 64){ printf( "%i, %i \n", cur[0], cur[1] ); }

		if ( !compare(ilast, cur) ) {
			printf( "Sort error @ %ld (%ld %ld) > (%ld %ld)\n", icount,
				ilast[0], ilast[1], cur[0], cur[1]);
			exit(EXIT_FAILURE);
		}
		ilast[0] = cur[0];
		ilast[1] = cur[1];
		icount++;
	}
	#endif 
	
	// sorting
	cout<<"mysort:: sorting started... "<<endl;
	string graphtopname = graphobj->getdataset().graphtopname;
	string path_edges;
	uint32_t block_count = 0;
	if ( slev == 0 ) {
		path_edges = datasetRootDir_4sort + "dataset/" + graphtopname + "/" + graphtopname + "_1by1/" + graphtopname + "_dup_0_0.edges";
		// FILE * pFile = fopen (path_edges.c_str(), "w"); // NEWCHANGE.
		/* if (pFile!=NULL){
			// fputs ("fopen example", pFile);
			fclose (pFile);
		} */
		
		// cout<<"---------------------------------------------------------- "<<endl;
		
		// std::ofstream ofs; ofs.open(path_edges.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();		//////////	
		
		// cout<<"--------------------------------------------+++++-------------- "<<endl;
  
		FILE* fin = fopen(path_edges.c_str(), "rb");
		
		// cout<<"--------------------------------------------^^^^-------------- "<<endl;
		
		uint32_t* block = (uint32_t*)malloc(sizeof(uint32_t)*2*SORT_BLOCK_SIZE);

		string edgesoutpath = datasetRootDir_4sort + "dataset/" + graphtopname + "/tmpfiles/temp_0.dat";
		// string edgesoutpath = datasetRootDir_4sort + "dataset/flickr/tmpfiles/temp_0.dat";
		// std::ofstream ofs; ofs.open(edgesoutpath.c_str(), std::ofstream::out | std::ofstream::trunc); ofs.close();	//////////
		FILE* fout = fopen(edgesoutpath.c_str(), "wb");
		
		// cout<<"--------------------------------------------^^^^ 2 -------------- "<<endl;

		while ( !feof(fin) ) {
			size_t readcount = fread(block, sizeof(uint32_t)*2, SORT_BLOCK_SIZE, fin);
			if ( readcount == 0 ) break;
			
			// cout<<"--------------------------------------------^^^^ 3 -------------- "<<endl;

			printf( "Sorting block %ld (%ld)\n", block_count, readcount );
			sort_block_q(block, readcount);
			
			fwrite(block, sizeof(uint32_t)*2, readcount, fout);
			block_count ++;
		}
		fclose(fout);
		fclose(fin);

		printf("Initial block-level sorting finished!\n");
	}

	char cur_in_filename[128];
	char cur_out_filename[128];
	// Mergesort stages
	int init_stage = 0;
	if ( slev > 0 ) init_stage = slev - 1;
	uint32_t merge_block_count = 1<<init_stage;
	for (int sort_stage = init_stage; ; sort_stage++) {
		string cur_in_filename_str = datasetRootDir_4sort + "dataset/" + graphtopname + "/tmpfiles/temp_%d.dat";
		sprintf(cur_in_filename, cur_in_filename_str.c_str(), sort_stage);
		FILE* fin1 = fopen(cur_in_filename, "rb");
		FILE* fin2 = fopen(cur_in_filename, "rb");

		if ( slev == 0 ) {
		/*
			&& block_count != bcnt ) {
			printf( "STRANGE! block_count does not match estimate from file %ld,0x%lx %ld,0x%lx\n", block_count, block_count, bcnt, bcnt );
			exit(1);
		*/
		}
		else {
			fseek(fin1, 0, SEEK_END);
			uint32_t fsz = ftell(fin1);
			fseek(fin1, 0, SEEK_SET);

			uint32_t bcnt = fsz/SORT_BLOCK_SIZE/sizeof(uint32_t)/2;
			printf( "Merging %ld,0x%lx blocks\n", bcnt, bcnt );
			block_count = bcnt;
		}

		string cur_out_filename_str = datasetRootDir_4sort + "dataset/" + graphtopname + "/tmpfiles/temp_%d.dat";
		sprintf(cur_out_filename, cur_out_filename_str.c_str(), sort_stage+1);
		printf("Sorting stage %d! %s -- %ld\n", sort_stage, cur_in_filename, block_count);
		FILE* fout =fopen(cur_out_filename,"wb");
		
		uint32_t cur_block_off = 0;
		// Mergesort blocks
		while (!feof(fin1) && !feof(fin2)) {
			uint32_t startoff1 = cur_block_off * SORT_BLOCK_SIZE*2*sizeof(uint32_t);
			uint32_t startoff2 = (cur_block_off + merge_block_count) * SORT_BLOCK_SIZE*2*sizeof(uint32_t);
			fseek(fin1, startoff1, SEEK_SET);
			fseek(fin2, startoff2, SEEK_SET);
			uint32_t buf1[2];
			uint32_t buf2[2];
			fread(buf1, sizeof(uint32_t)*2, 1, fin1);
			fread(buf2, sizeof(uint32_t)*2, 1, fin2);
			uint32_t off1 = 0;
			uint32_t off2 = 0;

			printf( "Sorting blocks %ld %ld (%ld)\n", startoff1,startoff2,merge_block_count );
			while (1) {
				if ( compare(buf1,buf2) ) {
					fwrite(buf1,sizeof(uint32_t)*2,1,fout);
					if ( fread(buf1, sizeof(uint32_t)*2, 1, fin1) == 0 ) break;
					off1 ++;
					if (off1 >= merge_block_count*SORT_BLOCK_SIZE) {
						while(off2 < merge_block_count*SORT_BLOCK_SIZE) {
							fwrite(buf2,sizeof(uint32_t)*2,1,fout);
							if ( fread(buf2, sizeof(uint32_t)*2, 1, fin2) == 0 ) break; 
							off2 ++;
						}
						break;
					}
				} else {
					fwrite(buf2,sizeof(uint32_t)*2,1,fout);
					off2 ++;
					if ( fread(buf2, sizeof(uint32_t)*2, 1, fin2) == 0 ||
						off2 >= merge_block_count*SORT_BLOCK_SIZE) {

						while(off1 < merge_block_count*SORT_BLOCK_SIZE) {
							fwrite(buf1,sizeof(uint32_t)*2,1,fout);
							if ( fread(buf1, sizeof(uint32_t)*2, 1, fin1) == 0 ) break;
							off1 ++;
						}
						break;
					}
				}
			}


			cur_block_off += merge_block_count*2;
		}
		
		merge_block_count *= 2;

		fclose(fin1);
		fclose(fin2);
		fclose(fout);
		if (sort_stage > 0 ) std::remove(cur_in_filename); //FIXME
		if ( merge_block_count >= block_count ) break;
	}

	// check after sorting
	printf( "[after sorting] Checking final file, %s\n", cur_out_filename );
	FILE* fchin = fopen(cur_out_filename, "rb");
	uint32_t last[2] = {0,0};
	uint32_t count = 0;
	while (!feof(fchin)) {
		uint32_t cur[2] = {0,0};
		if ( fread(cur, sizeof(uint32_t)*2, 1, fchin) == 0 ) break;
		
		if(count < 8){ printf( "%i, %i \n", cur[0], cur[1] ); }

		if ( !compare(last, cur) ) {
			printf( "Sort error @ %ld (%ld %ld) > (%ld %ld)\n", count,
				last[0], last[1], cur[0], cur[1]);
		}
		last[0] = cur[0];
		last[1] = cur[1];
		count++;
	}
	printf ( "Checked %ld samples\n", count );
	
	cout<<"mysort:: copying back file..."<<endl;
	// path_edges = datasetRootDir_4sort + "dataset/" + graphtopname + "/" + graphtopname + "_1by1/" + graphtopname + "_dup_0_0.edgessss";
	copyFile(cur_out_filename, path_edges.c_str());
	cout<<"mysort:: sort successful."<<endl;
}




