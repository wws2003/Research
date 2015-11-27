/*
 * DuplicateLookupResultFilterImpl.h
 *
 *  Created on: Nov 27, 2015
 *      Author: pham
 */

#ifndef DUPLICATELOOKUPRESULTFILTERIMPL_H_
#define DUPLICATELOOKUPRESULTFILTERIMPL_H_

#include "ILookupResultFilter.h"

template<typename T>
class DuplicateLookupResultFilterImpl: public ILookupResultFilter<T> {
public:
	virtual ~DuplicateLookupResultFilterImpl(){};

	//Override
	void filterLookupResults(std::vector<LookupResult<T> >& lookupResults,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			bool toCloneResult = false);

private:
	LookupResult<T> cloneLookupResult(const LookupResult<T>& lookupResult);
};


#endif /* DUPLICATELOOKUPRESULTFILTERIMPL_H_ */
