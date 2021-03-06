/*
 * FowlerSearchSpaceGeneratorTester.cpp
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#include "FowlerSearchSpaceGeneratorTester.h"
#include "GateSearchSpaceConstructorFowlerImpl.h"
#include "GateSearchSpaceConstructorImpl.h"
#include "EvaluateCommon.h"
#include "IIterator.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "SingleQubitGateStoreImpl.h"
#include "GateCombinerImpl.h"
#include "SingleQubitGateCombinabilityCheckerFactoryImpl.h"
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "MatrixFowlerDistanceCalculator.h"
#include "SystemTimer.h"
#include "TwoPivotsGateSetImpl.h"
#include <VectorBasedCollectionImpl.hpp>
#include <iostream>
#include <cassert>

FowlerSearchSpaceConstructorTester::FowlerSearchSpaceConstructorTester() {
	m_pMatrixFactory = (MatrixFactoryPtr)(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = (MatrixOperatorPtr)(new SampleMatrixOperator(m_pMatrixFactory));

	m_pGateStore = (GateStorePtr)(new SingleQubitGateStoreImpl(m_pMatrixOperator, m_pMatrixFactory));

	m_pGateCombinabilityCheckerFactory = (GateCombinabilityCheckerFactoryPtr)(new SingleQubitGateCombinabilityCheckerFactoryImpl());
	GateCombinabilityCheckers combinabilityCheckers;
	m_pGateCombinabilityCheckerFactory->getGateCombinabilityCheckers(L_HTS, combinabilityCheckers);

	m_pGateCombiner = (CombinerPtr<GatePtr>)(new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator));

	m_pMatrixDistanceCalculator = (MatrixDistanceCalculatorPtr)(new MatrixFowlerDistanceCalculator(NullPtr));
	m_pGateDistanceCalculator = (GateDistanceCalculatorPtr)(new GateDistanceCalculatorByMatrixImpl(m_pMatrixDistanceCalculator));
}

FowlerSearchSpaceConstructorTester::~FowlerSearchSpaceConstructorTester() {
	_destroy(m_pGateDistanceCalculator);
	_destroy(m_pMatrixDistanceCalculator);

	_destroy(m_pGateCombiner);
	_destroy(m_pGateCombinabilityCheckerFactory);

	_destroy(m_pGateStore);
	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

void FowlerSearchSpaceConstructorTester::testFowlerSearchSpaceConstructor() {
	std::vector<GatePtr> libraryGates;
	m_pGateStore->getLibraryGates(libraryGates, L_HTS);

	GateCollectionPtr pLibraryCollection = new VectorBasedCollectionImpl<GatePtr>(m_pGateDistanceCalculator);
	for(GatePtr pGate : libraryGates) {
		pLibraryCollection->addElement(pGate);
	}

	int maxLength = 10;

	GateSearchSpaceConstructorPtr pNormalConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorImpl(m_pGateCombiner)) ;
	GateCollectionPtr pNormalCollection = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>(m_pGateDistanceCalculator));
	pNormalConstructor->constructSearchSpace(pNormalCollection, pLibraryCollection, maxLength);
	std::cout << "Normally constructed collection size " << pNormalCollection->size() << "\n";

	GateSetPtr pGateSet = GateSetPtr(new TwoPivotsGateSetImpl(m_pGateDistanceCalculator));
	GateSearchSpaceConstructorPtr pFowlerConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorFowlerImpl(NullPtr,
			0,
			m_pGateCombiner,
			pGateSet)) ;

	GateCollectionPtr pFowlerCollection = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>(m_pGateDistanceCalculator));
	pFowlerConstructor->constructSearchSpace(pFowlerCollection, pLibraryCollection, maxLength);
	std::cout << "Fowler constructed collection size " << pFowlerCollection->size() << "\n";

	assertEquivCollections(pFowlerCollection, pNormalCollection);
	assertUniqueElementsCollections(pFowlerCollection);

	_destroy(pFowlerConstructor);
	_destroy(pGateSet);
	_destroy(pLibraryCollection);
	_destroy(pNormalCollection);
	_destroy(pFowlerCollection);
}

void FowlerSearchSpaceConstructorTester::testFowlerSearchSpaceConstructorFromBaseCollection() {
	std::vector<GatePtr> libraryGates;
	m_pGateStore->getLibraryGates(libraryGates, L_HTS);

	GateCollectionPtr pLibraryCollection = new VectorBasedCollectionImpl<GatePtr>(m_pGateDistanceCalculator);
	for(GatePtr pGate : libraryGates) {
		pLibraryCollection->addElement(pGate);
	}

	int baseLength = 9;
	int maxLength = baseLength + 1;

	SystemTimer systemTimer;
	double t1, t2;

	t1 = systemTimer.getTimeInMs();
	GateSetPtr pGateSet = GateSetPtr(new TwoPivotsGateSetImpl(m_pGateDistanceCalculator));

	GateSearchSpaceConstructorPtr pFowlerMaxLengthConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorFowlerImpl(NullPtr,
			0,
			m_pGateCombiner,
			pGateSet)) ;
	GateCollectionPtr pMaxLengthCollection = createSearchSpace(pFowlerMaxLengthConstructor, pLibraryCollection, maxLength);
	t2 = systemTimer.getTimeInMs();
	std::cout << "Fowler constructed max length collection size "
			<< pMaxLengthCollection->size()
			<< ". Construction time:"
			<< (t2 - t1)
			<< " ms\n";

	_destroy(pMaxLengthCollection);
	_destroy(pFowlerMaxLengthConstructor);

	GateSearchSpaceConstructorPtr pFowlerBaseConstructor = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorFowlerImpl(NullPtr,
			0,
			m_pGateCombiner,
			pGateSet)) ;
	GateCollectionPtr pBaseCollection = createSearchSpace(pFowlerBaseConstructor, pLibraryCollection, baseLength);
	std::cout << "Fowler constructed base collection size " << pBaseCollection->size() << "\n";

	t1 = systemTimer.getTimeInMs();
	GateSearchSpaceConstructorPtr pFowlerMaxLengthConstructorFromBaseCollection = GateSearchSpaceConstructorPtr(new GateSearchSpaceConstructorFowlerImpl(pBaseCollection,
			baseLength,
			m_pGateCombiner,
			pGateSet)) ;
	GateCollectionPtr pMaxLengthCollectionFromBaseCollection = createSearchSpace(pFowlerMaxLengthConstructorFromBaseCollection, pLibraryCollection, maxLength);
	t2 = systemTimer.getTimeInMs();
	std::cout << "Fowler constructed max length collection from base collection size "
			<< pMaxLengthCollectionFromBaseCollection->size()
			<< ". Construction time:"
			<< (t2 - t1)
			<< " ms\n";

	//assertEquivCollections(pMaxLengthCollectionFromBaseCollection, pMaxLengthCollection);

	_destroy(pGateSet);
	_destroy(pMaxLengthCollectionFromBaseCollection);
	_destroy(pBaseCollection);

	_destroy(pFowlerMaxLengthConstructorFromBaseCollection);
	_destroy(pFowlerBaseConstructor);

	_destroy(pLibraryCollection);
}

void FowlerSearchSpaceConstructorTester::assertEquivCollections(CollectionPtr<GatePtr> pCollectionConstructedByFowler,
		CollectionPtr<GatePtr> pSampleCollection) {
	GatePtrVector identityGates;
	m_pGateStore->getIdentityGates(identityGates, false);

	IteratorPtr<GatePtr> pGateIter = pSampleCollection->getReadonlyIteratorPtr();
	while(!pGateIter->isDone()) {
		GatePtr pGate = pGateIter->getObj();

		mreal_t dToIdentity = m_pGateDistanceCalculator->distance(pGate, identityGates[0]);

		mreal_t eps = 1e-7;
		if(dToIdentity > eps) {
			//Check if gate can be equivalently found in the collection constructed by Fowler
			//(only with gates different from I)
			IteratorPtr<LookupResult<GatePtr> > pIter = pCollectionConstructedByFowler->findNearestNeighbours(pGate, eps, true);
			if(pIter->isDone()) {
				std::cout << "Error at " << pGate->getLabelStr() << "\n";
			}
			assert(pIter != NullPtr && !pIter->isDone());
		}

		pGateIter->next();
	}
	_destroy(pGateIter);
}

void FowlerSearchSpaceConstructorTester::assertUniqueElementsCollections(CollectionPtr<GatePtr> pCollectionConstructedByFowler) {
	IteratorPtr<GatePtr> pGateIter = pCollectionConstructedByFowler->getReadonlyIteratorPtr();
	while(!pGateIter->isDone()) {
		GatePtr pGate = pGateIter->getObj();

		//Check if gate can be equivalently found in the collection constructed by Fowler
		mreal_t eps = 1e-7;
		IteratorPtr<LookupResult<GatePtr> > pIter = pCollectionConstructedByFowler->findNearestNeighbours(pGate, eps, true);
		assert(pIter != NullPtr && !pIter->isDone());
		pIter->next();
		assert(pIter == NullPtr || pIter->isDone());

		pGateIter->next();
	}
	_destroy(pGateIter);
}

GateCollectionPtr FowlerSearchSpaceConstructorTester::createSearchSpace(GateSearchSpaceConstructorPtr pSearchSpaceConstructor,
		GateCollectionPtr pLibraryCollection,
		int maxLength) {

	GateCollectionPtr pCollection = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>(m_pGateDistanceCalculator));
	pSearchSpaceConstructor->constructSearchSpace(pCollection, pLibraryCollection, maxLength);
	return pCollection;
}
