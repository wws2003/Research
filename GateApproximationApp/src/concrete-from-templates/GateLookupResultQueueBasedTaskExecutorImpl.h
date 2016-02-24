/*
 * GateLookupResultQueueBasedTaskExecutorImpl.h
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#ifndef GATELOOKUPRESULTQUEUEBASEDTASKEXECUTORIMPL_H_
#define GATELOOKUPRESULTQUEUEBASEDTASKEXECUTORIMPL_H_

#include "Gate.h"
#include "ICollection.h"
#include "ITaskExecutor.h"
#include "TaskCommon.h"
#include "QueueBasedTaskExecutorImpl.h"

typedef QueueBasedTaskExecutorImpl<LookupResult<GatePtr> > GateLookupResultQueueBasedTaskExecutorImpl;

#endif /* GATELOOKUPRESULTQUEUEBASEDTASKEXECUTORIMPL_H_ */
