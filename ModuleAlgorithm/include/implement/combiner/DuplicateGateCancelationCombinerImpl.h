/*
 * DuplicateGateCancelationCombinerImpl.h
 *
 *  Created on: Aug 19, 2015
 *      Author: pham
 */

#ifndef DUPLICATEGATECANCELATIONCOMBINERIMPL_H_
#define DUPLICATEGATECANCELATIONCOMBINERIMPL_H_

#include "GateCombinerImpl.h"
#include <set>

typedef std::set<std::string> GateLabelSet;

class DuplicateGateCancelationCombinerImpl: public GateCombinerImpl {
public:
	DuplicateGateCancelationCombinerImpl(const GateCombinabilityCheckers& combinabilityCheckers, MatrixOperatorPtr pMatrixOperator);
	virtual ~DuplicateGateCancelationCombinerImpl(){};

	//Override
	virtual void combine(GatePtr pGate1, GatePtr pGate2, GatePtr& result);

private:
	GateLabelSet m_gateLabelSet;
};



#endif /* DUPLICATEGATECANCELATIONCOMBINERIMPL_H_ */
