/*
 * ComposerBasedApproximatorContainer.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: pham
 */

#include "ComposerBasedApproximatorContainer.h"
#include "ComposerBasedGateApproximator.h"
#include "SimpleGateComposer.h"
#include "GateSetLogImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "GateCombinerImpl.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "ContainerResourceFactory.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "GateSetLogImpl.h"
#include "GateCoordinateCalculatorImpl.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "NearHalfBalanceGateDecomposer.h"

ComposerBasedApproximatorContainer::ComposerBasedApproximatorContainer(ComposerBasedApproximatorConfig approximatorConfig,
		CollectionConfig coreCollectionConfig) : m_approximatorConfig(approximatorConfig), m_coreCollectionConfig(coreCollectionConfig) {
	wireDependencies();
}

ComposerBasedApproximatorContainer::~ComposerBasedApproximatorContainer() {
	releaseDependencies();
}

GateApproximatorPtr ComposerBasedApproximatorContainer::getGateApproximator() {
	return GateApproximatorPtr(new ComposerBasedGateApproximator(m_pGateDecomposer,
			m_approximatorConfig.m_nbPartialQueries,
			m_pGateComposer,
			m_approximatorConfig.m_initialEpsilon));
}

void ComposerBasedApproximatorContainer::wireDependencies() {
	//Currently don't use type configuration for dependencies

	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	ResourceContainerFactory resourceContainerFactory;
	m_pResourceContainer = resourceContainerFactory.getResourceContainer(m_coreCollectionConfig.m_librarySet,
			m_pMatrixFactory,
			m_pMatrixOperator);

	GateCombinabilityCheckers checkers;
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	m_pGateSetLog = ElementSetLogPtr<GatePtr>(new GateSetLogImpl());
	m_pGateComposer = GateComposerPtr(new SimpleGateComposer(m_pGateCombiner, m_approximatorConfig.m_nbCandidates, m_pGateSetLog));

	//Instantiate decomposer
	MatrixPtrVector pBasis;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis, m_coreCollectionConfig.m_nbQubits);
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	if(m_approximatorConfig.m_nbPartialQueries == 2) {
		m_pGateDecomposer = GateDecomposerPtr(new NearHalfBalanceGateDecomposer(m_pGateRealCoordinateCalculator, m_pMatrixOperator));
	}
	else {
		//TODO Implement
	}
}

void ComposerBasedApproximatorContainer::releaseDependencies() {
	_destroy(m_pGateDecomposer);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pGateComposer);
	_destroy(m_pGateSetLog);
	_destroy(m_pGateCombiner);

	_destroy(m_pResourceContainer);
	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}
