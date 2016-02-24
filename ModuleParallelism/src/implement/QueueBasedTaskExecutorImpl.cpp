/*
 * QueueBasedTaskExecutorImpl.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: pham
 */

#include "QueueBasedTaskExecutorImpl.h"
#include "TaskConstants.h"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <list>

template<typename T>
QueueBasedTaskExecutorImpl<T>::QueueBasedTaskExecutorImpl(int nbThreads,
		TaskQueuePtr<QueuedTask<T> > pTaskQueue,
		bool allowExecuteTaskInMainThread) : m_nbThreads(nbThreads){
	m_pTaskQueue = pTaskQueue;
	m_allowExecuteTaskInMainThread = allowExecuteTaskInMainThread;
}

//Override
template<typename T>
int QueueBasedTaskExecutorImpl<T>::start() {
	m_workerThreads.clear();
	m_pTaskQueue->activate();
	for(int i = 0; i < m_nbThreads; i++) {
		std::thread thread([this]{this->workerJob();});
		m_workerThreads.push_back(std::move(thread));
	}
	std::cout << "Task executor started\n";

	return TASK_ERR_NONE;
}

//Override
template<typename T>
int QueueBasedTaskExecutorImpl<T>::shutDown() {
	//Deactivate task queue so that worker threads can terminate
	{
		//Need to acquire queue lock so assure worker threads entered wait status
		std::lock_guard<QueueMutex> queueLock(m_queueMutex);
		m_pTaskQueue->deactivate();
	}
	m_taskQueueCondvar.notify_all();

	//Join all worker threads
	for(std::thread& thread : m_workerThreads) {
		thread.join();
	}

	//TODO Handle remaining tasks if necessary
	std::cout << "Task executor terminated\n";
	return TASK_ERR_NONE;
}

//Override
template<typename T>
TaskFuturePtr<T> QueueBasedTaskExecutorImpl<T>::submitTask(TaskPtr<T> pTask) {
	TaskPromisePtr<T> pTaskPromise = new TaskPromise<T>();
	TaskFuturePtr<T> pTaskFuture = generateTaskFuture(pTaskPromise->get_future());

	QueuedTask<T> queuedTask;
	queuedTask.m_pTask = pTask;
	queuedTask.m_pTaskPromise = pTaskPromise;
	{
		std::lock_guard<QueueMutex> queueLock(m_queueMutex);
		m_pTaskQueue->addQueuedTask(queuedTask);
	}
	m_taskQueueCondvar.notify_one();

	return pTaskFuture;
}

//Override
template<typename T>
void QueueBasedTaskExecutorImpl<T>::executeAllRemaining() {
	if(m_allowExecuteTaskInMainThread) {
		//Get all remaing tasks in queue and execute in current thread
		std::list<QueuedTask<T> > queuingTasks;
		{
			std::lock_guard<QueueMutex> queueLock(m_queueMutex);
			while(m_pTaskQueue->getState() != ITaskQueue<QueuedTask<T> >::TQS_EMPTY) {
				QueuedTask<T> queuingTask = m_pTaskQueue->popFrontTask();
				queuingTasks.push_back(queuingTask);
			}
		}
		while(!queuingTasks.empty()) {
			QueuedTask<T> queuingTask = queuingTasks.front();

			//Execute task
			TaskResult<T> taskResult = queuingTask.m_pTask->execute();
			//Notify future instance
			queuingTask.m_pTaskPromise->set_value(taskResult);
			//Destroy promise pointer
			delete queuingTask.m_pTaskPromise;

			queuingTasks.pop_front();
		}
	}
}

template<typename T>
void QueueBasedTaskExecutorImpl<T>::workerJob() {
	std::unique_lock<QueueMutex> queueLock(m_queueMutex, std::defer_lock);
	auto queueState = ITaskQueue<QueuedTask<T> >::TQS_READY;

	while(queueState != ITaskQueue<QueuedTask<T> >::TQS_DEACTIVATED) {
		queueLock.lock();
		while((queueState = m_pTaskQueue->getState()) == ITaskQueue<QueuedTask<T> >::TQS_EMPTY) {
			//Queue lock is automatically unlocked in wait()
			m_taskQueueCondvar.wait(queueLock);
			//Queue lock has been automatically locked again after wait() returned
		}
		if(queueState == ITaskQueue<QueuedTask<T> >::TQS_READY) {
			//Get task from queue, then unlock so that task queue is available for other threads
			auto queuedTask = m_pTaskQueue->popFrontTask();
			queueLock.unlock();

			//Execute task
			TaskResult<T> taskResult = queuedTask.m_pTask->execute();
			//Notify future instance
			queuedTask.m_pTaskPromise->set_value(taskResult);
			//Destroy promise pointer
			delete queuedTask.m_pTaskPromise;
		}
		else {
			queueLock.unlock();
		}
	}
}

template<typename T>
TaskFuturePtr<T>  QueueBasedTaskExecutorImpl<T>::generateTaskFuture(std::future<TaskResult<T> > future) {
	return TaskFuturePtr<T>(new StdTaskFuture(std::move(future)));
}

//-----------Implementation of Task Future---------------//
template<typename T>
QueueBasedTaskExecutorImpl<T>::StdTaskFuture::StdTaskFuture(std::future<TaskResult<T> > future) {
	m_future = std::move(future);
}

//Override
template<typename T>
void QueueBasedTaskExecutorImpl<T>::StdTaskFuture::wait() {
	m_future.wait();
}

//Override
template<typename T>
bool QueueBasedTaskExecutorImpl<T>::StdTaskFuture::waitInDuration(long waitDurationInMs) {
	std::chrono::system_clock::time_point timeOut
	= std::chrono::system_clock::now() + std::chrono::microseconds(waitDurationInMs);
	return m_future.wait_until(timeOut) == std::future_status::ready;
}

//Override
template<typename T>
void QueueBasedTaskExecutorImpl<T>::StdTaskFuture::discard() {
	//FIXME: Temporary do nothing except set the result to be discarded
	TaskResult<T> discardedTaskResult;
	discardedTaskResult.m_status = TRS_DISCARDED;
	TaskPromise<T> promise;
	promise.set_value(discardedTaskResult);
	m_future = promise.get_future();
}

//Override
template<typename T>
TaskResult<T> QueueBasedTaskExecutorImpl<T>::StdTaskFuture::getResult() {
	return m_future.get();
}
