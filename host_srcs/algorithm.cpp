#include "algorithm.h"
using namespace std;

algorithm::algorithm(){}
algorithm::~algorithm(){} 

unsigned algorithm::vertex_initdata(std::string Algo){
	if(Algo == "pr"){ return 1; } 
	else if(Algo == "cf"){ return 1; } 
	else if(Algo == "hits"){ return 1; } 
	else if(Algo == "spmv"){ return 1; } 
	else if(Algo == "bfs"){ return 0b0; }
	// else if(Algo == "sssp"){ return MAXVDATA; } 
	else if(Algo == "sssp"){ return 0xFFFFFFFE; } 
	// else if(Algo == "sssp"){ return 0xFFFFFFFE; } 
	else { return 0; }
	return 0;
}

unsigned algorithm::get_algorithm_id(std::string Algo){
	if(Algo == "pr"){ return PAGERANK; } 
	else if(Algo == "cf"){ return CF; } 
	else if(Algo == "hits"){ return HITS; } 
	else if(Algo == "spmv"){ return SPMV; } 
	else if(Algo == "bfs"){ return BFS; }
	else if(Algo == "sssp"){ return SSSP; } 
	else { return 0; }
	return 0;
}






