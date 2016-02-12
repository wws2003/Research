/*
 * CoordinateAdditionBasedGateComposer.h
 *
 *  Created on: Dec 6, 2015
 *      Author: pham
 */

#ifndef COORDINATEADDITIONBASEDGATECOMPOSER_H_
#define COORDINATEADDITIONBASEDGATECOMPOSER_H_

#include "Coordinate.hpp"
#include "Gate.h"
#include "AlgoInternal.h"
#include "AdditionBasedElementComposer.h"

class CoordinateAdditionBasedGateComposer: public AdditionBasedElementComposer<RealCoordinate<GatePtr> > {
public:
	CoordinateAdditionBasedGateComposer(ComparatorPtr<RealCoordinate<GatePtr> > pElementComparator,
			CombinerPtr<RealCoordinate<GatePtr> > pCombiner,
			RealCoordinate<GatePtr>  epsilonElement,
			int maxResultsNumber);

	//Release
	void releaseIntermediateResult(RealCoordinate<GatePtr>  coord);
};



#endif /* COORDINATEADDITIONBASEDGATECOMPOSER_H_ */
