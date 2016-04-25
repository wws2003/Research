/*
 * ForTestCoordinateAdditionBasedGateComposer.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: pham
 */

#include "ForTestCoordinateAdditionBasedGateComposer.h"
#include "AdditionBasedElementComposer.cpp"

template class AdditionBasedElementComposer<RealCoordinate<GatePtr> >;

ForTestCoordinateAdditionBasedGateComposer::ForTestCoordinateAdditionBasedGateComposer(ComparatorPtr<GateRealCoordinate> pElementComparator,
			CombinerPtr<GateRealCoordinate> pCombiner,
			GateRealCoordinate  epsilonElement,
			int maxResultsNumber) : AdditionBasedElementComposer(pElementComparator, pCombiner, epsilonElement, maxResultsNumber) {

}

bool ForTestCoordinateAdditionBasedGateComposer::isValidCandidate(const GateRealCoordinate& candidate) {
	return true;
}

void ForTestCoordinateAdditionBasedGateComposer::releaseIntermediateResult(GateRealCoordinate& coord) {
	coord.clear();
}

