/*
 * RWSemaphore.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: pham
 */

#include "RWSemaphore.h"
#include <stdexcept>

RWSemaphore::RWSemaphore() : m_lock(m_mutex, std::defer_lock) {
	m_readCounter = 0;
}

RWSemaphore::~RWSemaphore() {

}

void RWSemaphore::acquire(AccessMode accessMode) {
	m_lock.lock();
	//R^n-R
	if(accessMode == AM_READ) {
		m_readCounter++;
		m_lock.unlock();
	}
	else {
		//R^n-W
		while(m_readCounter >= 1) {
			//locker is auto unlocked in wait(locker)
			m_condVar.wait(m_lock);
			//locker is auto locked again after wait(locker) returned
		}
		//W: Do nothing more
	}
}

void RWSemaphore::tryAcquire(AccessMode accessMode) {
	m_lock.try_lock();
	//R^n-R
	if(accessMode == AM_READ) {
		m_readCounter++;
		m_lock.unlock();
	}
	else {
		//R^n-W
		if(m_readCounter > 1) {
			throw std::runtime_error("Busy for reading");
		}
		//W: Do nothing more
	}
}

void RWSemaphore::release(AccessMode accessMode) {
	if(accessMode == AM_READ) {
		m_lock.lock();
		//R^n\R or R^n-W\R
		m_readCounter--;
		if(m_readCounter <= 0) {
			m_lock.unlock();
			m_condVar.notify_all();
		}
		else {
			m_lock.unlock();
		}
	}
	else {
		//W\W
		m_lock.unlock();
	}
}
