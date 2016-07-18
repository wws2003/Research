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
#include "AlgoInternal.h"
#include "ILookupResultFilter.h"
#include "ICollectionContainer.h"
#include "ApplicationCommon.h"
#include "IGateStore.h"
#include "Config.h"
#include "EvaluateCommon.h"
#include "ISearchSpaceConstructor.h"
#include "IRangeSearchTree.h"
#include "ISet.h"

#define USE_APPRX_DOUBLE (1)

class SampleCollectionContainerImpl: public ICollectionContainer {
public:
	SampleCollectionContainerImpl(const CollectionConfig& collectionConfig);
	virtual ~SampleCollectionContainerImpl();

	//Implement
	virtual GateCollectionPtr getGateCollection(GateDistanceCalculatorPtr pGateDistanceCalculator);

private:
	void wireDependencies();

	void setupGateSet();

	void constructGateCombinabilityCheckerFactory();

	void constructGateCollection(GateCollectionPtr pGateCollection, GateDistanceCalculatorPtr pGateDistanceCalculator);

	void probeBaseCollection(GateDistanceCalculatorPtr pGateDistanceCalculator);

	void fillBaseCollection(GateCollectionPtr pGateCollection);

	void releaseDependencies();

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	GateStorePtr m_pGateStore;
	GateCombinabilityCheckerFactoryPtr m_pCheckerFactory;

	GateWriterPtr m_pBinaryGateWriter;
	GateReaderPtr m_pBinaryGateReader;

	MatrixWriterPtr m_pBinaryMatrixWriter;
	MatrixReaderPtr m_pBinaryMatrixReader;

	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	GateDistanceCalculatorPtr m_pGateDistanceCalculator;

#ifdef USE_APPRX_DOUBLE
	RangeSearchTreePtr<GatePtr, double> m_pGateTree;
#else
	RangeSearchTreePtr<GatePtr, mreal_t> m_pGateTree;
#endif
	GateSetPtr m_pGateSet;

	GateDistanceCalculatorPtr m_pGateDistanceCalculatorForCollection;

	GateLookupResultProcessorPtr m_pGateLookupResultProcessor;

	GateCollectionPtr m_pUniversalSet;

	CombinerPtr<GatePtr> m_pGateCombiner;
	CollectionPtr<GatePtr> m_pBaseCollection;
	int m_baseSequenceLength;
	GateSearchSpaceConstructorPtr m_pGateSearchSpaceConstructor;

	CollectionConfig m_collectionConfig;
};

#endif /* SAMPLECOLLECTIONCONTAINERIMPL_H_ */
