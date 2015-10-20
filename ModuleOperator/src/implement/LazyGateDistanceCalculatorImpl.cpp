/*
 * LazyGateDistanceCalculatorImpl.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: pham
 */


#include "LazyGateDistanceCalculatorImpl.h"
#include "Gate.h"

LazyGateDistanceCalculatorImpl::LazyGateDistanceCalculatorImpl(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator,
		MatrixOperatorPtr pMatrixOperator,
		LibraryMatrixStorePtr pLibraryMatrixStore) {

	m_pMatrixDistanceCalculator = pMatrixDistanceCalculator;
	m_pMatrixOperator = pMatrixOperator;
	m_pLibraryMatrixStore = pLibraryMatrixStore;
}

mreal_t LazyGateDistanceCalculatorImpl::distance(GatePtr pGate1, GatePtr pGate2) {
	MatrixPtr pMatrix1 = pGate1->getMatrix();
	MatrixPtr pMatrix2 = pGate2->getMatrix();
	if(pMatrix1 == NullPtr) {
		provideMatrixToGate(pGate1);
	}
	if(pMatrix2 == NullPtr) {
		provideMatrixToGate(pGate2);
	}
	return m_pMatrixDistanceCalculator->distance(pGate1->getMatrix(), pGate2->getMatrix());
}

void LazyGateDistanceCalculatorImpl::provideMatrixToGate(GatePtr pGate) {
	MatrixPtr pMatrix = getMatrixFromGateLabelSequence(pGate->getLabelSeq());
	pGate->setMatrix(pMatrix);
}
MatrixPtr LazyGateDistanceCalculatorImpl::getMatrixFromGateLabelSequence(const LabelSeq& sequence) {
	if(sequence.empty()) {
		return NullPtr;
	}
	MatrixPtr pGateMatrix = m_pLibraryMatrixStore->getLibraryMatrixFromLabel(sequence[0])->clone();

	for(unsigned int i = 1; i < sequence.size(); i++) {
		MatrixPtr pNextMatrix = m_pLibraryMatrixStore->getLibraryMatrixFromLabel(sequence[i]);
		MatrixPtr pMatrix;
		m_pMatrixOperator->multiply(pGateMatrix, pNextMatrix, pMatrix);

		_destroy(pGateMatrix);
		pGateMatrix = pMatrix;
	}
	return pGateMatrix;
}

