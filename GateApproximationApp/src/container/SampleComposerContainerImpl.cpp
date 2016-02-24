/*
 * SampleComposerContainerImpl.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#include "SampleComposerContainerImpl.h"
#include "SimpleComposerContainerImpl.h"
#include "GateCoordinateAdditionBasedComposerContainerImpl.h"

SampleComposerContainerImpl::SampleComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig) {
	wireDependencies(cabConfig, collectionConfig);
}

SampleComposerContainerImpl::~SampleComposerContainerImpl() {
	releaseDependencies();
}

GateComposerPtr SampleComposerContainerImpl::getStandardGateComposer() {
	return m_pStandardComposerContainer->getGateComposer();
}

//Override
GateComposerPtr SampleComposerContainerImpl::getEvaluatedGateComposer() {
	return m_pEvaluatedComposerContainer->getGateComposer();
}

void SampleComposerContainerImpl::wireDependencies(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig) {
	m_pStandardComposerContainer = ComposerContainerPtr(new SimpleComposerContainerImpl());
	m_pEvaluatedComposerContainer = ComposerContainerPtr(new GateCoordinateAdditionBasedComposerContainerImpl(cabConfig, collectionConfig));
}

void SampleComposerContainerImpl::releaseDependencies() {
	_destroy(m_pEvaluatedComposerContainer);
	_destroy(m_pStandardComposerContainer);
}

