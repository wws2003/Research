/*
 * SystemTimer.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#include "SystemTimer.h"
#include <ctime>

double SystemTimer::getTimeInMs() {
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME, &tm);

	return tm.tv_sec * 1000 + tm.tv_nsec / 1e6;
}
