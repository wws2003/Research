/*
 * SpecialUnitaryMatrixCoordinateMapper.cpp
 *
 *  Created on: May 23, 2015
 *      Author: pham
 */

#include "SpecialUnitaryMatrixCoordinateMapper.h"

SpecialUnitaryMatrixCoordinateMapper::SpecialUnitaryMatrixCoordinateMapper(MatrixOperatorPtr pMatrixOperator) {
	m_pMatrixOperator = pMatrixOperator;
}

void SpecialUnitaryMatrixCoordinateMapper::calulateElementCoordinate(MatrixPtr pMatrix, MatrixCoordinatePtrRef prMatrixCoordinate) const {
	//TODO Implement

	//Trace-less Hermitian convert

	//Calculate coordinate of Trace-less Hermitian
}



