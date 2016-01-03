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

void MatrixRealInnerProductByTraceImpl::innerProduct(MatrixPtr pMatrix1, MatrixPtr pMatrix2, mreal_t& rInnerProduct) {
	//Return Real(trace(matrix1'*matrix2))

	//Try to calculate directly to avoid overhead
	int nbRows, nbColumns;
	pMatrix1->getSize(nbRows, nbColumns);

	if(m_pMatrixOperator == NullPtr) {
		ComplexVal traceConjugateTranposeM1M2(0.0, 0.0);
		for(int r1 = 0; r1 < nbRows; r1++) {
			for(int c1 = 0; c1 < nbColumns; c1++) {
				traceConjugateTranposeM1M2 += std::conj(pMatrix1->getValue(c1, r1)) * pMatrix2->getValue(c1, r1);
			}
		}
		rInnerProduct = traceConjugateTranposeM1M2.real();
	}
	else {
		MatrixPtr pConjugateTransposeMatrix1 = NullPtr;
		m_pMatrixOperator->conjugateTranpose(pMatrix1, pConjugateTransposeMatrix1);

		MatrixPtr pConjugateTransposeMatrix1MultipliedMatrix2 = NullPtr;
		m_pMatrixOperator->multiply(pConjugateTransposeMatrix1, pMatrix2, pConjugateTransposeMatrix1MultipliedMatrix2);

		rInnerProduct = m_pMatrixOperator->trace(pConjugateTransposeMatrix1MultipliedMatrix2).real();

		_destroy(pConjugateTransposeMatrix1MultipliedMatrix2);
		_destroy(pConjugateTransposeMatrix1);
	}

}
