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
#include "TwoPivotsGateSetImpl.h"
#include "SimpleGateKDTree.h"
#include "TreeBasedGateRealSet.h"
#include "TreeBasedGateDoubleSet.h"
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

	setupGateSet();

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

	m_pBaseCollection = NullPtr;
	m_baseSequenceLength = 0;
	m_pGateSearchSpaceConstructor = NullPtr;
}

void SampleCollectionContainerImpl::setupGateSet() {

	std::vector<GatePtr> pivots;

	MatrixPtrVector pBases;
	m_pGateStore->getMatrixOrthonormalBasis(pBases);
	for(MatrixPtr pBasis : pBases) {
		MatrixPtr pBasisMI = NullPtr;
		//pBasisMI = pBasis * -i
		m_pMatrixOperator->multiplyScalar(pBasis, ComplexVal(0, -1), pBasisMI);

		//pExpBasisMI = expm(pBasisMI)
		MatrixPtr pExpBasisMI = NullPtr;
		m_pMatrixOperator->exponential(pBasisMI, pExpBasisMI);

		_destroy(pBasisMI);

		GatePtr pPivot = (GatePtr)(new Gate(pExpBasisMI, 1, "#"));
		pivots.push_back(pPivot);
	}

	//Add Identity too
	std::vector<GatePtr> identityGates;
	m_pGateStore->getIdentityGates(identityGates, false);
	pivots.push_back(identityGates[0]);

	int nbPivots = pivots.size();

#ifdef USE_APPRX_DOUBLE
	double distanceToConsiderAsOne = 1e-7;
	std::vector<double> coordinateRanges(nbPivots, 3e-7);
	m_pGateTree = (RangeSearchTreePtr<GatePtr, double>)(new SimpleDoubleGateKDTree(nbPivots, 0.0));
	m_pGateTree->init();

	m_pGateSet = (GateSetPtr)(new TreeBasedGateDoubleSet(pivots,
			m_pGateDistanceCalculator,
			(mreal_t)distanceToConsiderAsOne,
			m_pGateTree,
			coordinateRanges));
#else
	mreal_t distanceToConsiderAsOne = 1e-7;
	std::vector<mreal_t> coordinateRanges(nbPivots, 3e-7);
	m_pGateTree = (RangeSearchTreePtr<GatePtr, mreal_t>)(new SimpleRealGateKDTree(nbPivots, 0.0));
	m_pGateTree->init();

	m_pGateSet = (GateSetPtr)(new TreeBasedGateRealSet(pivots,
			m_pGateDistanceCalculator,
			distanceToConsiderAsOne,
			m_pGateTree,
			coordinateRanges));
#endif
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
	_destroy(m_pBaseCollection);

	_destroy(m_pGateCombiner);

	m_pUniversalSet->purge();
	_destroy(m_pUniversalSet);

	_destroy(m_pGateDistanceCalculatorForCollection);

	_destroy(m_pGateLookupResultProcessor);

	_destroy(m_pGateTree);
	_destroy(m_pGateSet);

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

	probeBaseCollection(pGateDistanceCalculator);

	m_pGateSearchSpaceConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorFowlerImpl(m_pBaseCollection,
			m_baseSequenceLength,
			m_pGateCombiner,
			m_pGateSet));

	m_pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection,
			m_pUniversalSet,
			m_collectionConfig.m_maxSequenceLength);

	std::cout << "Begin to build data structure for collection of size: " << pGateCollection->size() << std::endl;
	pGateCollection->rebuildStructure();
}

void SampleCollectionContainerImpl::probeBaseCollection(GateDistanceCalculatorPtr pGateDistanceCalculator) {

	PersitableGateCollectionPtr pBaseCollection = PersitableGateCollectionPtr(new PersistableGNATGateCollectionImpl(pGateDistanceCalculator,
			m_pBinaryGateWriter,
			m_pBinaryGateReader,
			m_pGateLookupResultProcessor));

	int maxSequenceLength = m_collectionConfig.m_maxSequenceLength - 1;
	FilePathConfig pathConfig;

	while(maxSequenceLength > 0) {
		CollectionConfig baseCollectionConfig = m_collectionConfig;
		baseCollectionConfig.m_maxSequenceLength = maxSequenceLength;

		std::string persitenceFileName = pathConfig.getGateCollectionPersistenceFilePath(baseCollectionConfig);
		try {
			pBaseCollection->load(persitenceFileName);
			fillBaseCollection(pBaseCollection);
			std::cout << "Base collection loaded from storage"
					<< " size = "
					<< pBaseCollection->size()
					<< ", sequence max length = "
					<< maxSequenceLength
					<< ". Finished probing\n";
			break;
		}
		catch (std::exception& e) {
			std::cout << e.what() << ". Couldn't find saved collection with length "
					<<  maxSequenceLength
					<< ". Need to probe to length "
					<< maxSequenceLength - 1
					<<  "\n";
			maxSequenceLength--;
			continue;
		}
	}

	if(maxSequenceLength > 0) {
		m_pBaseCollection = pBaseCollection;
	}
	else {
		_destroy(pBaseCollection);
	}
	m_baseSequenceLength = maxSequenceLength;
}

void SampleCollectionContainerImpl::fillBaseCollection(GateCollectionPtr pGateCollection) {
	/*Just a manipulation to calculate matrices in base collection
	(Remember loaded persistable collection only stores null matrtrices
	until distance calculator is called)
	 */
	std::vector<GatePtr> identityGates;
	m_pGateStore->getIdentityGates(identityGates, false);

	GateIteratorPtr pGateIter = pGateCollection->getIteratorPtr();
	while(!pGateIter->isDone()) {
		m_pGateDistanceCalculatorForCollection->distance(pGateIter->getObj(), identityGates[0]);
		pGateIter->next();
	}
}

