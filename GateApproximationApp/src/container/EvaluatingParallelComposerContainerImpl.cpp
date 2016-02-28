/*
 * EvaluatingParallelComposerContainerImpl.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#include "EvaluatingParallelComposerContainerImpl.h"
#include "SimpleComposerContainerImpl.h"
#include "SimpleParallelComposerContainerImpl.h"

EvaluatingParallelComposerContainerImpl::EvaluatingParallelComposerContainerImpl(const ParallelConfig& parallelConfig) {
	wireDependencies(parallelConfig);
}

EvaluatingParallelComposerContainerImpl::~EvaluatingParallelComposerContainerImpl() {
	releaseDependencies();
}

GateComposerPtr EvaluatingParallelComposerContainerImpl::getStandardGateComposer() {
	return m_pStandardComposerContainer->getGateComposer();
}

//Override
GateComposerPtr EvaluatingParallelComposerContainerImpl::getEvaluatedGateComposer() {
	return m_pEvaluatedComposerContainer->getGateComposer();
}

void EvaluatingParallelComposerContainerImpl::wireDependencies(const ParallelConfig& parallelConfig) {
	m_pStandardComposerContainer = ComposerContainerPtr(new SimpleComposerContainerImpl());
	m_pEvaluatedComposerContainer = ComposerContainerPtr(new SimpleParallelComposerContainerImpl(0, parallelConfig));
}

void EvaluatingParallelComposerContainerImpl::releaseDependencies() {
	_destroy(m_pEvaluatedComposerContainer);
	_destroy(m_pStandardComposerContainer);
}
