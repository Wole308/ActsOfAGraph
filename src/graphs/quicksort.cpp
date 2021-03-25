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
#include "quicksort.h"

#ifdef AWS_PLATFORM
string datasetRootDir_4quicksort = "/oj2zf/";
#else
string datasetRootDir_4quicksort = "/localtmp/oj2zf/";
#endif

#include <algorithm>
#include <iostream>

using namespace std;

quicksort::quicksort(graph * _graphobj){
	graphobj = _graphobj;
}
quicksort::~quicksort() {}

int quicksort::partition(int* arr1, int* arr2, int left, int right){
	int i = left;
	int pivot_value = arr1[right];
	for (int j = left; j < right; j++)
	{
		if (arr1[j] <= pivot_value)
		{
			swap(arr1[i], arr1[j]);
			swap(arr2[i], arr2[j]);
			i++;
		}
	}
	swap(arr1[i], arr1[right]);
	swap(arr2[i], arr2[right]);
	return i;
}

void quicksort::quicksortop(int* arr1, int*arr2, int left, int right){
	cout<<"quicksortop seen."<<endl;
	if (left < right)
	{
		int pivot = partition(arr1, arr2, left, right);
		quicksortop(arr1, arr2, left, pivot - 1);
		quicksortop(arr1, arr2, pivot + 1, right);
	}
}

void quicksort::print(int* arr1, int* arr2, int n){
	for (int i = 0; i < n; i++){
		cout << arr1[i] << " ";
	}
	cout<<endl;
	for (int i = 0; i < n; i++){
		cout << arr2[i] << " ";
	}
	cout<<endl;
}

int quicksort::start(){
	unsigned int numedges = 2 * graphobj->get_num_edges();
	graphobj->openfilesforreading(0);
	
	edge2_type * edgedatabuffer;
	
	edgedatabuffer = new edge2_type[numedges];
	cout<<"quicksort: loading edges from file... graphobj->getedgessize(0): "<<graphobj->getedgessize(0)<<endl;
	graphobj->loadedgesfromfile(0, 0, edgedatabuffer, 0, graphobj->getedgessize(0));
	for(unsigned int i=0; i<16; i++){
		cout<<"quicksort:: edgedatabuffer["<<i<<"].srcvid: "<<edgedatabuffer[i].srcvid<<", edgedatabuffer["<<i<<"].dstvid: "<<edgedatabuffer[i].dstvid<<endl;
	}
	
	//
	
	cout<<"quicksort: sorting simple test..."<<endl;
	int arr1[] = { 5,2,60,52,10,1,33,22,11,9,12,12,12 };
	int arr2[] = { 50,20,600,520,100,10,330,220,110,90,120,120,120 };
	int size = sizeof(arr1) / sizeof(arr1[0]);
	quicksortop(arr1, arr2, 0, size - 1);
	print(arr1, arr2, size);
	
	//
	
	cout<<"quicksort: sorting graph edges..."<<endl;
	int * keys = new int[numedges];
	int * values = new int[numedges];
	for(unsigned int i=0; i<numedges; i++){ 
		keys[i] = edgedatabuffer[i].srcvid; 
		values[i] = edgedatabuffer[i].dstvid;
	}
	for(unsigned int i=0; i<16; i++){
		cout<<"quicksort:: keys["<<i<<"]: "<<keys[i]<<endl;
	}
	for(unsigned int i=0; i<16; i++){
		cout<<"quicksort:: values["<<i<<"]: "<<values[i]<<endl;
	}
	int size2 = numedges;
	// quicksortop(keys, values, 0, size2 - 1);
	// quicksortop(keys, values, 0, 1024 - 1);
	
	//

	cout<<"quicksort:: sort finished. closing files... "<<endl;
	graphobj->closefilesforreading();
	return 0;
}





