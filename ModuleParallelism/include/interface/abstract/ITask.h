/*
 * ITask.h
 *
 *  Created on: Feb 18, 2016
 *      Author: pham
 */

#ifndef ITASK_H_
#define ITASK_H_

#include "TaskCommon.h"

enum TaskResultStatus {
	TRS_INITIALIZED = 0,
	TRS_SUBMITTED = 1,
	TRS_READY = 2,
	TRS_DISCARDED = 3,
};

template<typename T>
struct TaskResult {
	TaskResultStatus m_status = TRS_INITIALIZED;
	int m_resultCode = -1;
	T m_executeResult;
};

template<typename T>
class ITask {
public:
	virtual ~ITask(){};

	virtual TaskResult<T> execute() = 0;
};



#endif /* ITASK_H_ */
