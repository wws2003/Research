/*
 * CpuTimer.cpp
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#include "CpuTimer.h"
#include <ctime>

double CpuTimer::getTimeInMs() {
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME, &tm);

	return tm.tv_sec * 1000 + tm.tv_nsec / 1000;
}


