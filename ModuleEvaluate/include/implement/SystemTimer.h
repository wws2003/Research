/*
 * SystemTimer.h
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#ifndef SYSTEMTIMER_H_
#define SYSTEMTIMER_H_

#include "ITimer.h"

class SystemTimer: public ITimer {
public:
	double getTimeInMs();
};

#endif /* SYSTEMTIMER_H_ */
