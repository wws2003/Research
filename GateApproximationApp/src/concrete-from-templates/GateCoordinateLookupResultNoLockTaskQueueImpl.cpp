/*
 * GateCoordinateLookupResultNoLockTaskQueueImpl.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: pham
 */

#include "GateCoordinateLookupResultNoLockTaskQueueImpl.h"
#include "NoLockTaskQueueImpl.cpp"

template class NoLockTaskQueueImpl<QueuedTask<LookupResult<GateRealCoordinate> > >;

