#ifndef ACTSSYNC_SW_H
#define ACTSSYNC_SW_H
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
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "../../acts/actsutility/actsutility.h"
using namespace std;

class actssync_sw{
public:
	actssync_sw();
	~actssync_sw();

private:
	actsutility * actsutilityobj;
};
#endif 






