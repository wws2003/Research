/*
 * LazyGateDistanceCalculatorImpl.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: pham
 */


#include "LazyGateDistanceCalculatorImpl.h"
#include "Gate.h"

LazyGateDistanceCalculatorImpl::LazyGateDistanceCalculatorImpl(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator,
		MatrixOperatorPtr pMatrixOperator) {

	m_pMatrixDistanceCalculator = pMatrixDistanceCalculator;
	m_pMatrixOperator = pMatrixOperator;
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

/*
void LazyGateDistanceCalculatorImpl::provideMatrixToGate(GatePtr pGate) {
	MatrixPtr pMatrix = getMatrixFromGateLabelSequence(pGate->getLabelSeq());
	pGate->setMatrix(pMatrix);
}

*/
