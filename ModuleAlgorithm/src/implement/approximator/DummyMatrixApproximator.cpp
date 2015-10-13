/*
 * DummyMatrixApproximator.cpp
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#include "DummyMatrixApproximator.h"
#include "IIterator.h"
#include "ICollection.h"
#include "IMatrix.h"

MatrixIteratorPtr DummyMatrixApproximator::getApproximateElements(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, mreal_t epsilon) {
	return pCoreCollection->findNearestNeighbour(pQuery, pDistanceCalculator, epsilon);
}


