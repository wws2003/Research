/*
 * SearchEvaluator.h
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#ifndef SEARCHEVALUATOR_H_
#define SEARCHEVALUATOR_H_

#include "common/Common.h"
#include <ostream>

class SearchEvaluator {

public:
	SearchEvaluator(MatrixCollectionPtr pMatrixCollection, MatrixDistanceCalculatorPtr pMatrixDistanceCalculator, MatrixWriterPtr pMatrixWriter);
	virtual ~SearchEvaluator(){};

	void evaluate(int numberOfCases, double epsilon, TargetMatrixFactoryPtr pTargetMatrixFactory, TimerPtr pTimer, std::ostream& outputStream);

private:
	MatrixCollectionPtr m_pMatrixCollection;
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	MatrixWriterPtr m_pMatrixWriter;
};



#endif /* SEARCHEVALUATOR_H_ */
