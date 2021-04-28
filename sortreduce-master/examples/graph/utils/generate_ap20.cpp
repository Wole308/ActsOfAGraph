#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

unsigned int numargs = 1; // (***switch choice HERE***)
unsigned int maxsize = 6; // 6;

int main( int argc, char** argv) {
	srand(time(0));
	
	// run 
	// nohup sudo ./obj/generate
	// nohup ./obj/generate

	uint32_t scale = 22;
	int edgefactor = 30; // 35*, 40, 50 // real

	// uint32_t scale = 18; // test // CRITICAL CHANGEME.
	// int edgefactor = 16;
	
	// uint32_t scale = 8; // test // CRITICAL CHANGEME.
	// int edgefactor = 8;
	
	unsigned int totalsize; // 4,8
	if(numargs == 1){ totalsize = maxsize; }
	else { totalsize = 1; }
	for(unsigned int topi=0; topi<totalsize; topi++){
	
		/* double seed[2][2] = {
			{0.57,0.19},
			{0.19,0.05}
		}; */
		/* double seed[2][2] = {
			{0.25,0.25},
			{0.25,0.25}
		}; */
		
		double seed[2][2]; /// GOODDD
		
		// irregular graphs
		if(topi==0){
			seed[0][1] = 0.25; 
			seed[1][0] = 0.25; 
			seed[1][1] = 0.25; 
			seed[0][0] = 1 - (seed[0][1] + seed[1][0] + seed[1][1]); 
		} else if(topi==1){
			seed[0][1] = 0.24; 
			seed[1][0] = 0.25; 
			seed[1][1] = 0.26; 
			seed[0][0] = 1 - (seed[0][1] + seed[1][0] + seed[1][1]); 
		} 
		
		// regular graphs
		else if(topi==2){
			seed[0][1] = 0.20; 
			seed[1][0] = 0.20; 
			seed[1][1] = 0.20; 
			seed[0][0] = 1 - (seed[0][1] + seed[1][0] + seed[1][1]); 
		} else if(topi==3){
			seed[0][1] = 0.20; 
			seed[1][0] = 0.21; 
			seed[1][1] = 0.22; 
			seed[0][0] = 1 - (seed[0][1] + seed[1][0] + seed[1][1]); 
		} 
		
		// regular graphs
		else if(topi==4){
			seed[0][1] = 0.16; 
			seed[1][0] = 0.16; 
			seed[1][1] = 0.16; 
			seed[0][0] = 1 - (seed[0][1] + seed[1][0] + seed[1][1]); 
		} else if(topi==5){
			seed[0][1] = 0.16; 
			seed[1][0] = 0.17; 
			seed[1][1] = 0.18; 
			seed[0][0] = 1 - (seed[0][1] + seed[1][0] + seed[1][1]); 
		} 
		
		 // (***switch choice HERE***)
		 // char ofilename[128];
		 // if(numargs == 1){
			char ofilename[128];
			sprintf(ofilename, "/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE%i/RMAT22_SPARSE%i.dat", topi, topi);
		 // } else {
			// char ofilename[128] = "/localtmp/oj2zf/dataset/kron-g500-logn22/kron-g500-logn22.dat";
		 // }
		 
		 
		/* // char ofilename[128] = "/localtmp/oj2zf/dataset/kron-g500-logn22/kron-g500-logn22.dat";
		// char ofilename[128] = "/localtmp/oj2zf/dataset/kron-g500-logn23/kron-g500-logn23.dat";
		
		// char ofilename[128];
		
		// sprintf(ofilename, "/localtmp/oj2zf/dataset/RMAT22_SPARSE%i/RMAT22_SPARSE%i.dat", topi, topi);
		// sprintf(ofilename, "/net/bigtemp/oj2zf/gunrock_wole/dataset/large/RMAT22_SPARSE%i/RMAT22_SPARSE%i.dat", topi, topi); */
		
		printf("seeds %f %f %f %f\n", seed[0][0], seed[0][1], seed[1][0], seed[1][1]);
		printf("ofilename: %s\n", ofilename);
		
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
		
		// uint32_t nodecount = (1L<<scale);
		// uint32_t edgecount = 212698418; // nodecount*edgefactor;

		uint32_t nodecount = (1L<<scale);
		uint32_t edgecount = nodecount*edgefactor;
		
		//int64_t edgecount = (int64_t)std::pow(probsum, scale);
		//fwrite(&nodecount, sizeof(uint32_t), 1, fout);
		//fwrite(&edgecount, sizeof(uint32_t), 1, fout);
		printf( "Generating %ld nodes\n", nodecount );
		printf( "Generating %ld edges\n", edgecount );
		printf( "Edgefactor: %d\n", edgefactor);

		uint32_t count[2][2] = {{0,0},{0,0}};

		for ( uint32_t e = 0; e < edgecount; e++ ) {
			uint32_t u = 0;
			uint32_t v = 0;
			for ( uint32_t i = 0; i < scale; i++ ) {
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
			
			if(u==0){ u=1; }
			if(v==0){ v=1; }
			
			// printf( "%i, %i \n", u, v );
			fwrite(&u, sizeof(uint32_t), 1, fout);
			fwrite(&v, sizeof(uint32_t), 1, fout);
		}
		fclose(fout);
		printf( "Finished generating graph\nGraph needs to be sorted!\n" );
		printf( "%ld %ld %ld %ld\n", count[0][0], count [0][1], count[1][0], count[1][1] );
	}
}
