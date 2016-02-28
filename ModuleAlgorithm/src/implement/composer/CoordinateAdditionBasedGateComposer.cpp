/*
 * CoordinateAdditionBasedGateComposer.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: pham
 */

#include "CoordinateAdditionBasedGateComposer.h"
#include "AdditionBasedElementComposer.cpp"

template class AdditionBasedElementComposer<RealCoordinate<GatePtr> >;

CoordinateAdditionBasedGateComposer::CoordinateAdditionBasedGateComposer(ComparatorPtr<GateRealCoordinate> pElementComparator,
			CombinerPtr<GateRealCoordinate> pCombiner,
			GateRealCoordinate  epsilonElement,
			int maxResultsNumber) : AdditionBasedElementComposer(pElementComparator, pCombiner, epsilonElement, maxResultsNumber) {

}

bool CoordinateAdditionBasedGateComposer::isValidCandidate(const GateRealCoordinate& candidate) {
	return candidate.getElement() != NullPtr;
}

void CoordinateAdditionBasedGateComposer::releaseIntermediateResult(RealCoordinate<GatePtr> coord) {
	_destroy(coord.getElement());
}
