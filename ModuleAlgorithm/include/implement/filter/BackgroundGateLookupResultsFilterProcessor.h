/*
 * BackgroundGateLookupResultsFilterProcessor.h
 *
 *  Created on: Dec 15, 2015
 *      Author: pham
 */

#ifndef BACKGROUNDGATELOOKUPRESULTSFILTERPROCESSOR_H_
#define BACKGROUNDGATELOOKUPRESULTSFILTERPROCESSOR_H_

#include "Gate.h"
#include "BackgroundLookupResultsFilterProcessor.h"

class BackgroundGateLookupResultsFilterProcessor: public BackgroundLookupResultsFilterProcessor<GatePtr> {
public:
	BackgroundGateLookupResultsFilterProcessor(DistanceCalculatorPtr<GatePtr> pDistanceCalculator);

protected:
	//Override
	bool sameElement(const GatePtr& pGate1, const GatePtr& pGate2) const;
};

#endif /* BACKGROUNDGATELOOKUPRESULTSFILTERPROCESSOR_H_ */
