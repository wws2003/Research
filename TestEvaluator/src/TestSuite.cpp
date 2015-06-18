/*
 * TestSuite.cpp
 *
 *  Created on: May 16, 2015
 *      Author: pham
 */

#include "TestSuite.h"
#include "SampleMatrixWriter.h"
#include "LabelOnlyMatrixWriterImpl.h"
#include "SimpleDenseMatrixImpl.h"
#include "SampleMatrixOperator.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SearchSpaceEvaluatorImpl.h"
#include "MatrixTraceDistanceCalculator.h"
#include "CpuTimer.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixCollectionImpl.h"
#include "SearchSpaceConstructorImpl.h"
#include "AlwaysTrueMultiplierMatrixCombinerImpl.h"
#include "InverseCancelationMultiplierMatrixCombinerImpl.h"
#include "MapBasedBinCollectionImpl.cpp"
#include "AlgoInternal.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "CoordinateOnOrthonormalBasisCalculatorImpl.cpp"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "Bin.hpp"

#include <iostream>
#include <cmath>
#include <cassert>

TestSuite::TestSuite() {
	m_pMatrixWriter = new LabelOnlyMatrixWriterImpl();
	m_pMatrixFactory = new SimpleDenseMatrixFactoryImpl();
	m_pMatrixOperator = new SampleMatrixOperator(m_pMatrixFactory);

	m_pTargets.push_back(m_pMatrixFactory->getIdentityMatrix(2));
	double epsilon = 1e-10;
	m_pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	m_pTimer = new CpuTimer();
	m_pSearchSpaceEvaluator = new SearchSpaceTimerEvaluatorImpl(m_pTargets, epsilon, m_pMatrixDistanceCalculator, m_pMatrixWriter, m_pTimer, std::cout);

	m_pMatrixCombiner = new MultiplierMatrixCombinerImpl(m_pMatrixOperator);
	m_pSearchSpaceConstructor = new SearchSpaceConstructorImpl(m_pMatrixCombiner);
}

TestSuite::~TestSuite() {
	delete m_pSearchSpaceConstructor;
	delete m_pMatrixCombiner;
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
	testInverseCancelingSearchSpaceConstructor();
	testSampleMatrixBinCollection();
	freeTestShowCoordinatesInSearchSpace();
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

	double inverSqrt2 = 1 / sqrt(2);
	ComplexVal arrayH[] = {ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(-1,0) * inverSqrt2};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), 0.0, 0.0, std::exp(ComplexVal(0,1) * M_PI / 4.0)};
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	MatrixPtr pMatrixHT = NullPtr;

	m_pMatrixOperator->multiply(pMatrixH, pMatrixT, pMatrixHT);

	pMatrixCollection->addElement(pMatrixH);
	pMatrixCollection->addElement(pMatrixT);

	assert(pMatrixCollection->size() == 2);

	MatrixPtr pI = m_pMatrixFactory->getIdentityMatrix(2);
	double epsilon = 1;

	MatrixIteratorPtr pResultIter = pMatrixCollection->findApproxElements(pI, m_pMatrixDistanceCalculator, epsilon);
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

	double inverSqrt2 = 1 / sqrt(2);
	ComplexVal arrayH[] = {ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(-1,0) * inverSqrt2};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), 0.0, 0.0, std::exp(ComplexVal(0,1) * M_PI / 4.0)};
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	int universalSetSize = 2;
	MatrixCollectionPtr pUniversalSet = new SampleMatrixCollectionImpl();
	pUniversalSet->addElement(pMatrixH);
	pUniversalSet->addElement(pMatrixT);
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

	MatrixIteratorPtr pResultIter = pMatrixCollection->findApproxElements(pHTHTH, m_pMatrixDistanceCalculator, 0);
	assert(pResultIter != NullPtr);
	pResultIter->toBegin();
	assert(!pResultIter->isDone());

	//I = H*H
	MatrixPtr pHH = NullPtr;
	m_pMatrixOperator->multiply(pMatrixH, pMatrixH, pHH);
	pResultIter = pMatrixCollection->findApproxElements(pHH, m_pMatrixDistanceCalculator, 0);
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

	double inverSqrt2 = 1 / sqrt(2);
	ComplexVal arrayH[] = {ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(-1,0) * inverSqrt2};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), 0.0, 0.0, std::exp(ComplexVal(0,1) * M_PI / 4.0)};
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	int universalSetSize = 2;
	MatrixCollectionPtr pUniversalSet = new SampleMatrixCollectionImpl();
	pUniversalSet->addElement(pMatrixH);
	pUniversalSet->addElement(pMatrixT);
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

void TestSuite::testInverseCancelingSearchSpaceConstructor() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	MatrixCombinerPtr pMatrixCombiner = new InverseCancelationMultiplierMatrixCombinerImpl(m_pMatrixOperator);
	SearchSpaceConstructorPtr pSearchSpaceConstructor = new SearchSpaceConstructorImpl(pMatrixCombiner);

	double inverSqrt2 = 1 / sqrt(2);

	ComplexVal arrayH[] = {ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(-1,0) * inverSqrt2};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), 0.0, 0.0, std::exp(ComplexVal(0,1) * M_PI / 4.0)};
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	MatrixPtr pTmpMatrixInverseH = NullPtr;
	m_pMatrixOperator->conjugateTranpose(pMatrixH, pTmpMatrixInverseH);
	ComplexValArray inverseArrayH = NullPtr;
	pTmpMatrixInverseH->toArray(inverseArrayH);
	MatrixPtr pMatrixInverseH = new SimpleDenseMatrixImpl(inverseArrayH, ROW_SPLICE, 2, 2, "h");
	delete[] inverseArrayH;
	delete pTmpMatrixInverseH;

	MatrixPtr pTmpMatrixInverseT = NullPtr;
	m_pMatrixOperator->conjugateTranpose(pMatrixT, pTmpMatrixInverseT);
	ComplexValArray inverseArrayT = NullPtr;
	pTmpMatrixInverseT->toArray(inverseArrayT);
	MatrixPtr pMatrixInverseT = new SimpleDenseMatrixImpl(inverseArrayT, ROW_SPLICE, 2, 2, "t");
	delete[] inverseArrayT;
	delete pTmpMatrixInverseT;
	int universalSetSize = 4;

	MatrixCollectionPtr pUniversalSet = new SampleMatrixCollectionImpl();
	pUniversalSet->addElement(pMatrixH);
	pUniversalSet->addElement(pMatrixT);
	pUniversalSet->addElement(pMatrixInverseH);
	pUniversalSet->addElement(pMatrixInverseT);

	assert(pUniversalSet->size() == universalSetSize);

	MatrixCollectionPtr pMatrixCollection = new SampleMatrixCollectionImpl();
	int maxSequenceLength = 5;
	int expectedSearchSpaceSize = universalSetSize * (std::pow(universalSetSize - 1, maxSequenceLength) - 1) / (universalSetSize - 2);

	pSearchSpaceConstructor->constructSearchSpace(pMatrixCollection, pUniversalSet, maxSequenceLength);

	/*MatrixIteratorPtr pSearchSpaceMatrixIter = pMatrixCollection->getIteratorPtr();

	while(!pSearchSpaceMatrixIter->isDone()) {
		m_pMatrixWriter->writeMatrix(pSearchSpaceMatrixIter->getObj(), std::cout);
		pSearchSpaceMatrixIter->next();
	}*/

	assert(pMatrixCollection->size() == expectedSearchSpaceSize);

	delete pMatrixInverseT;
	delete pMatrixInverseH;
	delete pMatrixT;
	delete pMatrixH;
	delete pSearchSpaceConstructor;
	delete pMatrixCombiner;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSampleMatrixBinCollection() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	BinPattern binPattern1 = "0011";
	MatrixBinPtr pMatrixBin1 = new MatrixBin(binPattern1);

	BinPattern binPattern2= "1000";
	MatrixBinPtr pMatrixBin2 = new MatrixBin(binPattern2);

	//distance bin1 -> bin2 = number of same digital in same position = 1 (the second digital of both is '0')
	int expectedBinDistance = 1;

	assert(pMatrixBin1->distance(*pMatrixBin2) == expectedBinDistance);

	MatrixBinCollectionPtr pBinCollection = new MapBasedBinCollectionImpl<MatrixPtr>();

	//Add to collection a matrix with bin pattern = binPattern2
	ComplexVal arrayH[] = {ComplexVal(1,0), ComplexVal(1,0), ComplexVal(1,0), ComplexVal(-1,0)};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");
	pBinCollection->addElement(pMatrixH, binPattern2);

	//Add to collection twice a matrix with bin pattern = binPattern1
	ComplexVal arrayT[] = {ComplexVal(1,0), 0.0, 0.0, std::exp(ComplexVal(0,1) * M_PI / 4.0)};
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");
	pBinCollection->addElement(pMatrixT, binPattern1);
	pBinCollection->addElement(pMatrixT, binPattern1);

	//Expect collection size is 2
	assert(pBinCollection->size() == 2);

	//Find in the bin collection the bins having distance to bin 2 no more than 2.
	int maxDistance = 2;
	MatrixBinIteratorPtr pBinIter = pBinCollection->findBinsCloseToBin(pMatrixBin2, maxDistance);

	//Expected to find bin 1
	assert(!pBinIter->isDone());

	//In the found bin 1, expected to have matrix T twice
	MatrixBinPtr pFoundBin = pBinIter->getObj();
	assert(pFoundBin->getElements().size() == 2);
	assert(pFoundBin->getElements()[0] == pMatrixT);
	assert(pFoundBin->getElements()[1] == pMatrixT);

	//And no more than bin 1
	pBinIter->next();
	assert(pBinIter->isDone());

	delete pMatrixT;
	delete pMatrixH;
	delete pBinCollection;
	delete pMatrixBin2;
	delete pMatrixBin1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::freeTestShowCoordinatesInSearchSpace() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	/*MatrixPtrVector pBasis4;
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, pBasis4);

	ComplexVal arrayCNOT[] = {1.0, 0.0, 0.0, 0.0
			, 0.0, 1.0, 0.0, 0.0
			, 0.0, 0.0, 0.0, 1.0
			, 0.0, 0.0, 1.0, 0.0};

	MatrixPtr pCNOT = new SimpleDenseMatrixImpl(arrayCNOT, ROW_SPLICE, 4, 4, "C");
	MatrixPtr pSCNOT = NullPtr;
	m_pMatrixOperator->specialUnitaryFromUnitary(pCNOT, pSCNOT);*/




	std::cout << __func__ << " under construction " << std::endl << "--------------------------"<<  std::endl ;
}
