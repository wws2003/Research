/*
 * IMatrixApproximator.h
 *
 *  Created on: May 13, 2015
 *      Author: pham
 */

#ifndef IMATRIXAPPROXIMATOR_H_
#define IMATRIXAPPROXIMATOR_H_

#include "AlgoCommon.h"
#include "OperatorCommon.h"

class IMatrixApproximator {
public:
	virtual ~IMatrixApproximator(){};

	/**
	 * Find in given collection, and moreover, if possible generate from it
	 approximate matrices to query (distance smaller than epsilon)
	 */
	virtual MatrixIteratorPtr getApproximateMatrices(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) = 0;
};


#endif /* IMATRIXAPPROXIMATOR_H_ */
