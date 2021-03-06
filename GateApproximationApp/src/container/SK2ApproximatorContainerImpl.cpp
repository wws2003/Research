/*
 * SK2ApproximatorContainerImpl.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "SKApproximatorContainerImpl.h"
#include "ComposerBasedGateApproximator.h"
#include "DummyGateDecomposer.h"
#include "ComposerBasedGateApproximator.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "SampleEvaluatingComposerContainerImpl.h"
#include "HarrowGateDecomposer.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "GateCoordinateAdditionBasedComposerContainerImpl.h"
#include "SKGateApproximator.h"
#include "SampleGateStoreFactoryImpl.h"
#include "SK2ApproximatorContainerImpl.h"

SK2ApproximatorContainerImpl::SK2ApproximatorContainerImpl(SKApproximatorConfig config,
		CollectionConfig coreCollectionConfig,
		CoordinateAdditionalBasedComposerConfig cadbConfig) : m_approximatorConfig(config), m_coreCollectionConfig(coreCollectionConfig) {
	initGateComposerContainer(cadbConfig);
	wireDependencies();
}

SK2ApproximatorContainerImpl::~SK2ApproximatorContainerImpl() {
	releaseDependencies();
}

GateApproximatorPtr SK2ApproximatorContainerImpl::getGateApproximator() {
	return GateApproximatorPtr(new SKGateApproximator(m_pHarrowGateDecomposer,
			m_pGateComposer,
			m_approximatorConfig.m_initialEpsilon,
			m_approximatorConfig.m_nbCandidates,
			m_approximatorConfig.m_recursiveLevels,
			m_pMatrixOperator,
			NullPtr));
}

void SK2ApproximatorContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	SampleGateStoreFactoryImpl gateStoreFactory(m_pMatrixOperator, m_pMatrixFactory);
	m_pGateStore = gateStoreFactory.getGateStore(m_coreCollectionConfig.m_nbQubits);

	MatrixPtrVector pBasis;
	m_pGateStore->getMatrixOrthonormalBasis(pBasis);

	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(NullPtr));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	m_pHarrowGateDecomposer = GateDecomposerPtr(new HarrowGateDecomposer(m_pGateRealCoordinateCalculator, m_pMatrixOperator));

	m_pGateComposer = m_pGateComposerContainer->getGateComposer();
}

void SK2ApproximatorContainerImpl::releaseDependencies() {
	_destroy(m_pGateComposer);
	_destroy(m_pGateComposerContainer);

	_destroy(m_pHarrowGateDecomposer);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pGateStore);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

void SK2ApproximatorContainerImpl::initGateComposerContainer(const CoordinateAdditionalBasedComposerConfig& cadbConfig) {
	m_pGateComposerContainer = ComposerContainerPtr(new GateCoordinateAdditionBasedComposerContainerImpl(cadbConfig, m_coreCollectionConfig));
}
