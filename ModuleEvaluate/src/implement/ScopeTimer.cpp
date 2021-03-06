/*
 * ScopeTimer.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#include "ScopeTimer.h"
#include "ITimer.h"

ScopeTimer::ScopeTimer(TimerPtr pTimer, double* pInterval): m_pTimer(pTimer), m_pInterval(pInterval) {
	m_startTime = m_pTimer->getTimeInMs();
}

ScopeTimer::~ScopeTimer() {
	double endTime = m_pTimer->getTimeInMs();
	*m_pInterval = endTime - m_startTime;
}

