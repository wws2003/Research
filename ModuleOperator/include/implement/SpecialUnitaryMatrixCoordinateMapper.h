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
#include "OperatorCommon.h"

class SpecialUnitaryMatrixCoordinateMapper : public IMatrixRealCoordinateCalculator {
public:
	SpecialUnitaryMatrixCoordinateMapper(MatrixOperatorPtr pMatrixOperator, MatrixRealCoordinateCalculatorPtr pHermitianMatrixCoordinateCalculator);
	virtual ~SpecialUnitaryMatrixCoordinateMapper(){};

	virtual void calulateElementCoordinate(MatrixPtr pSpecialUnitaryMatrix, MatrixCoordinatePtrRef prMatrixCoordinate) const;

private:
	void calculateTracelessHermitianFromSpecialUnitary(MatrixPtr pSpecialUnitaryMatrix, MatrixPtrRef prTracelessHermitianMatrix) const;

	void calculateTracelessHermitianFrom2kpiTraceHermitian(MatrixPtrRef prTracelessHermitianMatrix, mreal_t intialTrace) const;

	void addGlobalPhaseToSpecialUnitaryForTracelessHermitian(MatrixPtr pSpecialUnitaryMatrix, MatrixPtrRef prTracelessHermitianMatrix, mreal_t initialTrace) const;

	MatrixOperatorPtr m_pMatrixOperator;
	MatrixRealCoordinateCalculatorPtr m_pHermitianMatrixCoordinateCalculator;
};


#endif /* SPECIALUNITARYMATRIXCOORDINATEMAPPER_H_ */
