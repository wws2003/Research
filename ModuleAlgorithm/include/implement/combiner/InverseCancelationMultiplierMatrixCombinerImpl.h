/*
 * InverseCancelationMultiplierMatrixCombinerImpl.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef INVERSECANCELATIONMULTIPLIERMATRIXCOMBINERIMPL_H_
#define INVERSECANCELATIONMULTIPLIERMATRIXCOMBINERIMPL_H_

#include "AbstractMultiplierMatrixCombiner.h"

class InverseCancelationMultiplierMatrixCombinerImpl: public AbstractMultiplierMatrixCombiner {
public:

	InverseCancelationMultiplierMatrixCombinerImpl(MatrixOperatorPtr pMatrixOperator):AbstractMultiplierMatrixCombiner(pMatrixOperator){};

	//Override
	/**
	 * Do not combine if sequence 2 starting from inverse matrix of the last one in sequence 1
	 * Rule for inverse gate detection: Inverse gates are presented in lower-upper case of a character
	 * e.g. HsT should not combine with tsH
	 */
	virtual bool shouldCombine(MatrixPtr pMatrix1, MatrixPtr pMatrix2);

};


#endif /* INVERSECANCELATIONMULTIPLIERMATRIXCOMBINERIMPL_H_ */
