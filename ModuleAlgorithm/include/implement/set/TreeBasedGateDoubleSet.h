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

	virtual ~TreeBasedGateDoubleSet();

protected:
	//Override
	void calculateDistancesToPivots(GatePtr element, std::vector<double>& distances);

	//Override
	std::string getElementDistanceCacheKey(GatePtr element);

private:
	typedef std::complex<double> DoubleComplex;

	class ApprxDoubleComplexMatrix {
	public:
		ApprxDoubleComplexMatrix(MatrixPtr pMatrix);
		virtual ~ApprxDoubleComplexMatrix();

		DoubleComplex getValue(int row, int col);

	private:
		DoubleComplex* m_array;
		int m_nbRows;
		int m_nbColumns;
	};
	typedef ApprxDoubleComplexMatrix* ApprxDoubleComplexMatrixPtr;

	double getApprxDistance(ApprxDoubleComplexMatrixPtr pMatrix1, MatrixPtr pMatrix2);

	std::vector<ApprxDoubleComplexMatrixPtr> m_apprxPivots;
};




#endif /* TREEBASEDGATEDOUBLESET_H_ */
