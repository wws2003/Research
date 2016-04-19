/*
 * GateCoordinateCombiningTask.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#include "GateCoordinateCombiningTask.h"

GateCoordinateCombiningTask::GateCoordinateCombiningTask(const std::vector<GateRealCoordinate>& elements,
		CombinerPtr<GateRealCoordinate> pCombiner,
		bool elementsOrder) : m_elements(elements) {
	m_pCombiner = pCombiner;
	m_elementsOrder = elementsOrder;
}

TaskResult<LookupResult<GateRealCoordinate> > GateCoordinateCombiningTask::execute() {
	int resultCode = TaskResultCode::TRC_UNKNOWN;
	GateRealCoordinate combinedCoord;
	composeCandidate(m_elements, combinedCoord);

	resultCode = combinedCoord.getElement() != NullPtr ? TaskResultCode::TRC_VALID : TaskResultCode::TRC_INVALID;
	LookupResult<GateRealCoordinate> lookupResult(combinedCoord, LookupResult<GateRealCoordinate>::DISTANCE_UNKNOWN_VALUE);

	TaskResult<LookupResult<GateRealCoordinate> > result;
	result.m_resultCode = resultCode;
	result.m_executeResult = lookupResult;
	result.m_status = TRS_READY;
	return result;
}

void GateCoordinateCombiningTask::composeCandidate(const std::vector<GateRealCoordinate>& elements,
		GateRealCoordinate& result) {
	int firstElementIndex = m_elementsOrder ? 0 : elements.size() - 1;
	int endElementIndex = m_elementsOrder ? elements.size() : -1;
	int deltaIndex = m_elementsOrder ? 1 : -1;

	GateRealCoordinate combined = elements[firstElementIndex];
	int secondElementIndex = firstElementIndex + deltaIndex;

	for(int i = secondElementIndex; i != endElementIndex; i += deltaIndex) {
		GateRealCoordinate tmp;
		m_pCombiner->combine(combined, elements[i], tmp);
		if(i != secondElementIndex) {
			_destroy(combined.getElement());
		}
		combined = tmp;
	}
	result = combined;
}
