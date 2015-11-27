/*
 * SampleCollectionContainerImpl.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef SAMPLECOLLECTIONCONTAINERIMPL_H_
#define SAMPLECOLLECTIONCONTAINERIMPL_H_

#include "OperatorCommon.h"
#include "IOCommon.h"
#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "IResourceContainer.h"
#include "AlgoInternal.h"
#include "ILookupResultFilter.h"
#include "ICollectionContainer.h"
#include "ApplicationCommon.h"
#include "Config.h"

class SampleCollectionContainerImpl: public ICollectionContainer {
public:
	SampleCollectionContainerImpl(const CollectionConfig& collectionConfig);
	virtual ~SampleCollectionContainerImpl();

	virtual GateCollectionPtr getGateCollection(GateDistanceCalculatorPtr pGateDistanceCalculator);

	virtual PersitableGateCollectionPtr getPersitableGateCollection();

private:

	void initLibrarySetNameMap();

	void initLibrarySetPersistFileNameMap();

	void wireDependencies();

	std::string getGateCollectionPersistenceFileFullName(const CollectionConfig& config,
			const LibrarySetFileNameMap& librarySetFileNameMap,
			std::string fileExtension);

	void constructGateCollection(GateCollectionPtr pGateCollection, GateDistanceCalculatorPtr pGateDistanceCalculator);

	void releaseDependencies();

	LibrarySetFileNameMap m_librarySetPersistFileNameMap;

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	ResourceContainerPtr m_pResourceContainer;

	GateWriterPtr m_pBinaryGateWriter;
	GateReaderPtr m_pBinaryGateReader;

	MatrixWriterPtr m_pBinaryMatrixWriter;
	MatrixReaderPtr m_pBinaryMatrixReader;
	GateLookupResultFilterPtr m_pGateLookupResultFilter;

	GateCollectionPtr m_pUniversalSet;

	CombinerPtr<GatePtr> m_pGateCombiner;
	GateSearchSpaceConstructorPtr m_pGateSearchSpaceConstructor;

	CollectionConfig m_collectionConfig;

	const static std::string DEFAULT_GATE_COLLECTION_PERSIST_FILE_EXT;

};

#endif /* SAMPLECOLLECTIONCONTAINERIMPL_H_ */
