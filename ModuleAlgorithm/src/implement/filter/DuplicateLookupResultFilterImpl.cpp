/*
 * DuplicateLookupResultFilterImpl.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: pham
 */

#include "DuplicateLookupResultFilterImpl.h"
#include <cassert>
#include <algorithm>

template<typename T>
void DuplicateLookupResultFilterImpl<T>::filterLookupResults(std::vector<LookupResult<T> >& lookupResults,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		bool toCloneResult) {

	if(lookupResults.empty()) {
		return;
	}

	const mreal_t distanceToConsiderAsOne = 1e-8;

	int nonDuplicateLastPivot = 0;

	for(unsigned int i = 1; i < lookupResults.size(); i++) {
		bool duplicated = isDuplicateFound(lookupResults,
				nonDuplicateLastPivot,
				lookupResults[i],
				pDistanceCalculator);

		if(!duplicated) {
			if(nonDuplicateLastPivot + 1 != i) {
				lookupResults[nonDuplicateLastPivot + 1] = lookupResults[i];
			}
			nonDuplicateLastPivot++;
		}
	}

	if(toCloneResult) {
		for(unsigned int i = 0; i <= nonDuplicateLastPivot; i++) {
			lookupResults[i] = cloneLookupResult(lookupResults[i]);
		}
	}

	lookupResults.resize(nonDuplicateLastPivot + 1, LookupResult<T>(NullPtr, 0.0));
}

template<typename T>
LookupResult<T> DuplicateLookupResultFilterImpl<T>::cloneLookupResult(const LookupResult<T>& lookupResult) {
	LookupResult<T> clonedResult(lookupResult.m_resultElement->clone(), lookupResult.m_distanceToTarget);
	return clonedResult;
}

template<typename T>
bool DuplicateLookupResultFilterImpl<T>::isDuplicateFound(const std::vector<LookupResult<T> >& lookupResults,
		int nonDuplicateLastPivot,
		LookupResult<T> result,
		DistanceCalculatorPtr<T> pDistanceCalculator) {

	bool duplicated = false;
	T lElement = result.m_resultElement;
	mreal_t lDistanceToTarget = result.m_distanceToTarget;

	const mreal_t distanceToConsiderAsOne = 1e-7;

	//Evaluate duplicated
	for(unsigned int j = 0; j <= nonDuplicateLastPivot; j++) {
		T rElement = lookupResults[j].m_resultElement;
		//Raw estimate first
		if(sameElement(rElement, lElement)) {
			return true;
		}

		mreal_t rDistanceToTarget = lookupResults[j].m_distanceToTarget;

		//No need to check to far results
		if(mreal::abs(lDistanceToTarget - rDistanceToTarget) > distanceToConsiderAsOne) {
			continue;
		}
		if(pDistanceCalculator->distance(lElement, rElement) <= distanceToConsiderAsOne) {
			return true;
		}
	}
	return duplicated;

	//Test in case of  sorted vector
	/*LookupResult<T> lowerResult(NullPtr, lDistanceToTarget - distanceToConsiderAsOne);
		LookupResult<T> upperResult(NullPtr, lDistanceToTarget + distanceToConsiderAsOne);

		typename std::vector<LookupResult<T> >::const_iterator beginIter = lookupResults.begin();
		typename std::vector<LookupResult<T> >::const_iterator endIter = lookupResults.begin() + nonDuplicateLastPivot + 1;

		typename std::vector<LookupResult<T> >::const_iterator lIter = std::lower_bound(beginIter, endIter, lowerResult, DistanceComparator<T>());
		typename std::vector<LookupResult<T> >::const_iterator uIter = std::lower_bound(beginIter, endIter, upperResult, DistanceComparator<T>());

		for(typename std::vector<LookupResult<T> >::const_iterator rIter = lIter; rIter != uIter; rIter++) {
			T rElement = rIter->m_resultElement;
			mreal_t rDistanceToTarget = rIter->m_distanceToTarget;

			if(pDistanceCalculator->distance(lElement, rElement) <= distanceToConsiderAsOne) {
				return true;
			}
		}*/
}
