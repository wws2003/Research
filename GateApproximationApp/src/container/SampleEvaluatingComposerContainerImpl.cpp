/*
 * SampleComposerContainerImpl.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#include "SampleEvaluatingComposerContainerImpl.h"
#include "SimpleComposerContainerImpl.h"
#include "GateCoordinateAdditionBasedComposerContainerImpl.h"

SampleEvaluatingComposerContainerImpl::SampleEvaluatingComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig) {
	wireDependencies(cabConfig, collectionConfig);
}

SampleEvaluatingComposerContainerImpl::~SampleEvaluatingComposerContainerImpl() {
	releaseDependencies();
}

GateComposerPtr SampleEvaluatingComposerContainerImpl::getStandardGateComposer() {
	return m_pStandardComposerContainer->getGateComposer();
}

//Override
GateComposerPtr SampleEvaluatingComposerContainerImpl::getEvaluatedGateComposer() {
	return m_pEvaluatedComposerContainer->getGateComposer();
}

void SampleEvaluatingComposerContainerImpl::wireDependencies(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig) {
	m_pStandardComposerContainer = ComposerContainerPtr(new SimpleComposerContainerImpl());
	m_pEvaluatedComposerContainer = ComposerContainerPtr(new GateCoordinateAdditionBasedComposerContainerImpl(cabConfig, collectionConfig));
}

void SampleEvaluatingComposerContainerImpl::releaseDependencies() {
	_destroy(m_pEvaluatedComposerContainer);
	_destroy(m_pStandardComposerContainer);
}

