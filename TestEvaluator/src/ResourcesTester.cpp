/*
 * ResourcesTester.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: pham
 */

#include "ResourcesTester.h"
#include "SingleQubitGateCombinabilityCheckerFactoryImpl.h"
#include "SingleQubitGateStoreImpl.h"
#include "SampleMatrixOperator.h"
#include "GateSearchSpaceConstructorImpl.h"
#include "EvaluateCommon.h"
#include "ICombiner.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "GateCombinerImpl.h"
#include "VectorBasedCollectionImpl.hpp"
#include "IIterator.h"
#include <iostream>
#include <cassert>
#include <set>

ResourcesTester::ResourcesTester() {
	m_pMatrixFactory = new SimpleDenseMatrixFactoryImpl();
	m_pMatrixOperator = new SampleMatrixOperator(m_pMatrixFactory);

	m_pGateStore = new SingleQubitGateStoreImpl(m_pMatrixOperator, m_pMatrixFactory);
	m_pGateCombinabilityCheckerFactory = new SingleQubitGateCombinabilityCheckerFactoryImpl();
}

ResourcesTester::~ResourcesTester() {
	delete m_pGateCombinabilityCheckerFactory;
	delete m_pGateStore;
	delete m_pMatrixOperator;
	delete m_pMatrixFactory;
}

void ResourcesTester::testSimpleHTCombinationChecker() {
	//H, T
	GatePtrVector libraryGates;
	m_pGateStore->getLibraryGates(libraryGates, L_HT);

	//Create collection for only H, T library gates
	GateCollectionPtr pGateLibraryCollection = new VectorBasedCollectionImpl<GatePtr>(NullPtr);
	for(unsigned int i = 0; i < libraryGates.size(); i++) {
		pGateLibraryCollection->addElement(libraryGates[i]->clone());
	}

	GateCombinabilityCheckers combinabilityCheckers;
	m_pGateCombinabilityCheckerFactory->getGateCombinabilityCheckers(L_HT, combinabilityCheckers);
	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	//Make collection of upto 4
	int sequenceLength = 4;
	GateSearchSpaceConstructorPtr pGateSearchSpaceContructor = new GateSearchSpaceConstructorImpl(pGateCombiner);
	GateCollectionPtr pGateCollection = new VectorBasedCollectionImpl<GatePtr>(NullPtr);
	pGateSearchSpaceContructor->constructSearchSpace(pGateCollection, pGateLibraryCollection, sequenceLength);


	//Expected results
	//H, T
	//HT, TH, TT
	//HTH, HTT, THT, TTT, TTH
	//HTHT, HTTT, HTTH, THTH, THTT, TTHT, TTTT, TTTH
	std::set<std::string> expectedGateSequences = {"H", "T", "HT", "TH", "TT", "HTH", "HTT", "THT", "TTT", "TTH", "HTHT", "HTTT", "HTTH", "THTH", "THTT", "TTHT", "TTTT", "TTTH"};

	assert(pGateCollection->size() == expectedGateSequences.size());

	GateIteratorPtr pGateIter = pGateCollection->getReadonlyIteratorPtr();
	assert(pGateIter != NullPtr && !pGateIter->isDone());

	//Check gate generated correctly (at least name)
	while(!pGateIter->isDone()) {
		GatePtr pGate = pGateIter->getObj();
		assert(pGate->getMatrix() != NullPtr);

		std::set<std::string>::const_iterator sIter = expectedGateSequences.find(pGate->getLabelStr());
		assert(sIter != expectedGateSequences.end());
		expectedGateSequences.erase(sIter);

		pGateIter->next();
	}

	delete pGateCollection;
	delete pGateSearchSpaceContructor;

	delete pGateCombiner;

	delete pGateLibraryCollection;
}

void ResourcesTester::testSimpleHTSCombinationChecker() {
	//H, T, S
	GatePtrVector libraryGates;
	m_pGateStore->getLibraryGates(libraryGates, L_HTS);

	//Create collection for only H, T library gates
	GateCollectionPtr pGateLibraryCollection = new VectorBasedCollectionImpl<GatePtr>(NullPtr);
	for(unsigned int i = 0; i < libraryGates.size(); i++) {
		pGateLibraryCollection->addElement(libraryGates[i]->clone());
	}

	GateCombinabilityCheckers combinabilityCheckers;
	m_pGateCombinabilityCheckerFactory->getGateCombinabilityCheckers(L_HTS, combinabilityCheckers);
	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	//Make collection of upto 3
	int sequenceLength = 3;
	GateSearchSpaceConstructorPtr pGateSearchSpaceContructor = new GateSearchSpaceConstructorImpl(pGateCombiner);
	GateCollectionPtr pGateCollection = new VectorBasedCollectionImpl<GatePtr>(NullPtr);
	pGateSearchSpaceContructor->constructSearchSpace(pGateCollection, pGateLibraryCollection, sequenceLength);


	//Expected results
	//H, T, S
	//HT, HS, TS, TH, SH, SS
	//HTS, HTH, HSH, HSS, THT, THS, TSH, TSS, SHT, SHS, SSH, SSS
	std::set<std::string> expectedGateSequences = {"H", "T", "S",
			"HT", "HS", "TS", "TH", "SH", "SS",
			"HTS", "HTH", "HSH", "HSS", "THT", "THS", "TSH", "TSS", "SHT", "SHS", "SSH", "SSS"};

	assert(pGateCollection->size() == expectedGateSequences.size());

	GateIteratorPtr pGateIter = pGateCollection->getReadonlyIteratorPtr();
	assert(pGateIter != NullPtr && !pGateIter->isDone());

	//Check gate generated correctly (at least name)
	while(!pGateIter->isDone()) {
		GatePtr pGate = pGateIter->getObj();
		assert(pGate->getMatrix() != NullPtr);

		std::set<std::string>::const_iterator sIter = expectedGateSequences.find(pGate->getLabelStr());
		assert(sIter != expectedGateSequences.end());
		expectedGateSequences.erase(sIter);

		pGateIter->next();
	}

	delete pGateCollection;
	delete pGateSearchSpaceContructor;

	delete pGateCombiner;

	delete pGateLibraryCollection;
}
