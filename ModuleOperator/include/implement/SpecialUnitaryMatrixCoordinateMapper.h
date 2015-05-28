/*
 * SpecialUnitaryMatrixCoordinateMapper.h
 *
 *  Created on: May 23, 2015
 *      Author: pham
 */

#ifndef SPECIALUNITARYMATRIXCOORDINATEMAPPER_H_
#define SPECIALUNITARYMATRIXCOORDINATEMAPPER_H_

#include "IMatrixCoordinateMapper.h"
#include "IMatrixOperator.h"

class SpecialUnitaryMatrixCoordinateMapper : public IMatrixCoordinateMapper{
public:
	SpecialUnitaryMatrixCoordinateMapper(MatrixOperatorPtr pMatrixOperator);
	virtual ~SpecialUnitaryMatrixCoordinateMapper(){};

	virtual void calulateElementCoordinate(MatrixPtr pMatrix, MatrixCoordinatePtrRef prMatrixCoordinate) const;

private:
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* SPECIALUNITARYMATRIXCOORDINATEMAPPER_H_ */
