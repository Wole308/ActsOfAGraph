#ifndef EVALPARAMS_H
#define EVALPARAMS_H
#include <mutex>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "utility.h"
#include "algorithm.h"
#include "../include/common.h"
using namespace std;

class evalparams {
public:
	evalparams();
	~evalparams();

	void loadevalparams(unsigned int testid, uint512_vec_dt * kvstats);

private:

};
#endif







