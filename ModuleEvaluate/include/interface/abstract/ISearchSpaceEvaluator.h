/*
 * ISearchSpaceEvaluator.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef ISEARCHSPACEEVALUATOR_H_
#define ISEARCHSPACEEVALUATOR_H_

#include "AlgoCommon.h"

class ISearchSpaceEvaluator {
public:

	virtual ~ISearchSpaceEvaluator(){};

	/**
	 *Evalulate given matrix collection, possibly about running time to find approximation, etc.
	 */
	virtual void evaluateCollection(MatrixCollectionPtr pMatrixCollection) = 0;

	/**
	 *Evalulate given matrix approximator, possibly about running time to get approximation, etc.
	 */
	virtual void evaluateApproximator(MatrixApproximatorPtr pMatrixApproximator, MatrixCollectionPtr pCoreMatrixCollection) = 0;
};


#endif /* ISEARCHSPACEEVALUATOR_H_ */
