/*
 * ITaskQueue.h
 *
 *  Created on: Feb 19, 2016
 *      Author: pham
 */

#ifndef ITASKQUEUE_H_
#define ITASKQUEUE_H_

#include "ITask.h"
#include "TaskInternal.h"
#include "TaskCommon.h"

template<typename T>
class ITaskQueue {
public:
	virtual ~ITaskQueue(){};

	virtual void activate() = 0;

	virtual void deactivate() = 0;

	virtual void addQueuedTask(T& queuedTask) = 0;

	virtual T popFrontTask() = 0;

	enum TaskQueueState {
		TQS_EMPTY = 0,
		TQS_DEACTIVATED = 1,
		TQS_READY = 2
	};

	virtual TaskQueueState getState() = 0;
};



#endif /* TASKQUEUE_H_ */
