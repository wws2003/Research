/*
 * MultipleComparatorCoordinateAdditionBasedGateComposer.cpp
 *
 *  Created on: Feb 12, 2016
 *      Author: pham
 */

#include "MultipleComparatorCoordinateAdditionBasedGateComposer.h"
#include "MultipleComparatorAdditionBasedElementComposer.cpp"

template class MultipleComparatorAdditionBasedElementComposer<RealCoordinate<GatePtr> >;

MultipleComparatorCoordinateAdditionBasedGateComposer::MultipleComparatorCoordinateAdditionBasedGateComposer(ComparatorPtr<RealCoordinate<GatePtr> > pElementComparator,
			CombinerPtr<RealCoordinate<GatePtr> > pCombiner,
			RealCoordinate<GatePtr>  epsilonElement,
			int maxResultsNumber,
			const std::vector<ComparatorPtr<RealCoordinate<GatePtr> > >& secondaryComparators,
			const std::vector<RealCoordinate<GatePtr> > secondaryEpsilonElements) : MultipleComparatorAdditionBasedElementComposer<RealCoordinate<GatePtr> >(pElementComparator,
					pCombiner,
					epsilonElement,
					maxResultsNumber,
					secondaryComparators,
					secondaryEpsilonElements
					) {

}

bool MultipleComparatorCoordinateAdditionBasedGateComposer::isValidCandidate(const GateRealCoordinate& candidate) {
	return candidate.getElement() != NullPtr;
}

void MultipleComparatorCoordinateAdditionBasedGateComposer::releaseIntermediateResult(GateRealCoordinate& coord) {
	coord.clear();
}


