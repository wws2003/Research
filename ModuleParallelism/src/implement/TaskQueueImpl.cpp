/*
 * TaskQueueImpl.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: pham
 */

#include "TaskQueueImpl.h"
#include <algorithm>

template<typename T>
void TaskQueueImpl<T>::activate() {
	m_rwSemaphore.acquire(RWSemaphore::AM_WRITE);
	m_isActivated = true;
	m_rwSemaphore.release(RWSemaphore::AM_WRITE);
}

template<typename T>
void TaskQueueImpl<T>::deactivate() {
	m_rwSemaphore.acquire(RWSemaphore::AM_WRITE);
	m_isActivated = false;
	m_rwSemaphore.release(RWSemaphore::AM_WRITE);
}

template<typename T>
void TaskQueueImpl<T>::addQueuedTask(T& queuedTask) {
	m_tasks.push_back(std::move(queuedTask));
}

template<typename T>
T TaskQueueImpl<T>::popFrontTask() {
	T task = std::move(m_tasks.front());
	m_tasks.pop_front();
	return task;
}

template<typename T>
typename ITaskQueue<T>::TaskQueueState TaskQueueImpl<T>::getState() {
	m_rwSemaphore.acquire(RWSemaphore::AM_READ);
	if(!m_isActivated) {
		m_rwSemaphore.release(RWSemaphore::AM_READ);
		return ITaskQueue<T>::TQS_DEACTIVATED;
	}
	else {
		m_rwSemaphore.release(RWSemaphore::AM_READ);
		return m_tasks.empty() ? ITaskQueue<T>::TQS_EMPTY : ITaskQueue<T>::TQS_READY;
	}
}

