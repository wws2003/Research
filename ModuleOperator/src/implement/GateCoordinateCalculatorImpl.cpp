/*
 * GateCoordinateCalculatorImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "GateCoordinateCalculatorImpl.h"
#include "ICoordinateCalculator.h"
#include "Coordinate.hpp"
#include "Gate.h"

GateCoordinateCalculatorImpl::GateCoordinateCalculatorImpl(MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator) {
	m_pMatrixRealCoordinateCalculator = pMatrixRealCoordinateCalculator;
}

void GateCoordinateCalculatorImpl::calulateElementCoordinate(GatePtr pGate, GateRealCoordinatePtrRef& prGateRealCoordinatePtr) const {
	MatrixRealCoordinatePtr pMatrixRealCoordinate = NullPtr;
	m_pMatrixRealCoordinateCalculator->calulateElementCoordinate(pGate->getMatrix(), pMatrixRealCoordinate);

	prGateRealCoordinatePtr = new Coordinate<GatePtr, double>(pGate, pMatrixRealCoordinate->getCoordinates());

	_destroy(pMatrixRealCoordinate);
}




