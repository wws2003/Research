/*
 * DummyGateLookupResultProcessor.h
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#ifndef VECTORBASEDGATELOOKUPRESULTPROCESSOR_H_
#define VECTORBASEDGATELOOKUPRESULTPROCESSOR_H_

#include "Gate.h"
#include "VectorBasedLookupResultProcessor.h"

class VectorBasedGateLookupResultProcessor: public VectorBasedLookupResultProcessor<GatePtr> {
public:
	VectorBasedGateLookupResultProcessor(DistanceCalculatorPtr<GatePtr> pDistanceCalculator);

	virtual ~VectorBasedGateLookupResultProcessor(){};

protected:
	//Override
	bool sameElement(const GatePtr& pGate1, const GatePtr& pGate2) const;
};




#endif /* DUMMYGATELOOKUPRESULTPROCESSOR_H_ */
