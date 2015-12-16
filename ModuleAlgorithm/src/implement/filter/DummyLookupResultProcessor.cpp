/*
 * DummyLookupResultProcessor.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#include "DummyLookupResultProcessor.h"
#include <algorithm>

template<typename T>
DummyLookupResultProcessor<T>::DummyLookupResultProcessor(DistanceCalculatorPtr<T> pDistanceCalculator, storage_t storageType) {
	m_pDistanceCalculator = pDistanceCalculator;
	m_storageType = storageType;
}

template<typename T>
void DummyLookupResultProcessor<T>::init() {

}

template<typename T>
void DummyLookupResultProcessor<T>::reset() {
	m_storage.clear();
	m_resultMap.clear();
}

template<typename T>
void DummyLookupResultProcessor<T>::addLookupResultsBatch(const std::vector<LookupResult<T> >& result) {
	//m_storage.insert(m_storage.end(), result.begin(), result.end());
}

template<typename T>
void DummyLookupResultProcessor<T>::addLookupResult(const LookupResult<T>& result) {

	switch (m_storageType) {
	case STRG_SET:
	{
		bool isDuplicate;
		typename rmap::iterator insertHintIter = checkDuplicateResult(result, isDuplicate);
		if(!isDuplicate) {
			m_resultMap.insert(insertHintIter, result);
		}
	}
	break;
	case STRG_VECTOR:
		m_storage.push_back(result);
		break;
	default:
		break;
	}

}

template<typename T>
void DummyLookupResultProcessor<T>::retrieveProcessedLookupResults(std::vector<LookupResult<T> >& results, bool toSortResults) {
	switch (m_storageType) {
	case STRG_SET:
		results.reserve(m_resultMap.size());
		results.insert(results.end(), m_resultMap.begin(), m_resultMap.end());
		break;
	default:
		results.reserve(m_resultMap.size());
		results.insert(results.end(), m_storage.begin(), m_storage.end());
		filterLookupResults(results, m_pDistanceCalculator);
		if(toSortResults) {
			std::sort(results.begin(), results.end(), DistanceComparator<T>());
		}
		break;
	}
}

template<typename T>
typename DummyLookupResultProcessor<T>::rmap::iterator DummyLookupResultProcessor<T>::checkDuplicateResult(const LookupResult<T>& result, bool& isDuplicate) const {

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

template<typename T>
void DummyLookupResultProcessor<T>::filterLookupResults(std::vector<LookupResult<T> >& lookupResults,
		DistanceCalculatorPtr<T> pDistanceCalculator) {

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

	lookupResults.resize(nonDuplicateLastPivot + 1, LookupResult<T>(NullPtr, 0.0));
}

template<typename T>
bool DummyLookupResultProcessor<T>::isDuplicateFound(const std::vector<LookupResult<T> >& lookupResults,
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

		mreal_t rDistanceToTarget = lookupResults[j].m_distanceToTarget;

		//No need to check to far results
		if(mreal::abs(lDistanceToTarget - rDistanceToTarget) > distanceToConsiderAsOne) {
			continue;
		}
		if(sameElement(lElement, rElement) || pDistanceCalculator->distance(lElement, rElement) <= distanceToConsiderAsOne) {
			return true;
		}
	}
	return duplicated;
}



