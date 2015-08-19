/*
 * DuplicateGateCombinabilityCheckerImpl.h
 *
 *  Created on: Aug 19, 2015
 *      Author: pham
 */

#ifndef DUPLICATEGATECOMBINABILITYCHECKERIMPL_H_
#define DUPLICATEGATECOMBINABILITYCHECKERIMPL_H_

#include "IGateCombinabilityChecker.h"
#include <set>

typedef std::set<std::string> GateLabelSet;

class DuplicateGateCombinabilityCheckerImpl: public IGateCombinabilityChecker {
public:
	virtual ~DuplicateGateCombinabilityCheckerImpl(){};

	//Override
	/**
	 * Do not combine if the 2 input gates have been combined once before by this combiner
	 * Checking this condition is via a set of gate labels
	 */
	virtual bool canCombine(GatePtr pGate1, GatePtr pGate2) const;

private:
	GateLabelSet m_gateLabelSet;
};


#endif /* DUPLICATEGATECOMBINABILITYCHECKERIMPL_H_ */
