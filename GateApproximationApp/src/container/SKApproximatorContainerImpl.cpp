/*
 * SKApproximatorContainerImpl.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: pham
 */

#include "SKApproximatorContainerImpl.h"
#include "ComposerBasedGateApproximator.h"
#include "DummyGateDecomposer.h"
#include "ComposerBasedGateApproximator.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "HarrowGateDecomposer.h"
#include "SimpleGateComposer.h"
#include "SimpleComposerContainerImpl.h"
#include "GateCombinerImpl.h"
#include "SKGateApproximator.h"
#include "SampleGateStoreFactoryImpl.h"

SKApproximatorContainerImpl::SKApproximatorContainerImpl(SKApproximatorConfig config,
		CollectionConfig coreCollectionConfig) : m_approximatorConfig(config), m_coreCollectionConfig(coreCollectionConfig) {
	wireDependencies();
}

SKApproximatorContainerImpl::~SKApproximatorContainerImpl() {
	releaseDependencies();
}

GateApproximatorPtr SKApproximatorContainerImpl::getGateApproximator() {
	return GateApproximatorPtr(new SKGateApproximator(m_pHarrowGateDecomposer,
			m_pGateLoggingComposer,
			m_approximatorConfig.m_initialEpsilon,
			m_approximatorConfig.m_nbCandidates,
			m_approximatorConfig.m_recursiveLevels,
			m_pMatrixOperator,
			NullPtr));
}

void SKApproximatorContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	GateCombinabilityCheckers checkers;
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	SampleGateStoreFactoryImpl gateStoreFactory(m_pMatrixOperator, m_pMatrixFactory);
	m_pGateStore = gateStoreFactory.getGateStore(m_coreCollectionConfig.m_nbQubits);

	MatrixPtrVector pBasis;
	m_pGateStore->getMatrixOrthonormalBasis(pBasis);

	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(NullPtr));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	m_pHarrowGateDecomposer = GateDecomposerPtr(new HarrowGateDecomposer(m_pGateRealCoordinateCalculator, m_pMatrixOperator));
	m_pComposerContainer = ComposerContainerPtr(new SimpleComposerContainerImpl(10));
	m_pGateLoggingComposer = m_pComposerContainer->getGateComposer();
}

void SKApproximatorContainerImpl::releaseDependencies() {
	_destroy(m_pGateLoggingComposer);
	_destroy(m_pComposerContainer);

	_destroy(m_pHarrowGateDecomposer);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pGateCombiner);

	_destroy(m_pGateStore);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}
