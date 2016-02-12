/*
 * CoordinateAdditionBasedGateComposer.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: pham
 */

#include "CoordinateAdditionBasedGateComposer.h"
#include "AdditionBasedElementComposer.cpp"

template class AdditionBasedElementComposer<RealCoordinate<GatePtr> >;

CoordinateAdditionBasedGateComposer::CoordinateAdditionBasedGateComposer(ComparatorPtr<RealCoordinate<GatePtr> > pElementComparator,
			CombinerPtr<RealCoordinate<GatePtr> > pCombiner,
			RealCoordinate<GatePtr>  epsilonElement,
			int maxResultsNumber) : AdditionBasedElementComposer(pElementComparator, pCombiner, epsilonElement, maxResultsNumber) {

}

void CoordinateAdditionBasedGateComposer::releaseIntermediateResult(RealCoordinate<GatePtr> coord) {
	_destroy(coord.getElement());
}
