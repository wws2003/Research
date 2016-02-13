/*
 * MultipleComparatorCoordinateAdditionBasedMatrixComposer.cpp
 *
 *  Created on: Feb 12, 2016
 *      Author: pham
 */


#include "MultipleComparatorCoordinateAdditionBasedMatrixComposer.h"
#include "MultipleComparatorAdditionBasedElementComposer.cpp"

template class MultipleComparatorAdditionBasedElementComposer<RealCoordinate<MatrixPtr> >;

MultipleComparatorCoordinateAdditionBasedMatrixComposer::MultipleComparatorCoordinateAdditionBasedMatrixComposer(ComparatorPtr<RealCoordinate<MatrixPtr> > pElementComparator,
			CombinerPtr<RealCoordinate<MatrixPtr> > pCombiner,
			RealCoordinate<MatrixPtr>  epsilonElement,
			int maxResultsNumber,
			const std::vector<ComparatorPtr<RealCoordinate<MatrixPtr> > >& secondaryComparators,
			const std::vector<RealCoordinate<MatrixPtr> > secondaryEpsilonElements) : MultipleComparatorAdditionBasedElementComposer<RealCoordinate<MatrixPtr> >(pElementComparator,
					pCombiner,
					epsilonElement,
					maxResultsNumber,
					secondaryComparators,
					secondaryEpsilonElements
					) {

}

void MultipleComparatorCoordinateAdditionBasedMatrixComposer::releaseIntermediateResult(RealCoordinate<MatrixPtr> coord) {
	_destroy(coord.getElement());
}


