/*
 * EvaluatingParallelComposerContainerImpl.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#include "EvaluatingParallelComposerContainerImpl.h"
#include "SimpleComposerContainerImpl.h"
#include "SimpleParallelComposerContainerImpl.h"

EvaluatingParallelComposerContainerImpl::EvaluatingParallelComposerContainerImpl(int nbThreads) {
	m_nbThreads = nbThreads;
	wireDependencies();
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

void EvaluatingParallelComposerContainerImpl::wireDependencies() {
	m_pStandardComposerContainer = ComposerContainerPtr(new SimpleComposerContainerImpl());
	m_pEvaluatedComposerContainer = ComposerContainerPtr(new SimpleParallelComposerContainerImpl(0, m_nbThreads));
}

void EvaluatingParallelComposerContainerImpl::releaseDependencies() {
	_destroy(m_pEvaluatedComposerContainer);
	_destroy(m_pStandardComposerContainer);
}
