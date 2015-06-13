/*
 * SpecialUnitaryMatrixCoordinateMapper.cpp
 *
 *  Created on: May 23, 2015
 *      Author: pham
 */

#include "SpecialUnitaryMatrixCoordinateMapper.h"

#define apprx(x,y,eps) (std::abs(x-y) < eps)
#define gt(x,y,eps) (x - y > eps)

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

	_destroy(pTracelessHermitian);
}

void SpecialUnitaryMatrixCoordinateMapper::calculateTracelessHermitianFromSpecialUnitary(MatrixPtr pSpecialUnitaryMatrix, MatrixPtrRef prTracelessHermitianMatrix) const {
	MatrixPtr pSpecialUnitaryLog = NullPtr;
	m_pMatrixOperator->log(pSpecialUnitaryMatrix, pSpecialUnitaryLog);
	m_pMatrixOperator->multiplyScalar(pSpecialUnitaryLog, ComplexVal(0, -1), prTracelessHermitianMatrix);

	double hermitianInitialTrace = m_pMatrixOperator->trace(prTracelessHermitianMatrix).real();

	if(!apprx(hermitianInitialTrace, 0.0, 1e-9)) {
		calculateTracelessHermitianFrom2kpiTraceHermitian(prTracelessHermitianMatrix, hermitianInitialTrace);
	}

	_destroy(pSpecialUnitaryLog);
}

void SpecialUnitaryMatrixCoordinateMapper::calculateTracelessHermitianFrom2kpiTraceHermitian(MatrixPtrRef prTracelessHermitianMatrix, double initialTrace) const {
	ComplexVector eigValues;
	MatrixPtr eigVectors;
	m_pMatrixOperator->eig(prTracelessHermitianMatrix, eigValues, eigVectors);

	//Find proper eigen value to decrease/increase
	double newTrace = initialTrace;
	short traceSign = gt(initialTrace,0,1e-9) ? 1 : -1;
	double delta2pi = -traceSign * 2 * M_PI;

	/**
	 * Question:
	 * Should apply delta2pi into eigen values in order of magnitude.
	 * in order to avoid too bias?
	 *
	 */

	for(unsigned int i = 0; i < eigValues.size(); i++) {
		double eigRealValue = eigValues[i].real();
		short eigRealSign = gt(eigRealValue,0,1e-9) ? 1 : -1;

		if(eigRealSign == traceSign) {
			eigValues[i] += delta2pi;
			newTrace += delta2pi;
			if(apprx(newTrace, 0.0, 1e-9)) {
				break;
			}
		}
	}

	_destroy(prTracelessHermitianMatrix);

	m_pMatrixOperator->unitaryDediagonalize(eigVectors, eigValues, prTracelessHermitianMatrix);
}



