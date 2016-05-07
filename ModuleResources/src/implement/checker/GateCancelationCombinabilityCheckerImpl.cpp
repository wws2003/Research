/*
 * GateCancelationCombinabilityCheckerImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "GateCancelationCombinabilityCheckerImpl.h"

GateCancelationCombinabilityCheckerImpl::GateCancelationCombinabilityCheckerImpl(GateLabelCancelationMap cancelationMap) {
	m_cancelationMap = cancelationMap;
}

bool GateCancelationCombinabilityCheckerImpl::canCombine(GatePtr pGate1, GatePtr pGate2) const {
	if(pGate1->getLabelSeq().empty() || pGate2->getLabelSeq().empty()) {
		return true;
	}

	std::string firstSeq2GateLabel = pGate2->getLabelSeq().front();

	std::pair<GateLabelCancelationMap::const_iterator, GateLabelCancelationMap::const_iterator> lPair = m_cancelationMap.equal_range(firstSeq2GateLabel);
	GateLabelCancelationMap::const_iterator lIter = lPair.first;

	std::string gate1SeqLabel = pGate1->getLabelStr();

	while(lIter != lPair.second) {
		std::string lastSeqShouldBeCancel = lIter->second;

		//Check if lastSeqShouldBeCancel is a suffix of gate1SeqLabel
		size_t rIndex = gate1SeqLabel.rfind(lastSeqShouldBeCancel);
		if(rIndex != std::string::npos && rIndex + lastSeqShouldBeCancel.length() == gate1SeqLabel.length()) {
			return false;
		}

		lIter++;
	}

	return true;
}



