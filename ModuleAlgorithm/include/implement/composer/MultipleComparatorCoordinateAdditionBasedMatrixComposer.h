/*
 * MultipleComparatorCoordinateAdditionBasedMatrixComposer.h
 *
 *  Created on: Feb 12, 2016
 *      Author: pham
 */

#ifndef MULTIPLECOMPARATORCOORDINATEADDITIONBASEDMATRIXCOMPOSER_H_
#define MULTIPLECOMPARATORCOORDINATEADDITIONBASEDMATRIXCOMPOSER_H_

#include "Coordinate.hpp"
#include "IMatrix.h"
#include "MathConceptsCommon.h"
#include "AlgoInternal.h"
#include "MultipleComparatorAdditionBasedElementComposer.h"

class MultipleComparatorCoordinateAdditionBasedMatrixComposer: public MultipleComparatorAdditionBasedElementComposer<RealCoordinate<MatrixPtr> > {
public:
	MultipleComparatorCoordinateAdditionBasedMatrixComposer(ComparatorPtr<RealCoordinate<MatrixPtr> > pElementComparator,
			CombinerPtr<RealCoordinate<MatrixPtr> > pCombiner,
			RealCoordinate<MatrixPtr>  epsilonElement,
			int maxResultsNumber,
			const std::vector<ComparatorPtr<RealCoordinate<MatrixPtr> > >& secondaryComparators,
			const std::vector<RealCoordinate<MatrixPtr> > secondaryEpsilonElements);

	//Override
	bool isValidCandidate(const MatrixRealCoordinate& candidate);

	//Override
	void releaseIntermediateResult(RealCoordinate<MatrixPtr>  coord);
};



#endif /* MULTIPLECOMPARATORCOORDINATEADDITIONBASEDMATRIXCOMPOSER_H_ */
