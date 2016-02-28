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
	CoordinateAdditionBasedGateComposer(ComparatorPtr<GateRealCoordinate> pElementComparator,
			CombinerPtr<GateRealCoordinate> pCombiner,
			GateRealCoordinate  epsilonElement,
			int maxResultsNumber);

	//Override
	bool isValidCandidate(const GateRealCoordinate& candidate);

	//Override
	void releaseIntermediateResult(GateRealCoordinate&  coord);
};



#endif /* COORDINATEADDITIONBASEDGATECOMPOSER_H_ */
