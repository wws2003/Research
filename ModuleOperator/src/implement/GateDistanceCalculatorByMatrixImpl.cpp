/*
 * GateMatrixDistanceCalculatorImpl.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "Gate.h"
GateDistanceCalculatorByMatrixImpl::GateDistanceCalculatorByMatrixImpl(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator) {
	m_pMatrixDistanceCalculator = pMatrixDistanceCalculator;
}

double GateDistanceCalculatorByMatrixImpl::distance(GatePtr pGate1, GatePtr pGate2) {
	return m_pMatrixDistanceCalculator->distance(pGate1->getMatrix(), pGate2->getMatrix());
}




