/*
 * SingleThreadTaskExecutorImpl.h
 *
 *  Created on: Feb 20, 2016
 *      Author: pham
 */

#ifndef SINGLETHREADTASKEXECUTORIMPL_H_
#define SINGLETHREADTASKEXECUTORIMPL_H_

#include "TaskCommon.h"
#include "TaskConstants.h"
#include "ITaskExecutor.h"

template<typename T>
class SingleThreadTaskExecutorImpl: public ITaskExecutor<T> {
public:
	virtual ~SingleThreadTaskExecutorImpl(){};

	//Override
	int start();

	//Override
	int shutDown();

	//Override
	TaskFuturePtr<T> submitTask(TaskPtr<T> pTask);

	//Override
	void executeAllRemaining();

private:
	class SimpleTaskFuture: public ITaskFuture<T> {
	public:
		SimpleTaskFuture(TaskResult<T> result);
		virtual ~SimpleTaskFuture(){};

		//Override
		void wait();

		//Override
		bool waitInDuration(long waitDurationInMs);

		//Override
		void discard();

		//Override
		TaskResult<T> getResult();
	private:
		TaskResult<T> m_result;
	};
};

#endif /* SINGLETHREADTASKEXECUTORIMPL_H_ */
