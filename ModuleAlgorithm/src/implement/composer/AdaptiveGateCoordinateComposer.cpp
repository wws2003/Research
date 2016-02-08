/*
 * AdaptiveGateCoordinateComposer.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "AdaptiveGateCoordinateComposer.h"
#include "AdaptiveElementComposer.cpp"
#include <iostream>

template class AdaptiveElementComposer<GatePtr, RealCoordinate<GatePtr> >;

AdaptiveGateCoordinateComposer::AdaptiveGateCoordinateComposer(DistanceCalculatorPtr<RealCoordinate<GatePtr>> pDerivedDistanceCalculator,
		ComposerPtr<RealCoordinate<GatePtr> > pDerivedComposer,
		mreal_t derivedComposerEpsilon,
		ConverterPtr<GatePtr, RealCoordinate<GatePtr> > pConverter) :  AdaptiveElementComposer<GatePtr, RealCoordinate<GatePtr> >(
				pDerivedDistanceCalculator,
				pDerivedComposer,
				derivedComposerEpsilon,
				pConverter
		){

}

void AdaptiveGateCoordinateComposer::releaseDerivedItem(RealCoordinate<GatePtr>& gateCoord) {
	_destroy(gateCoord.getElement());
	gateCoord.setElement(NullPtr);
}
