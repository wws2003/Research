/*
 * GateLookupResultNoLockTaskQueueImpl.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#include "GateLookupResultNoLockTaskQueueImpl.h"
#include "NoLockTaskQueueImpl.cpp"

template class NoLockTaskQueueImpl<LookupResult<GatePtr> >;

