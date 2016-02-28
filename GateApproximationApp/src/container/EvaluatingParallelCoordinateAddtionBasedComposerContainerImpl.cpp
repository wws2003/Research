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
		const ParallelConfig& parallelConfig) {

	wireDependencies(cabConfig, collectionConfig, parallelConfig);
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
		const ParallelConfig& parallelConfig) {
	m_pStandardComposerContainer = ComposerContainerPtr(new GateCoordinateAdditionBasedComposerContainerImpl(cabConfig, collectionConfig));
	m_pEvaluatedComposerContainer = ComposerContainerPtr(new ParallelGateCoordinateAdditionBasedComposerContainerImpl(cabConfig, collectionConfig, parallelConfig));
}

void EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl::releaseDependencies() {
	_destroy(m_pEvaluatedComposerContainer);
	_destroy(m_pStandardComposerContainer);
}
