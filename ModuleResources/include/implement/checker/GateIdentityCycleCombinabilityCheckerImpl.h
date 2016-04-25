/*
 * GateIdentityCycleCombinabilityCheckerImpl.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef GATEIDENTITYCYCLECOMBINABILITYCHECKERIMPL_H_
#define GATEIDENTITYCYCLECOMBINABILITYCHECKERIMPL_H_

#include "IGateCombinabilityChecker.h"
#include <map>

typedef std::map<std::string, int> GateLabelIdentityCycleMap;
typedef std::pair<std::string, int> GateLabelIdentityCyclePair;

class GateIdentityCycleCombinabilityCheckerImpl : public IGateCombinabilityChecker {
public:

	GateIdentityCycleCombinabilityCheckerImpl(GateLabelIdentityCycleMap identityCycleMap);

	virtual ~GateIdentityCycleCombinabilityCheckerImpl(){};

	virtual bool canCombine(GatePtr pGate1, GatePtr pGate2) const;

private:
	GateLabelIdentityCycleMap m_identityCycleMap;
};


#endif /* GATEIDENTITYCYCLECOMBINABILITYCHECKERIMPL_H_ */
