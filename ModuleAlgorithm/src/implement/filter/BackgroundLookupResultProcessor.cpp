/*
 * BackgroundLookupResultProcessor.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: pham
 */

#include "BackgroundLookupResultProcessor.h"

template<typename T>
void* start_thread(void* arg)
{
	BackgroundLookupResultProcessor<T>* tp = (BackgroundLookupResultProcessor<T>*) arg;
	tp->doBackgroundJob();
	return NULL;
}

template<typename T>
BackgroundLookupResultProcessor<T>::BackgroundLookupResultProcessor() : m_tid(0), m_terminateFlag(false){
	//Init mutexes
	pthread_mutex_init(&m_requestQueueMutex, NULL);
	pthread_mutex_init(&m_internalContainerMutex, NULL);

	//Init cond var
	pthread_cond_init(&m_requestQueueCondVar, NULL);
}

template<typename T>
BackgroundLookupResultProcessor<T>::~BackgroundLookupResultProcessor() {
	if(m_tid) {
		//Try to stop thread
		notifyTerminate();

		void* result = NULL;
		pthread_join(m_tid, &result);
	}

	//Destroy cond var
	pthread_cond_destroy(&m_requestQueueCondVar);

	//Destroy mutexes
	pthread_mutex_destroy(&m_internalContainerMutex);
	pthread_mutex_destroy(&m_requestQueueMutex);

}

template<typename T>
void BackgroundLookupResultProcessor<T>::init() {
	if(!m_tid) {
		pthread_create(&m_tid, NULL, start_thread<T>, this);
	}
}

template<typename T>
void BackgroundLookupResultProcessor<T>::reset() {
	pthread_mutex_lock(&m_internalContainerMutex);
	resetInternalContainer();
	pthread_mutex_unlock(&m_internalContainerMutex);
}

template<typename T>
void BackgroundLookupResultProcessor<T>::addLookupResultsBatch(const std::vector<LookupResult<T> >& results) {
	if(!results.empty()) {
		notifyNewRequestsBatch(results);
	}
}

template<typename T>
void BackgroundLookupResultProcessor<T>::addLookupResult(const LookupResult<T>& result) {
	notifyNewRequest(result);
}

template<typename T>
void BackgroundLookupResultProcessor<T>::retrieveProcessedLookupResults(std::vector<LookupResult<T> >& results, bool toSortResults) {
	pthread_mutex_lock(&m_internalContainerMutex);
	exportInternalContainerToResults(results);
	pthread_mutex_unlock(&m_internalContainerMutex);
}

template<typename T>
void BackgroundLookupResultProcessor<T>::notifyNewRequest(const LookupResult<T>& result) {
	pthread_mutex_lock(&m_requestQueueMutex);
	m_requestQueue.push_back(result);
	pthread_cond_signal(&m_requestQueueCondVar);
	pthread_mutex_unlock(&m_requestQueueMutex);
}

template<typename T>
void BackgroundLookupResultProcessor<T>::notifyNewRequestsBatch(const std::vector<LookupResult<T> >& results) {
	pthread_mutex_lock(&m_requestQueueMutex);
	for(typename std::vector<LookupResult<T> >::const_iterator rIter = results.begin(); rIter != results.end(); rIter++) {
		m_requestQueue.push_back(*rIter);
	}
	pthread_cond_signal(&m_requestQueueCondVar);
	pthread_mutex_unlock(&m_requestQueueMutex);
}

template<typename T>
void BackgroundLookupResultProcessor<T>::notifyTerminate() {
	pthread_mutex_lock(&m_requestQueueMutex);
	m_terminateFlag = true;
	pthread_cond_signal(&m_requestQueueCondVar);
	pthread_mutex_unlock(&m_requestQueueMutex);
}

template<typename T>
void BackgroundLookupResultProcessor<T>::doBackgroundJob() {
	pthread_mutex_lock(&m_requestQueueMutex);

	while (!m_terminateFlag) {
		while(m_requestQueue.empty() && !m_terminateFlag) {
			pthread_cond_wait(&m_requestQueueCondVar, &m_requestQueueMutex);
		}

		//Request queue has been locked again, but still need to check to make sure
		if(!m_requestQueue.empty() && !m_terminateFlag) {
			std::vector<LookupResult<T> > requestBuffer;
			while(!m_requestQueue.empty()) {
				requestBuffer.push_back(m_requestQueue.front());
				m_requestQueue.pop_front();
			}

			//Unlock request queue to allow new request
			pthread_mutex_unlock(&m_requestQueueMutex);

			//Lock internal container to process with new request
			pthread_mutex_lock(&m_internalContainerMutex);
			processOnBackground(requestBuffer);
			pthread_mutex_unlock(&m_internalContainerMutex);

			pthread_mutex_lock(&m_requestQueueMutex);
		}
	}

	pthread_mutex_unlock(&m_requestQueueMutex);
}


