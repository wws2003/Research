/*
 * NoLockTaskQueueImpl.h
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#ifndef NOLOCKTASKQUEUEIMPL_H_
#define NOLOCKTASKQUEUEIMPL_H_

#include "ITaskQueue.h"
#include <deque>

template<typename T>
class NoLockTaskQueueImpl: public ITaskQueue<T> {
public:

	virtual ~NoLockTaskQueueImpl(){};

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
	bool m_isActivated;
	std::deque<T> m_tasks;
};

#endif /* NOLOCKTASKQUEUEIMPL_H_ */
