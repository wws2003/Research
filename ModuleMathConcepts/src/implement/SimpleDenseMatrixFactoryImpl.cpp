/*
 * SimpleDenseMatrixFactoryImpl.cpp
 *
 *  Created on: May 13, 2015
 *      Author: pham
 */

#include "SimpleDenseMatrixFactoryImpl.h"
#include "SimpleDenseMatrixImpl.h"
#include "SimpleIdentityMatrixImpl.h"

MatrixPtr SimpleDenseMatrixFactoryImpl::getMatrixFromValues(int rows, int columns, const ComplexVal* pVals, const ArraySpliceType arraySpliceType, const std::string label) {
	const ComplexVal* array = NULL;
	if(pVals == NULL) {
		array = new ComplexVal[rows * columns];
	}
	else {
		array = pVals;
	}
	MatrixPtr pMatrix = new SimpleDenseMatrixImpl(array, arraySpliceType, rows, columns, label);
	delete array;
	return pMatrix;
}
MatrixPtr SimpleDenseMatrixFactoryImpl::getIdentityMatrix(int dimension) {
	MatrixPtr pMatrix = new SimpleIdentityMatrixImpl(dimension);
	return pMatrix;
}


