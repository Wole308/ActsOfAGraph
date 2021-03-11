#ifndef EVALPARAMS_H
#define EVALPARAMS_H
#include <mutex>
#include <thread>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
using namespace std;

class evalparams {
public:
	evalparams();
	~evalparams();

	void loadevalparams(unsigned int testid, uint512_vec_dt * kvstats);

private:

};
#endif







