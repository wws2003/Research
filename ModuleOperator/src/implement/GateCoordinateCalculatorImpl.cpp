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

	prGateRealCoordinatePtr = new Coordinate<GatePtr, mreal_t>(pGate, pMatrixRealCoordinate->getCoordinates());

	_destroy(pMatrixRealCoordinate);
}

void GateCoordinateCalculatorImpl::calculateElementFromCoordinate(GateRealCoordinatePtr pGateCoordinate) const {
	RealCoordinate<MatrixPtr> realCoordinate(NullPtr, pGateCoordinate->getCoordinates());
	m_pMatrixRealCoordinateCalculator->calculateElementFromCoordinate(&realCoordinate);

	GatePtr pGate = GatePtr(new Gate(realCoordinate.getElement(), 0, ""));
	pGateCoordinate->setElement(pGate);
}




