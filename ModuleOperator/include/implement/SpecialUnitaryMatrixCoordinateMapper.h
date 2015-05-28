/*
 * SpecialUnitaryMatrixCoordinateMapper.h
 *
 *  Created on: May 23, 2015
 *      Author: pham
 */

#ifndef SPECIALUNITARYMATRIXCOORDINATEMAPPER_H_
#define SPECIALUNITARYMATRIXCOORDINATEMAPPER_H_

#include "IMatrixOperator.h"
#include "ICoordinateCalculator.h"

class SpecialUnitaryMatrixCoordinateMapper : public ICoordinateCalculator<MatrixPtr, double> {
public:
	SpecialUnitaryMatrixCoordinateMapper(MatrixOperatorPtr pMatrixOperator);
	virtual ~SpecialUnitaryMatrixCoordinateMapper(){};

	virtual void calulateElementCoordinate(MatrixPtr pMatrix, MatrixCoordinatePtrRef prMatrixCoordinate) const;

private:
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* SPECIALUNITARYMATRIXCOORDINATEMAPPER_H_ */
