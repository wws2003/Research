/*
 * CpuTimer.h
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#ifndef CPUTIMER_H_
#define CPUTIMER_H_

#include "ITimer.h"

class CpuTimer : public ITimer {
public:
	virtual double getTimeInMs();
};

#endif /* CPUTIMER_H_ */
