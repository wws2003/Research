/*
 * VectorBasedLookupResultProcessor.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#include "VectorBasedLookupResultProcessor.h"
#include <algorithm>

template<typename T>
VectorBasedLookupResultProcessor<T>::VectorBasedLookupResultProcessor(DistanceCalculatorPtr<T> pDistanceCalculator) {
	m_pDistanceCalculator = pDistanceCalculator;
}

template<typename T>
void VectorBasedLookupResultProcessor<T>::init() {

}

template<typename T>
void VectorBasedLookupResultProcessor<T>::reset() {
	m_storage.clear();
}

template<typename T>
void VectorBasedLookupResultProcessor<T>::addLookupResultsBatch(const std::vector<LookupResult<T> >& result) {
	m_storage.insert(m_storage.end(), result.begin(), result.end());
}

template<typename T>
void VectorBasedLookupResultProcessor<T>::addLookupResult(const LookupResult<T>& result) {
	m_storage.push_back(result);
}

template<typename T>
void VectorBasedLookupResultProcessor<T>::retrieveProcessedLookupResults(std::vector<LookupResult<T> >& results, bool toSortResults) {
	results.reserve(m_storage.size());
	results.insert(results.end(), m_storage.begin(), m_storage.end());
	filterLookupResults(results, m_pDistanceCalculator);
	if(toSortResults) {
		std::sort(results.begin(), results.end(), DistanceComparator<T>());
	}
}

template<typename T>
void VectorBasedLookupResultProcessor<T>::filterLookupResults(std::vector<LookupResult<T> >& lookupResults,
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
bool VectorBasedLookupResultProcessor<T>::isDuplicateFound(const std::vector<LookupResult<T> >& lookupResults,
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



