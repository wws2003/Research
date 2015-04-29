/*
 * IMatrixCollection.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXCOLLECTION_H_
#define IMATRIXCOLLECTION_H_

#include "IMatrix.h"
#include "AlgoCommon.h"
#include "OperatorCommon.h"

class IMatrixCollection {

public:
	virtual ~IMatrixCollection(){};

	virtual void addMatrix(MatrixPtr pm) = 0;

	virtual void clear() = 0;

	//Return iterator through a fixed set of matrix, regardless the collection
	//should be modified later
	virtual MatrixIteratorPtr getReadonlyIteratorPtr() = 0;

	//Return iterator through a set of matrix reflecting the changes in the collection
	virtual MatrixIteratorPtr getIteratorPtr() = 0;

	virtual MatrixCollectionSize_t size() const = 0;

	virtual MatrixIteratorPtr findApproxMatrices(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const = 0;
};


#endif /* IMATRIXCOLLECTION_H_ */
