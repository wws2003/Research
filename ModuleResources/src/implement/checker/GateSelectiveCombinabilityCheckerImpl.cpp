/*
 * GateSelectiveCombinabilityCheckerImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "GateSelectiveCombinabilityCheckerImpl.h"

GateSelectiveCombinabilityCheckerImpl::GateSelectiveCombinabilityCheckerImpl(CombinableGateLabelMap combinableMap) {
	m_combinableMap = combinableMap;
}

bool GateSelectiveCombinabilityCheckerImpl::canCombine(GatePtr pGate1, GatePtr pGate2) const {
	if(pGate1->getLabelSeq().empty() || pGate2->getLabelSeq().empty()) {
		return true;
	}
	std::string lastSeq1GateLabel = pGate1->getLabelSeq()[pGate1->getLabelSeq().size() - 1];
	std::string firstSeq2GateLabel = pGate2->getLabelSeq().front();

	std::pair<CombinableGateLabelMap::const_iterator, CombinableGateLabelMap::const_iterator> lPair = m_combinableMap.equal_range(lastSeq1GateLabel);
	CombinableGateLabelMap::const_iterator lIter = lPair.first;

	while(lIter != lPair.second) {

		//Check if firstSeq2GateLabel is combinable with lastSeq1GateLabel
		if(!(lIter->second.compare(firstSeq2GateLabel))) {
			return true;
		}

		lIter++;
	}

	return false;
}

