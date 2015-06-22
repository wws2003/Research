/*
 * IGateCombinabilityChecker.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef IGATECOMBINABILITYCHECKER_H_
#define IGATECOMBINABILITYCHECKER_H_

#include "AlgoCommon.h"
#include "Gate.h"

class IGateCombinabilityChecker {
public:
	virtual ~IGateCombinabilityChecker(){};

	virtual bool canCombine(GatePtr pGate1, GatePtr pGate2) const = 0;
};


#endif /* IGATECOMBINABILITYCHECKER_H_ */
