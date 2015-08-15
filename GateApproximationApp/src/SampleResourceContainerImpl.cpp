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

//Library (universal) gates: H and T
void initSingleQubitLibGates(MatrixOperatorPtr pMatrixOperator, CollectionPtr<GatePtr> pLibraryGates);

//Library (universal) gates: CNOT, H and T
void initTwoQubitsLibGates(MatrixOperatorPtr pMatrixOperator, CollectionPtr<GatePtr> pLibraryGates);

void copyCollection(const GateCollectionPtr pSrcCollection, GateCollectionPtr pDstCollection);

void initSingleQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void getSingleQubitGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

void getSingleQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);

void initTwoQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker);

void getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

void getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);

void releaseGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

SampleResourceContainerImpl::SampleResourceContainerImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	m_pMatrixOperator = pMatrixOperator;
	m_pMatrixFactory = pMatrixFactory;

	m_pSingleQubitLibraryGates = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());
	m_pTwoQubitLibraryGates = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());

	initSingleQubitLibGates(m_pMatrixOperator, m_pSingleQubitLibraryGates);
	initTwoQubitsLibGates(m_pMatrixOperator, m_pTwoQubitLibraryGates);

	initSingleQubitGateCombinabilityCheckers(m_singleQubitGateCombinerCheckers);
	initTwoQubitGateCombinabilityCheckers(m_twoQubitGateCombinerCheckers);

	m_pMatrixOperator->getTracelessHermitianMatricesBasis(2, m_basis2);
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, m_basis4);

	m_pIdentity2 = GatePtr(new Gate(m_pMatrixFactory->getIdentityMatrix(2), 100, "I2"));
	m_pIdentity4 = GatePtr(new Gate(m_pMatrixFactory->getIdentityMatrix(4), 100, "I4"));
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

void SampleResourceContainerImpl::getTargetGatesAndEpsilon(std::vector<GatePtr>& targets, double& epsilon, int nbQubits) {
	//Just a sample for near-identity
	epsilon = 0.4;

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

/*-----------------------Private functions-----------------------*/

void initSingleQubitLibGates(MatrixOperatorPtr pMatrixOperator, CollectionPtr<GatePtr> pLibraryGates) {
	double inverseSQRT2 = 1/sqrt(2);

	ComplexVal arrayH[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0),
			ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pHMat = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "");

	MatrixPtr pSHMat = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pHMat, pSHMat);

	delete pHMat;

	GatePtr pH = new Gate(pSHMat, 1, "H");
	pLibraryGates->addElement(pH);

	ComplexVal expmPi_8 = std::exp(ComplexVal(0, 1) * M_PI / 8.0);
	ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, -1) * M_PI / 8.0);

	ComplexVal arrayT[] = {expm_Pi_8, 0.0
			, 0.0, expmPi_8};

	MatrixPtr pTMat = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "");

	GatePtr pT = new Gate(pTMat, 4, "T");
	pLibraryGates->addElement(pT);

	ComplexVal arrayTInverse[] = {expmPi_8, 0.0
			, 0.0, expm_Pi_8};

	MatrixPtr pTInverseMat = new SimpleDenseMatrixImpl(arrayTInverse, ROW_SPLICE, 2, 2, "");

	GatePtr pTInverse = new Gate(pTInverseMat, 4, "t");
	pLibraryGates->addElement(pTInverse);

	//Add phases to make sure search space cover
	ComplexVal arrayPhasePi[] = {-1.0,0.0,
			0.0,-1.0};
	MatrixPtr pPhasePiMat = new SimpleDenseMatrixImpl(arrayPhasePi, ROW_SPLICE, 2, 2, "I");

	GatePtr pPhasePiGate = GatePtr(new Gate(pPhasePiMat, 0, "Ph_Pi"));
	pLibraryGates->addElement(pPhasePiGate);
}

void initTwoQubitsLibGates(MatrixOperatorPtr pMatrixOperator, CollectionPtr<GatePtr> pLibraryGates) {
	ComplexVal arrayCNOT1[] = {1.0, 0.0, 0.0, 0.0
			, 0.0, 1.0, 0.0, 0.0
			, 0.0, 0.0, 0.0, 1.0
			, 0.0, 0.0, 1.0, 0.0};

	MatrixPtr pCNOTMat1 = new SimpleDenseMatrixImpl(arrayCNOT1, ROW_SPLICE, 4, 4, "");
	MatrixPtr pSCNOTMat1 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat1, pSCNOTMat1);

	delete pCNOTMat1;

	ComplexVal arrayCNOT2[] = {0.0, 1.0, 0.0, 0.0
			, 1.0, 0.0, 0.0, 0.0
			, 0.0, 0.0, 1.0, 0.0
			, 0.0, 0.0, 0.0, 1.0};

	MatrixPtr pCNOTMat2 = new SimpleDenseMatrixImpl(arrayCNOT2, ROW_SPLICE, 4, 4, "");
	MatrixPtr pSCNOTMat2 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat2, pSCNOTMat2);

	delete pCNOTMat2;

	double inverseSQRT2 = 1/sqrt(2);

	ComplexVal arrayH1[] = {ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(inverseSQRT2, 0), 0.0
			, 0.0, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(inverseSQRT2, 0)
			, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(-inverseSQRT2, 0), 0.0
			, 0.0, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH1Mat = new SimpleDenseMatrixImpl(arrayH1, ROW_SPLICE, 4, 4, "");

	ComplexVal arrayH2[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0), 0.0, 0.0
			, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0), 0.0, 0.0
			, 0.0, 0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0)
			, 0.0, 0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH2Mat = new SimpleDenseMatrixImpl(arrayH2, ROW_SPLICE, 4, 4, "");

	ComplexVal expmPi_8 = std::exp(ComplexVal(0, 1) * M_PI / 8.0);
	ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, -1) * M_PI / 8.0);

	ComplexVal arrayT1[] = {expm_Pi_8, 0.0, 0.0, 0.0
			, 0.0, expm_Pi_8, 0.0, 0.0
			, 0.0, 0.0, expmPi_8, 0.0
			, 0.0, 0.0, 0.0, expmPi_8};

	MatrixPtr pT1Mat = new SimpleDenseMatrixImpl(arrayT1, ROW_SPLICE, 4, 4, "");

	ComplexVal arrayT2[] = {expm_Pi_8, 0.0, 0.0, 0.0
			, 0.0, expmPi_8, 0.0, 0.0
			, 0.0, 0.0, expm_Pi_8, 0.0
			, 0.0, 0.0, 0.0, expmPi_8};

	MatrixPtr pT2Mat = new SimpleDenseMatrixImpl(arrayT2, ROW_SPLICE, 4, 4, "");

	//Create gates from matrices and add to library set
	GatePtr pCNOT1 = new Gate(pSCNOTMat1, 1, "CNOT1");
	pLibraryGates->addElement(pCNOT1);

	GatePtr pCNOT2 = new Gate(pSCNOTMat2, 1, "CNOT2");
	pLibraryGates->addElement(pCNOT2);

	GatePtr pH1 = new Gate(pH1Mat, 4, "H1");
	pLibraryGates->addElement(pH1);

	GatePtr pH2 = new Gate(pH2Mat, 4, "H2");
	pLibraryGates->addElement(pH2);

	GatePtr pT1 = new Gate(pT1Mat, 1, "T1");
	pLibraryGates->addElement(pT1);

	GatePtr pT2 = new Gate(pT2Mat, 1, "T2");
	pLibraryGates->addElement(pT2);
}

void copyCollection(const GateCollectionPtr pSrcCollection, GateCollectionPtr pDstCollection) {
	pDstCollection->clear();
	GateIteratorPtr pGateIter = pSrcCollection->getReadonlyIteratorPtr();
	while(!pGateIter->isDone()) {
		pDstCollection->addElement(pGateIter->getObj());
		pGateIter->next();
	}
	_destroy(pGateIter);
}

void initSingleQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers) {
	GateCombinabilityCheckerPtr pChecker1;
	getSingleQubitGateIdentityCycleCombinabilityChecker(pChecker1);
	GateCombinabilityCheckerPtr pChecker2;
	getSingleQubitsGateCancelationCombinabilityChecker(pChecker2);

	checkers.push_back(pChecker1);
	checkers.push_back(pChecker2);
}

void getSingleQubitGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker) {
	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair("T", 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair("t", 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair("H", 2));

	pGateIdentityCycleCombinabilityChecker = new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap);
}

void getSingleQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker) {
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

void initTwoQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers) {
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

void releaseGateCombinabilityCheckers(GateCombinabilityCheckers& checkers) {
	for(GateCombinabilityCheckers::iterator cIter = checkers.begin(); cIter != checkers.end(); ) {
		GateCombinabilityCheckerPtr pChecker = *cIter;
		cIter = checkers.erase(cIter);
		delete pChecker;
	}
}

void getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker) {
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

void getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker) {
	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair("T1", 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair("T2", 8));

	pGateIdentityCycleCombinabilityChecker = new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap);
}

void getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker) {
	GateLabelCancelationMap cancelationMap;
	cancelationMap.insert(GateLabelPair("CNOT2", "CNOT1CNOT2CNOT1"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2CNOT1CNOT2"));
	cancelationMap.insert(GateLabelPair("T2", "CNOT1CNOT2T2CNOT1CNOT2"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2H1CNOT1CNOT2H1"));
	cancelationMap.insert(GateLabelPair("H1", "CNOT1CNOT2H1CNOT1CNOT2"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2H1CNOT2CNOT1H1"));

	pGateCancelationCombinabilityChecker = GateCombinabilityCheckerPtr(new GateCancelationCombinabilityCheckerImpl(cancelationMap));
}


