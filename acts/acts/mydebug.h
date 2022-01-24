#ifndef DEBUG_H
#define DEBUG_H
#ifdef SW 
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#endif
#include "mydebug.h"

class mydebug {
public:
	mydebug();
	~mydebug();
	
	void set(unsigned int index1, unsigned int index2, unsigned int data);
	unsigned int get(unsigned int index1, unsigned int index2);
	void increment(unsigned int index1, unsigned int index2, unsigned int incr);
	
private:
	unsigned int global_datas[8][128];
};
#endif








