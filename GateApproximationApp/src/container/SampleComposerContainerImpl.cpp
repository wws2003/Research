/*
 * SampleComposerContainerImpl.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#include "SampleComposerContainerImpl.h"
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

SampleComposerContainerImpl::SampleComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig) : m_coordinateAdditionalBasedComposerConfig(cabConfig),
		m_collectionConfig(collectionConfig) {
	wireDependencies();
}

SampleComposerContainerImpl::~SampleComposerContainerImpl() {
	releaseDependencies();
}

GateComposerPtr SampleComposerContainerImpl::getStandardGateComposer() {
	return GateComposerPtr(new SimpleGateComposer(m_pGateCombiner,
			0,
			m_pGateSetLog));
}

//Override
GateComposerPtr SampleComposerContainerImpl::getEvaluatedGateComposer() {
	return GateComposerPtr(new AdaptiveGateCoordinateComposer(NullPtr,
			m_pGateCoordinateComposer,
			0.0,
			m_pGateCoordinateConveter));
}

void SampleComposerContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	ResourceContainerFactory resourceContainerFactory;
	m_pResourceContainer = resourceContainerFactory.getResourceContainer(m_collectionConfig.m_librarySet,
				m_pMatrixFactory,
				m_pMatrixOperator);

	initCoordinateAdditionalBasedGateComposerElements();
}

void SampleComposerContainerImpl::releaseDependencies() {
	_destroy(m_pGateComposer);
	_destroy(m_pGateCoordinateComposer);
	_destroy(m_pGateCoordinateConveter);
	_destroy(m_pGateCoordinateCombiner);
	_destroy(m_pRealCoordinateComparator);

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

void SampleComposerContainerImpl::initCoordinateAdditionalBasedGateComposerElements() {
	MatrixPtrVector pBasis;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis, m_collectionConfig.m_nbQubits);
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

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

	bool phaseIgnored = m_collectionConfig.m_matrixDistanceCalculatorType == MDCT_FOWLER;
	m_pGateCoordinateConveter = ConverterPtr<GatePtr, RealCoordinate<GatePtr> >(new GateCoordinateConverterImpl(m_pGateRealCoordinateCalculator,
			m_pMatrixOperator,
			phaseIgnored));

	m_pGateSetLog = ElementSetLogPtr<GatePtr>(new GateSetLogImpl());
}

void SampleComposerContainerImpl::initRealCoordinateComparator() {
	switch (m_coordinateAdditionalBasedComposerConfig.m_coordinateComparatorType) {
	case CMP_DICTIONARY:
		m_pRealCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new DictionaryOrderCoordinateComparator<GatePtr>());
		break;
	default:
		m_pRealCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new SumCoordinateComparator<GatePtr>());
		break;
	}
}

void SampleComposerContainerImpl::initEpsilonRealCoordinate(RealCoordinate<GatePtr>& epsilonRealCoordinate) {
	int nbCoord = (m_collectionConfig.m_nbQubits == 1) ? 3 : 15;
	std::vector<mreal_t> coords(nbCoord, m_coordinateAdditionalBasedComposerConfig.m_coordinateEpsilon);
	epsilonRealCoordinate = RealCoordinate<GatePtr>(NullPtr, coords);
}
