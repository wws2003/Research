/*
 * IMatrixCoordinateMapper.h
 *
 *  Created on: May 18, 2015
 *      Author: pham
 */

#ifndef IMATRIXCOORDINATEMAPPER_H_
#define IMATRIXCOORDINATEMAPPER_H_

#include "OperatorCommon.h"
#include "MathConceptsCommon.h"

class IMatrixCoordinateMapper {
public:
	virtual ~IMatrixCoordinateMapper(){};
	virtual void mapMatrixToCoordinate(MatrixPtr pMatrix, MatrixCoordinatePtrRef prMatrixCoordinate) const = 0;
};


#endif /* IMATRIXCOORDINATEMAPPER_H_ */