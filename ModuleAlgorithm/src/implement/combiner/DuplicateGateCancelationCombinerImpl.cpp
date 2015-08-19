/*
 * DuplicateGateCancelationCombinerImpl.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: pham
 */

#include "DuplicateGateCancelationCombinerImpl.h"

DuplicateGateCancelationCombinerImpl::DuplicateGateCancelationCombinerImpl(const GateCombinabilityCheckers& combinabilityCheckers, MatrixOperatorPtr pMatrixOperator) : GateCombinerImpl(combinabilityCheckers, pMatrixOperator) {

}

//Override
void DuplicateGateCancelationCombinerImpl::combine(GatePtr pGate1, GatePtr pGate2, GatePtr& result) {
	std::string expectedCombinationGateLabel = pGate1->getLabelStr() + pGate2->getLabelStr();
	if(m_gateLabelSet.find(expectedCombinationGateLabel) != m_gateLabelSet.end()) {
		return;
	}
	else {
		m_gateLabelSet.insert(expectedCombinationGateLabel);
		GateCombinerImpl::combine(pGate1, pGate2, result);
	}
}



