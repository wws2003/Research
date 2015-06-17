/*
 * DuplicateCancelationMultiplierMatrixCombinerImpl.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef DUPLICATECANCELATIONMULTIPLIERMATRIXCOMBINERIMPL_H_
#define DUPLICATECANCELATIONMULTIPLIERMATRIXCOMBINERIMPL_H_

#include "AbstractMultiplierMatrixCombiner.h"
#include <set>
#include <string>

typedef std::set<std::string> MatrixLabelSet;

class DuplicateCancelationMultiplierMatrixCombinerImpl : public AbstractMultiplierMatrixCombiner {
public:

	DuplicateCancelationMultiplierMatrixCombinerImpl(MatrixOperatorPtr pMatrixOperator) : AbstractMultiplierMatrixCombiner(pMatrixOperator){};

	//Override
	/**
	 * Do not combine if the 2 input matrices have been combined once before by this combiner
	 * Checking this condition is via a set of matrix labels
	 */
	virtual bool shouldCombine(MatrixPtr pMatrix1, MatrixPtr pMatrix2);

	//Override
	virtual void combine(MatrixPtr pMatrix1, MatrixPtr pMatrix2, MatrixPtrRef rpResultMatrix);

private:
	MatrixLabelSet m_matrixLabelSet;
};


#endif /* DUPLICATECANCELATIONMULTIPLIERMATRIXCOMBINERIMPL_H_ */
