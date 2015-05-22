/*
 * MatrixCoordinateDistanceCalculator.cpp
 *
 *  Created on: May 22, 2015
 *      Author: pham
 */

#include "MatrixCoordinateDistanceCalculator.h"
#include "IDistanceCalculator.h"

MatrixCoordinateDistanceCalculator::MatrixCoordinateDistanceCalculator(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator) {
	m_pMatrixDistanceCalculator = pMatrixDistanceCalculator;
}

double MatrixCoordinateDistanceCalculator::distance(MatrixCoordinatePtr p1, MatrixCoordinatePtr p2) {
	MatrixPtr pMatrix1 = p1->getElement();
	MatrixPtr pMatrix2 = p2->getElement();
	return m_pMatrixDistanceCalculator->distance(pMatrix1, pMatrix2);
}

