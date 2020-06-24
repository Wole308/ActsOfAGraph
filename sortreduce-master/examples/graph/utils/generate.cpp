#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <cmath>

int main( int argc, char** argv) {
	srand(time(0));
	
	// run 
	// nohup sudo ./obj/generate
	// nohup ./obj/generate
	
	// kron30
	// uint64_t scale = 30; //*** defines NUM VERTICES
	// int edgefactor=16; //FIXME //*** defines NUM EDGES
	
	uint64_t scale = 26; //*** defines NUM VERTICES
	int edgefactor=16; //FIXME //*** defines NUM EDGES
	
	/* double seed[2][2] = {
		{0.57,0.19},
		{0.19,0.05}
	}; */
	double seed[2][2] = {
		{0.25,0.25},
		{0.25,0.25}
	};
	// char ofilename[128] = "/home/oj2zf/Documents/datasets/kronecker30/kronecker30.dat";
	char ofilename[128] = "/localtmp/oj2zf/datasets/kronecker26/kronecker26.dat";

	if ( argc >= 2 ) {
		strncpy(ofilename, argv[1], 128);
	}

	if ( argc >= 3 ) {
		scale = atoi(argv[2]);
	}

	double probsum = seed[0][0]+seed[0][1]+seed[1][0]+seed[1][1];


	double normfactor = 1024.0/probsum;
	int nseed[2][2] = {
		{(int)(seed[0][0]*normfactor), (int)(seed[0][1]*normfactor)},
		{(int)(seed[1][0]*normfactor), (int)(seed[1][1]*normfactor)}
	};
	nseed[0][1] += nseed[0][0];
	nseed[1][0] += nseed[0][1];
	nseed[1][1] += nseed[1][0];

	FILE* fout = fopen(ofilename, "wb");

	uint64_t nodecount = (1L<<scale);
	//int64_t edgecount = (int64_t)std::pow(probsum, scale);
	uint64_t edgecount = nodecount*edgefactor;
	//fwrite(&nodecount, sizeof(uint64_t), 1, fout);
	//fwrite(&edgecount, sizeof(uint64_t), 1, fout);
	printf( "Generating %ld nodes\n", nodecount );
	printf( "Generating %ld edges\n", edgecount );
	printf( "Edgefactor: %d\n", edgefactor);

	uint64_t count[2][2] = {{0,0},{0,0}};

	for ( uint64_t e = 0; e < edgecount; e++ ) {
		uint64_t u = 0;
		uint64_t v = 0;
		for ( uint64_t i = 0; i < scale; i++ ) {
			int rv = rand()%1024;
			u <<= 1;
			v <<= 1;
			if ( rv < nseed[0][0] ) {
				if ( !i ) count[0][0]++;
			} else if (rv <nseed[0][1]) {
				if ( !i ) count[0][1]++;
				u |= 1;
			} else if (rv <nseed[1][0]) {
				if ( !i ) count[1][0]++;
				v |= 1;
			} else {
				if ( !i ) count[1][1]++;
				u |= 1;
				v |= 1;
			}
		}
		// printf( "%i, %i \n", u, v );
		fwrite(&u, sizeof(uint64_t), 1, fout);
		fwrite(&v, sizeof(uint64_t), 1, fout);
	}
	fclose(fout);
	printf( "Finished generating graph\nGraph needs to be sorted!\n" );
	printf( "%ld %ld %ld %ld\n", count[0][0], count [0][1], count[1][0], count[1][1] );
}
