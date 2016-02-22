/*
 * NoLockTaskQueueImpl.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#include "NoLockTaskQueueImpl.h"
#include <algorithm>

template<typename T>
void NoLockTaskQueueImpl<T>::activate() {
	m_isActivated = true;
}

template<typename T>
void NoLockTaskQueueImpl<T>::deactivate() {
	m_isActivated = false;
}

template<typename T>
void NoLockTaskQueueImpl<T>::addQueuedTask(T& queuedTask) {
	m_tasks.push_back(std::move(queuedTask));
}

template<typename T>
T NoLockTaskQueueImpl<T>::popFrontTask() {
	T task = std::move(m_tasks.front());
	m_tasks.pop_front();
	return task;
}

template<typename T>
typename ITaskQueue<T>::TaskQueueState NoLockTaskQueueImpl<T>::getState() {
	if(!m_isActivated) {
		return ITaskQueue<T>::TQS_DEACTIVATED;
	}
	else {
		return m_tasks.empty() ? ITaskQueue<T>::TQS_EMPTY : ITaskQueue<T>::TQS_READY;
	}
}

