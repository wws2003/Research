/*
 * SimpleDenseMatrixFactoryImpl.cpp
 *
 *  Created on: May 13, 2015
 *      Author: pham
 */

#include "SimpleDenseMatrixFactoryImpl.h"
#include "SimpleDenseMatrixImpl.h"

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
	int rows = dimension, columns = dimension;
	ComplexValArray array = new ComplexVal[rows * columns];
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < columns; j++) {
			array[i * columns + j] = (i == j) ? 1 : 0;
		}
	}
	MatrixPtr pMatrix = new SimpleDenseMatrixImpl(array, ROW_SPLICE, rows, columns, "I");
	delete array;
	return pMatrix;
}


