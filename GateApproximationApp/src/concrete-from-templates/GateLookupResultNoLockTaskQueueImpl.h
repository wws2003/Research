/*
 * GateLookupResultNoLockTaskQueueImpl.h
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#ifndef GATELOOKUPRESULTNOLOCKTASKQUEUEIMPL_H_
#define GATELOOKUPRESULTNOLOCKTASKQUEUEIMPL_H_

#include "Gate.h"
#include "ICollection.h"
#include "MathConceptsCommon.h"
#include "NoLockTaskQueueImpl.h"
#include "QueueBasedTaskExecutorImpl.h"

typedef NoLockTaskQueueImpl<QueuedTask<LookupResult<GatePtr> >  > GateLookupResultNoLockTaskQueueImpl;

#endif /* GATELOOKUPRESULTNOLOCKTASKQUEUEIMPL_H_ */
