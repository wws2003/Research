/*
 * SpecialUnitaryMatrixCoordinateMapper.cpp
 *
 *  Created on: May 23, 2015
 *      Author: pham
 */

#include "SpecialUnitaryMatrixCoordinateMapper.h"

SpecialUnitaryMatrixCoordinateMapper::SpecialUnitaryMatrixCoordinateMapper(MatrixOperatorPtr pMatrixOperator, MatrixRealCoordinateCalculatorPtr pHermitianMatrixCoordinateCalculator) {
	m_pMatrixOperator = pMatrixOperator;
	m_pHermitianMatrixCoordinateCalculator = pHermitianMatrixCoordinateCalculator;
}

void SpecialUnitaryMatrixCoordinateMapper::calulateElementCoordinate(MatrixPtr pSpecialUnitaryMatrix, MatrixCoordinatePtrRef prMatrixCoordinate) const {

	//Trace-less Hermitian convert
	MatrixPtr pTracelessHermitian = NullPtr;
	calculateTracelessHermitianFromSpecialUnitary(pSpecialUnitaryMatrix, pTracelessHermitian);

	//Calculate coordinate of Trace-less Hermitian
	m_pHermitianMatrixCoordinateCalculator->calulateElementCoordinate(pTracelessHermitian, prMatrixCoordinate);
}

void SpecialUnitaryMatrixCoordinateMapper::calculateTracelessHermitianFromSpecialUnitary(MatrixPtr pSpecialUnitaryMatrix, MatrixPtrRef prTracelessHermitianMatrix) const {
	MatrixPtr pSpecialUnitaryLog = NullPtr;
	m_pMatrixOperator->log(pSpecialUnitaryLog, pSpecialUnitaryLog);
	m_pMatrixOperator->multiplyScalar(pSpecialUnitaryLog, ComplexVal(0, -1), prTracelessHermitianMatrix);

	//TODO Handle the case trace(prTracelessHermitianMatrix) = 2k*pi

	_destroy(pSpecialUnitaryLog);
}




