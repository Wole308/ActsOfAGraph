#include "algorithm.h"
using namespace std;

algorithm::algorithm(){}
algorithm::~algorithm(){} 

unsigned int algorithm::vertex_initdata(unsigned int Algo){
	if(Algo == PAGERANK){ return 1; } 
	else if(Algo == CF){ return 1; } 
	else if(Algo == HITS){ return 1; } 
	else if(Algo == SPMV){ return 1; } 
	else if(Algo == BFS){ return 0xFFFFFFFE; }
	// else if(Algo == SSSP){ return 0xFFFFFFFE; } 
	else if(Algo == SSSP){ return 0xFFFFFFEE; } 
	else { return 0; }
	return 0;
}

unsigned int algorithm::get_algorithm_id(std::string Algo){
	if(Algo == "pr"){ return PAGERANK; } 
	else if(Algo == "cf"){ return CF; } 
	else if(Algo == "hits"){ return HITS; } 
	else if(Algo == "spmv"){ return SPMV; } 
	else if(Algo == "bfs"){ return BFS; }
	else if(Algo == "sssp"){ return SSSP; } 
	else { return 0; }
	return 0;
}






