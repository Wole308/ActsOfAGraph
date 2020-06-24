#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// #define EDGEDRAMSZ 268434944

// utils/obj/encode flickr.txt N Y
// utils/obj/encode /home/wole.j/Documents/graphs/flickr.graph N Y
// utils/obj/encode /home/wole.j/Documents/graphs/twitter7.graph N Y
// utils/obj/encode /home/wole.j/Documents/graphs/twitter7.graph Y Y //***

// nohup utils/obj/encode /projects/DPE/wole/kronecker28/kronecker28.graph Y Y
// nohup utils/obj/encode /projects/DPE/wole/kronecker30/kronecker30.graph Y Y
// nohup utils/obj/encode /mnt/data1/kronecker30_32/kronecker30_32.graph Y Y
// nohup utils/obj/encode /mnt/data1/kronecker30_64/kronecker30_64.graph Y Y

// nohup obj/encode /localtmp/oj2zf/datasets/kronecker26/kronecker26.graph Y Y

int main(int argc, char** argv) {
	if ( argc < 2 ) {
		fprintf(stderr, "usage: %s filename [32bit? Y/N] [text source? Y/N]\n", argv[0]);
		exit(1);
	}

	FILE* fin = fopen(argv[1], "rb");
	if ( fin == NULL ) {
		fprintf(stderr, "Filename %s not found\n", argv[1]);
		exit(1);
	}
	FILE* fint = fopen(argv[1], "r");


	bool file32 = true;
	if ( argc >= 3 ) {
		if ( argv[2][0] == 'Y' || argv[2][0] == 'y' ) {
			file32 = true;
		} else {
			file32 = false;
		}
	}
	
	bool textsource = false;
	if ( argc >= 4 ) {
		if ( argv[3][0] == 'Y' || argv[3][0] == 'y' ) {
			textsource = true;
		} else {
			textsource = false;
		}
	}

	printf( "Starting encoding!\n Input: %s Output: %dbit\n", textsource?"text":"binary", file32?32:64 );

	// FILE* fidx = fopen("ridx.dat", "wb");
	// FILE* fdat = fopen("matrix.dat", "wb");
	
	// FILE* fidx = fopen("/projects/DPE/wole/ridx.dat", "wb");
	// FILE* fdat = fopen("/projects/DPE/wole/matrix.dat", "wb");
	
	// FILE* fidx = fopen("/mnt/data1/wdc/ridx.dat", "wb");
	// FILE* fdat = fopen("/mnt/data1/wdc/matrix.dat", "wb");
	
	// FILE* fidx = fopen("/projects/DPE/wole/kronecker28/ridx.dat", "wb");
	// FILE* fdat = fopen("/projects/DPE/wole/kronecker28/matrix.dat", "wb");
	
	// FILE* fidx = fopen("/projects/DPE/wole/kronecker30/ridx.dat", "wb");
	// FILE* fdat = fopen("/projects/DPE/wole/kronecker30/matrix.dat", "wb");
	
	// FILE* fidx = fopen("/mnt/data1/kronecker30_32/ridx.dat", "wb");
	// FILE* fdat = fopen("/mnt/data1/kronecker30_32/matrix.dat", "wb");
	
	FILE* fidx = fopen("/localtmp/oj2zf/datasets/kronecker26/ridx.dat", "wb");
	FILE* fdat = fopen("/localtmp/oj2zf/datasets/kronecker26/matrix.dat", "wb");
	
	///projects/DPE/wole

	uint64_t cur_node = 0;
	uint64_t cur_boff = 0; // byte offset of matrix

	uint64_t cur[2] = {0,0};
	fwrite(&cur_boff, sizeof(uint64_t), 1, fidx);
	uint64_t last_edge = 0;//xFFFFFFFFFFFFFFFF;

	uint64_t total_edges = 0;
	uint64_t nodes_noedge = 0;
	char line[256];
	
	/* #ifdef EDGEDRAMSZ
	int cnt = 0;
	#endif */
		
	while (!feof(fin)) {
		if ( textsource == true ) {
			if( !fgets(line, 256, fint) ) break;
			
			bool isvalid = true; for(int t=0; t<256; t++){ if(line[t] == '%'){ isvalid = false; } }
			if(isvalid == false){ continue; }
			
			char* from = strtok(line, " \t");
			char* to = strtok(NULL, " \t");
			// char* to = strtok(line, " \t");
			// char* from = strtok(NULL, " \t");

			cur[0] = atoll(from);
			cur[1] = atoll(to);
		} else {
			if ( fread(cur, sizeof(uint64_t)*2, 1, fin) == 0 ) break;
		}

		uint64_t rnode = cur[0];
		uint64_t redge = cur[1];

		if ( rnode == redge ) continue;
		if ( rnode < cur_node ) {
			printf( "node order wrong! %ld -> %ld\n", cur_node, rnode );
			continue;
		}

		if ( cur_node == rnode ) {
			if ( last_edge > redge ) {
				printf( "edge order wrong! %ld -> %ld\n", last_edge, redge );
				continue;
			}
			if ( last_edge > redge ) {
				printf( "Error!! Dest edge not sorted!\n" ); // FIXME! rnode < cur_node already checks for this...
				exit(1);
			}
			if ( last_edge == redge ) continue;

			if ( file32 ) {
				uint32_t sedge = (uint32_t)redge;
				fwrite(&sedge, sizeof(uint32_t), 1, fdat);
			} else {
				fwrite(&redge, sizeof(uint64_t), 1, fdat);
			}
			if ( file32 ) {
				cur_boff+=sizeof(uint32_t);
			} else {
				cur_boff+=sizeof(uint64_t);
			}
			last_edge = redge;
			total_edges++;
		} else {
			if ( cur_node > rnode ) { // FIXME rnode< cur_node already checks for this
				printf( "Error!! Source node not sorted!\n" );
				exit(0);
			}
			if ( rnode-cur_node > 1 ) {
				nodes_noedge += rnode-cur_node-1;
			}

			while (cur_node < rnode) {
				fwrite(&cur_boff, sizeof(uint64_t), 1, fidx);
				cur_node++;
				//cur_boff+=sizeof(uint64_t);
				//printf("Node %lx Edge boffset %lx\n", cur_node, cur_boff);

			}
			if ( file32 ) {
				uint32_t sedge = (uint32_t)redge;
				fwrite(&sedge, sizeof(uint32_t), 1, fdat);
			} else {
				fwrite(&redge, sizeof(uint64_t), 1, fdat);
			}
			total_edges++;
			if ( file32 ) {
				cur_boff+=sizeof(uint32_t);
			} else {
				cur_boff+=sizeof(uint64_t);
			}
			last_edge = redge;
		}

		if ( total_edges % (1024L*1024L*64L) == 0 ) {
			printf( "Total edges: %ld\n", total_edges );
		}
		
		/* #ifdef EDGEDRAMSZ
		cnt += 1;
		if(cnt > EDGEDRAMSZ){ break; }
		#endif */
	}
	// add one more index entry to specify the end of the last vertex
	fwrite(&cur_boff, sizeof(uint64_t), 1, fidx);
	//cur_node++;
	printf("Total edges: %ld\n", total_edges);
	printf("Isolated nodes: %ld\n", nodes_noedge);
	fclose(fidx);
}
