/*
 * ThreadSpawnTaskExecutorImpl.h
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#ifndef THREADSPAWNTASKEXECUTORIMPL_H_
#define THREADSPAWNTASKEXECUTORIMPL_H_

#include "ITask.h"
#include "ITaskExecutor.h"
#include "TaskConstants.h"
#include <thread>
#include <mutex>
#include <vector>
#include <future>

template<typename T>
class ThreadSpawnTaskExecutorImpl: public ITaskExecutor<T> {
public:
	virtual ~ThreadSpawnTaskExecutorImpl(){};

	//Override
	int start();

	//Override
	int shutDown();

	//Override
	TaskFuturePtr<T> submitTask(TaskPtr<T> pTask);

	//Override
	void executeAllRemaining();

private:
	TaskResult<T> executeTask(TaskPtr<T> pTask);

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

	class WrapperTaskFuture: public ITaskFuture<T> {
	public:
		WrapperTaskFuture(TaskFuturePtr<T> pTaskFutureDelegate);
		virtual ~WrapperTaskFuture(){};

		//Override
		void wait();

		//Override
		bool waitInDuration(long waitDurationInMs);

		//Override
		void discard();

		//Override
		TaskResult<T> getResult();
	private:
		TaskFuturePtr<T> m_pTaskFutureDelegate;
	};

	std::vector<TaskFuturePtr<T> > m_taskFutures;
};


#endif /* THREADSPAWNTASKEXECUTORIMPL_H_ */
