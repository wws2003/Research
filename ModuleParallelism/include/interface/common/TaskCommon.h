/*
 * TaskCommon.h
 *
 *  Created on: Feb 18, 2016
 *      Author: pham
 */

#ifndef TASKCOMMON_H_
#define TASKCOMMON_H_

#include <cstdlib>

template<typename T>
class ITask;

template<typename T>
class ITaskFuture;

template<typename T>
class ITaskExecutor;

template<typename T>
using TaskPtr = ITask<T>*;

template<typename T>
using TaskFuturePtr = ITaskFuture<T>*;

template<typename T>
using TaskExecutorPtr = ITaskExecutor<T>*;

#ifndef NullPtr
#define NullPtr (NULL)
#endif

#ifndef _destroy
#define _destroy(a)if(a != NULL)delete a
#endif

#endif /* TASKCOMMON_H_ */
