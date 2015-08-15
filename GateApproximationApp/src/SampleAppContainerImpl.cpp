/*
 * SampleAppContainerImpl.cpp
 *
 *  Created on: Aug 15, 2015
 *      Author: pham
 */

#include "SampleAppContainerImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "PersistableGNATGateCollectionImpl.h"
#include "BinaryGateWriterImpl.h"
#include "BinaryGateReaderImpl.h"
#include "BinaryMatrixWriterImpl.h"
#include "BinaryMatrixReaderImpl.h"
#include "MatrixFowlerDistanceCalculator.h"
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "ICombiner.h"
#include "GateSearchSpaceConstructorImpl.h"
#include "VectorBasedCollectionImpl.hpp"
#include "SampleResourceContainerImpl.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "GateCoordinateCalculatorImpl.h"
#include "SampleRealCoordinateWriterImpl.hpp"
#include "GateCombinerImpl.h"
#include "LabelOnlyGateWriterImpl.h"
#include "CpuTimer.h"
#include "GateSearchSpaceTimerEvaluatorImpl.h"
#include <cstdio>
#include <iostream>
#include <stdexcept>

const int SampleAppContainerImpl::MAX_SEQUENCE_LENGTH = 7;
const std::string SampleAppContainerImpl::GATE_COLLECTION_PERSIST_FILE_NAME = "gnat2";
const std::string SampleAppContainerImpl::GATE_COLLECTION_PERSIST_FILE_EXT = "dat";

std::string getGateCollectionPersistenceFileFullName(std::string fileName,
													int maxSequenceLength,
													std::string fileExtension);

SampleAppContainerImpl::SampleAppContainerImpl() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	m_pResourceContainer = ResourceContainerPtr(new SampleResourceContainerImpl(m_pMatrixOperator, m_pMatrixFactory));

	m_pBinaryMatrixWriter = MatrixWriterPtr(new BinaryMatrixWriterImpl());
	m_pBinaryMatrixReader = MatrixReaderPtr(new BinaryMatrixReaderImpl(m_pMatrixFactory));
	m_pBinaryGateWriter = GateWriterPtr(new BinaryGateWriterImpl(m_pBinaryMatrixWriter));
	m_pBinaryGateReader = GateReaderPtr(new BinaryGateReaderImpl(m_pBinaryMatrixReader));

	m_pUniversalSet = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());
	m_pResourceContainer->getUniversalGates(m_pUniversalSet);

	GateCombinabilityCheckers checkers;
	m_pResourceContainer->getGateCombinabilityCheckers(checkers);
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	m_pGateSearchSpaceConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorImpl(m_pGateCombiner));

	m_pMatrixDistanceCalculator = MatrixDistanceCalculatorPtr(new MatrixFowlerDistanceCalculator(m_pMatrixOperator));
	m_pGateDistanceCalculator = GateDistanceCalculatorPtr(new GateDistanceCalculatorByMatrixImpl(m_pMatrixDistanceCalculator));

	MatrixPtrVector pBasis4;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis4);
	m_pGateWriterInEvaluator = GateWriterPtr(new LabelOnlyGateWriterImpl(","));
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis4));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));
	m_pCoordinateWriter = RealCoordinateWriterPtr<GatePtr>(new SampleRealCoordinateWriterImpl<GatePtr>());

	m_pTimer = TimerPtr(new CpuTimer());
}

SampleAppContainerImpl::~SampleAppContainerImpl() {
	_destroy(m_pTimer);
	_destroy(m_pCoordinateWriter);
	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);
	_destroy(m_pGateWriterInEvaluator);

	_destroy(m_pGateDistanceCalculator);
	_destroy(m_pMatrixDistanceCalculator);
	_destroy(m_pGateSearchSpaceConstructor);

	_destroy(m_pGateCombiner);

	_destroy(m_pUniversalSet);

	_destroy(m_pBinaryGateReader);
	_destroy(m_pBinaryGateWriter);
	_destroy(m_pBinaryMatrixReader);
	_destroy(m_pBinaryMatrixWriter);

	_destroy(m_pResourceContainer);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

GateCollectionPtr SampleAppContainerImpl::getGateCollection() {
	PersistableGNATGateCollectionImpl* pGateCollection = new PersistableGNATGateCollectionImpl(m_pBinaryGateWriter,
			m_pBinaryGateReader);

	std::string persitenceFileName = getGateCollectionPersistenceFileFullName(GATE_COLLECTION_PERSIST_FILE_NAME,
											MAX_SEQUENCE_LENGTH,
											GATE_COLLECTION_PERSIST_FILE_EXT);
	try {
		pGateCollection->load(persitenceFileName);
	}
	catch (std::exception& e) {
		std::cout << e.what() << ". Need to build collection from scratch\n";
		constructGateCollection(pGateCollection);
		pGateCollection->save(persitenceFileName);
	}

	return pGateCollection;
}

GateSearchSpaceEvaluatorPtr SampleAppContainerImpl::getGateSearchSpaceEvaluator() {
	double epsilon = 0.0;
	TargetElements<GatePtr> targets;

	m_pResourceContainer->getTargetGatesAndEpsilon(targets, epsilon);

	GateSearchSpaceEvaluatorPtr pGateSearchSpaceEvaluator = new GateSearchSpaceTimerEvaluatorImpl(targets,
			epsilon,
			m_pGateDistanceCalculator,
			m_pGateRealCoordinateCalculator,
			m_pCoordinateWriter,
			m_pGateWriterInEvaluator,
			m_pTimer,
			std::cout);

	return pGateSearchSpaceEvaluator;
}

void SampleAppContainerImpl::recycle(GateCollectionPtr& rpGateCollection) {
	_destroy(rpGateCollection);
	rpGateCollection = NullPtr;
}

void SampleAppContainerImpl::recycle(GateSearchSpaceEvaluatorPtr& rpGateSearchSpaceEvaluator) {
	_destroy(rpGateSearchSpaceEvaluator);
	rpGateSearchSpaceEvaluator = NullPtr;
}

void SampleAppContainerImpl::constructGateCollection(GateCollectionPtr pGateCollection) {
	m_pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection,
			m_pUniversalSet,
			MAX_SEQUENCE_LENGTH);

	pGateCollection->rebuildStructure(m_pGateDistanceCalculator);
}

std::string getGateCollectionPersistenceFileFullName(std::string fileName,
													int maxSequenceLength,
													std::string fileExtension) {
	char fullName[256];
	sprintf(fullName, "%s_%d.%s", fileName.c_str(), maxSequenceLength, fileExtension.c_str());
	return std::string(fullName);
}

