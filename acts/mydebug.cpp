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

















