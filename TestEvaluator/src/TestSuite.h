/*
 * TestSuite.h
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include "EvaluateCommon.h"
#include "OperatorCommon.h"
#include "IOCommon.h"
#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include <vector>

class TestSuite {
public:
	TestSuite();
	virtual ~TestSuite();
	void test();

private:
	void testSimpleWriter();
	void testSimpleCollection();
	void testSimpleSearchSpaceConstructor();
	void testSimpleEvaluator();
	void testInverseCancelingSearchSpaceConstructor();

	//SearchSpaceEvaluatorPtr m_pSearchSpaceEvaluator;
	MatrixWriterPtr m_pMatrixWriter;
	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	std::vector<MatrixPtr> m_pTargets;
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	TimerPtr m_pTimer;
	SearchSpaceEvaluatorPtr m_pSearchSpaceEvaluator;

	MatrixCombinerPtr m_pMatrixCombiner;
	SearchSpaceConstructorPtr m_pSearchSpaceConstructor;
};



#endif /* TESTSUITE_H_ */
