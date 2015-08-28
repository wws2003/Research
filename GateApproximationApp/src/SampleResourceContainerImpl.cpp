/*
 * SampleResourceContainerImpl.cpp
 *
 *  Created on: Aug 15, 2015
 *      Author: pham
 */

#include "SampleResourceContainerImpl.h"
#include "IMatrixOperator.h"
#include "VectorBasedCollectionImpl.hpp"
#include "GateSearchSpaceConstructorImpl.h"
#include "GateCombinerImpl.h"
#include "ICombiner.h"
#include "SimpleDenseMatrixImpl.h"
#include "GateSelectiveCombinabilityCheckerImpl.h"
#include "GateCancelationCombinabilityCheckerImpl.h"
#include "GateIdentityCycleCombinabilityCheckerImpl.h"
#include "SampleMatrixOperator.h"
#include "SimpleDenseMatrixFactoryImpl.h"

void copyCollection(const GateCollectionPtr pSrcCollection, GateCollectionPtr pDstCollection);

void releaseGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

SampleResourceContainerImpl::SampleResourceContainerImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	m_pMatrixOperator = pMatrixOperator;
	m_pMatrixFactory = pMatrixFactory;
	m_pGateFactory = GateFactoryPtr(new GateFactory(m_pMatrixOperator, m_pMatrixFactory));

	m_pSingleQubitLibraryGates = NullPtr;
	m_pTwoQubitLibraryGates = NullPtr;

	m_pIdentity2 = NullPtr;
	m_pIdentity4 = NullPtr;
}

SampleResourceContainerImpl::~SampleResourceContainerImpl() {
	_destroy(m_pIdentity4);
	_destroy(m_pIdentity2);

	releaseGateCombinabilityCheckers(m_twoQubitGateCombinerCheckers);
	releaseGateCombinabilityCheckers(m_singleQubitGateCombinerCheckers);

	//Explicitly release universal (library) gates. Do not rely on release in collection
	m_pTwoQubitLibraryGates->purge();
	m_pSingleQubitLibraryGates->purge();

	_destroy(m_pTwoQubitLibraryGates) ;
	_destroy(m_pSingleQubitLibraryGates);

	_destroy(m_pGateFactory);
}

void SampleResourceContainerImpl::setup() {
	m_pSingleQubitLibraryGates = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());
	m_pTwoQubitLibraryGates = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());

	m_pMatrixOperator->getTracelessHermitianMatricesBasis(2, m_basis2);
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, m_basis4);

	m_pIdentity2 = GatePtr(new Gate(m_pMatrixFactory->getIdentityMatrix(2), 100, "I2"));
	m_pIdentity4 = GatePtr(new Gate(m_pMatrixFactory->getIdentityMatrix(4), 100, "I4"));

	GateNameCostMap singleQubitGateMap;
	initSingleQubitLibGates(singleQubitGateMap);
	collectLibraryGatesFromMap(singleQubitGateMap, m_pGateFactory, m_pSingleQubitLibraryGates);

	GateNameCostMap twoQubitsGateMap;
	initTwoQubitsLibGates(twoQubitsGateMap);
	collectLibraryGatesFromMap(twoQubitsGateMap, m_pGateFactory, m_pTwoQubitLibraryGates);

	initSingleQubitGateCombinabilityCheckers(m_singleQubitGateCombinerCheckers);
	initTwoQubitGateCombinabilityCheckers(m_twoQubitGateCombinerCheckers);
}

void SampleResourceContainerImpl::getUniversalGates(GateCollectionPtr pLibraryGates, int nbQubits) {
	switch(nbQubits) {
	case 1:
		copyCollection(m_pSingleQubitLibraryGates, pLibraryGates);
		break;
	case 2:
		copyCollection(m_pTwoQubitLibraryGates, pLibraryGates);
		break;
	default:
		break;
	}
}

void SampleResourceContainerImpl::getGateCombinabilityCheckers(GateCombinabilityCheckers& checkers, int nbQubits) {
	switch(nbQubits) {
	case 1:
		checkers.insert(checkers.end(), m_singleQubitGateCombinerCheckers.begin(), m_singleQubitGateCombinerCheckers.end());
		break;
	case 2:
		checkers.insert(checkers.end(), m_twoQubitGateCombinerCheckers.begin(), m_twoQubitGateCombinerCheckers.end());
		break;
	default:
		break;
	}
}

void SampleResourceContainerImpl::getMatrixOrthonormalBasis(MatrixPtrVector& pBasis, int nbQubits) {
	switch(nbQubits) {
	case 1:
		pBasis.insert(pBasis.end(), m_basis2.begin(), m_basis2.end());
		break;
	case 2:
		pBasis.insert(pBasis.end(), m_basis4.begin(), m_basis4.end());
		break;
	default:
		break;
	}
}

void SampleResourceContainerImpl::getTargetGatesAndEpsilon(std::vector<GatePtr>& targets, int nbQubits) {
	//Just a sample for near-identity

	targets.clear();
	switch(nbQubits) {
	case 1:
		targets.push_back(m_pIdentity2);
		break;
	case 2:
		targets.push_back(m_pIdentity4);
		break;
	default:
		break;
	}
}

//Library (universal) gates: H, T, and T'
void SampleResourceContainerImpl::initSingleQubitLibGates(GateNameCostMap& gateNameCostMap) {
	//TODO Possibly move to config file ?
	gateNameCostMap.clear();
	gateNameCostMap["H"] = 1;
	gateNameCostMap["T"] = 2;
	gateNameCostMap["t"] = 2;
}

//Library (universal) gates: CNOT, H and T
void SampleResourceContainerImpl::initTwoQubitsLibGates(GateNameCostMap& gateNameCostMap) {
	gateNameCostMap.clear();
	gateNameCostMap["CNOT1"] = 1;
	gateNameCostMap["CNOT2"] = 1;
	gateNameCostMap["H1"] = 10;
	gateNameCostMap["H2"] = 10;
	gateNameCostMap["T1"] = 20;
	gateNameCostMap["T2"] = 20;
}

void SampleResourceContainerImpl::initSingleQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers) {
	GateCombinabilityCheckerPtr pChecker1;
	getSingleQubitGateIdentityCycleCombinabilityChecker(pChecker1);
	GateCombinabilityCheckerPtr pChecker2;
	getSingleQubitsGateCancelationCombinabilityChecker(pChecker2);

	checkers.push_back(pChecker1);
	checkers.push_back(pChecker2);
}

void SampleResourceContainerImpl::getSingleQubitGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker) {
	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair("T", 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair("t", 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair("H", 2));

	pGateIdentityCycleCombinabilityChecker = new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap);
}

void SampleResourceContainerImpl::getSingleQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker) {
	GateLabelCancelationMap cancelationMap;

	//Cancel T*t = I
	cancelationMap.insert(GateLabelPair("T", "t"));
	cancelationMap.insert(GateLabelPair("t", "T"));

	//Cancel all phase added after any gates. Phase should only appear in the beginning of the sequence
	cancelationMap.insert(GateLabelPair("Ph_Pi", "t"));
	cancelationMap.insert(GateLabelPair("Ph_Pi", "T"));
	cancelationMap.insert(GateLabelPair("Ph_Pi", "H"));
	cancelationMap.insert(GateLabelPair("Ph_Pi", "Ph_Pi"));

	pGateCancelationCombinabilityChecker = new GateCancelationCombinabilityCheckerImpl(cancelationMap);
}

void SampleResourceContainerImpl::initTwoQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers) {
	GateCombinabilityCheckerPtr pChecker1;
	getTwoQubitsGateCancelationCombinabilityChecker(pChecker1);
	GateCombinabilityCheckerPtr pChecker2;
	getTwoQubitsGateIdentityCycleCombinabilityChecker(pChecker2);
	GateCombinabilityCheckerPtr pChecker3;
	getTwoQubitsGateSelectiveCombinabilityChecker(pChecker3);

	checkers.push_back(pChecker1);
	checkers.push_back(pChecker2);
	checkers.push_back(pChecker3);
}

void SampleResourceContainerImpl::getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker) {
	CombinableGateLabelMap combinableLabelMap;
	combinableLabelMap.insert(CombinableGateLabelPair("T1","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","CNOT1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("H1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","CNOT1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("T2","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","CNOT1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("H2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H2","CNOT1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H2","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","CNOT1"));

	pGateSelectiveCombinabilityChecker = GateCombinabilityCheckerPtr(new GateSelectiveCombinabilityCheckerImpl(combinableLabelMap));
}

void SampleResourceContainerImpl::getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker) {
	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair("T1", 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair("T2", 8));

	pGateIdentityCycleCombinabilityChecker = new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap);
}

void SampleResourceContainerImpl::getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker) {
	GateLabelCancelationMap cancelationMap;
	cancelationMap.insert(GateLabelPair("CNOT2", "CNOT1CNOT2CNOT1"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2CNOT1CNOT2"));
	cancelationMap.insert(GateLabelPair("T2", "CNOT1CNOT2T2CNOT1CNOT2"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2H1CNOT1CNOT2H1"));
	cancelationMap.insert(GateLabelPair("H1", "CNOT1CNOT2H1CNOT1CNOT2"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2H1CNOT2CNOT1H1"));

	pGateCancelationCombinabilityChecker = GateCombinabilityCheckerPtr(new GateCancelationCombinabilityCheckerImpl(cancelationMap));
}

void SampleResourceContainerImpl::collectLibraryGatesFromMap(const GateNameCostMap& gateNameCostMap, GateFactoryPtr pGateFactory, CollectionPtr<GatePtr> pLibraryGates) {
	for(GateNameCostMap::const_iterator gIter = gateNameCostMap.begin(); gIter != gateNameCostMap.end(); gIter++) {
		std::string gateName = gIter->first;
		cost_t gateCost = gIter->second;
		GatePtr pGate = pGateFactory->getLibraryGateInstance(gateName, gateCost);
		pLibraryGates->addElement(pGate);
	}
}

/*-----------------------Private functions-----------------------*/

void copyCollection(const GateCollectionPtr pSrcCollection, GateCollectionPtr pDstCollection) {
	pDstCollection->clear();
	GateIteratorPtr pGateIter = pSrcCollection->getReadonlyIteratorPtr();
	while(!pGateIter->isDone()) {
		pDstCollection->addElement(pGateIter->getObj());
		pGateIter->next();
	}
	_destroy(pGateIter);
}

void releaseGateCombinabilityCheckers(GateCombinabilityCheckers& checkers) {
	for(GateCombinabilityCheckers::iterator cIter = checkers.begin(); cIter != checkers.end(); ) {
		GateCombinabilityCheckerPtr pChecker = *cIter;
		cIter = checkers.erase(cIter);
		delete pChecker;
	}
}
