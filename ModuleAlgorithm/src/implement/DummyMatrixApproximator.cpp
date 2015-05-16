/*
 * DummyMatrixApproximator.cpp
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#include "DummyMatrixApproximator.h"
#include "IIterator.h"
#include "IMatrixCollection.h"
#include "IMatrix.h"

MatrixIteratorPtr DummyMatrixApproximator::getApproximateMatrices(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) {
	return pCoreCollection->findApproxMatrices(pQuery, pDistanceCalculator, epsilon);
}


