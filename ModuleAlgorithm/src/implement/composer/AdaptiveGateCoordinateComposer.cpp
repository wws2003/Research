/*
 * AdaptiveGateCoordinateComposer.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "AdaptiveGateCoordinateComposer.h"
#include "AdaptiveElementComposer.cpp"

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

void AdaptiveGateCoordinateComposer::releaseDerivedElements(std::vector<RealCoordinate<GatePtr> >& derivedElements) {
	for(std::vector<RealCoordinate<GatePtr> >::iterator cIter = derivedElements.begin(); cIter != derivedElements.end();) {
		GatePtr pGate = cIter->getElement();
		cIter = derivedElements.erase(cIter);
		_destroy(pGate);
	}
}

