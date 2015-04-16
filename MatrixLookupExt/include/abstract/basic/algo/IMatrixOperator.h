/*
 * IMatrixOperator.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXOPERATOR_H_
#define IMATRIXOPERATOR_H_

#include "../math_concept/IMatrix.h"

class IMatrixOperator {
public:
	virtual ~IMatrixOperator(){};

	virtual void add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pSum) = 0;
	virtual void multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pProduct) = 0;
	virtual void subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pSubtract) = 0;
	virtual ComplexVal det(MatrixPtr pm) = 0;

	virtual void inverse(SquareMatrixPtr pm, SquareMatrixPtrRef ppInverseMatrix) = 0;
	virtual void exponential(SquareMatrixPtr pm, SquareMatrixPtrRef ppExpMatrix) = 0;
	virtual void eig(SquareMatrixPtr pm, VectorPtrRef ppEigVals, SquareMatrixPtrRef ppEigVects);

};

#endif /* IMATRIXOPERATOR_H_ */
