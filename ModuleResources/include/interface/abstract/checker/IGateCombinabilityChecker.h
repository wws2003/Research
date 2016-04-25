/*
 * IGateCombinabilityChecker.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pham
 */

#ifndef IGATECOMBINABILITYCHECKER_H_
#define IGATECOMBINABILITYCHECKER_H_

#include "Gate.h"
#include "MathConceptsCommon.h"
#include "ResourcesCommon.h"

class IGateCombinabilityChecker {
public:
	virtual ~IGateCombinabilityChecker(){};

	virtual bool canCombine(GatePtr pGate1, GatePtr pGate2) const = 0;
};


#endif /* IGATECOMBINABILITYCHECKER_H_ */
