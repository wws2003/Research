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
	bool createArrayFlag = false;

	if(pVals == NULL) {
		ComplexValArray pBuffer = new ComplexVal[rows * columns];
		std::fill(pBuffer, pBuffer + rows * columns, ComplexVal(0.0,0.0));
		array = pBuffer;
		createArrayFlag = true;
	}
	else {
		array = pVals;
	}

	MatrixPtr pMatrix = new SimpleDenseMatrixImpl(array, arraySpliceType, rows, columns, label);

	if(createArrayFlag) {
		delete[] array;
	}

	return pMatrix;
}
MatrixPtr SimpleDenseMatrixFactoryImpl::getIdentityMatrix(int dimension) {
	MatrixPtr pMatrix = new SimpleIdentityMatrixImpl(dimension);
	return pMatrix;
}

