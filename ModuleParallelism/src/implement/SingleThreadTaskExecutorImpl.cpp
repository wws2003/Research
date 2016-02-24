/*
 * SingleThreadTaskExecutorImpl.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: pham
 */

#include "SingleThreadTaskExecutorImpl.h"
#include <stdexcept>

template<typename T>
int SingleThreadTaskExecutorImpl<T>::start() {
	//Do nothing
	return TASK_ERR_NONE;
}

template<typename T>
int SingleThreadTaskExecutorImpl<T>::shutDown() {
	//Do nothing
	return TASK_ERR_NONE;
}

template<typename T>
TaskFuturePtr<T> SingleThreadTaskExecutorImpl<T>::submitTask(TaskPtr<T> pTask) {
	TaskResult<T> taskResult = pTask->execute();
	TaskFuturePtr<T> pTaskFuture = TaskFuturePtr<T>(new SimpleTaskFuture(taskResult));
	return pTaskFuture;
}

template<typename T>
void SingleThreadTaskExecutorImpl<T>::executeAllRemaining() {
	//Do nothing since every tasks are executed right after beiing submitted
}

//---------------Inner class-------------------//
template<typename T>
SingleThreadTaskExecutorImpl<T>::SimpleTaskFuture::SimpleTaskFuture(TaskResult<T> result) {
	m_result = result;
}

//Override
template<typename T>
void SingleThreadTaskExecutorImpl<T>::SimpleTaskFuture::wait() {
	//Do nothing
}

//Override
template<typename T>
bool SingleThreadTaskExecutorImpl<T>::SimpleTaskFuture::waitInDuration(long waitDurationInMs) {
	//Do nothing
	throw std::runtime_error("Can not wait by duration in single thread mode");
}

//Override
template<typename T>
void SingleThreadTaskExecutorImpl<T>::SimpleTaskFuture::discard() {
	TaskResult<T> discardedTaskResult;
	discardedTaskResult.m_status = TRS_DISCARDED;
	m_result = discardedTaskResult;
}

//Override
template<typename T>
TaskResult<T> SingleThreadTaskExecutorImpl<T>::SimpleTaskFuture::getResult() {
	return m_result;
}
