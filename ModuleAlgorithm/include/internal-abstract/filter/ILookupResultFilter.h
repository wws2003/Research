/*
 * ILookupResultFilter.h
 *
 *  Created on: Nov 27, 2015
 *      Author: pham
 */

#ifndef ILOOKUPRESULTFILTER_H_
#define ILOOKUPRESULTFILTER_H_

#include "AlgoCommon.h"
#include "AlgoInternal.h"
#include "ICollection.h"
#include "IDistanceCalculator.h"

template<typename T>
class ILookupResultFilter {
public:
	virtual ~ILookupResultFilter(){};

	virtual void filterLookupResults(std::vector<LookupResult<T> >& lookupResults,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			bool toCloneResult = false) = 0;
};

#endif /* ILOOKUPRESULTFILTER_H_ */
