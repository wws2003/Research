/*
 * IMatrixOperator.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXOPERATOR_H_
#define IMATRIXOPERATOR_H_

#include "IMatrix.h"

class IMatrixOperator {
public:
	virtual ~IMatrixOperator(){};

	virtual void add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pSum) = 0;
	virtual void multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pProduct) = 0;
	virtual void subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pSubtract) = 0;

	virtual void inverse(MatrixPtr pm, MatrixPtrRef inverseMatrix) = 0;

	virtual void exponential(SquareMatrixPtr pm, SquareMatrixPtr expMatrix) = 0;

	virtual ComplexVal det(MatrixPtr pm) = 0;
};

#endif /* IMATRIXOPERATOR_H_ */