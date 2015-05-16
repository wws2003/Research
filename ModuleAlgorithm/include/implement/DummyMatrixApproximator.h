/*
 * DummyMatrixApproximator.h
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#ifndef DUMMYMATRIXAPPROXIMATOR_H_
#define DUMMYMATRIXAPPROXIMATOR_H_

#include "AlgoCommon.h"
#include "IMatrixApproximator.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"

class DummyMatrixApproximator: public IMatrixApproximator {
public:
	virtual ~DummyMatrixApproximator(){};

	/**
	 * This implementation is just done by finding approximation of the core collection, doesn't generate any new candidates
	 */
	virtual MatrixIteratorPtr getApproximateMatrices(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon);

};



#endif /* DUMMYMATRIXAPPROXIMATOR_H_ */