/*
 * SetBasedLookupResultProcessor.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#include "SetBasedLookupResultProcessor.h"
#include <algorithm>

template<typename T>
SetBasedLookupResultProcessor<T>::SetBasedLookupResultProcessor(DistanceCalculatorPtr<T> pDistanceCalculator) {
	m_pDistanceCalculator = pDistanceCalculator;
}

template<typename T>
void SetBasedLookupResultProcessor<T>::init() {

}

template<typename T>
void SetBasedLookupResultProcessor<T>::reset() {
	m_resultMap.clear();
}

template<typename T>
void SetBasedLookupResultProcessor<T>::addLookupResultsBatch(const std::vector<LookupResult<T> >& result) {
	for(unsigned int i = 0; i < result.size(); i++) {
		addLookupResult(result[i]);
	}
}

template<typename T>
void SetBasedLookupResultProcessor<T>::addLookupResult(const LookupResult<T>& result) {
	bool isDuplicate;
	typename rmap::iterator insertHintIter = checkDuplicateResult(result, isDuplicate);
	if(!isDuplicate) {
		m_resultMap.insert(insertHintIter, result);
	}
}

template<typename T>
void SetBasedLookupResultProcessor<T>::retrieveProcessedLookupResults(std::vector<LookupResult<T> >& results, bool toSortResults) {
	results.reserve(m_resultMap.size());
	results.insert(results.end(), m_resultMap.begin(), m_resultMap.end());
}

template<typename T>
typename SetBasedLookupResultProcessor<T>::rmap::iterator SetBasedLookupResultProcessor<T>::checkDuplicateResult(const LookupResult<T>& result, bool& isDuplicate) const {

	static const mreal_t DISTANCE_TO_CONSIDER_AS_ONE = 1e-8;

	isDuplicate = false;
	T lElement = result.m_resultElement;
	mreal_t lDistanceToTarget = result.m_distanceToTarget;

	LookupResult<T> lowerResult(NullPtr, lDistanceToTarget - DISTANCE_TO_CONSIDER_AS_ONE);
	LookupResult<T> upperResult(NullPtr, lDistanceToTarget + DISTANCE_TO_CONSIDER_AS_ONE);

	typename rmap::const_iterator lIter = m_resultMap.lower_bound(lowerResult);
	typename rmap::const_iterator uIter = m_resultMap.upper_bound(upperResult);

	for(typename rmap::const_iterator rIter = lIter; rIter != uIter; rIter++) {
		T rElement = rIter->m_resultElement;

		if(sameElement(lElement, rElement) || m_pDistanceCalculator->distance(lElement, rElement) <= DISTANCE_TO_CONSIDER_AS_ONE) {
			isDuplicate = true;
			return rIter;
		}
	}

	return uIter;
}



