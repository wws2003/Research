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

ComposerBasedApproximatorContainer::ComposerBasedApproximatorContainer(ComposerBasedApproximatorConfig approximatorConfig,
		CollectionConfig coreCollectionConfig) : m_approximatorConfig(approximatorConfig),
		m_coreCollectionConfig(coreCollectionConfig) {
	wireDependencies();
}

ComposerBasedApproximatorContainer::ComposerBasedApproximatorContainer(ComposerBasedApproximatorConfig approximatorConfig,
		CoordinateAdditionalBasedComposerConfig coordinateAdditionalBasedComposerConfig,
		CollectionConfig coreCollectionConfig) : m_approximatorConfig(approximatorConfig), m_coordinateAdditionalBasedComposerConfig(coordinateAdditionalBasedComposerConfig) , m_coreCollectionConfig(coreCollectionConfig) {
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

	ResourceContainerFactory resourceContainerFactory;
	m_pResourceContainer = resourceContainerFactory.getResourceContainer(m_coreCollectionConfig.m_librarySet,
			m_pMatrixFactory,
			m_pMatrixOperator);

	//Instantiate decomposer
	MatrixPtrVector pBasis;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis, m_coreCollectionConfig.m_nbQubits);
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	m_pGateDecomposer = generateDecomposerFromConfig(m_approximatorConfig);

	//Instantiate composer
	initCoordinateAdditionalBasedGateComposerElements();
	m_pGateSetLog = ElementSetLogPtr<GatePtr>(new GateSetLogImpl());

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
	_destroy(m_pGateCoordinateComposer);
	_destroy(m_pGateCoordinateConveter);
	_destroy(m_pGateCoordinateCombiner);
	_destroy(m_pRealCoordinateComparator);

	_destroy(m_pGateDecomposer);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pGateSetLog);
	_destroy(m_pGateCombiner);

	_destroy(m_pResourceContainer);
	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

void ComposerBasedApproximatorContainer::initCoordinateAdditionalBasedGateComposerElements() {
	initRealCoordinateComparator();

	RealCoordinate<GatePtr> epsilonRealCoordinate;
	initEpsilonRealCoordinate(epsilonRealCoordinate);

	GateCombinabilityCheckers checkers;
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	m_pGateCoordinateCombiner = CombinerPtr<RealCoordinate<GatePtr> >(new GateCoordinateCombinerImpl(m_pGateCombiner));
	m_pGateCoordinateComposer = ComposerPtr<RealCoordinate<GatePtr> >(new CoordinateAdditionBasedGateComposer(m_pRealCoordinateComparator,
			m_pGateCoordinateCombiner,
			epsilonRealCoordinate,
			-1));

	bool phaseIgnored = m_coreCollectionConfig.m_matrixDistanceCalculatorType == MDCT_FOWLER;
	m_pGateCoordinateConveter = ConverterPtr<GatePtr, RealCoordinate<GatePtr> >(new GateCoordinateConverterImpl(m_pGateRealCoordinateCalculator,
			m_pMatrixOperator,
			phaseIgnored));
}

void ComposerBasedApproximatorContainer::initRealCoordinateComparator() {
	switch (m_coordinateAdditionalBasedComposerConfig.m_primaryCoordinateComparatorConfig.m_coordinateComparatorType) {
	case CMP_DICTIONARY:
		m_pRealCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new DictionaryOrderCoordinateComparator<GatePtr>());
		break;
	default:
		m_pRealCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new SumCoordinateComparator<GatePtr>());
		break;
	}
}

void ComposerBasedApproximatorContainer::initEpsilonRealCoordinate(RealCoordinate<GatePtr>& epsilonRealCoordinate) {
	int nbCoord = (m_coreCollectionConfig.m_nbQubits == 1) ? 3 : 15;
	std::vector<mreal_t> coords(nbCoord, m_coordinateAdditionalBasedComposerConfig.m_primaryCoordinateComparatorConfig.m_coordinateEpsilon);
	epsilonRealCoordinate = RealCoordinate<GatePtr>(NullPtr, coords);
}

GateComposerPtr ComposerBasedApproximatorContainer::generateComposerFromConfig(ComposerBasedApproximatorConfig approximatorConfig) {
	GateComposerPtr pGateComposerPtr = NullPtr;
	switch (m_approximatorConfig.m_buildingBlockComposerType) {
	case CT_SIMPLE:
		pGateComposerPtr = GateComposerPtr(new SimpleGateComposer(m_pGateCombiner,
				approximatorConfig.m_nbCandidates,
				m_pGateSetLog));
		break;
	case CT_COORDINATE_ADDTIONAL_BASED:
		pGateComposerPtr = GateComposerPtr(new AdaptiveGateCoordinateComposer(m_pGateCoordinateDistanceCalculator,
				m_pGateCoordinateComposer,
				0.0,
				m_pGateCoordinateConveter));
		break;
	default:
		break;
	}
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
