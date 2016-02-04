/*
 * AdaptiveMatrixCoordinateComposer.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "AdaptiveMatrixCoordinateComposer.h"
#include "AdaptiveElementComposer.cpp"

template class AdaptiveElementComposer<MatrixPtr, RealCoordinate<MatrixPtr> >;

void AdaptiveMatrixCoordinateComposer::releaseDerivedElements(std::vector<RealCoordinate<MatrixPtr> >& derivedElements) {
	for(std::vector<RealCoordinate<MatrixPtr> >::iterator cIter = derivedElements.begin(); cIter != derivedElements.end();) {
		MatrixPtr pMatrix = cIter->getElement();
		cIter = derivedElements.erase(cIter);
		_destroy(pMatrix);
	}
}

