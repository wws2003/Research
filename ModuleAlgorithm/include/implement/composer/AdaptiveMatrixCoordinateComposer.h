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

typedef class AdaptiveElementComposer<MatrixPtr, RealCoordinate<MatrixPtr> > AdaptiveMatrixCoordinateComposer;

#endif /* ADAPTIVEMATRIXCOORDINATECOMPOSER_H_ */
