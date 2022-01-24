#include "mydebug.h"
using namespace std;

mydebug::mydebug(){
	for(int i=0; i<8; i++){
		for(int j=0; j<128; j++){
			global_datas[i][j] = 0;
		}
	}
}
mydebug::~mydebug(){}

void mydebug::set(unsigned int index1, unsigned int index2, unsigned int data){
	global_datas[index1][index2] = data;
}

unsigned int mydebug::get(unsigned int index1, unsigned int index2){
	return global_datas[index1][index2];
}

void mydebug::increment(unsigned int index1, unsigned int index2, unsigned int incr){
	global_datas[index1][index2] += incr;
}















