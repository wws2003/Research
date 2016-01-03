/*
 * CoordinateAdditionBasedMatrixComposer.h
 *
 *  Created on: Dec 6, 2015
 *      Author: pham
 */

#ifndef COORDINATEADDITIONBASEDMATRIXCOMPOSER_H_
#define COORDINATEADDITIONBASEDMATRIXCOMPOSER_H_

#include "Coordinate.hpp"
#include "IMatrix.h"
#include "AlgoInternal.h"
#include "AdditionBasedElementComposer.h"

typedef class AdditionBasedElementComposer<RealCoordinate<MatrixPtr> > CoordinateAdditionBasedMatrixComposer;


#endif /* COORDINATEADDITIONBASEDMATRIXCOMPOSER_H_ */
