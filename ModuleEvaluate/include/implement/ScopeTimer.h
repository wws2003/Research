/*
 * ScopeTimer.h
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#ifndef SCOPETIMER_H_
#define SCOPETIMER_H_

#include "EvaluateCommon.h"

class ScopeTimer {
public:
	ScopeTimer(TimerPtr pTimer, double* pInterval);
	virtual ~ScopeTimer();

private:
	TimerPtr m_pTimer;
	double* m_pInterval;
	double m_startTime;
};


#endif /* SCOPETIMER_H_ */
