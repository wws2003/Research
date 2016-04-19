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
			const ParallelConfig& parallelConfig);

	virtual ~ParallelGateCoordinateAdditionBasedComposerContainerImpl();

protected:
	//To reset gate composer
	void overrideCoordinateAdditionalBasedGateComposer();

private:
	void wireTasksDependencies();

	void releaseTasksDependencies();

	ParallelConfig m_parallelConfig;
	TaskExecutorPtr<LookupResult<GateRealCoordinate> > m_pTaskExecutor;
	TaskQueuePtr<QueuedTask<LookupResult<GateRealCoordinate> > > m_pTaskQueue;
};

#endif /* PARALLELGATECOORDINATEADDITIONBASEDCOMPOSERCONTAINERIMPL_H_ */
