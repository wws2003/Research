/*
 * SK2ApproximatorContainerImpl.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "SKApproximatorContainerImpl.h"
#include "SampleApproximatorContainerImpl.h"
#include "ComposerBasedGateApproximator.h"
#include "DuplicateGateCancelationCombinerImpl.h"
#include "ContainerResourceFactory.h"
#include "DummyGateDecomposer.h"
#include "ComposerBasedGateApproximator.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "SampleLibraryMatrixStore.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "ContainerResourceFactory.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "HarrowGateDecomposer.h"
#include "AdaptiveGateCoordinateComposer.h"
#include "CoordinateAdditionBasedGateComposer.h"
#include "GateCoordinateConverterImpl.h"
#include "GateCoordinateCombinerImpl.h"
#include "SKGateApproximator.h"
#include "DictionaryOrderCoordinateComparator.hpp"
#include "SumCoordinateComparator.hpp"
#include "SK2ApproximatorContainerImpl.h"

SK2ApproximatorContainerImpl::SK2ApproximatorContainerImpl(SKApproximatorConfig2 config,
		CollectionConfig coreCollectionConfig) : m_approximatorConfig(config), m_coreCollectionConfig(coreCollectionConfig) {
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
			m_pMatrixOperator));
}

void SK2ApproximatorContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	ResourceContainerFactory resourceContainerFactory;
	m_pResourceContainer = resourceContainerFactory.getResourceContainer(m_coreCollectionConfig.m_librarySet,
			m_pMatrixFactory,
			m_pMatrixOperator);

	GateCombinabilityCheckers checkers;
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	MatrixPtrVector pBasis;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis, m_coreCollectionConfig.m_nbQubits);
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(NullPtr));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	m_pHarrowGateDecomposer = GateDecomposerPtr(new HarrowGateDecomposer(m_pGateRealCoordinateCalculator, m_pMatrixOperator));

	m_pGateCoordinateConveter = ConverterPtr<GatePtr, RealCoordinate<GatePtr> >(new GateCoordinateConverterImpl(m_pGateRealCoordinateCalculator));

	initRealCoordinateComparator();

	RealCoordinate<GatePtr> epsilonRealCoordinate;
	initEpsilonRealCoordinate(epsilonRealCoordinate);

	m_pGateCoordinateCombiner = CombinerPtr<RealCoordinate<GatePtr> >(new GateCoordinateCombinerImpl(m_pGateCombiner));

	m_pGateCoordinateComposer = ComposerPtr<RealCoordinate<GatePtr> >(new CoordinateAdditionBasedGateComposer(m_pRealCoordinateComparator,
			m_pGateCoordinateCombiner,
			epsilonRealCoordinate,
			-1));

	m_pGateComposer = GateComposerPtr(new AdaptiveGateCoordinateComposer(m_pGateCoordinateDistanceCalculator,
			m_pGateCoordinateComposer,
			0.0,
			m_pGateCoordinateConveter));
}

void SK2ApproximatorContainerImpl::releaseDependencies() {

	_destroy(m_pGateComposer);
	_destroy(m_pGateCoordinateComposer);
	_destroy(m_pGateCoordinateConveter);
	_destroy(m_pGateCoordinateCombiner);
	_destroy(m_pRealCoordinateComparator);

	_destroy(m_pHarrowGateDecomposer);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pGateCombiner);

	_destroy(m_pResourceContainer);
	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

void SK2ApproximatorContainerImpl::initRealCoordinateComparator() {
	switch (m_approximatorConfig.m_coordinateComparatorType) {
		case 1:
			m_pRealCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new DictionaryOrderCoordinateComparator<GatePtr>());
			break;
		default:
			m_pRealCoordinateComparator = ComparatorPtr<RealCoordinate<GatePtr> >(new SumCoordinateComparator<GatePtr>());
			break;
	}
}

void SK2ApproximatorContainerImpl::initEpsilonRealCoordinate(RealCoordinate<GatePtr>& epsilonRealCoordinate) {
	int nbCoord = (m_coreCollectionConfig.m_nbQubits == 1) ? 3 : 15;
	std::vector<mreal_t> coords(nbCoord, m_approximatorConfig.m_coordinateEpsilon);
	epsilonRealCoordinate = RealCoordinate<GatePtr>(NullPtr, coords);
}
