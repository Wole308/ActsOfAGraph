#ifndef QUICKSORT_H
#define QUICKSORT_H
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../../include/host_common.h"
#include "../../include/common.h"

class quicksort {
public:
	quicksort(graph * _graphobj);						
	~quicksort();
	
	int partition(int* arr1, int* arr2, int left, int right);
	void quicksortop(int* arr1, int*arr2, int left, int right);
	void print(int* arr1, int* arr2, int n);
	int start();

	
private:
	graph * graphobj;
};
#endif








