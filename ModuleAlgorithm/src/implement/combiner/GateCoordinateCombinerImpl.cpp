/*
 * GateCoordinateCombinerImpl.cpp
 *
 *  Created on: Dec 7, 2015
 *      Author: pham
 */


#include "GateCoordinateCombinerImpl.h"

GateCoordinateCombinerImpl::GateCoordinateCombinerImpl(CombinerPtr<GatePtr> pGateCombiner) {
	m_pGateCombiner = pGateCombiner;
}

void GateCoordinateCombinerImpl::combine(RealCoordinate<GatePtr> gateCoord1, RealCoordinate<GatePtr> gateCoord2, RealCoordinate<GatePtr>& gateCoordResult) {
	GatePtr pCombinedGate = NullPtr;

	if(gateCoord1.getElement() != NullPtr && gateCoord2.getElement() != NullPtr) {
		m_pGateCombiner->combine(gateCoord1.getElement(), gateCoord2.getElement(), pCombinedGate);
	}

	//Just a temporary coordinate addition, may be re-calculate later
	gateCoordResult = gateCoord1 + gateCoord2;

	gateCoordResult.setElement(pCombinedGate);
}
