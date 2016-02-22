/*
 * QueueBasedTaskExecutorImpl.h
 *
 *  Created on: Feb 18, 2016
 *      Author: pham
 */

#ifndef QUEUEBASEDTASKEXECUTORIMPL_H_
#define QUEUEBASEDTASKEXECUTORIMPL_H_

#include "TaskCommon.h"
#include "TaskConstants.h"
#include "ITaskExecutor.h"
#include "ITaskQueue.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <future>
#include <iostream>

template<typename T>
using TaskPromise = std::promise<TaskResult<T> > ;

template<typename T>
using TaskPromisePtr = TaskPromise<T>*;

template<typename T>
struct QueuedTask {
	TaskPtr<T> m_pTask;
	TaskPromisePtr<T> m_pTaskPromise = NULL;
};

template<typename T>
class QueueBasedTaskExecutorImpl: public ITaskExecutor<T> {
public:
	QueueBasedTaskExecutorImpl(int nbThreads, TaskQueuePtr<QueuedTask<T> > pTaskQueue);
	virtual ~QueueBasedTaskExecutorImpl(){};

	//Override
	int start();

	//Override
	int shutDown();

	//Override
	TaskFuturePtr<T> submitTask(TaskPtr<T> pTask);

	void workerJob();

private:
	typedef std::mutex QueueMutex;
	typedef std::mutex TaskExecuteMutex;
	typedef std::condition_variable QueueConditionalVariable;
	typedef std::condition_variable TaskExecuteConditionalVariable;

	class StdTaskFuture: public ITaskFuture<T> {
	public:
		StdTaskFuture(std::future<TaskResult<T> > future);
		virtual ~StdTaskFuture(){};

		//Override
		void wait();

		//Override
		bool waitInDuration(long waitDurationInMs);

		//Override
		void discard();

		//Override
		TaskResult<T> getResult();
	private:
		std::future<TaskResult<T> > m_future;
	};

	TaskFuturePtr<T> generateTaskFuture(std::future<TaskResult<T> > future);

	TaskQueuePtr<QueuedTask<T> > m_pTaskQueue;
	QueueConditionalVariable m_taskQueueCondvar;
	QueueMutex m_queueMutex;

	std::vector<std::thread> m_workerThreads;
	int m_nbThreads;
};



#endif /* QUEUEBASEDTASKEXECUTORIMPL_H_ */
