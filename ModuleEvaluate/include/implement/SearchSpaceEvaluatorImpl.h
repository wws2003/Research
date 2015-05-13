/*
 * SearchSpaceTimerEvaluatorImpl.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef SEARCHSPACETIMEREVALUATORIMPL_H_
#define SEARCHSPACETIMEREVALUATORIMPL_H_

#include "ISearchSpaceEvaluator.h"
#include "EvaluateCommon.h"
#include "OperatorCommon.h"
#include "IOCommon.h"
#include <ostream>
#include <vector>

#ifndef TargetMatrices
typedef std::vector<MatrixPtr> TargetMatrices;
#endif

class SearchSpaceTimerEvaluatorImpl : public ISearchSpaceEvaluator {
public:
	SearchSpaceTimerEvaluatorImpl(TargetMatrices pTargets, double epsilon, MatrixDistanceCalculatorPtr pMatrixDistanceCalculator, MatrixWriterPtr pMatrixWriter, TimerPtr pTimer, std::ostream& outputStream);
	virtual ~SearchSpaceTimerEvaluatorImpl(){};

	//Override
	virtual void evaluateCollection(MatrixCollectionPtr pMatrixCollection);

	//Override
	virtual void evaluateApproximator(MatrixApproximatorPtr pMatrixApproximator, MatrixCollectionPtr pCoreMatrixCollection);

private:
	TargetMatrices m_targetMatrices;
	double m_epsilon;
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	TimerPtr m_pTimer;
	MatrixWriterPtr m_pMatrixWriter;
	std::ostream& m_ostream;
};


#endif /* SEARCHSPACETIMEREVALUATORIMPL_H_ */
