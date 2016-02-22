/*
 * ElementsCombinationVerifyTask.cpp
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#include "ElementsCombinationVerifyTask.h"

template<typename T>
ElementsCombinationVerifyTask<T>::ElementsCombinationVerifyTask(const std::vector<T>& elements,
		CombinerPtr<T> pCombiner,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		T target,
		mreal_t epsilon) : m_elements(elements) {

	m_pCombiner = pCombiner;
	m_pDistanceCalculator = pDistanceCalculator;
	m_target = target;
	m_epsilon = epsilon;
}

template<typename T>
TaskResult<LookupResult<T> > ElementsCombinationVerifyTask<T>::execute() {
	int resultCode = TaskResultCode::TRC_UNKNOWN;
	LookupResult<T> lookupResult(NullPtr, LookupResult<T>::DISTANCE_UNKNOWN_VALUE);

	T candidate;
	composeCandidate(m_elements, candidate);
	if(candidate != NullPtr) {
		if(m_pDistanceCalculator != NullPtr) {
			mreal_t distanceToTarget = m_pDistanceCalculator->distance(candidate, m_target);
			if(distanceToTarget <= m_epsilon) {
				lookupResult.m_distanceToTarget = distanceToTarget;
				lookupResult.m_resultElement = candidate;
				onCombinationVerified(lookupResult);
				resultCode = TaskResultCode::TRC_POSITIVE;
			}
			else {
				_destroy(candidate);
				resultCode = TaskResultCode::TRC_NEGATIVE;
			}
		}
		else {
			//Store candidate in result for future purpose
			lookupResult.m_resultElement = candidate;
		}
	}

	TaskResult<LookupResult<T> > result;
	result.m_resultCode = resultCode;
	result.m_executeResult = lookupResult;
	result.m_status = TRS_READY;
	return result;
}

template<typename T>
void ElementsCombinationVerifyTask<T>::composeCandidate(const std::vector<T>& elements, T& result) {
	T combined = elements.empty() ? NullPtr : elements[0]->clone();
	for(unsigned int i = 1; i < elements.size() && combined != NullPtr; i++) {
		if(elements[i] != NullPtr) {
			T tmp = NullPtr;
			m_pCombiner->combine(combined, elements[i], tmp);
			_destroy(combined);
			combined = tmp;
		}
	}
	result = combined;
}

template<typename T>
void ElementsCombinationVerifyTask<T>::onCombinationVerified(const LookupResult<T>& verifiedResult) {
	//Do nothing here. Possilby override in sub-class
}

