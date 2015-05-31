/*
 * MatrixRealInnerProductByTraceImpl.cpp
 *
 *  Created on: May 31, 2015
 *      Author: pham
 */

#include "MatrixRealInnerProductByTraceImpl.h"

MatrixRealInnerProductByTraceImpl::MatrixRealInnerProductByTraceImpl(MatrixOperatorPtr pMatrixOperator) {
	m_pMatrixOperator = pMatrixOperator;
}

void MatrixRealInnerProductByTraceImpl::innerProduct(MatrixPtr pMatrix1, MatrixPtr pMatrix2, double& rInnerProduct) {
	//Return Real(trace(matrix1'*matrix2))
	MatrixPtr pConjugateTransposeMatrix1 = NullPtr;
	m_pMatrixOperator->conjugateTranpose(pMatrix1, pConjugateTransposeMatrix1);

	MatrixPtr pConjugateTransposeMatrix1MultipliedMatrix2 = NullPtr;
	m_pMatrixOperator->multiply(pConjugateTransposeMatrix1, pMatrix2, pConjugateTransposeMatrix1MultipliedMatrix2);

	rInnerProduct = m_pMatrixOperator->trace(pConjugateTransposeMatrix1MultipliedMatrix2).real();

	_destroy(pConjugateTransposeMatrix1MultipliedMatrix2);
	_destroy(pConjugateTransposeMatrix1);
}



