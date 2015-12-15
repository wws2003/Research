/*
 * BackgroundLookupResultsFilterProcessor.cpp
 *
 *  Created on: Dec 15, 2015
 *      Author: pham
 */

#include "BackgroundLookupResultsFilterProcessor.h"

template<typename T>
BackgroundLookupResultsFilterProcessor<T>::BackgroundLookupResultsFilterProcessor(DistanceCalculatorPtr<T> pDistanceCalculator) : BackgroundLookupResultProcessor<T>(){
	m_pDistanceCalculator = pDistanceCalculator;
}

template<typename T>
void BackgroundLookupResultsFilterProcessor<T>::processOnBackground(const LookupResult<T>& result) {
	bool isDuplicate;

	typename rmap::iterator insertHintIter = checkDuplicateResult(result, isDuplicate);

	if(!isDuplicate) {
		m_resultMap.insert(insertHintIter, result);
	}
}

template<typename T>
void BackgroundLookupResultsFilterProcessor<T>::exportInternalContainerToResults(std::vector<LookupResult<T> >& results, bool toSortResults) const {
	results.reserve(m_resultMap.size());
	results.insert(results.end(), m_resultMap.begin(), m_resultMap.end());
}

template<typename T>
void BackgroundLookupResultsFilterProcessor<T>::resetInternalContainer() {
	m_resultMap.clear();
}

template<typename T>
typename BackgroundLookupResultsFilterProcessor<T>::rmap::iterator BackgroundLookupResultsFilterProcessor<T>::checkDuplicateResult(const LookupResult<T>& result, bool& isDuplicate) const {

	static const mreal_t DISTANCE_TO_CONSIDER_AS_ONE = 1e-8;

	isDuplicate = false;
	T lElement = result.m_resultElement;
	mreal_t lDistanceToTarget = result.m_distanceToTarget;

	LookupResult<T> lowerResult(NullPtr, lDistanceToTarget - DISTANCE_TO_CONSIDER_AS_ONE);
	LookupResult<T> upperResult(NullPtr, lDistanceToTarget + DISTANCE_TO_CONSIDER_AS_ONE);

	typename rmap::const_iterator lIter = m_resultMap.lower_bound(lowerResult);
	typename rmap::const_iterator uIter = m_resultMap.lower_bound(upperResult);

	for(typename rmap::const_iterator rIter = lIter; rIter != uIter; rIter++) {
		T rElement = rIter->m_resultElement;
		mreal_t rDistanceToTarget = rIter->m_distanceToTarget;

		if(m_pDistanceCalculator->distance(lElement, rElement) <= DISTANCE_TO_CONSIDER_AS_ONE) {
			isDuplicate = true;
			return rIter;
		}
	}

	return uIter;
}
