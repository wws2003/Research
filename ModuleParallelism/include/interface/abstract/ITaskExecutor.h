/*
 * ITaskExecutor.h
 *
 *  Created on: Feb 18, 2016
 *      Author: pham
 */

#ifndef ITASKEXECUTOR_H_
#define ITASKEXECUTOR_H_

#include "ITask.h"
#include "TaskCommon.h"

template<typename T>
class ITaskFuture {
public:
	virtual ~ITaskFuture(){};

	virtual void wait() = 0;
	virtual bool waitInDuration(long waitDurationInMs) = 0;

	virtual void discard() = 0;

	virtual TaskResult<T> getResult() = 0;
};

template<typename T>
class ITaskExecutor {
public:
	virtual ~ITaskExecutor(){};

	virtual int start() = 0;
	virtual int shutDown() = 0;

	virtual TaskFuturePtr<T> submitTask(TaskPtr<T> pTask) = 0;
};


#endif /* ITASKEXECUTOR_H_ */
