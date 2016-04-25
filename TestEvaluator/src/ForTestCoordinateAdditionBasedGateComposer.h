/*
 * ForTestCoordinateAdditionBasedGateComposer.h
 *
 *  Created on: Apr 25, 2016
 *      Author: pham
 */

#ifndef FORTESTCOORDINATEADDITIONBASEDGATECOMPOSER_H_
#define FORTESTCOORDINATEADDITIONBASEDGATECOMPOSER_H_

#include "Coordinate.hpp"
#include "Gate.h"
#include "AlgoInternal.h"
#include "AdditionBasedElementComposer.h"

class ForTestCoordinateAdditionBasedGateComposer: public AdditionBasedElementComposer<RealCoordinate<GatePtr> > {
public:
	ForTestCoordinateAdditionBasedGateComposer(ComparatorPtr<GateRealCoordinate> pElementComparator,
			CombinerPtr<GateRealCoordinate> pCombiner,
			GateRealCoordinate  epsilonElement,
			int maxResultsNumber);

	//Override
	bool isValidCandidate(const GateRealCoordinate& candidate);

	//Override
	void releaseIntermediateResult(GateRealCoordinate&  coord);
};
#endif /* FORTESTCOORDINATEADDITIONBASEDGATECOMPOSER_H_ */
