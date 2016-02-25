/*
 * GateCoordinateLookupResultNoLockTaskQueueImpl.h
 *
 *  Created on: Feb 25, 2016
 *      Author: pham
 */

#ifndef GATECOORDINATELOOKUPRESULTNOLOCKTASKQUEUEIMPL_H_
#define GATECOORDINATELOOKUPRESULTNOLOCKTASKQUEUEIMPL_H_

#include "Gate.h"
#include "ICollection.h"
#include "Coordinate.hpp"
#include "MathConceptsCommon.h"
#include "NoLockTaskQueueImpl.h"
#include "QueueBasedTaskExecutorImpl.h"

typedef class NoLockTaskQueueImpl<QueuedTask<LookupResult<GateRealCoordinate> > > GateCoordinateLookupResultNoLockTaskQueueImpl;

#endif /* GATECOORDINATELOOKUPRESULTNOLOCKTASKQUEUEIMPL_H_ */
