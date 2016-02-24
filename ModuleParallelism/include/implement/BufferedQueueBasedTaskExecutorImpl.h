/*
 * BufferedQueueBasedTaskExecutorImpl.h
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#ifndef BUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_
#define BUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_

#include "QueueBasedTaskExecutorImpl.h"

template<typename T>
class BufferedQueueBasedTaskExecutorImpl: public QueueBasedTaskExecutorImpl<T> {
public:
	BufferedQueueBasedTaskExecutorImpl(int nbThreads,
			TaskQueuePtr<QueuedTask<T> > pTaskQueue,
			int taskBufferSize,
			bool allowExecuteTaskInMainThread = false);

	virtual ~BufferedQueueBasedTaskExecutorImpl(){};

	//Override
	TaskFuturePtr<T> submitTask(TaskPtr<T> pTask);

	//Override
	void executeAllRemaining();

	//Override
	void workerJob();

private:
	typedef std::vector<QueuedTask<T> > QueuedTaskBuffer;

	TaskFuturePtr<T> addTaskToBuffer(TaskPtr<T> pTask);

	void flushTaskBufferToQueue();

	void extractTasksFromQueue(QueuedTaskBuffer& taskBuffer);

	void executeTasks(const QueuedTaskBuffer& taskBuffer);

	QueuedTaskBuffer m_queuedTaskBuffer;
	int m_taskBufferSize;
};


#endif /* BUFFEREDQUEUEBASEDTASKEXECUTORIMPL_H_ */
