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
#include "MatrixRealInnerProductByTraceImpl.h"
#include "GateSetLogImpl.h"
#include "AdaptiveGateCoordinateComposer.h"
#include "CoordinateAdditionBasedGateComposer.h"
#include "GateCoordinateConverterImpl.h"
#include "GateCoordinateCombinerImpl.h"
#include "GateCoordinateCalculatorImpl.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "NearHalfBalanceGateDecomposer.h"
#include "DuplicateGateLookupResultFilterImpl.h"
#include "DictionaryOrderCoordinateComparator.hpp"
#include "SumCoordinateComparator.hpp"
#include "MultiNearBalancePartsGateDecomposer.h"
#include "SimpleComposerContainerImpl.h"
#include "GateCoordinateAdditionBasedComposerContainerImpl.h"
#include "SampleGateStoreFactoryImpl.h"

ComposerBasedApproximatorContainer::ComposerBasedApproximatorContainer(ComposerBasedApproximatorConfig approximatorConfig,
		CollectionConfig coreCollectionConfig) : m_approximatorConfig(approximatorConfig),
		m_coreCollectionConfig(coreCollectionConfig) {
	wireDependencies();
}

ComposerBasedApproximatorContainer::ComposerBasedApproximatorContainer(ComposerBasedApproximatorConfig approximatorConfig,
		CoordinateAdditionalBasedComposerConfig coordinateAdditionalBasedComposerConfig,
		CollectionConfig coreCollectionConfig) : m_approximatorConfig(approximatorConfig),
				m_coordinateAdditionalBasedComposerConfig(coordinateAdditionalBasedComposerConfig),
				m_coreCollectionConfig(coreCollectionConfig) {
	wireDependencies();
}

ComposerBasedApproximatorContainer::~ComposerBasedApproximatorContainer() {
	releaseDependencies();
}

GateApproximatorPtr ComposerBasedApproximatorContainer::getGateApproximator() {
	return GateApproximatorPtr(new ComposerBasedGateApproximator(m_pGateDecomposer,
			m_approximatorConfig.m_nbPartialQueries,
			m_approximatorConfig.m_nbCandidates,
			m_pGateComposer,
			m_approximatorConfig.m_initialEpsilon,
			m_pGateLookupResultFilter));
}

void ComposerBasedApproximatorContainer::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	SampleGateStoreFactoryImpl gateStoreFactory(m_pMatrixOperator, m_pMatrixFactory);
	m_pGateStore = gateStoreFactory.getGateStore(m_coreCollectionConfig.m_nbQubits);

	//Instantiate decomposer
	MatrixPtrVector pBasis;
	m_pGateStore->getMatrixOrthonormalBasis(pBasis);

	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	m_pGateDecomposer = generateDecomposerFromConfig(m_approximatorConfig);

	m_pGateComposer = generateComposerFromConfig(m_approximatorConfig);

	if(m_approximatorConfig.m_userFilter) {
		m_pGateLookupResultFilter = GateLookupResultFilterPtr(new DuplicateGateLookupResultFilterImpl());
	}
	else {
		m_pGateLookupResultFilter = NullPtr;
	}
}

void ComposerBasedApproximatorContainer::releaseDependencies() {
	_destroy(m_pGateLookupResultFilter);

	_destroy(m_pGateComposer);
	_destroy(m_pComposerContainer);

	_destroy(m_pGateCoordinateComposer);
	_destroy(m_pGateCoordinateConveter);
	_destroy(m_pGateCoordinateCombiner);
	_destroy(m_pRealCoordinateComparator);

	_destroy(m_pGateDecomposer);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pGateStore);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

GateComposerPtr ComposerBasedApproximatorContainer::generateComposerFromConfig(ComposerBasedApproximatorConfig approximatorConfig) {
	GateComposerPtr pGateComposerPtr = NullPtr;
	switch (m_approximatorConfig.m_buildingBlockComposerType) {
	case CT_SIMPLE:
		m_pComposerContainer = ComposerContainerPtr(new SimpleComposerContainerImpl());
		break;
	case CT_COORDINATE_ADDTIONAL_BASED:
		m_pComposerContainer = ComposerContainerPtr(new GateCoordinateAdditionBasedComposerContainerImpl(m_coordinateAdditionalBasedComposerConfig, m_coreCollectionConfig));
		break;
	default:
		break;
	}
	pGateComposerPtr = m_pComposerContainer->getGateComposer();
	return pGateComposerPtr;
}

GateDecomposerPtr ComposerBasedApproximatorContainer::generateDecomposerFromConfig(ComposerBasedApproximatorConfig approximatorConfig) {
	GateDecomposerPtr pGateDecomposer = NullPtr;
	switch (m_approximatorConfig.m_queryDecomposerType) {
	case DT_NEAR_BALANCE:
		pGateDecomposer = GateDecomposerPtr(new MultiNearBalancePartsGateDecomposer(m_pGateRealCoordinateCalculator, m_pMatrixOperator));
		break;
	case DT_GC_MULTI_LEVELS:
		//TODO Implement
		break;
	default:
		break;
	}

	return pGateDecomposer;
}
