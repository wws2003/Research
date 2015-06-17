/*
 * IMatrixCombiner.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef IMATRIXCOMBINER_H_
#define IMATRIXCOMBINER_H_

#include "AlgoCommon.h"
#include "MathConceptsCommon.h"
#include "IMatrix.h"

class IMatrixCombiner {
public:
	virtual ~IMatrixCombiner(){};

	/**
	 * Normally append sequence pMatrix2 to sequence pMatrix1
	 * Respecting to implementations, may conduct other process (eliminate inverse sequences, duplicated sequence, etc.)
	 */
	virtual void combine(MatrixPtr pMatrix1, MatrixPtr pMatrix2, MatrixPtrRef prResultMatrix) = 0;
};


#endif /* IMATRIXCOMBINER_H_ */
