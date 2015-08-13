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

class FullTestSuite {
public:
	FullTestSuite();
	virtual ~FullTestSuite();
	void test();

private:
	void testSimpleWriter();
	void testSimpleCollection();
	void testSimpleSearchSpaceConstructor();
	void testSimpleEvaluator();
	void testInverseCancelingSearchSpaceConstructor();
	void testSampleMatrixBinCollection();
	void testCalculateCoordinatesInSearchSpace();
	void testGNATCollectionBuild();
	void testGNATCollectionPersistence();
	void testGNATSearch();

	void freeTestGateCollectionEvaluator();

	//SearchSpaceEvaluatorPtr m_pSearchSpaceEvaluator;
	MatrixWriterPtr m_pMatrixWriter;
	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	std::vector<MatrixPtr> m_targets;
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	TimerPtr m_pTimer;
	MatrixSearchSpaceEvaluatorPtr m_pSearchSpaceEvaluator;

	MatrixCombinerPtr m_pMatrixCombiner;
	MatrixSearchSpaceConstructorPtr m_pSearchSpaceConstructor;

	const static std::string GNAT_COLLECTION_PERSIST_FILE;
};



#endif /* TESTSUITE_H_ */
