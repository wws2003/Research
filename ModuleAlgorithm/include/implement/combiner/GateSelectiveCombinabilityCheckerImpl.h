/*
 * GateSelectiveCombinabilityCheckerImpl.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef GATESELECTIVECOMBINABILITYCHECKERIMPL_H_
#define GATESELECTIVECOMBINABILITYCHECKERIMPL_H_

#include "IGateCombinabilityChecker.h"
#include <map>
#include <string>

typedef std::multimap<std::string, std::string> CombinableGateLabelMap;
typedef std::pair<std::string, std::string> CombinableGateLabelPair;

class GateSelectiveCombinabilityCheckerImpl : public IGateCombinabilityChecker {
public:
	GateSelectiveCombinabilityCheckerImpl(CombinableGateLabelMap combinableMap);
	virtual ~GateSelectiveCombinabilityCheckerImpl(){};

	virtual bool canCombine(GatePtr pGate1, GatePtr pGate2) const;

private:
	CombinableGateLabelMap m_combinableMap;
};



#endif /* GATESELECTIVECOMBINABILITYCHECKERIMPL_H_ */
