/*
 * GateCombinerImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "GateCombinerImpl.h"
#include "IGateCombinabilityChecker.h"

GateCombinerImpl::GateCombinerImpl(const GateCombinabilityCheckers& combinabilityCheckers, MatrixOperatorPtr pMatrixOperator) {
	m_combinabilityCheckers = combinabilityCheckers;
	m_pMatrixOperator = pMatrixOperator;
}

void GateCombinerImpl::combine(GatePtr pGate1, GatePtr pGate2, GatePtr& result) {
	result = NullPtr;

	for(unsigned int i = 0; i< m_combinabilityCheckers.size(); i++) {
		if(!m_combinabilityCheckers[i]->canCombine(pGate1, pGate2)) {
			return;
		}
	}

	MatrixPtr pCombinedMatrix = NullPtr;
	m_pMatrixOperator->multiply(pGate1->getMatrix(), pGate2->getMatrix(), pCombinedMatrix);

	cost_t combinedCost = pGate1->getCost() + pGate2->getCost();

	LabelSeq combinedLabel;
	combinedLabel.insert(combinedLabel.end(), pGate1->getLabelSeq().begin(), pGate1->getLabelSeq().end());
	combinedLabel.insert(combinedLabel.end(), pGate2->getLabelSeq().begin(), pGate2->getLabelSeq().end());

	std::string combinedLabelStr = pGate1->getLabelStr() + pGate2->getLabelStr();

	result = GatePtr(new Gate(pCombinedMatrix, combinedCost, combinedLabel, combinedLabelStr));

}


