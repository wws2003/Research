/*
 * GateLookupResultBufferedQueueBasedTaskExecutorImpl.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#include "GateLookupResultBufferedQueueBasedTaskExecutorImpl.h"
#include "BufferedQueueBasedTaskExecutorImpl.cpp"

template class BufferedQueueBasedTaskExecutorImpl<LookupResult<GatePtr> >;
