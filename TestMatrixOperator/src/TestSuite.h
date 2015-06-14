/*
 * TestSuite.h
 *
 *  Created on: May 14, 2015
 *      Author: pham
 */

#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include <iostream>
#include <cassert>
#include <cmath>
#include "MathConceptsCommon.h"
#include "IMatrix.h"
#include "SimpleDenseMatrixImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "OperatorCommon.h"
#include "IInnerProductCalculator.h"

class TestSuite {
public:
	TestSuite();
	virtual ~TestSuite();

	virtual void test();

private:
	void testMatrixGenerator();

	void testMatrixAdd();

	void testMatrixMultiply();

	void testMatrixExp();

	void testMatrixInverse();

	void testMultiplyScalar();

	void testConjugate();

	void testTranspose();

	void testTrace();

	void testPower();

	void testSqrt();

	void testEig();

	void testDistanceCalculator();

	void testMatrixTraceInnerProduct();

	void testCoordinateOnOrthonormalBasisCalculator();

	void testTracelessHermitianBasis();

	void testSpecializeUnitary();

	void testSimpleUnitaryCoordinateMapper();

	void testSpecialUnitaryCoordinateMapper();

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixPtrVector m_pPauliMatrices;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateOnOrthonormalBasisCalculator;
	MatrixRealCoordinateCalculatorPtr m_pSpecialUnitaryCoordinateCalculator;
};



#endif /* TESTSUITE_H_ */
