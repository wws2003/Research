/*
 * AlwaysTrueMultiplierMatrixCombinerImpl.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef ALWAYSTRUEMULTIPLIERMATRIXCOMBINERIMPL_H_
#define ALWAYSTRUEMULTIPLIERMATRIXCOMBINERIMPL_H_

#include "AbstractMultiplierMatrixCombiner.h"

class AlwaysTrueMultiplierMatrixCombinerImpl: public AbstractMultiplierMatrixCombiner {
public:
	//Override

	/**
	 * Always allow combine 2 matrices
	 */
	virtual bool shouldCombine(MatrixPtr pMatrix1, MatrixPtr pMatrix2) {
		return true;
	}
};


#endif /* ALWAYSTRUEMULTIPLIERMATRIXCOMBINERIMPL_H_ */
