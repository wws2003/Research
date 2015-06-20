/*
 * SampleGateCombinerImpl.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#include "SampleGateCombinerImpl.h"

bool checkCost(GatePtr pGate1, GatePtr pGate2, cost_t maxCost);

bool checkGateCancelation(GatePtr pGate1, GatePtr pGate2, const GateLabelCancelationMap& cancelationMap);

bool checkGateIdentityCycle(GatePtr pGate1, GatePtr pGate2, GateLabelIdentityCycleMap identityCycleMap);

SampleGateCombinerImpl::SampleGateCombinerImpl(cost_t maxCost, const GateLabelCancelationMap& cancelationMap, const GateLabelIdentityCycleMap& identityCycleMap, MatrixOperatorPtr pMatrixOperator) {
	m_maxCost = maxCost;
	m_cancelationMap = cancelationMap;
	m_identityCycleMap = identityCycleMap;
	m_pMatrixOperator = pMatrixOperator;
}

void SampleGateCombinerImpl::combine(GatePtr pGate1, GatePtr pGate2, GatePtr& result) {
	result = NullPtr;
	if(checkCost(pGate1, pGate2, m_maxCost) && checkGateCancelation(pGate1, pGate2, m_cancelationMap) && checkGateIdentityCycle(pGate1, pGate2, m_identityCycleMap)) {
		MatrixPtr pCombinedMatrix = NullPtr;
		m_pMatrixOperator->multiply(pGate1->getMatrix(), pGate2->getMatrix(), pCombinedMatrix);

		cost_t combinedCost = pGate1->getCost() + pGate2->getCost();

		label_t combinedLabel;
		combinedLabel.insert(combinedLabel.end(), pGate1->getLabel().begin(), pGate1->getLabel().end());
		combinedLabel.insert(combinedLabel.end(), pGate2->getLabel().begin(), pGate2->getLabel().end());

		std::string combinedLabelStr = pGate1->getLabelStr() + pGate2->getLabelStr();

		result = GatePtr(new Gate(pCombinedMatrix, combinedCost, combinedLabel, combinedLabelStr));
	}

}

bool checkCost(GatePtr pGate1, GatePtr pGate2, cost_t maxCost) {
	cost_t costGate1 = pGate1->getCost();
	cost_t costGate2 = pGate2->getCost();
	return costGate1 + costGate2 <= maxCost;
}

bool checkGateCancelation(GatePtr pGate1, GatePtr pGate2, const GateLabelCancelationMap& cancelationMap) {
	if(pGate1->getLabel().empty() || pGate2->getLabel().empty()) {
		return true;
	}
	std::string lastSeq1GateLabel = pGate1->getLabel()[pGate1->getLabel().size() - 1];
	std::string firstSeq2GateLabel = pGate2->getLabel().front();

	std::pair<GateLabelCancelationMap::const_iterator, GateLabelCancelationMap::const_iterator> lPair = cancelationMap.equal_range(lastSeq1GateLabel);
	GateLabelCancelationMap::const_iterator lIter = lPair.first;
	while(lIter != lPair.second) {
		if(!lIter->second.compare(firstSeq2GateLabel)) {
			return false;
		}
		lIter++;
	}

	return true;
}

bool checkGateIdentityCycle(GatePtr pGate1, GatePtr pGate2, GateLabelIdentityCycleMap identityCycleMap) {

	if(pGate1->getLabel().empty() || pGate2->getLabel().empty()) {
		return true;
	}
	std::string firstSeq2GateLabel = pGate2->getLabel().front();

	if(identityCycleMap.find(firstSeq2GateLabel) != identityCycleMap.end()) {
		int identityCycle = identityCycleMap[firstSeq2GateLabel];

		//Check if gate2 has been repeated enough to form a identity
		int identityCycleCounter = identityCycle - 1;
		for(int i = pGate1->getLabel().size() - 1; i >= 0 && !(pGate1->getLabel()[i].compare(firstSeq2GateLabel)); i--) {
			identityCycleCounter--;
		}

		return (identityCycleCounter > 0);
	}

	return true;
}

