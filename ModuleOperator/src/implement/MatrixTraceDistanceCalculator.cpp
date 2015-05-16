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

bool isZeroMatrix(MatrixPtr pMatrix);

//Check if trace of matrix is real value, i.e. image part is small enough to be ignored
void validateRealTrace(const ComplexVal& complexTrace);

MatrixTraceDistanceCalculator::MatrixTraceDistanceCalculator(MatrixOperatorPtr pMatrixOperator) : m_pMatrixOperator(pMatrixOperator) {

}

double MatrixTraceDistanceCalculator::distance(MatrixPtr p1, MatrixPtr p2) {
	//Return trace(sqrt((p1 - p2)'*(p1 - p2)))

	MatrixPtr pDelta = NullPtr;
	m_pMatrixOperator->subtract(p1, p2, pDelta);

	ComplexVal distanceTrace = 0.0;

	//Handle the case delta = 0, i.e. sqrt(delta'*delta) can not be determined
	//FIXME Just a band-aided solution
	if(!isZeroMatrix(pDelta)) {
		MatrixPtr pDeltaConjugateTranspose = NullPtr;
		m_pMatrixOperator->conjugateTranpose(pDelta, pDeltaConjugateTranspose);

		MatrixPtr pDeltaConjugateTransposeDelta = NullPtr;
		m_pMatrixOperator->multiply(pDeltaConjugateTranspose, pDelta, pDeltaConjugateTransposeDelta);

		MatrixPtr pSqrtDeltaConjugateTransposeDelta = NullPtr;
		m_pMatrixOperator->sqrt(pDeltaConjugateTransposeDelta, pSqrtDeltaConjugateTransposeDelta);

		distanceTrace = m_pMatrixOperator->trace(pSqrtDeltaConjugateTransposeDelta).real();

		validateRealTrace(distanceTrace);

		//Release intermediate pointers
		_destroy(pSqrtDeltaConjugateTransposeDelta);
		_destroy(pDeltaConjugateTransposeDelta);
		_destroy(pDeltaConjugateTranspose);
	}

	_destroy(pDelta);

	return distanceTrace.real();
}

bool isZeroMatrix(MatrixPtr pMatrix) {
	int nbRows, nbColumns;
	const double resolution = 2e-15;
	pMatrix->getSize(nbRows, nbColumns);
	for(int i = 0; i < nbRows; i++) {
		for(int j = 0; j < nbRows; j++) {
			if(std::norm(pMatrix->getValue(i,j)) > resolution) {
				return false;
			}
		}
	}
	return true;
}

void validateRealTrace(const ComplexVal& complexTrace) {
	const double eps = 1e-30;
	if(complexTrace.imag() > eps) {
		throw std::logic_error("Can not calculate trace distance");
	}
}

