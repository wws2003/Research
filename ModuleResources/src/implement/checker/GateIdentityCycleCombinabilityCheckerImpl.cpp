/*
 * GateIdentityCycleCombinabilityCheckerImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "GateIdentityCycleCombinabilityCheckerImpl.h"

GateIdentityCycleCombinabilityCheckerImpl::GateIdentityCycleCombinabilityCheckerImpl(GateLabelIdentityCycleMap identityCycleMap) {
	m_identityCycleMap = identityCycleMap;
}

bool GateIdentityCycleCombinabilityCheckerImpl::canCombine(GatePtr pGate1, GatePtr pGate2) const {
	if(pGate1->getLabelSeq().empty() || pGate2->getLabelSeq().empty()) {
		return true;
	}
	std::string firstSeq2GateLabel = pGate2->getLabelSeq().front();

	GateLabelIdentityCycleMap::const_iterator cIter = m_identityCycleMap.find(firstSeq2GateLabel);

	if(cIter != m_identityCycleMap.end()) {
		int identityCycle = cIter->second;

		//Check if gate2 has been repeated enough to form a identity
		int identityCycleCounter = identityCycle - 1;
		for(int i = pGate1->getLabelSeq().size() - 1; i >= 0 && !(pGate1->getLabelSeq()[i].compare(firstSeq2GateLabel)); i--) {
			identityCycleCounter--;
		}

		return (identityCycleCounter > 0);
	}

	return true;
}



