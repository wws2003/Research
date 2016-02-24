/*
 * SimpleParallelComposerContainer.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#include "SimpleParallelComposerContainerImpl.h"
#include "SimpleParallelGateComposer.h"
#include "GateSetLogImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "GateCombinerImpl.h"
#include "GateLookupResultQueueBasedTaskExecutorImpl.h"
#include "GateLookupResultNoLockTaskQueueImpl.h"
#include "GateLookupResultBufferedQueueBasedTaskExecutorImpl.h"
#include "GateSetLogImpl.h"

SimpleParallelComposerContainerImpl::SimpleParallelComposerContainerImpl(int nbCandidates, int nbThreads) {
	m_nbCandidates = nbCandidates;
	m_nbThreads = nbThreads;
	wireDependencies();
}

SimpleParallelComposerContainerImpl::~SimpleParallelComposerContainerImpl() {
	releaseDependencies();
}

GateComposerPtr SimpleParallelComposerContainerImpl::getGateComposer() {
	return GateComposerPtr(new SimpleParallelGateComposer(m_pGateCombiner,
			m_nbCandidates,
			m_pGateSetLog,
			m_pTaskExecutor));
}

void SimpleParallelComposerContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));
	m_pGateSetLog = ElementSetLogPtr<GatePtr>(new GateSetLogImpl());
	GateCombinabilityCheckers checkers;
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	m_pTaskQueue = TaskQueuePtr<QueuedTask<LookupResult<GatePtr> > >(new GateLookupResultNoLockTaskQueueImpl());
	m_pTaskExecutor = TaskExecutorPtr<LookupResult<GatePtr> >(new GateLookupResultBufferedQueueBasedTaskExecutorImpl(m_nbThreads,
			m_pTaskQueue,
			3300));
}

void SimpleParallelComposerContainerImpl::releaseDependencies() {
	_destroy(m_pTaskExecutor);
	_destroy(m_pTaskQueue);

	_destroy(m_pGateCombiner);
	_destroy(m_pGateSetLog);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

