/*
 * ComposerBasedSK2ApproximatorContainerImpl.cpp
 *
 *  Created on: Jan 6, 2016
 *      Author: pham
 */

#include "ComposerBasedSK2ApproximatorContainerImpl.h"
#include "ComposerBasedApproximatorContainer.h"
#include "SKGateApproximator.h"

ComposerBasedSK2ApproximatorContainerImpl::ComposerBasedSK2ApproximatorContainerImpl(SKApproximatorConfig2 config,
		CollectionConfig coreCollectionConfig,
		ComposerBasedApproximatorConfig composerBasedHelperApproximatorConfig) : SK2ApproximatorContainerImpl(config, coreCollectionConfig) {
	initHelperApproximatorContainer(composerBasedHelperApproximatorConfig);
}

ComposerBasedSK2ApproximatorContainerImpl::~ComposerBasedSK2ApproximatorContainerImpl() {
	releaseHelperApproximatorContainer();
}

GateApproximatorPtr ComposerBasedSK2ApproximatorContainerImpl::getGateApproximator() {
	return GateApproximatorPtr(new SKGateApproximator(m_pHarrowGateDecomposer,
			m_pGateComposer,
			m_approximatorConfig.m_initialEpsilon,
			m_approximatorConfig.m_nbCandidates,
			m_approximatorConfig.m_recursiveLevels,
			m_pMatrixOperator,
			m_pHelperApproximatorContainer->getGateApproximator()));
}

void ComposerBasedSK2ApproximatorContainerImpl::initHelperApproximatorContainer(ComposerBasedApproximatorConfig composerBasedHelperApproximatorConfig) {
	m_pHelperApproximatorContainer = ApproximatorContainerPtr(new ComposerBasedApproximatorContainer(composerBasedHelperApproximatorConfig, m_coreCollectionConfig));
}

void ComposerBasedSK2ApproximatorContainerImpl::releaseHelperApproximatorContainer() {
	_destroy(m_pHelperApproximatorContainer);
}

