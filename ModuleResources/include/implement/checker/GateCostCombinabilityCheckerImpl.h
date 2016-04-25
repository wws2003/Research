/*
 * GateCostCombinabilityCheckerImpl.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef GATECOSTCOMBINABILITYCHECKERIMPL_H_
#define GATECOSTCOMBINABILITYCHECKERIMPL_H_

#include "IGateCombinabilityChecker.h"

class GateCostCombinabilityCheckerImpl : public IGateCombinabilityChecker {
public:
	GateCostCombinabilityCheckerImpl(cost_t maxCost);

	virtual ~GateCostCombinabilityCheckerImpl(){};

	virtual bool canCombine(GatePtr pGate1, GatePtr pGate2) const;

private:
	cost_t m_maxCost;
};


#endif /* GATECOSTCOMBINABILITYCHECKERIMPL_H_ */
