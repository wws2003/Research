/*
 * TaskInternal.h
 *
 *  Created on: Feb 19, 2016
 *      Author: pham
 */

#ifndef TASKINTERNAL_H_
#define TASKINTERNAL_H_

template<typename T>
class ITaskQueue;

template<typename T>
using TaskQueuePtr = ITaskQueue<T>*;

#endif /* TASKINTERNAL_H_ */
