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

	virtual void add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSum) = 0;
	virtual void multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prProduct) = 0;
	virtual void multiplyScalar(MatrixPtr pm, ComplexVal scalar, MatrixPtrRef prProduct) = 0;
	virtual void subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSubtract) = 0;
	virtual ComplexVal det(MatrixPtr pm) = 0;

	virtual void conjugateTranpose(MatrixPtr pm, MatrixPtrRef prConjugate) = 0;
	virtual void transpose(MatrixPtr pm, MatrixPtrRef prTranspose) = 0;

	virtual void inverse(MatrixPtr pm, MatrixPtrRef prInverseMatrix) = 0;
	virtual void exponential(MatrixPtr pm, MatrixPtrRef prExpMatrix) = 0;
	virtual void log(MatrixPtr pm, MatrixPtrRef prLog) = 0;
	virtual void eig(MatrixPtr pm, VectorPtrRef ppEigVals, MatrixPtrRef prEigVects) = 0;

};

#endif /* IMATRIXOPERATOR_H_ */
