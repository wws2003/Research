/*
 * ParallelGateCoordinateAdditionBasedComposerContainerImpl.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: pham
 */

#include "ParallelGateCoordinateAdditionBasedComposerContainerImpl.h"
#include "GateCoordinateLookupResultBufferedQueueBasedTaskExecutorImpl.h"
#include "GateCoordinateLookupResultNoLockTaskQueueImpl.h"
#include "ParallelMultipleComparatorCoordinateAdditionBasedGateComposer.h"

ParallelGateCoordinateAdditionBasedComposerContainerImpl::ParallelGateCoordinateAdditionBasedComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig,
		const ParallelConfig& parallelConfig) : GateCoordinateAdditionBasedComposerContainerImpl(cabConfig, collectionConfig) {
	m_parallelConfig = parallelConfig;
	wireTasksDependencies();
	overrideCoordinateAdditionalBasedGateComposer();
}

ParallelGateCoordinateAdditionBasedComposerContainerImpl::~ParallelGateCoordinateAdditionBasedComposerContainerImpl() {
	releaseTasksDependencies();
}

void ParallelGateCoordinateAdditionBasedComposerContainerImpl::overrideCoordinateAdditionalBasedGateComposer() {
	_destroy(m_pGateCoordinateComposer);

	RealCoordinate<GatePtr> epsilonRealCoordinate;
	GateCoordinateAdditionBasedComposerContainerImpl::initEpsilonRealCoordinate(epsilonRealCoordinate, m_coordinateAdditionalBasedComposerConfig.m_primaryCoordinateComparatorConfig.m_coordinateEpsilon);

	std::vector<GateRealCoordinate> epsilonRealCoordinates;
	for(unsigned int i = 0; i < m_coordinateAdditionalBasedComposerConfig.m_secondaryCoordinateComparatorConfigs.size(); i++) {
		RealCoordinate<GatePtr> secondaryEpsilonRealCoordinate;
		GateCoordinateAdditionBasedComposerContainerImpl::initEpsilonRealCoordinate(secondaryEpsilonRealCoordinate, m_coordinateAdditionalBasedComposerConfig.m_secondaryCoordinateComparatorConfigs[i].m_coordinateEpsilon);
		epsilonRealCoordinates.push_back(secondaryEpsilonRealCoordinate);
	}
	m_pGateCoordinateComposer = ComposerPtr<GateRealCoordinate>(new ParallelMultipleComparatorCoordinateAdditionBasedGateComposer(m_pPrimaryRealCoordinateComparator,
			m_pGateCoordinateCombiner,
			epsilonRealCoordinate,
			m_parallelConfig.m_taskFutureBufferSize,
			m_secondaryRealCoordinateComparators,
			epsilonRealCoordinates,
			m_pTaskExecutor));
}

void ParallelGateCoordinateAdditionBasedComposerContainerImpl::wireTasksDependencies() {
	m_pTaskQueue = TaskQueuePtr<QueuedTask<LookupResult<GateRealCoordinate> > >(new GateCoordinateLookupResultNoLockTaskQueueImpl());
	m_pTaskExecutor = TaskExecutorPtr<LookupResult<GateRealCoordinate> >(new GateCoordinateLookupResultBufferedQueueBasedTaskExecutorImpl(m_parallelConfig.m_nbThreads,
			m_pTaskQueue,
			m_parallelConfig.m_taskBufferSize));
}

void ParallelGateCoordinateAdditionBasedComposerContainerImpl::releaseTasksDependencies() {
	_destroy(m_pTaskExecutor);
	_destroy(m_pTaskQueue);
}
