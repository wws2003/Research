/*
 * SampleApproximatorContainerImpl.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#include "SampleApproximatorContainerImpl.h"
#include "ComposerBasedGateApproximator.h"
#include "MapBasedGateBinCollectionImpl.h"
#include "DuplicateGateCancelationCombinerImpl.h"
#include "HTVBasedResourceContainerImpl.h"
#include "HVBasedResourceContainerImpl.h"
#include "DummyGateDecomposer.h"
#include "NearIdentityGateBinBasedComposer.h"
#include "ComposerBasedGateApproximator.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "IResourceContainer.h"
#include "SampleLibraryMatrixStore.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"

SampleApproximatorContainerImpl::SampleApproximatorContainerImpl(NearIdentityApproximatorConfig approximatorConfig,
		CollectionConfig coreCollectionConfig) : m_approximatorConfig(approximatorConfig), m_coreCollectionConfig(coreCollectionConfig) {
	wireDependencies();
}

SampleApproximatorContainerImpl::~SampleApproximatorContainerImpl() {
	releaseDependencies();
}

GateApproximatorPtr SampleApproximatorContainerImpl::getGateApproximator() {
	GateApproximatorPtr pGateApproximator = GateApproximatorPtr(new ComposerBasedGateApproximator(m_pGateDecomposer,
			1,
			m_pGateComposer,
			m_approximatorConfig.m_initialEpsilon));

	return pGateApproximator;
}

void SampleApproximatorContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	setupResourceContainer();

	GateCombinabilityCheckers checkers;
	m_pResourceContainer->getGateCombinabilityCheckers(checkers, m_coreCollectionConfig.m_nbQubits);
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	MatrixPtrVector pBasis;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis, m_coreCollectionConfig.m_nbQubits);
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	GateCombinabilityCheckers emptyCheckers;
	m_pGateInBinCombiner = CombinerPtr<GatePtr>(new DuplicateGateCancelationCombinerImpl(emptyCheckers, m_pMatrixOperator));
	m_pBinCollection = BinCollectionPtr<GatePtr>(new MapBasedGateBinCollectionImpl());

	m_pGateDecomposer = DecomposerPtr<GatePtr>(new DummyGateDecomposer());
	m_pGateComposer = ComposerPtr<GatePtr>(new NearIdentityGateBinBasedComposer(m_pGateRealCoordinateCalculator,
			m_pGateCombiner,
			m_pBinCollection,
			getComposerConfig()));
}

void SampleApproximatorContainerImpl::releaseDependencies() {
	_destroy(m_pGateDecomposer);
	_destroy(m_pGateComposer);

	_destroy(m_pBinCollection);
	_destroy(m_pGateInBinCombiner);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pGateCombiner);

	_destroy(m_pResourceContainer);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

void SampleApproximatorContainerImpl::setupResourceContainer() {
	switch(m_coreCollectionConfig.m_librarySet) {
	case L_HT:
		m_pResourceContainer = ResourceContainerPtr(new SampleResourceContainerImpl(m_pMatrixOperator, m_pMatrixFactory));
		break;
	case L_HCV:
		m_pResourceContainer = ResourceContainerPtr(new HVBasedResourceContainerImpl(m_pMatrixOperator, m_pMatrixFactory));
		break;
	case L_HTCNOT:
		m_pResourceContainer = ResourceContainerPtr(new SampleResourceContainerImpl(m_pMatrixOperator, m_pMatrixFactory));
		break;
	case L_HTCV:
		m_pResourceContainer = ResourceContainerPtr(new HTVBasedResourceContainerImpl(m_pMatrixOperator, m_pMatrixFactory));
		break;
	default:
		break;
	}

	m_pResourceContainer->setup();
}

NearIdentityElementBinBasedComposer<GatePtr>::Config SampleApproximatorContainerImpl::getComposerConfig() {
	NearIdentityElementBinBasedComposer<GatePtr>::Config config;
	config.m_maxMergedBinDistance = m_approximatorConfig.m_maxMergedBinDistance;
	config.m_maxCandidateEpsilon = m_approximatorConfig.m_maxCandidateEpsilon;
	config.m_maxCandidateEpsilonDecreaseFactor = m_approximatorConfig.m_maxCandidateEpsilonDecreaseFactor;
	config.m_iterationMaxSteps = m_approximatorConfig.m_iterationMaxSteps;
	config.m_maxResultNumber = m_approximatorConfig.m_maxResultNumber;
	return config;
}
