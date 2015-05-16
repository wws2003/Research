/*
 * SampleSearchSpaceEvaluatorFactoryImpl.h
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#ifndef SAMPLESEARCHSPACEEVALUATORFACTORYIMPL_H_
#define SAMPLESEARCHSPACEEVALUATORFACTORYIMPL_H_

#include "SearchSpaceEvaluatorImpl.h"
#include "ISearchSpaceEvaluatorFactory.h"
#include <ostream>
#include <vector>

class SampleSearchSpaceEvaluatorFactoryImpl: public ISearchSpaceEvaluatorFactory {
public:
	SampleSearchSpaceEvaluatorFactoryImpl(const TargetMatrices& pTargets, double epsilon, MatrixDistanceCalculatorPtr pMatrixDistanceCalculator, MatrixWriterPtr pMatrixWriter, TimerPtr pTimer, std::ostream& outputStream);

	virtual SearchSpaceEvaluatorPtr getSearchSpaceEvaluatorPtr();

	void setTargetMatrices(const TargetMatrices& pTargets);

	void setEpsilon(double epsilon);

private:
	TargetMatrices m_targetMatrices;
	double m_epsilon;
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	TimerPtr m_pTimer;
	MatrixWriterPtr m_pMatrixWriter;
	std::ostream& m_ostream;
};


#endif /* SAMPLESEARCHSPACEEVALUATORFACTORYIMPL_H_ */
