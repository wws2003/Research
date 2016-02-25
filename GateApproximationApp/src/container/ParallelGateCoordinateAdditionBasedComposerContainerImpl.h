/*
 * ParallelGateCoordinateAdditionBasedComposerContainerImpl.h
 *
 *  Created on: Feb 25, 2016
 *      Author: pham
 */

#ifndef PARALLELGATECOORDINATEADDITIONBASEDCOMPOSERCONTAINERIMPL_H_
#define PARALLELGATECOORDINATEADDITIONBASEDCOMPOSERCONTAINERIMPL_H_

#include "GateCoordinateAdditionBasedComposerContainerImpl.h"
#include "QueueBasedTaskExecutorImpl.h"
#include "ITaskExecutor.h"
#include "ICollection.h"
#include "TaskCommon.h"
#include "ITaskQueue.h"

class ParallelGateCoordinateAdditionBasedComposerContainerImpl: public GateCoordinateAdditionBasedComposerContainerImpl {
public:
	ParallelGateCoordinateAdditionBasedComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig,
			int nbThreads);

	virtual ~ParallelGateCoordinateAdditionBasedComposerContainerImpl();

protected:
	//To reset gate composer
	void overrideCoordinateAdditionalBasedGateComposer();

private:
	void wireTasksDependencies();

	void releaseTasksDependencies();

	int m_nbThreads;
	TaskExecutorPtr<LookupResult<GateRealCoordinate> > m_pTaskExecutor;
	TaskQueuePtr<QueuedTask<LookupResult<GateRealCoordinate> > > m_pTaskQueue;
};

#endif /* PARALLELGATECOORDINATEADDITIONBASEDCOMPOSERCONTAINERIMPL_H_ */
