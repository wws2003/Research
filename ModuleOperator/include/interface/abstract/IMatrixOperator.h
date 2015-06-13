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

	/**
	 * Add 2 matrix pm1, pm2 and store result to reference prSum
	 * Should throw logic exception if matrices size not matched
	 */
	virtual void add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSum) = 0;

	/**
	 * Multiply 2 matrix pm1, pm2 and store result to reference prProduct
	 * Should throw logic exception if matrices size not matched
	 */
	virtual void multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prProduct) = 0;

	/**
	 * Multiply matrix pm1 with scalar value scalar and store result to reference prProduct
	 */
	virtual void multiplyScalar(MatrixPtr pm, ComplexVal scalar, MatrixPtrRef prProduct) = 0;

	/**
	 * Subtract 2 matrix pm1 by pm2 and store result to reference prSubtract
	 * Should throw logic exception if matrices size not matched
	 */
	virtual void subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSubtract) = 0;

	/***
	 * Return determinant of matrix pm
	 */
	virtual ComplexVal det(MatrixPtr pm) = 0;

	/**
	 * Return determinant of matrix pm.
	 * This method is actually a shorthand for manual calculating sum of elements on matrix diagon
	 */
	virtual ComplexVal trace(MatrixPtr pm) = 0;

	/**
	 * Calculate pm ^ exponent and store result to prPower
	 * Should throw logic exception if matrices size not matched
	 * exponent should be negative, zero or negative
	 */
	virtual void power(MatrixPtr pm, int exponent, MatrixPtrRef prPower) = 0;

	/**
	 * Calculate sqrt of pm and store result to prSqrt
	 * Should throw logic exception if matrix has no sqrt (i.e. not a (semi?)positive definitive matrix)
	 */
	virtual void sqrt(MatrixPtr pm, MatrixPtrRef prSqrt) = 0;

	/***
	 * Calculate conjugate tranpose of matrix pm and store to prConjugate
	 */
	virtual void conjugateTranpose(MatrixPtr pm, MatrixPtrRef prConjugate) = 0;

	/***
	 * Calculate tranpose of matrix pm and store to prTranspose
	 */
	virtual void transpose(MatrixPtr pm, MatrixPtrRef prTranspose) = 0;

	/***
	 * Calculate inverse of matrix pm and store to prInverseMatrix
	 * Should throw logic exception if matrix is not inversible
	 */
	virtual void inverse(MatrixPtr pm, MatrixPtrRef prInverseMatrix) = 0;

	/***
	 * Calculate exponential of matrix pm and store to prExpMatrix
	 * Should throw logic exception if matrix is not exponetiable
	 */
	virtual void exponential(MatrixPtr pm, MatrixPtrRef prExpMatrix) = 0;

	/***
	 * Calculate logarithm of matrix pm and store to prLog
	 * Should throw logic exception if matrix is not logarithable
	 */
	virtual void log(MatrixPtr pm, MatrixPtrRef prLog) = 0;

	/**
	 * Calculate eigen vectors and eigen values of matrix pm.
	 * Eigen values are stored in vector prEigVals
	 * Eigen vectors are stored in prEigVects
	 * Should throw logic exception if matrix has no eigen value
	 */
	virtual void eig(MatrixPtr pm, ComplexVectorRef prEigVals, MatrixPtrRef prEigVects) = 0;

	/**
	 * Just an utility operation for U*diag*U'
	 * return root = U*diag*U'
	 */
	virtual void unitaryDediagonalize(MatrixPtr pU, const ComplexVectorRef diag, MatrixPtr prRoot) = 0;

	/**
	 * Just an utility
	 */
	virtual void specialUnitaryFromUnitary(MatrixPtr pU, MatrixPtrRef prSU) = 0;
};

#endif /* IMATRIXOPERATOR_H_ */
