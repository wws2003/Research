/*
 * ThreadSpawnTaskExecutorImpl.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#include "ThreadSpawnTaskExecutorImpl.h"
#include <chrono>

template<typename T>
int ThreadSpawnTaskExecutorImpl<T>::start() {
	return TASK_ERR_NONE;
}

template<typename T>
int ThreadSpawnTaskExecutorImpl<T>::shutDown() {
	for(unsigned int i = 0; i < m_taskFutures.size(); i++) {
		TaskFuturePtr<T> pTaskFuture = m_taskFutures[i];
		m_taskFutures[i] = NullPtr;
		pTaskFuture->wait();
		_destroy(pTaskFuture);
	}
	m_taskFutures.clear();
	return TASK_ERR_NONE;
}

template<typename T>
TaskFuturePtr<T> ThreadSpawnTaskExecutorImpl<T>::submitTask(TaskPtr<T> pTask) {
	//Start thread to handle task
	std::future<TaskResult<T> > future = std::async(std::launch::async, &ThreadSpawnTaskExecutorImpl<T>::executeTask, this, pTask);

	//Store future instances
	TaskFuturePtr<T> pTaskFuture = TaskFuturePtr<T>(new StdTaskFuture(std::move(future)));
	m_taskFutures.push_back(pTaskFuture);

	return TaskFuturePtr<T>(new WrapperTaskFuture(pTaskFuture));
}

template<typename T>
void ThreadSpawnTaskExecutorImpl<T>::executeAllRemaining() {
	//Do nothing since any task has it own executing thread
}

template<typename T>
TaskResult<T> ThreadSpawnTaskExecutorImpl<T>::executeTask(TaskPtr<T> pTask) {
	return pTask->execute();
}

//-----------Implementation of Task Futures---------------//
template<typename T>
ThreadSpawnTaskExecutorImpl<T>::StdTaskFuture::StdTaskFuture(std::future<TaskResult<T> > future) {
	m_future = std::move(future);
}

//Override
template<typename T>
void ThreadSpawnTaskExecutorImpl<T>::StdTaskFuture::wait() {
	//If m_future.get is already called, do nothing
	if(m_future.valid()) {
		m_future.wait();
	}
}

//Override
template<typename T>
bool ThreadSpawnTaskExecutorImpl<T>::StdTaskFuture::waitInDuration(long waitDurationInMs) {
	std::chrono::system_clock::time_point timeOut
	= std::chrono::system_clock::now() + std::chrono::microseconds(waitDurationInMs);
	return m_future.wait_until(timeOut) == std::future_status::ready;
}

//Override
template<typename T>
void ThreadSpawnTaskExecutorImpl<T>::StdTaskFuture::discard() {
	//FIXME: Temporary do nothing except set the result to be discarded
	TaskResult<T> discardedTaskResult;
	discardedTaskResult.m_status = TRS_DISCARDED;
	std::promise<TaskResult<T> > promise;
	promise.set_value(discardedTaskResult);
	m_future = promise.get_future();
}

//Override
template<typename T>
TaskResult<T> ThreadSpawnTaskExecutorImpl<T>::StdTaskFuture::getResult() {
	return m_future.get();
}

//--------------Task future wrapper-----------------//
template<typename T>
ThreadSpawnTaskExecutorImpl<T>::WrapperTaskFuture::WrapperTaskFuture(TaskFuturePtr<T> pTaskFutureDelegate) {
	m_pTaskFutureDelegate = pTaskFutureDelegate;
}

//Override
template<typename T>
void ThreadSpawnTaskExecutorImpl<T>::WrapperTaskFuture::wait() {
	m_pTaskFutureDelegate->wait();
}

//Override
template<typename T>
bool ThreadSpawnTaskExecutorImpl<T>::WrapperTaskFuture::waitInDuration(long waitDurationInMs) {
	return m_pTaskFutureDelegate->waitInDuration(waitDurationInMs);
}

//Override
template<typename T>
void ThreadSpawnTaskExecutorImpl<T>::WrapperTaskFuture::discard() {
	m_pTaskFutureDelegate->discard();
}

//Override
template<typename T>
TaskResult<T> ThreadSpawnTaskExecutorImpl<T>::WrapperTaskFuture::getResult() {
	return m_pTaskFutureDelegate->getResult();
}
