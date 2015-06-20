/*
 * SampleGateCombinerImpl.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#include "SampleGateCombinerImpl.h"

bool checkCost(GatePtr pGate1, GatePtr pGate2, cost_t maxCost);

bool checkGateCancelation(GatePtr pGate1, GatePtr pGate2, const GateLabelCancelationMap& cancelationMap);

SampleGateCombinerImpl::SampleGateCombinerImpl(cost_t maxCost, const GateLabelCancelationMap& cancelationMap, MatrixOperatorPtr pMatrixOperator) {
	m_maxCost = maxCost;
	m_cancelationMap = cancelationMap;
	m_pMatrixOperator = pMatrixOperator;
}

void SampleGateCombinerImpl::combine(GatePtr pGate1, GatePtr pGate2, GatePtr& result) {
	result = NullPtr;
	if(checkCost(pGate1, pGate2, m_maxCost) && checkGateCancelation(pGate1, pGate2, m_cancelationMap)) {
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

	GateLabelCancelationMap::const_iterator lIter = cancelationMap.find(lastSeq1GateLabel);

	while(lIter != cancelationMap.end()) {
		if(!lIter->second.compare(firstSeq2GateLabel)) {
			return false;
		}
		lIter++;
	}

	return true;
}


