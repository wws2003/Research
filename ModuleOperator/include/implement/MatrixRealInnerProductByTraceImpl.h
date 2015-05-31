/*
 * MatrixRealInnerProductByTraceImpl.h
 *
 *  Created on: May 31, 2015
 *      Author: pham
 */

#ifndef MATRIXREALINNERPRODUCTBYTRACEIMPL_H_
#define MATRIXREALINNERPRODUCTBYTRACEIMPL_H_

#include "OperatorCommon.h"
#include "IMatrixOperator.h"
#include "IInnerProductCalculator.h"

class MatrixRealInnerProductByTraceImpl : IMatrixRealInnerProductCalculator {
public:
	MatrixRealInnerProductByTraceImpl(MatrixOperatorPtr pMatrixOperator);
	virtual ~MatrixRealInnerProductByTraceImpl(){};

	virtual void innerProduct(MatrixPtr pMatrix1, MatrixPtr pMatrix2, double& rInnerProduct);

private:
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* MATRIXREALINNERPRODUCTBYTRACEIMPL_H_ */
