/*
 * TaskFutureBuffer.hpp
 *
 *  Created on: Feb 28, 2016
 *      Author: pham
 */

#ifndef TASKFUTUREBUFFER_HPP_
#define TASKFUTUREBUFFER_HPP_

#include "ITaskExecutor.h"
#include "ICollection.h"
#include "TaskCommon.h"
#include <vector>

template<typename T>
class TaskFutureBuffer {
public:
	TaskFutureBuffer(TaskExecutorPtr<LookupResult<T> > pSharedTaskExecutor, size_t maxBufferSize, int positiveResultCode);
	virtual ~TaskFutureBuffer(){};

	void addTaskFuturePair(TaskFuturePtr<LookupResult<T> > pTaskFuture, TaskPtr<LookupResult<T> > pTask);
	void collectResults(std::vector<LookupResult<T> >& resultBuffer);

private:
	void moveResultsFromFutureBuffers();

	TaskExecutorPtr<LookupResult<T> > m_pSharedTaskExecutor;

	size_t m_maxBufferSize;
	int m_positiveResultCode;

	std::vector<LookupResult<T> > m_results;
	std::vector<TaskFuturePtr<LookupResult<T> > > m_taskFuturesBuffer;
	std::vector<TaskPtr<LookupResult<T> > > m_tasks;
};

template<typename T>
TaskFutureBuffer<T>::TaskFutureBuffer(TaskExecutorPtr<LookupResult<T> > pSharedTaskExecutor,
		size_t maxBufferSize,
		int positiveResultCode) {
	m_pSharedTaskExecutor = pSharedTaskExecutor;
	m_maxBufferSize = maxBufferSize;
	m_positiveResultCode = positiveResultCode;
}

template<typename T>
void TaskFutureBuffer<T>::addTaskFuturePair(TaskFuturePtr<LookupResult<T> > pTaskFuture,
		TaskPtr<LookupResult<T> > pTask) {
	m_taskFuturesBuffer.push_back(pTaskFuture);
	m_tasks.push_back(pTask);

	if(m_taskFuturesBuffer.size() >= m_maxBufferSize && m_maxBufferSize > 0) {
		moveResultsFromFutureBuffers();
	}
}

template<typename T>
void TaskFutureBuffer<T>::collectResults(std::vector<LookupResult<T> >& resultBuffer) {
	//Retrieve results from remaining futures
	moveResultsFromFutureBuffers();
	resultBuffer.reserve(resultBuffer.size() + m_results.size());
	resultBuffer.insert(resultBuffer.end(), m_results.begin(), m_results.end());
	m_results.clear();
}

template<typename T>
void TaskFutureBuffer<T>::moveResultsFromFutureBuffers() {
	m_pSharedTaskExecutor->executeAllRemaining();
	for(unsigned int i = 0; i < m_taskFuturesBuffer.size(); i++) {
		TaskFuturePtr<LookupResult<T> > pTaskFuture = m_taskFuturesBuffer[i];
		TaskPtr<LookupResult<T> > pTask = m_tasks[i];

		//Store verified result
		pTaskFuture->wait();
		TaskResult<LookupResult<T> > verifyTaskResult = pTaskFuture->getResult();
		if(verifyTaskResult.m_resultCode == m_positiveResultCode) {
			m_results.push_back(verifyTaskResult.m_executeResult);
		}

		//Release memory for future and task
		m_taskFuturesBuffer[i] = NullPtr;
		m_tasks[i] = NullPtr;
		_destroy(pTaskFuture);
		_destroy(pTask);
	}
	m_taskFuturesBuffer.clear();
	m_tasks.clear();
}




#endif /* TASKFUTUREBUFFER_HPP_ */
