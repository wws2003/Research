/*
 * GateCoordinateAdditionBasedComposerContainerImpl.cpp
 *
 *  Created on: Feb 22, 2016
 *      Author: pham
 */

#include "SampleEvaluatingComposerContainerImpl.h"
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
#include "MatrixFowlerDistanceCalculator.h"
#include "OneElementCoordinateComparator.hpp"
#include "MultipleComparatorCoordinateAdditionBasedGateComposer.h"
#include "MultiNearBalancePartsGateDecomposer.h"
#include "GateCoordinateAdditionBasedComposerContainerImpl.h"
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "GateCoordinateDistanceCalculator.h"

GateCoordinateAdditionBasedComposerContainerImpl::GateCoordinateAdditionBasedComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig) : m_coordinateAdditionalBasedComposerConfig(cabConfig),
		m_collectionConfig(collectionConfig) {
	wireDependencies();
}

GateCoordinateAdditionBasedComposerContainerImpl::~GateCoordinateAdditionBasedComposerContainerImpl() {
	releaseDependencies();
}

GateComposerPtr GateCoordinateAdditionBasedComposerContainerImpl::getGateComposer() {
	return GateComposerPtr(new AdaptiveGateCoordinateComposer(m_pGateCoordinateDistanceCalculator,
			m_pGateCoordinateComposer,
			m_pGateCoordinateConveter,
			false));
}


void GateCoordinateAdditionBasedComposerContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	ResourceContainerFactory resourceContainerFactory;
	m_pResourceContainer = resourceContainerFactory.getResourceContainer(m_collectionConfig.m_librarySet,
			m_pMatrixFactory,
			m_pMatrixOperator);

	initCoordinateAdditionalBasedGateComposerElements();

	//Coordinate calculator
	MatrixPtrVector pBasis;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis, m_collectionConfig.m_nbQubits);
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	//Coordinate distance calculator
	m_pMatrixDistanceCalculator = DistanceCalculatorPtr<MatrixPtr>(new MatrixFowlerDistanceCalculator(NullPtr));
	m_pGateDistanceCalculator = DistanceCalculatorPtr<GatePtr>(new GateDistanceCalculatorByMatrixImpl(m_pMatrixDistanceCalculator));
	m_pGateCoordinateDistanceCalculator = DistanceCalculatorPtr<GateRealCoordinate>(new GateCoordinateDistanceCalculator(m_pGateDistanceCalculator));

	//Converter and misc
	bool phaseIgnored = m_collectionConfig.m_matrixDistanceCalculatorType == MDCT_FOWLER;
	m_pGateCoordinateConveter = ConverterPtr<GatePtr, RealCoordinate<GatePtr> >(new GateCoordinateConverterImpl(m_pGateRealCoordinateCalculator,
			m_pMatrixOperator,
			phaseIgnored));
}

void GateCoordinateAdditionBasedComposerContainerImpl::releaseDependencies() {
	_destroy(m_pGateCoordinateConveter);

	_destroy(m_pGateCoordinateDistanceCalculator);
	_destroy(m_pGateDistanceCalculator);
	_destroy(m_pMatrixDistanceCalculator);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pGateCoordinateComposer);
	releaseSecondaryCoordinateComparators();
	_destroy(m_pPrimaryRealCoordinateComparator);
	_destroy(m_pGateCoordinateCombiner);

	_destroy(m_pGateCombiner);

	_destroy(m_pResourceContainer);
	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

void GateCoordinateAdditionBasedComposerContainerImpl::initCoordinateAdditionalBasedGateComposerElements() {
	//Combiner
	GateCombinabilityCheckers checkers;
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));
	m_pGateCoordinateCombiner = CombinerPtr<RealCoordinate<GatePtr> >(new GateCoordinateCombinerImpl(m_pGateCombiner));

	//Coordinate comparators
	m_pPrimaryRealCoordinateComparator = getRealCoordinateComparator(m_coordinateAdditionalBasedComposerConfig.m_primaryCoordinateComparatorConfig);

	for(unsigned int i = 0; i < m_coordinateAdditionalBasedComposerConfig.m_secondaryCoordinateComparatorConfigs.size(); i++) {
		CoordinateComparatorConfig secondaryCoordinateComparatorConfig = m_coordinateAdditionalBasedComposerConfig.m_secondaryCoordinateComparatorConfigs[i];
		GateRealCoordinateComparatorPtr pSecondaryCoordinateComparator = getRealCoordinateComparator(secondaryCoordinateComparatorConfig);
		m_secondaryRealCoordinateComparators.push_back(pSecondaryCoordinateComparator);
	}

	//Gate coordinate composer
	initCoordinateAdditionalBasedGateComposer();
}

void GateCoordinateAdditionBasedComposerContainerImpl::initCoordinateAdditionalBasedGateComposer() {
	RealCoordinate<GatePtr> epsilonRealCoordinate;
	initEpsilonRealCoordinate(epsilonRealCoordinate, m_coordinateAdditionalBasedComposerConfig.m_primaryCoordinateComparatorConfig.m_coordinateEpsilon);

	if(m_secondaryRealCoordinateComparators.empty()) {
		m_pGateCoordinateComposer = ComposerPtr<RealCoordinate<GatePtr> >(new CoordinateAdditionBasedGateComposer(m_pPrimaryRealCoordinateComparator,
				m_pGateCoordinateCombiner,
				epsilonRealCoordinate,
				-1));
	}
	else {
		std::vector<RealCoordinate<GatePtr> > epsilonRealCoordinates;
		for(unsigned int i = 0; i < m_coordinateAdditionalBasedComposerConfig.m_secondaryCoordinateComparatorConfigs.size(); i++) {
			RealCoordinate<GatePtr> secondaryEpsilonRealCoordinate;
			initEpsilonRealCoordinate(secondaryEpsilonRealCoordinate, m_coordinateAdditionalBasedComposerConfig.m_secondaryCoordinateComparatorConfigs[i].m_coordinateEpsilon);
			epsilonRealCoordinates.push_back(secondaryEpsilonRealCoordinate);
		}
		m_pGateCoordinateComposer = ComposerPtr<RealCoordinate<GatePtr> >(new MultipleComparatorCoordinateAdditionBasedGateComposer(m_pPrimaryRealCoordinateComparator,
				m_pGateCoordinateCombiner,
				epsilonRealCoordinate,
				-1,
				m_secondaryRealCoordinateComparators,
				epsilonRealCoordinates));
	}
}

GateCoordinateAdditionBasedComposerContainerImpl::GateRealCoordinateComparatorPtr GateCoordinateAdditionBasedComposerContainerImpl::getRealCoordinateComparator(CoordinateComparatorConfig coordinateComparatorConfig) {
	GateRealCoordinateComparatorPtr pCoordinateComparator = NullPtr;
	switch (coordinateComparatorConfig.m_coordinateComparatorType) {
	case CMP_DICTIONARY:
		pCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new DictionaryOrderCoordinateComparator<GatePtr>());
		break;
	case CMP_SUM:
		pCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new SumCoordinateComparator<GatePtr>());
		break;
	case CMP_ONE_ELEMENT:
	{
		int comparedElementIndex = coordinateComparatorConfig.m_appliedCoordinateIndices[0];
		pCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new OneElementCoordinateComparator<GatePtr>(comparedElementIndex));
		break;
	}
	default:
		break;
	}
	return pCoordinateComparator;
}

void GateCoordinateAdditionBasedComposerContainerImpl::initEpsilonRealCoordinate(RealCoordinate<GatePtr>& epsilonRealCoordinate, mreal_t coordinateEpsilon) {
	int nbCoord = (m_collectionConfig.m_nbQubits == 1) ? 3 : 15;
	std::vector<mreal_t> coords(nbCoord, coordinateEpsilon);
	epsilonRealCoordinate = RealCoordinate<GatePtr>(NullPtr, coords);
}

void GateCoordinateAdditionBasedComposerContainerImpl::releaseSecondaryCoordinateComparators() {
	for(GateRealCoordinateComparatorList::iterator cIter = m_secondaryRealCoordinateComparators.begin(); cIter!= m_secondaryRealCoordinateComparators.end();) {
		GateRealCoordinateComparatorPtr pCoordinateComparator = *cIter;
		cIter = m_secondaryRealCoordinateComparators.erase(cIter);
		_destroy(pCoordinateComparator);
	}
}
