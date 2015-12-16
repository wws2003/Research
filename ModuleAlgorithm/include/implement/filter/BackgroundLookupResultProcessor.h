/*
 * BackgroundLookupResultProcessor.h
 *
 *  Created on: Dec 14, 2015
 *      Author: pham
 */

#ifndef BACKGROUNDLOOKUPRESULTPROCESSOR_H_
#define BACKGROUNDLOOKUPRESULTPROCESSOR_H_

#include "ILookupResultProcessor.h"
#include <deque>
#include <queue>
#include <pthread.h>

template<typename T>
class BackgroundLookupResultProcessor : public ILookupResultProcessor<T> {
public:
	BackgroundLookupResultProcessor();

	virtual ~BackgroundLookupResultProcessor();

	//Override
	void init();

	//Override
	void reset();

	//Override
	void addLookupResultsBatch(const std::vector<LookupResult<T> >& result);

	//Override
	void addLookupResult(const LookupResult<T>& result);

	//Override
	void retrieveProcessedLookupResults(std::vector<LookupResult<T> >& results, bool toSortResults = true);

	void doBackgroundJob();

protected:
	virtual void processOnBackground(const LookupResult<T>& result) = 0;

	virtual void processOnBackground(const std::vector<LookupResult<T> > requestsBuffer) = 0;

	virtual void exportInternalContainerToResults(std::vector<LookupResult<T> >& results, bool toSortResults = true) const = 0;

	virtual void resetInternalContainer() = 0;

private:
	void notifyNewRequest(const LookupResult<T>& result);

	void notifyNewRequestsBatch(const std::vector<LookupResult<T> >& results);

	void notifyTerminate();

	pthread_t m_tid;
	pthread_mutex_t m_requestQueueMutex;
	pthread_cond_t m_requestQueueCondVar;

	std::deque<LookupResult<T> > m_requestQueue;

	pthread_mutex_t m_internalContainerMutex;

	bool m_terminateFlag;
};


#endif /* BACKGROUNDLOOKUPRESULTPROCESSOR_H_ */
