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
#include "NearIdentityGateApproximator.h"
#include "MapBasedGateBinCollectionImpl.h"
#include <cstdio>
#include <iostream>
#include <stdexcept>

const int SampleAppContainerImpl::DEFAULT_MAX_SEQUENCE_LENGTH = 14;
const int SampleAppContainerImpl::DEFAULT_NB_QUBITS = 1;
const double SampleAppContainerImpl::DEFAULT_COLLECTION_EPSILON = 1e-2;
const double SampleAppContainerImpl::DEFAULT_APPROXIMATOR_EPSILON = 1e-2;
const NearIdentityElementApproximator<GatePtr>::Config SampleAppContainerImpl::DEFAULT_NEAR_IDENTITY_APPROXIMATOR_CONFIG = {2e-1, 3, 1.5e-1, 7};

const std::string SampleAppContainerImpl::GATE_COLLECTION_PERSIST_FILE_NAME = "gnat";
const std::string SampleAppContainerImpl::GATE_COLLECTION_PERSIST_FILE_EXT = "dat";

std::string getGateCollectionPersistenceFileFullName(std::string fileName,
		int maxSequenceLength,
		int nbQubits,
		std::string fileExtension);

SampleAppContainerImpl::SampleAppContainerImpl(std::string evaluatorConfigFile, std::string approximatorConfigFile) {
	readEvaluatorConfigFromFile(evaluatorConfigFile);
	readApproximatorConfigFromFile(approximatorConfigFile);
	wireDependencies();
}

SampleAppContainerImpl::~SampleAppContainerImpl() {
	releaseDependencies();
}

GateCollectionPtr SampleAppContainerImpl::getGateCollection() {
	PersistableGNATGateCollectionImpl* pGateCollection = new PersistableGNATGateCollectionImpl(m_pBinaryGateWriter,
			m_pBinaryGateReader);

	std::string persitenceFileName = getGateCollectionPersistenceFileFullName(GATE_COLLECTION_PERSIST_FILE_NAME,
			m_maxSequenceLength,
			m_nbQubits,
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

GateApproximatorPtr SampleAppContainerImpl::getGateApproximator() {
	GateApproximatorPtr pGateApproximator = GateApproximatorPtr(new NearIdentityGateApproximator(m_pGateRealCoordinateCalculator,
			m_pGateCombiner,
			m_pBinCollection,
			m_nearIdentityApproximatorConfig));

	return pGateApproximator;
}

GateSearchSpaceEvaluatorPtr SampleAppContainerImpl::getGateSearchSpaceEvaluator() {
	TargetElements<GatePtr> targets;

	m_pResourceContainer->getTargetGatesAndEpsilon(targets, m_nbQubits);

	GateSearchSpaceEvaluatorPtr pGateSearchSpaceEvaluator = new GateSearchSpaceTimerEvaluatorImpl(targets,
			m_collectionEpsilon,
			m_approximatorEpsilon,
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

void SampleAppContainerImpl::recycle(GateApproximatorPtr& rpGateApproximator) {
	_destroy(rpGateApproximator);
	rpGateApproximator = NullPtr;
}

void SampleAppContainerImpl::recycle(GateSearchSpaceEvaluatorPtr& rpGateSearchSpaceEvaluator) {
	_destroy(rpGateSearchSpaceEvaluator);
	rpGateSearchSpaceEvaluator = NullPtr;
}

void SampleAppContainerImpl::readEvaluatorConfigFromFile(std::string configFile) {
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		inputStream >> m_maxSequenceLength;
		inputStream >> m_nbQubits;
		inputStream >> m_collectionEpsilon;
		inputStream >> m_approximatorEpsilon;
	}
	else {
		std::cout << "Can not open config file, use default params" << "\r\n";
		m_maxSequenceLength = DEFAULT_MAX_SEQUENCE_LENGTH;
		m_nbQubits = DEFAULT_NB_QUBITS;
		m_collectionEpsilon = DEFAULT_COLLECTION_EPSILON;
		m_approximatorEpsilon = DEFAULT_APPROXIMATOR_EPSILON;
	}
}

void SampleAppContainerImpl::readApproximatorConfigFromFile(std::string configFile) {
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		inputStream >> m_nearIdentityApproximatorConfig.m_initialEpsilon;
		inputStream >> m_nearIdentityApproximatorConfig.m_maxMergedBinDistance;
		inputStream >> m_nearIdentityApproximatorConfig.m_maxCandidateEpsilon;
		inputStream >> m_nearIdentityApproximatorConfig.m_iterationMaxSteps;
	}
	else {
		std::cout << "Can not open config file, use default params" << "\r\n";
		m_nearIdentityApproximatorConfig = DEFAULT_NEAR_IDENTITY_APPROXIMATOR_CONFIG;
	}
}


void SampleAppContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	m_pResourceContainer = ResourceContainerPtr(new SampleResourceContainerImpl(m_pMatrixOperator, m_pMatrixFactory));

	m_pBinaryMatrixWriter = MatrixWriterPtr(new BinaryMatrixWriterImpl());
	m_pBinaryMatrixReader = MatrixReaderPtr(new BinaryMatrixReaderImpl(m_pMatrixFactory));
	m_pBinaryGateWriter = GateWriterPtr(new BinaryGateWriterImpl(m_pBinaryMatrixWriter));
	m_pBinaryGateReader = GateReaderPtr(new BinaryGateReaderImpl(m_pBinaryMatrixReader));

	m_pUniversalSet = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());
	m_pResourceContainer->getUniversalGates(m_pUniversalSet, m_nbQubits);

	GateCombinabilityCheckers checkers;
	m_pResourceContainer->getGateCombinabilityCheckers(checkers, m_nbQubits);
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	m_pGateSearchSpaceConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorImpl(m_pGateCombiner));

	m_pMatrixDistanceCalculator = MatrixDistanceCalculatorPtr(new MatrixFowlerDistanceCalculator(m_pMatrixOperator));
	m_pGateDistanceCalculator = GateDistanceCalculatorPtr(new GateDistanceCalculatorByMatrixImpl(m_pMatrixDistanceCalculator));

	MatrixPtrVector pBasis;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis, m_nbQubits);
	m_pGateWriterInEvaluator = GateWriterPtr(new LabelOnlyGateWriterImpl(","));
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));
	m_pCoordinateWriter = RealCoordinateWriterPtr<GatePtr>(new SampleRealCoordinateWriterImpl<GatePtr>(","));

	m_pTimer = TimerPtr(new CpuTimer());

	GateCombinabilityCheckers emptyCheckers;
	m_pGateInBinCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(emptyCheckers, m_pMatrixOperator));
	m_pBinCollection = BinCollectionPtr<GatePtr>(new MapBasedGateBinCollectionImpl());

}

void SampleAppContainerImpl::releaseDependencies() {
	_destroy(m_pBinCollection);
	_destroy(m_pGateInBinCombiner);

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

void SampleAppContainerImpl::constructGateCollection(GateCollectionPtr pGateCollection) {
	m_pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection,
			m_pUniversalSet,
			m_maxSequenceLength);

	pGateCollection->rebuildStructure(m_pGateDistanceCalculator);
}

std::string getGateCollectionPersistenceFileFullName(std::string fileName,
		int maxSequenceLength,
		int nbQubits,
		std::string fileExtension) {
	char fullName[256];
	sprintf(fullName, "%s_%d_%d.%s",
			fileName.c_str(),
			nbQubits,
			maxSequenceLength,
			fileExtension.c_str());
	return std::string(fullName);
}

