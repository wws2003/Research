/*
 * BufferedQueueBasedTaskExecutorImpl.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#include "BufferedQueueBasedTaskExecutorImpl.h"
#include <mutex>

template<typename T>
BufferedQueueBasedTaskExecutorImpl<T>::BufferedQueueBasedTaskExecutorImpl(int nbThreads, TaskQueuePtr<QueuedTask<T> > pTaskQueue, int taskBufferSize) : QueueBasedTaskExecutorImpl<T>(nbThreads, pTaskQueue) {
	m_taskBufferSize = taskBufferSize;
	m_queuedTaskBuffer.reserve(taskBufferSize);
}

template<typename T>
TaskFuturePtr<T> BufferedQueueBasedTaskExecutorImpl<T>::submitTask(TaskPtr<T> pTask) {
	TaskFuturePtr<T> pTaskFuture = addTaskToBuffer(pTask);
	if(m_queuedTaskBuffer.size() >= m_taskBufferSize) {
		flushTaskBufferToQueue();
	}
	return pTaskFuture;
}

template<typename T>
void BufferedQueueBasedTaskExecutorImpl<T>::workerJob() {
	std::unique_lock<std::mutex> queueLock(QueueBasedTaskExecutorImpl<T>::m_queueMutex, std::defer_lock);
	auto queueState = ITaskQueue<QueuedTask<T> >::TQS_READY;

	while(queueState != ITaskQueue<QueuedTask<T> >::TQS_DEACTIVATED) {
		queueLock.lock();
		while((queueState = QueueBasedTaskExecutorImpl<T>::m_pTaskQueue->getState()) == ITaskQueue<QueuedTask<T> >::TQS_EMPTY) {
			//Queue lock is automatically unlocked in wait()
			QueueBasedTaskExecutorImpl<T>::m_taskQueueCondvar.wait(queueLock);
			//Queue lock has been automatically locked again after wait() returned
		}
		if(queueState == ITaskQueue<QueuedTask<T> >::TQS_READY) {
			//Get task from queue, then unlock so that task queue is available for other threads
			QueuedTaskBuffer tasksToExecute;
			extractTasksFromQueue(tasksToExecute);
			queueLock.unlock();

			executeTasks(tasksToExecute);
		}
		else {
			queueLock.unlock();
		}
	}
}

template<typename T>
TaskFuturePtr<T> BufferedQueueBasedTaskExecutorImpl<T>::addTaskToBuffer(TaskPtr<T> pTask) {
	TaskPromisePtr<T> pTaskPromise = new TaskPromise<T>();
	TaskFuturePtr<T> pTaskFuture = QueueBasedTaskExecutorImpl<T>::generateTaskFuture(pTaskPromise->get_future());

	QueuedTask<T> queuedTask;
	queuedTask.m_pTask = pTask;
	queuedTask.m_pTaskPromise = pTaskPromise;

	m_queuedTaskBuffer.push_back(queuedTask);

	return pTaskFuture;
}

template<typename T>
void BufferedQueueBasedTaskExecutorImpl<T>::flushTaskBufferToQueue() {
	{
		std::lock_guard<std::mutex> queueLock(QueueBasedTaskExecutorImpl<T>::m_queueMutex);
		for(QueuedTask<T> queuedTask : m_queuedTaskBuffer) {
			QueueBasedTaskExecutorImpl<T>::m_pTaskQueue->addQueuedTask(queuedTask);
		}
		m_queuedTaskBuffer.clear();
	}
	QueueBasedTaskExecutorImpl<T>::m_taskQueueCondvar.notify_one();
}

template<typename T>
void BufferedQueueBasedTaskExecutorImpl<T>::extractTasksFromQueue(QueuedTaskBuffer& taskBuffer) {
	for(int i = 0; i < m_taskBufferSize && QueueBasedTaskExecutorImpl<T>::m_pTaskQueue->getState() != ITaskQueue<QueuedTask<T> >::TQS_EMPTY; i++) {
		auto queuedTask = QueueBasedTaskExecutorImpl<T>::m_pTaskQueue->popFrontTask();
		taskBuffer.push_back(queuedTask);
	}
}

template<typename T>
void BufferedQueueBasedTaskExecutorImpl<T>::executeTasks(const QueuedTaskBuffer& taskBuffer) {
	for(auto queuedTask : taskBuffer) {
		//Execute task
		TaskResult<T> taskResult = queuedTask.m_pTask->execute();
		//Notify future instance
		queuedTask.m_pTaskPromise->set_value(taskResult);
		//Destroy promise pointer
		delete queuedTask.m_pTaskPromise;
	}
}


