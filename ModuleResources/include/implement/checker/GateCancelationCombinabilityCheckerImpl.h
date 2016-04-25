/*
 * GateCancelationCombinabilityCheckerImpl.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef GATECANCELATIONCOMBINABILITYCHECKERIMPL_H_
#define GATECANCELATIONCOMBINABILITYCHECKERIMPL_H_

#include "IGateCombinabilityChecker.h"
#include <map>

typedef std::multimap<std::string, std::string> GateLabelCancelationMap;
typedef std::pair<std::string, std::string> GateLabelPair;

class GateCancelationCombinabilityCheckerImpl : public IGateCombinabilityChecker {
public:
	GateCancelationCombinabilityCheckerImpl(GateLabelCancelationMap cancelationMap);
	virtual ~GateCancelationCombinabilityCheckerImpl(){};

	virtual bool canCombine(GatePtr pGate1, GatePtr pGate2) const;

private:
	GateLabelCancelationMap m_cancelationMap;
};


#endif /* GATECANCELATIONCOMBINABILITYCHECKERIMPL_H_ */
