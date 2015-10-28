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
#include "BinaryGateWriterImpl.h"
#include "BinaryGateReaderImpl.h"
#include "BinaryMatrixWriterImpl.h"
#include "BinaryMatrixReaderImpl.h"
#include "ICombiner.h"
#include "GateSearchSpaceConstructorImpl.h"
#include "VectorBasedCollectionImpl.hpp"
#include "SampleResourceContainerImpl.h"
#include "GateCombinerImpl.h"
#include "LabelOnlyGateWriterImpl.h"
#include "DuplicateGateCancelationCombinerImpl.h"
#include "ContainerResourceFactory.h"
#include "SampleLibraryMatrixStore.h"
#include "PersistableGNATCollectionImpl.h"
#include "IPersistableCollection.h"
#include <stdexcept>

const std::string SampleCollectionContainerImpl::DEFAULT_GATE_COLLECTION_PERSIST_FILE_EXT = "dat";

SampleCollectionContainerImpl::SampleCollectionContainerImpl(const CollectionConfig& collectionConfig) : m_collectionConfig(collectionConfig) {
	initLibrarySetPersistFileNameMap();
	wireDependencies();
}

SampleCollectionContainerImpl::~SampleCollectionContainerImpl() {
	releaseDependencies();
}

GateCollectionPtr SampleCollectionContainerImpl::getGateCollection(GateDistanceCalculatorPtr pGateDistanceCalculator) {
	PersitableGateCollectionPtr pGateCollection = PersitableGateCollectionPtr(new PersistableGNATGateCollectionImpl(m_pBinaryGateWriter,
			m_pBinaryGateReader));

	std::string persitenceFileName = getGateCollectionPersistenceFileFullName(m_collectionConfig,
			m_librarySetPersistFileNameMap,
			DEFAULT_GATE_COLLECTION_PERSIST_FILE_EXT);
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
	return PersitableGateCollectionPtr(new PersistableGNATGateCollectionImpl(m_pBinaryGateWriter,
			m_pBinaryGateReader));
}

void SampleCollectionContainerImpl::initLibrarySetPersistFileNameMap() {
	m_librarySetPersistFileNameMap[L_HT] = "gnat";
	m_librarySetPersistFileNameMap[L_HTCNOT] = "gnat";
	m_librarySetPersistFileNameMap[L_HCV] = "gnat_hv";
	m_librarySetPersistFileNameMap[L_HTCV] = "gnat_htcv";
}

void SampleCollectionContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	ResourceContainerFactory resourceContainerFactory;
	m_pResourceContainer = resourceContainerFactory.getResourceContainer(m_collectionConfig.m_librarySet,
			m_pMatrixFactory,
			m_pMatrixOperator);

	m_pBinaryMatrixWriter = MatrixWriterPtr(new BinaryMatrixWriterImpl());
	m_pBinaryMatrixReader = MatrixReaderPtr(new BinaryMatrixReaderImpl(m_pMatrixFactory));
	m_pBinaryGateWriter = GateWriterPtr(new BinaryGateWriterImpl(NullPtr));
	m_pBinaryGateReader = GateReaderPtr(new BinaryGateReaderImpl(NullPtr));

	m_pUniversalSet = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());
	m_pResourceContainer->getUniversalGates(m_pUniversalSet, m_collectionConfig.m_nbQubits);

	GateCombinabilityCheckers checkers;
	m_pResourceContainer->getGateCombinabilityCheckers(checkers, m_collectionConfig.m_nbQubits);
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	m_pGateSearchSpaceConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorImpl(m_pGateCombiner));
}

void SampleCollectionContainerImpl::releaseDependencies() {
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

std::string SampleCollectionContainerImpl::getGateCollectionPersistenceFileFullName(const CollectionConfig& config,
		const LibrarySetFileNameMap& librarySetFileNameMap,
		std::string fileExtension) {

	int nbQubits = config.m_nbQubits;
	int maxSequenceLength = config.m_maxSequenceLength;
	std::string fileName = librarySetFileNameMap.find(config.m_librarySet)->second;

#if MPFR_REAL
	std::string precisionPostFix = "_mpfr";
#else
	std::string precisionPostFix = "";
#endif

	char fullName[256];
	sprintf(fullName, "%s%s_%d_%d.%s",
			fileName.c_str(),
			precisionPostFix.c_str(),
			nbQubits,
			maxSequenceLength,
			fileExtension.c_str());

	return std::string(fullName);
}

void SampleCollectionContainerImpl::constructGateCollection(GateCollectionPtr pGateCollection,
		GateDistanceCalculatorPtr pGateDistanceCalculator) {
	m_pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection,
			m_pUniversalSet,
			m_collectionConfig.m_maxSequenceLength);

	pGateCollection->rebuildStructure(pGateDistanceCalculator);
}

