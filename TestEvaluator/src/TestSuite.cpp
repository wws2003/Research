/*
 * TestSuite.cpp
 *
 *  Created on: May 16, 2015
 *      Author: pham
 */

#include "TestSuite.h"
#include "SampleMatrixWriter.h"
#include "SimpleDenseMatrixImpl.h"
#include "SampleMatrixOperator.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SearchSpaceEvaluatorImpl.h"
#include "MatrixTraceDistanceCalculator.h"
#include "CpuTimer.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixCollectionImpl.h"
#include "SearchSpaceConstructorImpl.h"
#include <iostream>
#include <cmath>
#include <cassert>

TestSuite::TestSuite() {
	m_pMatrixWriter = new SampleMatrixWriterImpl();
	m_pMatrixFactory = new SimpleDenseMatrixFactoryImpl();
	m_pMatrixOperator = new SampleMatrixOperator(m_pMatrixFactory);

	m_pTargets.push_back(m_pMatrixFactory->getIdentityMatrix(2));
	double epsilon = 1e-10;
	m_pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	m_pTimer = new CpuTimer();
	m_pSearchSpaceEvaluator = new SearchSpaceTimerEvaluatorImpl(m_pTargets, epsilon, m_pMatrixDistanceCalculator, m_pMatrixWriter, m_pTimer, std::cout);

	m_pSearchSpaceConstructor = new SearchSpaceConstructorImpl(m_pMatrixOperator);
}

TestSuite::~TestSuite() {
	delete m_pSearchSpaceConstructor;

	delete m_pSearchSpaceEvaluator;
	delete m_pTimer;
	delete m_pMatrixDistanceCalculator;

	for(std::vector<MatrixPtr>::iterator pMatrixIter = m_pTargets.begin(); pMatrixIter != m_pTargets.end();) {
		delete *pMatrixIter;
		pMatrixIter = m_pTargets.erase(pMatrixIter);
	}

	delete m_pMatrixOperator;
	delete m_pMatrixFactory;
	delete m_pMatrixWriter;
}
void TestSuite::test(){
	//testSimpleWriter();
	testSimpleCollection();
	testSimpleSearchSpaceConstructor();
	testSimpleEvaluator();
	testSpecialUnitaryCoordinateCalculator();
}

void TestSuite::testSimpleWriter() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray arrayH = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	arrayH[0] = ComplexVal(1,0) * inverSqrt2;
	arrayH[1] = ComplexVal(1,0) * inverSqrt2;
	arrayH[2] = ComplexVal(1,0) * inverSqrt2;
	arrayH[3] = ComplexVal(-1,0) * inverSqrt2;

	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	m_pMatrixWriter->writeMatrix(pMatrixH, std::cout);

	ComplexValArray arrayT = new ComplexVal[4];
	arrayT[0] = ComplexVal(1,0);
	arrayT[1] = 0.0;
	arrayT[2] = 0.0;
	arrayT[3] = std::exp(ComplexVal(0,1) * M_PI / 4.0);

	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	m_pMatrixWriter->writeMatrix(pMatrixT, std::cout);

	MatrixPtr pMatrixHT = NullPtr;

	m_pMatrixOperator->multiply(pMatrixH, pMatrixT, pMatrixHT);

	m_pMatrixWriter->writeMatrix(pMatrixHT, std::cout);

	delete pMatrixHT;
	delete pMatrixT;
	delete pMatrixH;
	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSimpleCollection() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	MatrixCollectionPtr pMatrixCollection = new SampleMatrixCollectionImpl();
	ComplexValArray arrayH = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	arrayH[0] = ComplexVal(1,0) * inverSqrt2;
	arrayH[1] = ComplexVal(1,0) * inverSqrt2;
	arrayH[2] = ComplexVal(1,0) * inverSqrt2;
	arrayH[3] = ComplexVal(-1,0) * inverSqrt2;
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexValArray arrayT = new ComplexVal[4];
	arrayT[0] = ComplexVal(1,0);
	arrayT[1] = 0.0;
	arrayT[2] = 0.0;
	arrayT[3] = std::exp(ComplexVal(0,1) * M_PI / 4.0);
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	MatrixPtr pMatrixHT = NullPtr;

	m_pMatrixOperator->multiply(pMatrixH, pMatrixT, pMatrixHT);

	pMatrixCollection->addMatrix(pMatrixH);
	pMatrixCollection->addMatrix(pMatrixT);

	assert(pMatrixCollection->size() == 2);

	MatrixPtr pI = m_pMatrixFactory->getIdentityMatrix(2);
	double epsilon = 1;

	MatrixIteratorPtr pResultIter = pMatrixCollection->findApproxMatrices(pI, m_pMatrixDistanceCalculator, epsilon);
	pResultIter->toBegin();

	//Must find T because d(T,I) ~ 0.76537 < epsilon = 1
	//Must not find H because d(H,I) = 2 > epsilon = 1

	assert(!pResultIter->isDone());

	MatrixPtr pResult = pResultIter->getObj();
	assert(pResult == pMatrixT);

	pResultIter->next();
	assert(pResultIter->isDone());

	//m_pSearchSpaceEvaluator->evaluateCollection(pMatrixCollection);

	pMatrixCollection->clear();

	delete pResultIter;
	delete pMatrixHT;
	delete pMatrixT;
	delete pMatrixH;

	delete pMatrixCollection;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSimpleSearchSpaceConstructor() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray arrayH = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	arrayH[0] = ComplexVal(1,0) * inverSqrt2;
	arrayH[1] = ComplexVal(1,0) * inverSqrt2;
	arrayH[2] = ComplexVal(1,0) * inverSqrt2;
	arrayH[3] = ComplexVal(-1,0) * inverSqrt2;
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexValArray arrayT = new ComplexVal[4];
	arrayT[0] = ComplexVal(1,0);
	arrayT[1] = 0.0;
	arrayT[2] = 0.0;
	arrayT[3] = std::exp(ComplexVal(0,1) * M_PI / 4.0);
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	int universalSetSize = 2;
	MatrixCollectionPtr pUniversalSet = new SampleMatrixCollectionImpl();
	pUniversalSet->addMatrix(pMatrixH);
	pUniversalSet->addMatrix(pMatrixT);
	assert(pUniversalSet->size() == universalSetSize);

	MatrixCollectionPtr pMatrixCollection = new SampleMatrixCollectionImpl();

	int maxSequenceLength = 5;
	int expectedSearchSpaceSize = universalSetSize * (std::pow(universalSetSize, maxSequenceLength) - 1) / (universalSetSize - 1);

	m_pSearchSpaceConstructor->constructSearchSpace(pMatrixCollection, pUniversalSet, maxSequenceLength);

	assert(pMatrixCollection->size() == expectedSearchSpaceSize);

	//Make sure constructed search space (collection) has a certain sequence
	MatrixPtr pHT = NullPtr;
	m_pMatrixOperator->multiply(pMatrixH, pMatrixT, pHT);
	MatrixPtr pHTH = NullPtr;
	m_pMatrixOperator->multiply(pHT, pMatrixH, pHTH);
	MatrixPtr pHTHT = NullPtr;
	m_pMatrixOperator->multiply(pHTH, pMatrixT, pHTHT);
	MatrixPtr pHTHTH = NullPtr;
	m_pMatrixOperator->multiply(pHTHT, pMatrixT, pHTHTH);

	MatrixIteratorPtr pResultIter = pMatrixCollection->findApproxMatrices(pHTHTH, m_pMatrixDistanceCalculator, 0);
	assert(pResultIter != NullPtr);
	pResultIter->toBegin();
	assert(!pResultIter->isDone());

	//I = H*H
	MatrixPtr pHH = NullPtr;
	m_pMatrixOperator->multiply(pMatrixH, pMatrixH, pHH);
	pResultIter = pMatrixCollection->findApproxMatrices(pHH, m_pMatrixDistanceCalculator, 0);
	assert(pResultIter != NullPtr);
	pResultIter->toBegin();
	assert(!pResultIter->isDone());

	MatrixPtr pI = m_pMatrixFactory->getIdentityMatrix(2);
	double dI_HH = m_pMatrixDistanceCalculator->distance(pI, pHH);
	assert(dI_HH < 1e-30);

	delete pI;
	delete pHH;
	delete pHTHTH;
	delete pHTHT;
	delete pHTH;
	delete pHT;
	delete pMatrixCollection;
	delete pUniversalSet;
	delete pMatrixT;
	delete pMatrixH;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSimpleEvaluator() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray arrayH = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	arrayH[0] = ComplexVal(1,0) * inverSqrt2;
	arrayH[1] = ComplexVal(1,0) * inverSqrt2;
	arrayH[2] = ComplexVal(1,0) * inverSqrt2;
	arrayH[3] = ComplexVal(-1,0) * inverSqrt2;
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexValArray arrayT = new ComplexVal[4];
	arrayT[0] = ComplexVal(1,0);
	arrayT[1] = 0.0;
	arrayT[2] = 0.0;
	arrayT[3] = std::exp(ComplexVal(0,1) * M_PI / 4.0);
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	int universalSetSize = 2;
	MatrixCollectionPtr pUniversalSet = new SampleMatrixCollectionImpl();
	pUniversalSet->addMatrix(pMatrixH);
	pUniversalSet->addMatrix(pMatrixT);
	assert(pUniversalSet->size() == universalSetSize);

	MatrixCollectionPtr pMatrixCollection = new SampleMatrixCollectionImpl();

	int maxSequenceLength = 5;

	m_pSearchSpaceConstructor->constructSearchSpace(pMatrixCollection, pUniversalSet, maxSequenceLength);

	m_pSearchSpaceEvaluator->evaluateCollection(pMatrixCollection);

	delete pMatrixCollection;
	delete pUniversalSet;
	delete pMatrixT;
	delete pMatrixH;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSpecialUnitaryCoordinateCalculator() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}
