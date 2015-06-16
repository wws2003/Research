/*
 * DuplicateCancelationMultiplierMatrixCombinerImpl.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#include "DuplicateCancelationMultiplierMatrixCombinerImpl.h"

bool DuplicateCancelationMultiplierMatrixCombinerImpl::shouldCombine(MatrixPtr pMatrix1, MatrixPtr pMatrix2) {
	std::string expectedCombinationMatrixLabel = pMatrix1->getLabel() + pMatrix2->getLabel();
	return (m_matrixLabelSet.find(expectedCombinationMatrixLabel) == m_matrixLabelSet.end());
}

void DuplicateCancelationMultiplierMatrixCombinerImpl::combine(MatrixPtr pMatrix1, MatrixPtr pMatrix2, MatrixPtrRef rpResultMatrix) {
	AbstractMultiplierMatrixCombiner::combine(pMatrix1, pMatrix2, rpResultMatrix);
	if(rpResultMatrix != NullPtr) {
		m_matrixLabelSet.insert(rpResultMatrix->getLabel());
	}
}



