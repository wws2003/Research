/*
 * TreeBasedGateRealSet.h
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#ifndef TREEBASEDGATEREALSET_H_
#define TREEBASEDGATEREALSET_H_

#include "AbstractTreeBasedSet.h"
#include "Gate.h"

class TreeBasedGateRealSet : public AbstractTreeBasedSet<GatePtr, mreal_t> {
public:
	TreeBasedGateRealSet(const std::vector<GatePtr>& pivots,
				DistanceCalculatorPtr<GatePtr> pDistanceCalculator,
				mreal_t distanceToConsiderAsOne,
				RangeSearchTreePtr<GatePtr, mreal_t> pRangeSearchTree,
				const std::vector<mreal_t>& coordinateRanges);

	virtual ~TreeBasedGateRealSet(){};

protected:
	//Override
	void calculateDistancesToPivots(GatePtr element, std::vector<mreal_t>& distances);
};


#endif /* TREEBASEDGATEREALSET_H_ */
