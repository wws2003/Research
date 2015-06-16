/*
 * AbstractMultiplierMatrixCombiner.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#include "AbstractMultiplierMatrixCombiner.h"

AbstractMultiplierMatrixCombiner::AbstractMultiplierMatrixCombiner(MatrixOperatorPtr pMatrixOperator) {
	m_pMatrixOperator = pMatrixOperator;
}

void AbstractMultiplierMatrixCombiner::combine(MatrixPtr pMatrix1, MatrixPtr pMatrix2, MatrixPtrRef rpResultMatrix) {
	rpResultMatrix = NullPtr;

	if(shouldCombine(pMatrix1, pMatrix2)) {
		m_pMatrixOperator->multiply(pMatrix1, pMatrix2, rpResultMatrix);
	}
}




