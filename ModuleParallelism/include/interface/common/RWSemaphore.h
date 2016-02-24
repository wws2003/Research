/*
 * RWSemaphore.h
 *
 *  Created on: Feb 20, 2016
 *      Author: pham
 */

#ifndef RWSEMAPHORE_H_
#define RWSEMAPHORE_H_

#include "TaskCommon.h"
#include <mutex>
#include <condition_variable>

class RWSemaphore {
public:
	RWSemaphore();

	virtual ~RWSemaphore();

	enum AccessMode{
		AM_READ = 0,
		AM_WRITE
	};

	void acquire(AccessMode accessMode);

	void tryAcquire(AccessMode accessMode);

	void release(AccessMode accessMode);

private:
	int m_readCounter;
	std::mutex m_mutex;
	std::unique_lock<std::mutex> m_lock;
	std::condition_variable m_condVar;
};


#endif /* RWSEMAPHORE_H_ */
