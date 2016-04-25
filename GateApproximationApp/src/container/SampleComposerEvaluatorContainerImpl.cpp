/*
 * SampleComposerEvaluatorContainerImpl.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#include "SampleComposerEvaluatorContainerImpl.h"
#include "GateComposerEvaluatorImpl.h"
#include "SampleEvaluatorContainerImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "BinaryGateWriterImpl.h"
#include "BinaryGateReaderImpl.h"
#include "BinaryMatrixWriterImpl.h"
#include "BinaryMatrixReaderImpl.h"
#include "MatrixFowlerDistanceCalculator.h"
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "GateSearchSpaceConstructorImpl.h"
#include "VectorBasedCollectionImpl.hpp"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "GateCoordinateCalculatorImpl.h"
#include "SampleRealCoordinateWriterImpl.hpp"
#include "GateCombinerImpl.h"
#include "LabelOnlyGateWriterImpl.h"
#include "CpuTimer.h"
#include "SystemTimer.h"
#include "GateSearchSpaceTimerEvaluatorImpl.h"
#include "MapBasedGateBinCollectionImpl.h"
#include "SQLiteLazyGateDistanceCalculator.h"
#include "SampleCollectionContainerImpl.h"
#include "VectorBasedMatrixCollectionImpl.h"
#include "MultiNearBalancePartsGateDecomposer.h"
#include "HarrowGateDecomposer.h"
#include "FilePathConfig.h"
#include "SampleGateStoreFactoryImpl.h"
#include <stdexcept>
#include <iostream>

SampleComposerEvaluatorContainerImpl::SampleComposerEvaluatorContainerImpl(const ComposerEvaluatorConfig& composerEvaluatorConfig,
		const CollectionConfig& collectionConfig) : m_composerEvaluatorConfig(composerEvaluatorConfig), m_collectionConfig(collectionConfig) {
	wireDependencies();
}

SampleComposerEvaluatorContainerImpl::~SampleComposerEvaluatorContainerImpl() {
	releaseDependencies();
}

GateComposerEvaluatorPtr SampleComposerEvaluatorContainerImpl::getGateComposerEvaluator() {
	return GateComposerEvaluatorPtr(new GateComposerEvaluatorImpl(m_targetGates,
			m_composerEvaluatorConfig.m_nbPartialQueries,
			m_composerEvaluatorConfig.m_nbCandidates,
			m_pCoreCollection,
			m_pHelperCollection,
			m_pGateDecomposer,
			m_composerEvaluatorConfig.m_composeEpsilon,
			m_composerEvaluatorConfig.m_initialEpsilon,
			m_pTimer,
			m_pGateRealCoordinateCalculator,
			m_pCoordinateWriter,
			m_pGateWriterInEvaluator,
			std::cout));
}

void SampleComposerEvaluatorContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	SampleGateStoreFactoryImpl gateStoreFactory(m_pMatrixOperator, m_pMatrixFactory);
	m_pGateStore = gateStoreFactory.getGateStore(m_collectionConfig.m_nbQubits);

	//Setup targets
	getTargetsForEvaluation(m_targetGates);

	//Setup distance calculators
	FilePathConfig pathConfig;
	m_pMatrixDistanceCalculator = MatrixDistanceCalculatorPtr(new MatrixFowlerDistanceCalculator(NullPtr));
	m_pGateDistanceCalculator = GateDistanceCalculatorPtr(new SQLiteLazyGateDistanceCalculator(m_pMatrixDistanceCalculator,
			m_pMatrixFactory,
			pathConfig.getMatrixDBFilePath(m_collectionConfig),
			m_collectionConfig.m_nbQubits == 1 ? 2 : 4));

	//Setup collections
	m_pCollectionContainer = CollectionContainerPtr(new SampleCollectionContainerImpl(m_collectionConfig));
	m_pCoreCollection = m_pCollectionContainer->getGateCollection(m_pGateDistanceCalculator);
	m_pHelperCollection = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>(NullPtr));

	//Setup decomposers
	MatrixPtrVector pBasis;
	m_pGateStore->getMatrixOrthonormalBasis(pBasis);

	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));
	m_pGateDecomposer = generateDecomposerFromConfig(m_composerEvaluatorConfig);

	//For evaluator itself
	m_pGateWriterInEvaluator = GateWriterPtr(new LabelOnlyGateWriterImpl(","));
	m_pCoordinateWriter = RealCoordinateWriterPtr<GatePtr>(new SampleRealCoordinateWriterImpl<GatePtr>(20, ","));
	m_pTimer = TimerPtr(new SystemTimer());
}

void SampleComposerEvaluatorContainerImpl::releaseDependencies() {
	_destroy(m_pTimer);
	_destroy(m_pCoordinateWriter);
	_destroy(m_pGateWriterInEvaluator);

	_destroy(m_pGateDecomposer);
	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pHelperCollection);
	_destroy(m_pCoreCollection);
	_destroy(m_pCollectionContainer);

	_destroy(m_pGateDistanceCalculator);
	_destroy(m_pMatrixDistanceCalculator);

	//Release generate targets
	std::for_each(m_targetGates.begin(), m_targetGates.end(), [](GatePtr pGate){_destroy(pGate);});

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

void SampleComposerEvaluatorContainerImpl::getTargetsForEvaluation(std::vector<GatePtr>& targets) {
	targets.clear();
	m_pGateStore->getRotationTargets(targets, m_composerEvaluatorConfig.m_rotationTargets);
}

GateDecomposerPtr SampleComposerEvaluatorContainerImpl::generateDecomposerFromConfig(ComposerEvaluatorConfig evalConfig) {
	GateDecomposerPtr pGateDecomposer = NullPtr;
	switch (evalConfig.m_queryDecomposerType) {
	case DT_NEAR_BALANCE:
		pGateDecomposer = GateDecomposerPtr(new MultiNearBalancePartsGateDecomposer(m_pGateRealCoordinateCalculator,
				m_pMatrixOperator));
		break;
	case DT_GC_SIMPLE:
		pGateDecomposer = GateDecomposerPtr(new HarrowGateDecomposer(m_pGateRealCoordinateCalculator,
				m_pMatrixOperator));
		break;
	case DT_GC_MULTI_LEVELS:
		//TODO Implement
		break;
	default:
		break;
	}

	return pGateDecomposer;
}

