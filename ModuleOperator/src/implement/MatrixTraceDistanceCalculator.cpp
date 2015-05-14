/*
 * MatrixTraceDistanceCalculator.cpp
 *
 *  Created on: May 14, 2015
 *      Author: pham
 */

#include "MatrixTraceDistanceCalculator.h"
#include "IMatrix.h"
#include "IMatrixOperator.h"
#include <stdexcept>

//Check if trace of matrix is real value, i.e. image part is small enough to be ignored
void validateRealTrace(const ComplexVal& complexTrace);

MatrixTraceDistanceCalculator::MatrixTraceDistanceCalculator(MatrixOperatorPtr pMatrixOperator) : m_pMatrixOperator(pMatrixOperator) {

}

double MatrixTraceDistanceCalculator::distance(MatrixPtr p1, MatrixPtr p2) {
	//Return trace(sqrt((p1 - p2)'*(p1 - p2)))

	MatrixPtr pDelta = NullPtr;
	m_pMatrixOperator->subtract(p1, p2, pDelta);

	MatrixPtr pDeltaConjugateTranspose = NullPtr;
	m_pMatrixOperator->conjugateTranpose(pDelta, pDeltaConjugateTranspose);

	MatrixPtr pDeltaConjugateTransposeDelta = NullPtr;
	m_pMatrixOperator->multiply(pDeltaConjugateTranspose, pDelta, pDeltaConjugateTransposeDelta);

	MatrixPtr pSqrtDeltaConjugateTransposeDelta = NullPtr;
	m_pMatrixOperator->sqrt(pDeltaConjugateTransposeDelta, pSqrtDeltaConjugateTransposeDelta);

	ComplexVal distanceTrace = m_pMatrixOperator->trace(pSqrtDeltaConjugateTransposeDelta).real();

	validateRealTrace(distanceTrace);

	return distanceTrace.real();
}

void validateRealTrace(const ComplexVal& complexTrace) {
	const double eps = 1e-30;
	if(complexTrace.imag() > eps) {
		throw std::logic_error("Can not calculate trace distance");
	}
}

