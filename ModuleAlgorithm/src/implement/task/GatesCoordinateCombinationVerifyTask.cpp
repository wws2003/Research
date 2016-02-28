/*
 * GatesCoordinateCombinationVerifyTask.cpp
 *
 *  Created on: Feb 26, 2016
 *      Author: pham
 */

#include "GatesCoordinateCombinationVerifyTask.h"

GatesCoordinateCombinationVerifyTask::GatesCoordinateCombinationVerifyTask(const std::vector<GateRealCoordinate>& elements,
		CombinerPtr<GateRealCoordinate> pCombiner,
		DistanceCalculatorPtr<GateRealCoordinate> pDistanceCalculator,
		GateRealCoordinate target,
		mreal_t epsilon,
		bool elementsOrder) : m_elements(elements) {

	m_pCombiner = pCombiner;
	m_pDistanceCalculator = pDistanceCalculator;
	m_target = target;
	m_epsilon = epsilon;
	m_elementsOrder = elementsOrder;
}

TaskResult<LookupResult<GateRealCoordinate> > GatesCoordinateCombinationVerifyTask::execute() {
	int resultCode = TaskResultCode::TRC_UNKNOWN;
	LookupResult<GateRealCoordinate> lookupResult;

	GateRealCoordinate candidate;
	composeCandidate(m_elements, candidate);

	if(candidate.getElement() != NullPtr) {
		if(m_pDistanceCalculator != NullPtr) {
			mreal_t distanceToTarget = m_pDistanceCalculator->distance(candidate, m_target);
			if(distanceToTarget <= m_epsilon) {
				lookupResult.m_distanceToTarget = distanceToTarget;
				resultCode = TaskResultCode::TRC_POSITIVE;
			}
			else {
				candidate.clear();
				resultCode = TaskResultCode::TRC_NEGATIVE;
			}
		}
	}
	lookupResult.m_resultElement = candidate;

	TaskResult<LookupResult<GateRealCoordinate> > result;
	result.m_resultCode = resultCode;
	result.m_executeResult = lookupResult;
	result.m_status = TRS_READY;
	return result;
}

void GatesCoordinateCombinationVerifyTask::composeCandidate(const std::vector<GateRealCoordinate>& elements, GateRealCoordinate& result) {
	int firstElementIndex = m_elementsOrder ? 0 : elements.size() - 1;
	int endElementIndex = m_elementsOrder ? elements.size() : -1;
	int deltaIndex = m_elementsOrder ? 1 : -1;

	GateRealCoordinate combined = elements[firstElementIndex];
	int secondElementIndex = firstElementIndex + deltaIndex;

	for(int i = secondElementIndex; i != endElementIndex && combined.getElement() != NullPtr; i += deltaIndex) {
		GateRealCoordinate tmp;
		m_pCombiner->combine(combined, elements[i], tmp);
		if(i != secondElementIndex) {
			combined.clear();
		}
		combined = tmp;
	}
	result = combined;
}


