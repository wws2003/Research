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

protected:
	virtual bool sameElement(const T& t1, const T& t2) const = 0;

private:
	bool isDuplicateFound(const std::vector<LookupResult<T> >& lookupResults,
			int nonDuplicateLastPivot,
			LookupResult<T> result,
			DistanceCalculatorPtr<T> pDistanceCalculator);

	LookupResult<T> cloneLookupResult(const LookupResult<T>& lookupResult);
};


#endif /* DUPLICATELOOKUPRESULTFILTERIMPL_H_ */
