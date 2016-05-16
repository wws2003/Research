/*
 * SampleCollectionContainerImpl.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "SampleCollectionContainerImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "PersistableGNATGateCollectionImpl.h"
#include "SQLiteGateWriterImpl.h"
#include "BinaryGateReaderImpl.h"
#include "BinaryMatrixWriterImpl.h"
#include "BinaryMatrixReaderImpl.h"
#include "ICombiner.h"
#include "GateSearchSpaceConstructorImpl.h"
#include "VectorBasedCollectionImpl.hpp"
#include "GateCombinerImpl.h"
#include "LabelOnlyGateWriterImpl.h"
#include "PersistableGNATCollectionImpl.h"
#include "IPersistableCollection.h"
#include "DuplicateGateLookupResultFilterImpl.h"
#include "BackgroundGateLookupResultsFilterProcessor.h"
#include "SetBasedGateLookupResultProcessor.h"
#include "VectorBasedGateLookupResultProcessor.h"
#include "MatrixFowlerDistanceCalculator.h"
#include "LazyGateDistanceCalculatorImpl.h"
#include "SQLiteLazyGateDistanceCalculator.h"
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "SampleGateStoreFactoryImpl.h"
#include "FilePathConfig.h"
#include "SingleQubitGateCombinabilityCheckerFactoryImpl.h"
#include "TwoQubitsGateCombinabilityCheckerFactoryImpl.h"
#include "GateSearchSpaceConstructorFowlerImpl.h"
#include <stdexcept>

SampleCollectionContainerImpl::SampleCollectionContainerImpl(const CollectionConfig& collectionConfig) : m_collectionConfig(collectionConfig) {
	wireDependencies();
}

SampleCollectionContainerImpl::~SampleCollectionContainerImpl() {
	releaseDependencies();
}

GateCollectionPtr SampleCollectionContainerImpl::getGateCollection(GateDistanceCalculatorPtr pGateDistanceCalculator) {
	PersitableGateCollectionPtr pGateCollection = PersitableGateCollectionPtr(new PersistableGNATGateCollectionImpl(pGateDistanceCalculator,
			m_pBinaryGateWriter,
			m_pBinaryGateReader,
			m_pGateLookupResultProcessor));

	FilePathConfig pathConfig;
	std::string persitenceFileName = pathConfig.getGateCollectionPersistenceFilePath(m_collectionConfig);
	try {
		pGateCollection->load(persitenceFileName);
		std::cout << "Collection loaded from storage. Distance calculator is ignored !\n";
	}
	catch (std::exception& e) {
		std::cout << e.what() << ". Need to build collection from scratch\n";
		constructGateCollection(pGateCollection, pGateDistanceCalculator);
		pGateCollection->save(persitenceFileName);
	}

	return pGateCollection;
}

PersitableGateCollectionPtr SampleCollectionContainerImpl::getPersitableGateCollection() {
	return PersitableGateCollectionPtr(new PersistableGNATGateCollectionImpl(m_pGateDistanceCalculatorForCollection,
			m_pBinaryGateWriter,
			m_pBinaryGateReader,
			m_pGateLookupResultProcessor));
}

void SampleCollectionContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	SampleGateStoreFactoryImpl gateStoreFactory(m_pMatrixOperator, m_pMatrixFactory);
	m_pGateStore = gateStoreFactory.getGateStore(m_collectionConfig.m_nbQubits);

	m_pBinaryMatrixWriter = MatrixWriterPtr(new BinaryMatrixWriterImpl());
	m_pBinaryMatrixReader = MatrixReaderPtr(new BinaryMatrixReaderImpl(m_pMatrixFactory));

	FilePathConfig pathConfig;
	std::string matrixDBName = pathConfig.getMatrixDBFilePath(m_collectionConfig);
	m_pBinaryGateWriter = GateWriterPtr(new SQLiteGateWriterImpl(NullPtr, matrixDBName));
	m_pBinaryGateReader = GateReaderPtr(new BinaryGateReaderImpl(NullPtr));

	m_pMatrixDistanceCalculator = MatrixDistanceCalculatorPtr(new MatrixFowlerDistanceCalculator(NullPtr));
	m_pGateDistanceCalculator = GateDistanceCalculatorPtr(new GateDistanceCalculatorByMatrixImpl(m_pMatrixDistanceCalculator));

	m_pGateDistanceCalculatorForCollection = GateDistanceCalculatorPtr(new SQLiteLazyGateDistanceCalculator(m_pMatrixDistanceCalculator,
			m_pMatrixFactory,
			matrixDBName,
			m_collectionConfig.m_nbQubits == 1 ? 2 : 4));

	//m_pGateLookupResultProcessor = GateLookupResultProcessorPtr(new BackgroundGateLookupResultsFilterProcessor(m_pGateDistanceCalculator));
	m_pGateLookupResultProcessor = GateLookupResultProcessorPtr(new SetBasedGateLookupResultProcessor(m_pGateDistanceCalculator, true));
	m_pGateLookupResultProcessor->init();

	std::vector<GatePtr> universalSet;
	m_pGateStore->getLibraryGates(universalSet, m_collectionConfig.m_librarySet);
	m_pUniversalSet = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>(m_pGateDistanceCalculator));
	for(unsigned int i = 0; i < universalSet.size(); i++) {
		m_pUniversalSet->addElement(universalSet[i]->clone());
	}

	GateCombinabilityCheckers checkers;
	constructGateCombinabilityCheckerFactory();
	m_pCheckerFactory->getGateCombinabilityCheckers(m_collectionConfig.m_librarySet, checkers);

	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	m_pGateSearchSpaceConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorFowlerImpl(m_pGateCombiner, m_pGateDistanceCalculator));
}

void SampleCollectionContainerImpl::constructGateCombinabilityCheckerFactory() {
	switch (m_collectionConfig.m_nbQubits) {
	case 1:
		m_pCheckerFactory = GateCombinabilityCheckerFactoryPtr(new SingleQubitGateCombinabilityCheckerFactoryImpl());
		break;
	case 2:
		m_pCheckerFactory = GateCombinabilityCheckerFactoryPtr(new TwoQubitsGateCombinabilityCheckerFactoryImpl());
		break;
	default:
		throw std::logic_error("Not available number of qubits");
		break;
	}
}
void SampleCollectionContainerImpl::releaseDependencies() {
	_destroy(m_pGateSearchSpaceConstructor);

	_destroy(m_pGateCombiner);

	_destroy(m_pUniversalSet);

	_destroy(m_pGateDistanceCalculatorForCollection);

	_destroy(m_pGateLookupResultProcessor);
	_destroy(m_pGateDistanceCalculator);
	_destroy(m_pMatrixDistanceCalculator);

	_destroy(m_pBinaryGateReader);
	_destroy(m_pBinaryGateWriter);
	_destroy(m_pBinaryMatrixReader);
	_destroy(m_pBinaryMatrixWriter);

	_destroy(m_pCheckerFactory);
	_destroy(m_pGateStore);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

void SampleCollectionContainerImpl::constructGateCollection(GateCollectionPtr pGateCollection,
		GateDistanceCalculatorPtr pGateDistanceCalculator) {
	m_pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection,
			m_pUniversalSet,
			m_collectionConfig.m_maxSequenceLength);

	std::cout << "Begin to build data structure for collection of size: " << pGateCollection->size() << std::endl;
	pGateCollection->rebuildStructure();
}
