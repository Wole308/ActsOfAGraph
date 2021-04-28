#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
// #include <iostream> //
#include <cstring> //
#include <cstdio>

#define SORT_BLOCK_SIZE (1024*1024)

using namespace std;

// run

// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/kron_g500-logn22/kron_g500-logn22.dat

// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/RMAT22_SPARSE0/sparse.dat
// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/RMAT22_SPARSE1/sparse.dat
// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/RMAT22_SPARSE2/sparse.dat
// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/RMAT22_SPARSE3/sparse.dat
// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/RMAT22_SPARSE4/sparse.dat
// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/RMAT22_SPARSE5/sparse.dat
// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/RMAT22_SPARSE6/sparse.dat
// nohup ./obj/sort 0 /localtmp/oj2zf/dataset/RMAT22_SPARSE7/sparse.dat

// #define VARYBYNONE
// #define VARYBYSPARSITY
#define VARYBYRANGE

// returns true if a <= b
bool compare(uint32_t a[2], uint32_t b[2]) {
	if ( a[0] > b[0] ) return false;
	if ( a[0] < b[0] ) return true;
	if ( a[1] > b[1] ) return false;
	return true;
}
bool compareeq(uint32_t a[2], uint32_t b[2]) {
	if ( a[0] == b[0] && a[1] == b[1] ) return true;
	return false;
}

void swap(uint32_t* buffer, uint32_t a, uint32_t b) {
	uint32_t temp[2] = {buffer[a*2], buffer[a*2+1]};
	buffer[a*2] = buffer[b*2];
	buffer[a*2+1] = buffer[b*2+1];
	
	buffer[b*2] = temp[0];
	buffer[b*2+1] = temp[1];
}
void sort_block(uint32_t* buffer, int count) {
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
			printf( "Block sort has errors!\n" );
		}
		last[0] = cur[0];
		last[1] = cur[1];
	}
}

bool check_sorted(uint32_t* buffer, int count) {
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
void sort_block_q(uint32_t* buffer, int count) {
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

int main( int argc, char** argv ) {
	if ( argc < 2 ) {
		fprintf(stderr, "usage: %s startlev [filename]\n", argv[0]);
		exit(1);
	}
	
	int slev = atoi(argv[1]);

	#ifdef VARYBYNONE
	unsigned int totalsize = 1;
	#else 
	unsigned int totalsize = 6;	
	#endif
	for(unsigned int topi=0; topi<totalsize; topi++){
		
		printf(">>> Sorting process. topi: %i \n", topi);
	
		uint32_t block_count = 0;
		if ( slev == 0 ) { 
			#ifdef VARYBYNONE
			if ( argc < 3 ) {
				printf( "Initial filename not given!\n" );
				exit(1);
			}
			#endif 
			
			FILE* fin;
			#ifdef VARYBYNONE
			fin = fopen(argv[2], "rb");
			#endif 
			#ifdef VARYBYSPARSITY
			char sfoutt[128]; sprintf(sfoutt, "/localtmp/oj2zf/dataset/RMAT22_SPARSE%i/RMAT22_SPARSE%i.dat", topi, topi);
			fin = fopen(sfoutt, "rb");
			#endif
			#ifdef VARYBYRANGE
			char sfoutt[128]; sprintf(sfoutt, "/localtmp/oj2zf/dataset/RMAT_RANGE%i/RMAT_RANGE%i.dat", topi, topi);
			fin = fopen(sfoutt, "rb");
			#endif
			
			if ( fin == NULL ) {
				fprintf(stderr, "Filename %s not found\n", argv[1]);
				exit(1);
			}

			uint32_t* block = (uint32_t*)malloc(sizeof(uint32_t)*2*SORT_BLOCK_SIZE);

			#ifdef VARYBYNONE
			FILE* fout = fopen("/localtmp/oj2zf/dataset/kron_g500-logn22/temp_0.dat", "wb");
			#endif
			#ifdef VARYBYSPARSITY
			char sfout[128]; sprintf(sfout, "/localtmp/oj2zf/dataset/RMAT22_SPARSE%i/temp_0.dat", topi);
			FILE* fout = fopen(sfout, "wb");
			#endif 
			#ifdef VARYBYRANGE
			char sfout[128]; sprintf(sfout, "/localtmp/oj2zf/dataset/RMAT_RANGE%i/temp_0.dat", topi);
			FILE* fout = fopen(sfout, "wb");
			#endif 

			while ( !feof(fin) ) {
				size_t readcount = fread(block, sizeof(uint32_t)*2, SORT_BLOCK_SIZE, fin);
				if ( readcount == 0 ) break;

				printf( "Sorting block %ld (%ld)\n", block_count, readcount );
				//if ( !check_sorted( block, readcount ) ) {
					sort_block_q(block, readcount);
				//}
				
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
			#ifdef VARYBYNONE
			sprintf(cur_in_filename, "/localtmp/oj2zf/dataset/kron_g500-logn22/temp_%d.dat", sort_stage);
			#endif 
			#ifdef VARYBYSPARSITY
			sprintf(cur_in_filename, "/localtmp/oj2zf/dataset/RMAT22_SPARSE%i/temp_%d.dat", topi, sort_stage);
			#endif 
			#ifdef VARYBYRANGE
			sprintf(cur_in_filename, "/localtmp/oj2zf/dataset/RMAT_RANGE%i/temp_%d.dat", topi, sort_stage);
			#endif 
			
			FILE* fin1 = fopen(cur_in_filename, "rb");
			FILE* fin2 = fopen(cur_in_filename, "rb");

			if ( slev == 0 ) {
			}
			else {
				fseek(fin1, 0, SEEK_END);
				uint32_t fsz = ftell(fin1);
				fseek(fin1, 0, SEEK_SET);

				uint32_t bcnt = fsz/SORT_BLOCK_SIZE/sizeof(uint32_t)/2;
				printf( "Merging %ld,0x%lx blocks\n", bcnt, bcnt );
				block_count = bcnt;
			}

			#ifdef VARYBYNONE
			sprintf(cur_out_filename, "/localtmp/oj2zf/dataset/kron_g500-logn22/temp_%d.dat", sort_stage+1);
			#endif 
			#ifdef VARYBYSPARSITY
			sprintf(cur_out_filename, "/localtmp/oj2zf/dataset/RMAT22_SPARSE%i/temp_%d.dat", topi, sort_stage+1);
			#endif 
			#ifdef VARYBYRANGE
			sprintf(cur_out_filename, "/localtmp/oj2zf/dataset/RMAT_RANGE%i/temp_%d.dat", topi, sort_stage+1);
			#endif 
			
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
			
			printf( "----- sort_stage: %i \n", sort_stage);

			fclose(fin1);
			fclose(fin2);
			fclose(fout);
			if (sort_stage > 0 ) std::remove(cur_in_filename); //FIXME
			if ( merge_block_count >= block_count ) break;
		}
		
		#ifdef VARYBYNONE
		FILE * file_graph;
		file_graph = fopen ("/localtmp/oj2zf/dataset/kron_g500-logn22/kron_g500-logn22.mtx", "w");
		if (file_graph==NULL)	{ printf("extract_vertices::extract_vertices : error : cannot find the location of input file (vertices): /localtmp/oj2zf/dataset/kron_g500-logn22/kron_g500-logn22.mtx"); }			
		else { printf("extract_vertices::extract_vertices : extracting vertices from /localtmp/oj2zf/dataset/kron_g500-logn22/kron_g500-logn22.mtx"); }
		#endif 
		#ifdef VARYBYSPARSITY
		char sfile_graph[128]; sprintf(sfile_graph, "/localtmp/oj2zf/dataset/RMAT22_SPARSE%i/RMAT22_SPARSE%i.mtx", topi, topi);
		FILE * file_graph;
		file_graph = fopen(sfile_graph, "w");
		if (file_graph==NULL)	{ printf("extract_vertices::extract_vertices : error : cannot find the location of input file (vertices): %s", sfile_graph); }			
		else { printf("extract_vertices::extract_vertices : extracting vertices from %s", sfile_graph); }
		#endif 
		#ifdef VARYBYRANGE
		char sfile_graph[128]; sprintf(sfile_graph, "/localtmp/oj2zf/dataset/RMAT_RANGE%i/RMAT_RANGE%i.mtx", topi, topi);
		FILE * file_graph;
		file_graph = fopen(sfile_graph, "w");
		if (file_graph==NULL)	{ printf("extract_vertices::extract_vertices : error : cannot find the location of input file (vertices): %s", sfile_graph); }			
		else { printf("extract_vertices::extract_vertices : extracting vertices from %s", sfile_graph); }
		#endif 
		
		printf( "Checking final file, %s\n", cur_out_filename );
		
		FILE* fchin = fopen(cur_out_filename, "rb");

		uint32_t last[2] = {0,0};

		uint32_t count = 0;
		while (!feof(fchin)) {
			uint32_t cur[2] = {0,0};
			if ( fread(cur, sizeof(uint32_t)*2, 1, fchin) == 0 ) break;
			
			// printf( "%i, %i \n", cur[0], cur[1] );
			// fprintf(file_graph, "%lu %lu \n", cur[0], cur[1]); // SRC_DEST
			fprintf(file_graph, "%lu %lu \n", cur[1], cur[0]); // DEST_SRC

			if ( !compare(last, cur) ) {
				printf( "Sort error @ %ld (%ld %ld) > (%ld %ld)\n", count,
					last[0], last[1], cur[0], cur[1]);
			}
			last[0] = cur[0];
			last[1] = cur[1];
			count++;
			
			if ((count % 1000000) == 0){ printf( "sort:: current edge count: %i\n", count ); }
		}
		printf ( "Checked %ld samples\n", count );
	}
}
