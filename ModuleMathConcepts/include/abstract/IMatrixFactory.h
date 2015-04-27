/*
 * IMatrixFactory.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXFACTORY_H_
#define IMATRIXFACTORY_H_

#include "IMatrix.h"
#include "ISquareMatrix.h"

class IMatrixFactory {
public:
	virtual ~IMatrixFactory(){};

	virtual MatrixPtr getMatrixFromValues(int row, int columns, const ComplexVal** ppVals = NullPtr, const std::string label = "") = 0;

	virtual SquareMatrixPtr getIdentityMatrix(int dimension) = 0;
};

#endif /* IMATRIXFACTORY_H_ */
