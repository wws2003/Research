/*
 * AbstractMultiplierMatrixCombiner.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef ABSTRACTMULTIPLIERMATRIXCOMBINER_H_
#define ABSTRACTMULTIPLIERMATRIXCOMBINER_H_

#include "ICombiner.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"

class AbstractMultiplierMatrixCombiner : public IMatrixCombiner {
public:
	AbstractMultiplierMatrixCombiner(MatrixOperatorPtr pMatrixOperator);

	virtual ~AbstractMultiplierMatrixCombiner(){};

	virtual void combine(MatrixPtr pMatrix1, MatrixPtr pMatrix2, MatrixPtrRef rpResultMatrix);

	virtual bool shouldCombine(MatrixPtr pMatrix1, MatrixPtr pMatrix2) = 0;

protected:
	MatrixOperatorPtr m_pMatrixOperator;
};



#endif /* ABSTRACTMULTIPLIERMATRIXCOMBINER_H_ */
