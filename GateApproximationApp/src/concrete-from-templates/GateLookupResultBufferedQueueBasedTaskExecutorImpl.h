/*
 * GateLookupResultBufferedQueueBasedTaskExecutorImpl.h
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#ifndef GATELOOKUPRESULTBUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_
#define GATELOOKUPRESULTBUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_

#include "ICollection.h"
#include "Gate.h"
#include "MathConceptsCommon.h"
#include "BufferedQueueBasedTaskExecutorImpl.h"

typedef class BufferedQueueBasedTaskExecutorImpl<LookupResult<GatePtr> > GateLookupResultBufferedQueueBasedTaskExecutorImpl;

#endif /* GATELOOKUPRESULTBUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_ */
