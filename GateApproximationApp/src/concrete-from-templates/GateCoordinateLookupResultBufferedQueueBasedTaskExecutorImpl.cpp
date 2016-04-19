/*
 * GateCoordinateLookupResultBufferedQueueBasedTaskExecutorImpl.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#include "GateCoordinateLookupResultBufferedQueueBasedTaskExecutorImpl.h"
#include "BufferedQueueBasedTaskExecutorImpl.cpp"
#include "QueueBasedTaskExecutorImpl.cpp"

template class QueueBasedTaskExecutorImpl<LookupResult<GateRealCoordinate> >;
template class BufferedQueueBasedTaskExecutorImpl<LookupResult<GateRealCoordinate> >;

