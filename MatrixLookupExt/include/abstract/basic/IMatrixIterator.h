/*
 * IMatrixIterator.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXITERATOR_H_
#define IMATRIXITERATOR_H_

#include "IMatrix.h"

class IMatrixIterator {
public:
	virtual ~IMatrixIterator(){};

	virtual void toBegin() = 0;

	virtual void next() = 0;

	virtual void prev() = 0;

	virtual bool isDone() = 0;

	virtual MatrixPtr getMatrixObj() = 0;

};


#endif /* IMATRIXITERATOR_H_ */
