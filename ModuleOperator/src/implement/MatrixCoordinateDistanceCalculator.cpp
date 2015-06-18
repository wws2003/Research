/*
 * MatrixCoordinateDistanceCalculator.cpp
 *
 *  Created on: May 22, 2015
 *      Author: pham
 */

#include "MatrixCoordinateDistanceCalculator.h"
#include "IDistanceCalculator.h"

#if USE_TEMPLATE
#else
MatrixCoordinateDistanceCalculator::MatrixCoordinateDistanceCalculator(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator) {
	m_pMatrixDistanceCalculator = pMatrixDistanceCalculator;
}

double MatrixCoordinateDistanceCalculator::distance(MatrixRealCoordinatePtr p1, MatrixRealCoordinatePtr p2) {
	MatrixPtr pMatrix1 = p1->getElement();
	MatrixPtr pMatrix2 = p2->getElement();
	return m_pMatrixDistanceCalculator->distance(pMatrix1, pMatrix2);
}
#endif

