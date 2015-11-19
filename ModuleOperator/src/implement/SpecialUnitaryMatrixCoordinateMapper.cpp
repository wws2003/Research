/*
 * SpecialUnitaryMatrixCoordinateMapper.cpp
 *
 *  Created on: May 23, 2015
 *      Author: pham
 */

#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "Coordinate.hpp"

#define gt(x,y,eps) (x - y > eps)
#define apprx(x,y,eps) (mreal::abs(x-y) < eps)

#if MPFR_REAL
#define NOISE_THRESOLD 1e-33
#else
#define NOISE_THRESOLD 1e-12
#endif

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

void SpecialUnitaryMatrixCoordinateMapper::calculateElementFromCoordinate(MatrixRealCoordinatePtr pCoordinate) const {
	real_coordinate_t matrixCoordinate = pCoordinate->getCoordinates();
	int nbAxis = matrixCoordinate.size();

	ComplexVector coordinateVector;
	for(int i = 0; i < nbAxis; i++) {
		coordinateVector.push_back(matrixCoordinate[i]);
	}

	MatrixPtrVector hermitianBasis;
	int dimension = std::sqrt(nbAxis + 1.1);
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(dimension, hermitianBasis);

	MatrixPtr pH = NullPtr;
	m_pMatrixOperator->sumProduct(hermitianBasis, coordinateVector, pH);

	MatrixPtr pU = NullPtr;
	MatrixPtr pHi = NullPtr;
	m_pMatrixOperator->multiplyScalar(pH, ComplexVal(0.0,1.0), pHi);
	m_pMatrixOperator->exponential(pHi, pU);
	_destroy(pHi);
	_destroy(pH);

	pCoordinate->setElement(pU);
}

void SpecialUnitaryMatrixCoordinateMapper::calculateTracelessHermitianFromSpecialUnitary(MatrixPtr pSpecialUnitaryMatrix, MatrixPtrRef prTracelessHermitianMatrix) const {
	MatrixPtr pSpecialUnitaryLog = NullPtr;
	m_pMatrixOperator->log(pSpecialUnitaryMatrix, pSpecialUnitaryLog);
	m_pMatrixOperator->multiplyScalar(pSpecialUnitaryLog, ComplexVal(0, -1), prTracelessHermitianMatrix);

	mreal_t hermitianInitialTrace = m_pMatrixOperator->trace(prTracelessHermitianMatrix).real();

	if(!apprx(hermitianInitialTrace, 0.0, 1e-9)) {
		MatrixPtr pTracelessHermitian = prTracelessHermitianMatrix;
		addGlobalPhaseToSpecialUnitaryForTracelessHermitian(pSpecialUnitaryMatrix, prTracelessHermitianMatrix, hermitianInitialTrace);
		//calculateTracelessHermitianFrom2kpiTraceHermitian(pTracelessHermitian, hermitianInitialTrace);

		_destroy(prTracelessHermitianMatrix);
		prTracelessHermitianMatrix = pTracelessHermitian;
	}

	_destroy(pSpecialUnitaryLog);
}

void SpecialUnitaryMatrixCoordinateMapper::calculateTracelessHermitianFrom2kpiTraceHermitian(MatrixPtrRef prTracelessHermitianMatrix, mreal_t initialTrace) const {
	ComplexVector eigValues;
	MatrixPtr eigVectors;
	m_pMatrixOperator->eig(prTracelessHermitianMatrix, eigValues, eigVectors);

	//Find proper eigen value to decrease/increase
	mreal_t newTrace = initialTrace;
	short traceSign = gt(initialTrace,0,1e-9) ? 1 : -1;
	double delta2pi = -traceSign * 2 * M_PI;

	while(!apprx(newTrace, 0.0, 1e-9)) {
		int maxAdjustmentIndex = -1;
		mreal_t maxAdjustment = -1.0;

		//Find the highest magnitude in the eigen values, on which adjustment should be applied
		for(unsigned int i = 0; i < eigValues.size(); i++) {
			mreal_t eigRealValue = eigValues[i].real();
			short eigRealSign = gt(eigRealValue,0,1e-9) ? 1 : -1;

			if(eigRealSign == traceSign && mreal::abs(eigRealValue) >= maxAdjustment) {
				maxAdjustment = mreal::abs(eigRealValue);
				maxAdjustmentIndex = i;
			}
		}

		//Checking >= 0 for maxAdjustmentIndex is just for carefulness
		if(maxAdjustmentIndex >= 0) {
			eigValues[maxAdjustmentIndex] += delta2pi;
			newTrace += delta2pi;
		}
	}

	m_pMatrixOperator->unitaryDediagonalize(eigVectors, eigValues, prTracelessHermitianMatrix);

}

void SpecialUnitaryMatrixCoordinateMapper::addGlobalPhaseToSpecialUnitaryForTracelessHermitian(MatrixPtr pSpecialUnitaryMatrix, MatrixPtrRef prTracelessHermitianMatrix, mreal_t initialTrace) const {
	int nbRows, nbColumns;
	pSpecialUnitaryMatrix->getSize(nbRows, nbColumns);

	short traceSign = gt(initialTrace, 0, 1e-9) ? 1 : -1;
	double delta2pi = -traceSign * 2 * M_PI / (nbRows + 0.0);

	ComplexVal phase = std::exp(ComplexVal(0, delta2pi));

	MatrixPtr pAdjustedUntitary = NullPtr;
	m_pMatrixOperator->multiplyScalar(pSpecialUnitaryMatrix, phase, pAdjustedUntitary);

	MatrixPtr pSpecialUnitaryLog = NullPtr;
	m_pMatrixOperator->log(pAdjustedUntitary, pSpecialUnitaryLog);
	m_pMatrixOperator->multiplyScalar(pSpecialUnitaryLog, ComplexVal(0, -1), prTracelessHermitianMatrix);

	_destroy(pSpecialUnitaryLog);
	_destroy(pAdjustedUntitary);
}
