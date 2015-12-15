/*
 * ILookupResultProcessor.h
 *
 *  Created on: Dec 14, 2015
 *      Author: pham
 */

#ifndef ILOOKUPRESULTPROCESSOR_H_
#define ILOOKUPRESULTPROCESSOR_H_

#include "AlgoCommon.h"
#include "AlgoInternal.h"
#include "ICollection.h"
#include "IDistanceCalculator.h"

template<typename T>
class ILookupResultProcessor {
public:
	virtual ~ILookupResultProcessor(){};

	virtual void init() = 0;

	virtual void reset() = 0;

	virtual void addLookupResult(const LookupResult<T>& result) = 0;

	virtual void retrieveProcessedLookupResults(std::vector<LookupResult<T> >& results, bool toSortResults = true) = 0;
};


#endif /* ILOOKUPRESULTPROCESSOR_H_ */
