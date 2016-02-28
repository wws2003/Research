/*
 * MultipleComparatorCoordinateAdditionalBasedGateComposer.h
 *
 *  Created on: Feb 12, 2016
 *      Author: pham
 */

#ifndef MULTIPLECOMPARATORCOORDINATEADDITIONALBASEDGATECOMPOSER_H_
#define MULTIPLECOMPARATORCOORDINATEADDITIONALBASEDGATECOMPOSER_H_

#include "Coordinate.hpp"
#include "Gate.h"
#include "AlgoInternal.h"
#include "MultipleComparatorAdditionBasedElementComposer.h"

class MultipleComparatorCoordinateAdditionBasedGateComposer: public MultipleComparatorAdditionBasedElementComposer<RealCoordinate<GatePtr> > {
public:
	MultipleComparatorCoordinateAdditionBasedGateComposer(ComparatorPtr<RealCoordinate<GatePtr> > pElementComparator,
			CombinerPtr<RealCoordinate<GatePtr> > pCombiner,
			RealCoordinate<GatePtr> epsilonElement,
			int maxResultsNumber,
			const std::vector<ComparatorPtr<RealCoordinate<GatePtr> > >& secondaryComparators,
			const std::vector<RealCoordinate<GatePtr> > secondaryEpsilonElements);

	//Override
	bool isValidCandidate(const GateRealCoordinate& candidate);

	//Override
	void releaseIntermediateResult(RealCoordinate<GatePtr>  coord);
};

#endif /* MULTIPLECOMPARATORCOORDINATEADDITIONALBASEDGATECOMPOSER_H_ */
