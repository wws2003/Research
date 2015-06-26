/*
 * Util.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: pham
 */

#include "AssemblerUtil.h"
#include "IMatrixOperator.h"
#include "VectorBasedCollectionImpl.hpp"
#include "SearchSpaceConstructorImpl.cpp"
#include "GateCombinerImpl.h"
#include "ICombiner.h"
#include "SimpleDenseMatrixImpl.h"
#include "GateSelectiveCombinabilityCheckerImpl.h"
#include "GateCancelationCombinabilityCheckerImpl.h"
#include "GateIdentityCycleCombinabilityCheckerImpl.h"
#include "SampleMatrixOperator.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include <vector>

void initSingleQubitLibGates(MatrixOperatorPtr pMatrixOperator, CollectionPtr<GatePtr> pLibraryGates);

void initTwoQubitsLibGates(MatrixOperatorPtr pMatrixOperator, CollectionPtr<GatePtr> pLibraryGates);

void initSingleQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void getSingleQubitGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

void getSingleQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);

void initTwoQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker);

void getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

void getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);

void releaseGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void getGlobalPhaseEquivalentQueries(MatrixOperatorPtr pMatrixOperator, GatePtr rootQuery, GatePtrVector& queries);

AssemblerUtil::AssemblerUtil() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	m_pSingleQubitLibraryGates = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());
	m_pTwoQubitLibraryGates = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());

	initSingleQubitLibGates(m_pMatrixOperator, m_pSingleQubitLibraryGates);
	initTwoQubitsLibGates(m_pMatrixOperator, m_pTwoQubitLibraryGates);

	initSingleQubitGateCombinabilityCheckers(m_singleQubitGateCombinerCheckers);
	initTwoQubitGateCombinabilityCheckers(m_twoQubitGateCombinerCheckers);

	m_pSingleQubitGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(m_singleQubitGateCombinerCheckers, m_pMatrixOperator));
	m_pSingleQubitGateSearchSpaceConstructor = GateSearchSpaceConstructorPtr(new SearchSpaceConstructorImpl<GatePtr>(m_pSingleQubitGateCombiner));

	m_pTwoQubitGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(m_twoQubitGateCombinerCheckers, m_pMatrixOperator));
	m_pTwoQubitGateSearchSpaceConstructor = GateSearchSpaceConstructorPtr(new SearchSpaceConstructorImpl<GatePtr>(m_pTwoQubitGateCombiner));

}

AssemblerUtil::~AssemblerUtil() {
	delete m_pTwoQubitGateSearchSpaceConstructor;
	delete m_pTwoQubitGateCombiner;
	delete m_pSingleQubitGateSearchSpaceConstructor;
	delete m_pSingleQubitGateCombiner;

	releaseGateCombinabilityCheckers(m_twoQubitGateCombinerCheckers);
	releaseGateCombinabilityCheckers(m_singleQubitGateCombinerCheckers);

	delete m_pTwoQubitLibraryGates;
	delete m_pSingleQubitLibraryGates;

	delete m_pMatrixOperator;
	delete m_pMatrixFactory;
}

void AssemblerUtil::getLibraryGates(GateCollectionPtr& pLibraryGates, int nbQubits) {
	switch(nbQubits) {
	case 1:
		pLibraryGates = m_pSingleQubitLibraryGates;
		break;
	case 2:
		pLibraryGates = m_pTwoQubitLibraryGates;
		break;
	default:
		break;
	}
}

void AssemblerUtil::getSampleGateSearchSpaceCollection(GateCollectionPtr& pGateCollection, int nbQubits, int maxSequenceLength) {
	pGateCollection = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>());
	switch(nbQubits) {
	case 1:
		m_pSingleQubitGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, m_pSingleQubitLibraryGates, maxSequenceLength);
		break;
	case 2:
		m_pTwoQubitGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, m_pTwoQubitLibraryGates, maxSequenceLength);
		break;
	default:
		break;
	}
}

void AssemblerUtil::getSampleQueryGate(GatePtrVector& queries, int nbQubits) {
	//Currently only returns X to test
	ComplexVal arrayX2[] = {0.0,ComplexVal(0,1),ComplexVal(0,1),0.0};
	MatrixPtr pX2 = new SimpleDenseMatrixImpl(arrayX2, ROW_SPLICE, 2, 2, "X");
	GatePtr pX2Gate = new Gate(pX2, 9999, "Unknown");

	ComplexVal arrayI4[] = {1.0,0.0,0.0,0.0,
			0.0,1.0,0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0};

	MatrixPtr pI4 = new SimpleDenseMatrixImpl(arrayI4, ROW_SPLICE, 2, 2, "I");
	GatePtr pI4Gate = new Gate(pI4, 9999, "Unknown");

	switch(nbQubits) {
	case 1:
		queries.push_back(pX2Gate);
		//getGlobalPhaseEquivalentQueries(m_pMatrixOperator, pI2Gate, queries);
		break;
	case 2:
		queries.push_back(pI4Gate);
		getGlobalPhaseEquivalentQueries(m_pMatrixOperator, pI4Gate, queries);
		break;
	default:
		break;
	}
}

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

	GatePtr pCNOT1 = new Gate(pSCNOTMat1, 1, "CNOT1");
	pLibraryGates->addElement(pCNOT1);

	ComplexVal arrayCNOT2[] = {0.0, 1.0, 0.0, 0.0
			, 1.0, 0.0, 0.0, 0.0
			, 0.0, 0.0, 1.0, 0.0
			, 0.0, 0.0, 0.0, 1.0};

	MatrixPtr pCNOTMat2 = new SimpleDenseMatrixImpl(arrayCNOT2, ROW_SPLICE, 4, 4, "");
	MatrixPtr pSCNOTMat2 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat2, pSCNOTMat2);

	delete pCNOTMat2;

	GatePtr pCNOT2 = new Gate(pSCNOTMat2, 1, "CNOT2");
	pLibraryGates->addElement(pCNOT2);

	double inverseSQRT2 = 1/sqrt(2);

	ComplexVal arrayH1[] = {ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(inverseSQRT2, 0), 0.0
			, 0.0, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(inverseSQRT2, 0)
			, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(-inverseSQRT2, 0), 0.0
			, 0.0, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH1Mat = new SimpleDenseMatrixImpl(arrayH1, ROW_SPLICE, 4, 4, "");

	GatePtr pH1 = new Gate(pH1Mat, 4, "H1");
	pLibraryGates->addElement(pH1);

	ComplexVal arrayH2[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0), 0.0, 0.0
			, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0), 0.0, 0.0
			, 0.0, 0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0)
			, 0.0, 0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH2Mat = new SimpleDenseMatrixImpl(arrayH2, ROW_SPLICE, 4, 4, "");

	GatePtr pH2 = new Gate(pH2Mat, 4, "H2");
	pLibraryGates->addElement(pH2);

	ComplexVal expmPi_8 = std::exp(ComplexVal(0, 1) * M_PI / 8.0);
	ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, -1) * M_PI / 8.0);

	ComplexVal arrayT1[] = {expm_Pi_8, 0.0, 0.0, 0.0
			, 0.0, expm_Pi_8, 0.0, 0.0
			, 0.0, 0.0, expmPi_8, 0.0
			, 0.0, 0.0, 0.0, expmPi_8};

	MatrixPtr pT1Mat = new SimpleDenseMatrixImpl(arrayT1, ROW_SPLICE, 4, 4, "");

	GatePtr pT1 = new Gate(pT1Mat, 1, "T1");
	pLibraryGates->addElement(pT1);

	ComplexVal arrayT2[] = {expm_Pi_8, 0.0, 0.0, 0.0
			, 0.0, expmPi_8, 0.0, 0.0
			, 0.0, 0.0, expm_Pi_8, 0.0
			, 0.0, 0.0, 0.0, expmPi_8};

	MatrixPtr pT2Mat = new SimpleDenseMatrixImpl(arrayT2, ROW_SPLICE, 4, 4, "");

	GatePtr pT2 = new Gate(pT2Mat, 1, "T2");
	pLibraryGates->addElement(pT2);
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

	pGateSelectiveCombinabilityChecker = new GateSelectiveCombinabilityCheckerImpl(combinableLabelMap);
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
	pGateCancelationCombinabilityChecker = new GateCancelationCombinabilityCheckerImpl(cancelationMap);
}

void getGlobalPhaseEquivalentQueries(MatrixOperatorPtr pMatrixOperator, GatePtr rootQuery, GatePtrVector& queries) {
	//Test add global phase to queries

	MatrixPtr pU = rootQuery->getMatrix();
	int nbRows, nbColumns;
	pU->getSize(nbRows, nbColumns);

	MatrixPtr pQueryWithPhasePI = NullPtr;

	ComplexVal expmPi = std::exp(ComplexVal(0, 1) * M_PI);
	pMatrixOperator->multiplyScalar(pU, expmPi, pQueryWithPhasePI);

	queries.push_back(GatePtr(new Gate(pQueryWithPhasePI, 99999, "Unknown")));

	if(nbRows > 2) {
		MatrixPtr pQueryWithPhasePI_2 = NullPtr;
		MatrixPtr pQueryWithPhase_PI_2 = NullPtr;

		ComplexVal expmPi_2 = std::exp(ComplexVal(0, 1) * M_PI / 2.0);
		ComplexVal expm_Pi_2 = std::exp(ComplexVal(0, -1) * M_PI / 2.0);

		pMatrixOperator->multiplyScalar(pU, expmPi_2, pQueryWithPhasePI_2);
		pMatrixOperator->multiplyScalar(pU, expm_Pi_2, pQueryWithPhase_PI_2);

		queries.push_back(GatePtr(new Gate(pQueryWithPhasePI_2, 99999, "Unknown")));
		queries.push_back(GatePtr(new Gate(pQueryWithPhase_PI_2, 99999, "Unknown")));

	}

}


