/*
 * GateCoordinateConverterImpl.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "GateCoordinateConverterImpl.h"

GateCoordinateConverterImpl::GateCoordinateConverterImpl(GateRealCoordinateCalculatorPtr pGateCoordinateCalculator) {
	m_pGateCoordinateCalculator = pGateCoordinateCalculator;
}

//Override
void GateCoordinateConverterImpl::convert12(const GatePtr& pGate, RealCoordinate<GatePtr>& gateCoord) {
	RealCoordinatePtr<GatePtr> pGateCoord = NullPtr;
	m_pGateCoordinateCalculator->calulateElementCoordinate(pGate, pGateCoord);
	gateCoord = *pGateCoord;
}

//Override
void GateCoordinateConverterImpl::convert21(const RealCoordinate<GatePtr>& gateCoord, GatePtr& pGate) {
	pGate = gateCoord.getElement();
}


