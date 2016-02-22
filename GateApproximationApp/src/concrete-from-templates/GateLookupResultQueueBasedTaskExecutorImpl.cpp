/*
 * GateLookupResultQueueBasedTaskExecutorImpl.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#include "GateLookupResultQueueBasedTaskExecutorImpl.h"
#include "QueueBasedTaskExecutorImpl.cpp"

template class QueueBasedTaskExecutorImpl<LookupResult<GatePtr> >;
