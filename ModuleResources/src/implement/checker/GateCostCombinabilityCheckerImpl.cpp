/*
 * GateCostCombinabilityCheckerImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "GateCostCombinabilityCheckerImpl.h"

GateCostCombinabilityCheckerImpl::GateCostCombinabilityCheckerImpl(cost_t maxCost) {
	m_maxCost = maxCost;
}

bool GateCostCombinabilityCheckerImpl::canCombine(GatePtr pGate1, GatePtr pGate2) const {
	return m_maxCost <= pGate1->getCost() + pGate2->getCost();
}


