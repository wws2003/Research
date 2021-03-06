/*
 * AdaptiveMatrixCoordinateComposer.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "AdaptiveMatrixCoordinateComposer.h"
#include "AdaptiveElementComposer.cpp"

template class AdaptiveElementComposer<MatrixPtr, RealCoordinate<MatrixPtr> >;

void AdaptiveMatrixCoordinateComposer::releaseDerivedItem(RealCoordinate<MatrixPtr>& matrixCoord) {
	_destroy(matrixCoord.getElement());
	matrixCoord.setElement(NullPtr);
}
