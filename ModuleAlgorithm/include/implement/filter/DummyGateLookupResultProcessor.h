/*
 * DummyGateLookupResultProcessor.h
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#ifndef DUMMYGATELOOKUPRESULTPROCESSOR_H_
#define DUMMYGATELOOKUPRESULTPROCESSOR_H_

#include "Gate.h"
#include "DummyLookupResultProcessor.h"

class DummyGateLookupResultProcessor: public DummyLookupResultProcessor<GatePtr> {
public:
	DummyGateLookupResultProcessor(DistanceCalculatorPtr<GatePtr> pDistanceCalculator, storage_t storageType = STRG_VECTOR);

	virtual ~DummyGateLookupResultProcessor(){};

protected:
	//Override
	bool sameElement(const GatePtr& pGate1, const GatePtr& pGate2) const;
};




#endif /* DUMMYGATELOOKUPRESULTPROCESSOR_H_ */
