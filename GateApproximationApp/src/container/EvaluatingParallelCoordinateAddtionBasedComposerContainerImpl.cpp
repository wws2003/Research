/*
 * EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: pham
 */

#include "EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl.h"
#include "GateCoordinateAdditionBasedComposerContainerImpl.h"
#include "ParallelGateCoordinateAdditionBasedComposerContainerImpl.h"

EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl::EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig,
		int nbThreads) {
	wireDependencies(cabConfig, collectionConfig, nbThreads);
}

EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl::~EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl() {
	releaseDependencies();
}

GateComposerPtr EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl::getStandardGateComposer() {
	return m_pStandardComposerContainer->getGateComposer();
}

GateComposerPtr EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl::getEvaluatedGateComposer() {
	return m_pEvaluatedComposerContainer->getGateComposer();
}

void EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl::wireDependencies(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig,
		int nbThreads) {
	m_pStandardComposerContainer = ComposerContainerPtr(new GateCoordinateAdditionBasedComposerContainerImpl(cabConfig, collectionConfig));
	m_pEvaluatedComposerContainer = ComposerContainerPtr(new ParallelGateCoordinateAdditionBasedComposerContainerImpl(cabConfig, collectionConfig, nbThreads));
}

void EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl::releaseDependencies() {
	_destroy(m_pEvaluatedComposerContainer);
	_destroy(m_pStandardComposerContainer);
}
