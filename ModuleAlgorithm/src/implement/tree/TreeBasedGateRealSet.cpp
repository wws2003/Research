/*
 * TreeBasedGateRealSet.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#include "TreeBasedGateRealSet.h"
#include "IDistanceCalculator.h"
#include "AbstractTreeBasedSet.cpp"

template class AbstractTreeBasedSet<GatePtr, mreal_t>;

TreeBasedGateRealSet::TreeBasedGateRealSet(const std::vector<GatePtr>& pivots,
				DistanceCalculatorPtr<GatePtr> pDistanceCalculator,
				mreal_t distanceToConsiderAsOne,
				RangeSearchTreePtr<GatePtr, mreal_t> pRangeSearchTree,
				const std::vector<mreal_t>& coordinateRanges) : AbstractTreeBasedSet<GatePtr, mreal_t>(pivots,
						pDistanceCalculator,
						distanceToConsiderAsOne,
						pRangeSearchTree,
						coordinateRanges) {

}

void TreeBasedGateRealSet::calculateDistancesToPivots(GatePtr element, std::vector<mreal_t>& distances) {
	//Just calculate as normal
	distances.clear();
	for(GatePtr pPivot : m_pivots) {
		mreal_t distanceToPivot = m_pDistanceCalculator->distance(element, pPivot);
		distances.push_back(distanceToPivot);
	}
}


