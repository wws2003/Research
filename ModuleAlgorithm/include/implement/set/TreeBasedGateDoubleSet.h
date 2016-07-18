/*
 * TreeBasedGateDoubleSet.h
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#ifndef TREEBASEDGATEDOUBLESET_H_
#define TREEBASEDGATEDOUBLESET_H_

#include "AbstractTreeBasedSet.h"
#include "Gate.h"

class TreeBasedGateDoubleSet : public AbstractTreeBasedSet<GatePtr, double> {
public:
	TreeBasedGateDoubleSet(const std::vector<GatePtr>& pivots,
				DistanceCalculatorPtr<GatePtr> pDistanceCalculator,
				mreal_t distanceToConsiderAsOne,
				RangeSearchTreePtr<GatePtr, double> pRangeSearchTree,
				const std::vector<double>& coordinateRanges);

	virtual ~TreeBasedGateDoubleSet(){};

protected:
	//Override
	void calculateDistancesToPivots(GatePtr element, std::vector<double>& distances);

private:
	double getApprxDistance(MatrixPtr pMatrix1, MatrixPtr pMatrix2);
};




#endif /* TREEBASEDGATEDOUBLESET_H_ */
