#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include "algorithm.h"
using namespace std;

algorithm::algorithm(){}
algorithm::~algorithm(){} 

int algorithm::amin(size_t val1, size_t val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
#ifdef PR_ALGORITHM
uint32_t algorithm::vertex_update(uint32_t a, uint32_t b) {
	float af = *(float*)&a;
	float bf = *(float*)&b;
	float tf = af+bf;
	
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
uint32_t algorithm::edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	float af = *(float*)&value;
	float tf = af/(float)fanout;
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
size_t g_vertex_countt = 1;
uint32_t algorithm::finalize_program(uint32_t oldval, uint32_t val) {
	float damp = 0.15/(float)g_vertex_countt;
	float af = *(float*)&val;
	float tf = damp + af*0.85;
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
bool algorithm::is_active(uint32_t old, uint32_t newv, bool marked) {
	if ( !marked ) return false;

	float af = *(float*)&old;
	float bf = *(float*)&newv;

	if ( af > bf ) {
		if (af-bf < 0.0001 ) return false;
		else return true;
	} else {
		if (bf-af < 0.0001 ) return false;
		else return true;
	}
}
value_t algorithm::cummulate(value_t data1, value_t data2){
	return data1 + data2;
}
value_t algorithm::vertex_initdata(){ return 1; }
value_t algorithm::vertex_inittempdata(){ return 1; }
uint32_t algorithm::apply(value_t vtempdata, value_t vdata) {
	return 0.1;
	// return (0.1 + ((1 − 0.1) * vtempdata)); // / vprop.outdegree;
}
#elif defined(BFS_ALGORITHM)
uint32_t algorithm::vertex_update(uint32_t a, uint32_t b) {
	uint32_t ret = a;
	return ret;
}
uint32_t algorithm::edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	return vid;
}
uint32_t algorithm::finalize_program(uint32_t oldval, uint32_t val) {
	return val;
}
bool algorithm::is_active(uint32_t old, uint32_t newv, bool marked) {
	//printf( "Comparing %lx %lx %s\n", old, newv, marked?"Y":"N" );
	if ( old == 0xffffffff ) return true;
	//printf( "Comparing %x %x %s\n", old, newv, marked?"Y":"N" );
	return false;
}
value_t algorithm::cummulate(value_t data1, value_t data2){
	if(data1 < data2){ return data1; }
	else { return data2; }
}
value_t algorithm::vertex_initdata(){ 
	#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
	return UNVISITED;
	#else 
	return INFINITI; 
	#endif 
}
value_t algorithm::vertex_inittempdata(){ return INFINITI; }
uint32_t algorithm::apply(value_t vtempdata, value_t vdata){
	return vtempdata;
}
#elif defined(SSSP_ALGORITHM)
uint32_t algorithm::vertex_update(uint32_t a, uint32_t b) {
	uint32_t ret = a;
	return ret;
}
uint32_t algorithm::edge_program(uint32_t vid, uint32_t value, uint32_t edgeweight) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	return value + edgeweight;
}
uint32_t algorithm::finalize_program(uint32_t oldval, uint32_t val) {
	return val;
}
bool algorithm::is_active(uint32_t old, uint32_t newv, bool marked) {
	//printf( "Comparing %lx %lx %s\n", old, newv, marked?"Y":"N" );
	if ( old == 0xffffffff ) return true;
	//printf( "Comparing %x %x %s\n", old, newv, marked?"Y":"N" );
	return false;
}
value_t algorithm::cummulate(value_t data1, value_t data2){
	if(data1 < data2){ return data1; }
	else { return data2; }
}
value_t algorithm::vertex_initdata(){ 
	#if defined(INMEMORYGP) && defined(SSSP_ALGORITHM)
	return 0x3FFFFFFF;
	#else 
	return INFINITI; 
	#endif
}
value_t algorithm::vertex_inittempdata(){ return INFINITI; }
uint32_t algorithm::apply(value_t vtempdata, value_t vdata){
	return vtempdata;
	// if(vtempdata < vdata){ return vtempdata; }
	// else { return vdata; }
}
#elif defined(BC_ALGORITHM)
uint32_t algorithm::vertex_update_add(uint32_t a, uint32_t b) {
	return a+b;
}
uint32_t algorithm::vertex_update(uint32_t a, uint32_t b) {
	uint32_t ret = a;
	return ret;
}
uint32_t algorithm::edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	return vid;
}
uint32_t algorithm::finalize_program(uint32_t oldval, uint32_t newval) {
	return newval;
}
bool algorithm::is_active(uint32_t old, uint32_t newv, bool marked) {
	//printf( "Comparing %lx %lx %s\n", old, newv, marked?"Y":"N" );
	if ( old == 0xffffffff ) return true;
	//printf( "Comparing %x %x %s\n", old, newv, marked?"Y":"N" );
	return false;
}
value_t algorithm::cummulate(value_t data1, value_t data2){
	return data1 + data2;
}
value_t algorithm::vertex_initdata(){ return INFINITI; }
value_t algorithm::vertex_inittempdata(){ return INFINITI; }
uint32_t algorithm::apply(value_t vtempdata, value_t vdata){
	// return min(vtempdata, vdata);
	return vtempdata;
}
#endif 







