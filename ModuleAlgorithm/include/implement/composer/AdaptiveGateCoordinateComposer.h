/*
 * AdaptiveGateCoordinateComposer.h
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#ifndef ADAPTIVEGATECOORDINATECOMPOSER_H_
#define ADAPTIVEGATECOORDINATECOMPOSER_H_

#include "Gate.h"
#include "AdaptiveElementComposer.h"
#include "Coordinate.hpp"

class AdaptiveGateCoordinateComposer : public AdaptiveElementComposer<GatePtr, RealCoordinate<GatePtr> >  {
public:
	AdaptiveGateCoordinateComposer(DistanceCalculatorPtr<RealCoordinate<GatePtr>> pDerivedDistanceCalculator,
			ComposerPtr<RealCoordinate<GatePtr> > pDerivedComposer,
			mreal_t derivedComposerEpsilon,
			ConverterPtr<GatePtr, RealCoordinate<GatePtr> > pConverter,
			bool toVerifyDerivedResults = true);

	AdaptiveGateCoordinateComposer(DistanceCalculatorPtr<RealCoordinate<GatePtr>> pDerivedDistanceCalculator,
				ComposerPtr<RealCoordinate<GatePtr> > pDerivedComposer,
				ConverterPtr<GatePtr, RealCoordinate<GatePtr> > pConverter,
				bool toVerifyDerivedResults = true);

protected:
	//Override
	void releaseDerivedItem(RealCoordinate<GatePtr>& gateCoord);
};

#endif /* ADAPTIVEGATECOORDINATECOMPOSER_H_ */
