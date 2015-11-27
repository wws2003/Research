/*
 * DuplicateLookupResultFilterImpl.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: pham
 */

#include "DuplicateLookupResultFilterImpl.h"
#include <cassert>

template<typename T>
void DuplicateLookupResultFilterImpl<T>::filterLookupResults(std::vector<LookupResult<T> >& lookupResults,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		bool toCloneResult) {

	std::vector<LookupResult<T> > resultsBuffer;

	const mreal_t distanceToConsiderAsOne = 1e-7;

	for(typename std::vector<LookupResult<T> >::iterator rIter = lookupResults.begin(); rIter != lookupResults.end();) {
		LookupResult<T> result = *rIter;
		T lElement = result.m_resultElement;
		mreal_t lDistanceToTarget = result.m_distanceToTarget;

		//Check if the result is actually a duplicate of some other
		bool duplicated = false;
		for(unsigned int j = 0; j < resultsBuffer.size() && !duplicated; j++) {
			T rElement = resultsBuffer[j].m_resultElement;
			mreal_t rDistanceToTarget = resultsBuffer[j].m_distanceToTarget;

			//No need to check to far results
			if(mreal::abs(lDistanceToTarget - rDistanceToTarget) > distanceToConsiderAsOne) {
				continue;
			}

			if(pDistanceCalculator->distance(lElement, rElement) <= distanceToConsiderAsOne) {
				duplicated = true;
			}
		}

		//Push to result buffer the unique result
		if(!duplicated) {
			resultsBuffer.push_back(toCloneResult ? cloneLookupResult(result) : result);
		}

		rIter = lookupResults.erase(rIter);
	}

	//Push back unique results
	lookupResults.insert(lookupResults.end(), resultsBuffer.begin(), resultsBuffer.end());
}

template<typename T>
LookupResult<T> DuplicateLookupResultFilterImpl<T>::cloneLookupResult(const LookupResult<T>& lookupResult) {
	LookupResult<T> clonedResult(lookupResult.m_resultElement->clone(), lookupResult.m_distanceToTarget);
	return clonedResult;
}

