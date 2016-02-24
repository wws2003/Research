/*
 * TaskQueueImpl.h
 *
 *  Created on: Feb 19, 2016
 *      Author: pham
 */

#ifndef TASKQUEUEIMPL_H_
#define TASKQUEUEIMPL_H_

#include "ITaskQueue.h"
#include "TaskCommon.h"
#include "RWSemaphore.h"
#include <deque>

template<typename T>
class TaskQueueImpl: public ITaskQueue<T> {
public:

	virtual ~TaskQueueImpl(){};

	//Override
	void activate();

	//Override
	void deactivate();

	//Override
	void addQueuedTask(T& queuedTask);

	//Override
	T popFrontTask();

	//Override
	typename ITaskQueue<T>::TaskQueueState getState();

private:
	RWSemaphore m_rwSemaphore;
	bool m_isActivated;
	std::deque<T> m_tasks;
};



#endif /* TASKQUEUEIMPL_H_ */
