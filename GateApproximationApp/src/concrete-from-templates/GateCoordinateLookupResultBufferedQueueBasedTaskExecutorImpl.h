/*
 * GateCoordinateLookupResultBufferedQueueBasedTaskExecutorImpl.h
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#ifndef GATECOORDINATELOOKUPRESULTBUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_
#define GATECOORDINATELOOKUPRESULTBUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_

#include "Gate.h"
#include "Coordinate.hpp"
#include "MathConceptsCommon.h"
#include "BufferedQueueBasedTaskExecutorImpl.h"
#include "ICollection.h"

typedef class BufferedQueueBasedTaskExecutorImpl<LookupResult<GateRealCoordinate> > GateCoordinateLookupResultBufferedQueueBasedTaskExecutorImpl;

#endif /* GATECOORDINATELOOKUPRESULTBUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_ */
