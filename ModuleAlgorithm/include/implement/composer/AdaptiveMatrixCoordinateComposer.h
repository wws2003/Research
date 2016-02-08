/*
 * AdaptiveMatrixCoordinateComposer.h
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#ifndef ADAPTIVEMATRIXCOORDINATECOMPOSER_H_
#define ADAPTIVEMATRIXCOORDINATECOMPOSER_H_

#include "IMatrix.h"
#include "AdaptiveElementComposer.h"
#include "Coordinate.hpp"

class AdaptiveMatrixCoordinateComposer: public AdaptiveElementComposer<MatrixPtr, RealCoordinate<MatrixPtr> >  {
protected:
	//Override
	void releaseDerivedItem(RealCoordinate<MatrixPtr>& matrixCoord);
};

#endif /* ADAPTIVEMATRIXCOORDINATECOMPOSER_H_ */
