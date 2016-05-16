/*
 * SetBasedGateLookupResultProcessor.h
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#ifndef SETBASEDGATELOOKUPRESULTPROCESSOR_H_
#define SETBASEDGATELOOKUPRESULTPROCESSOR_H_

#include "Gate.h"
#include "SetBasedLookupResultProcessor.h"

class SetBasedGateLookupResultProcessor: public SetBasedLookupResultProcessor<GatePtr> {
public:
	SetBasedGateLookupResultProcessor(DistanceCalculatorPtr<GatePtr> pDistanceCalculator, bool isUniqueElements);

	virtual ~SetBasedGateLookupResultProcessor(){};

protected:
	//Override
	bool sameElement(const GatePtr& pGate1, const GatePtr& pGate2) const;
};




#endif /* DUMMYGATELOOKUPRESULTPROCESSOR_H_ */
