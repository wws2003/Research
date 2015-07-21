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
#include "VectorBasedMatrixCollectionImpl.h"
#include "SearchSpaceConstructorImpl.cpp"
#include "AlwaysTrueMultiplierMatrixCombinerImpl.h"
#include "InverseCancelationMultiplierMatrixCombinerImpl.h"
#include "MapBasedBinCollectionImpl.cpp"
#include "AlgoInternal.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "CoordinateOnOrthonormalBasisCalculatorImpl.cpp"
#include "ISearchSpaceEvaluator.h"
#include "SearchSpaceTimerEvaluator.cpp"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "Bin.hpp"
#include "Gate.h"
#include "GateCombinerImpl.h"
#include "GateSelectiveCombinabilityCheckerImpl.h"
#include "GateIdentityCycleCombinabilityCheckerImpl.h"
#include "GateCancelationCombinabilityCheckerImpl.h"
#include "GateCoordinateCalculatorImpl.h"
#include "LabelOnlyGateWriterImpl.h"
#include "SampleRealCoordinateWriterImpl.hpp"
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "FullGateWriterImpl.h"
#include "GNATCollectionImpl.cpp"
#include "MatrixFowlerDistanceCalculator.h"
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cassert>
#include <cstdlib>

#define my_small_rand(factor) (double)(rand() - (RAND_MAX / 2)) / (double)RAND_MAX * factor

void initTwoQubitsLibGates(MatrixOperatorPtr pMatrixOperator, GatePtr& pCNOT1, GatePtr& pCNOT2, GatePtr& pH1, GatePtr& pH2, GatePtr& pT1, GatePtr& pT2);

void calculateSpecialUnitaryFromTracelessHermitianCoordinates(MatrixOperatorPtr pMatrixOperator, std::vector<double> coordinates, MatrixPtrVector pHermitianBasis, MatrixPtrRef pU);

void initTwoQubitsGateUniversalSet(MatrixOperatorPtr pMatrixOperator, GateCollectionPtr& pUniversalSet);

void releaseTwoQubitsGateUniversalSet(GateCollectionPtr& pUniversalSet);

void initTwoQubitsGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void releaseTwoQubitsGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void getGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker);

void getGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

void getGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);

void getNearIdentityGate(MatrixOperatorPtr pMatrixOperator, MatrixPtrVector pHermitianBasis, TargetElements<GatePtr>& targets);

void getNumberOfNeighbors(GateCollectionPtr pCollection, const TargetElements<GatePtr>& targets, GateDistanceCalculatorPtr pDistanceCalculator, double epsilon, std::vector<int>& resultNumbers);

void getNumberOfNeighborsFromIterator(GateCollectionPtr pCollection, const TargetElements<GatePtr>& targets, GateDistanceCalculatorPtr pDistanceCalculator, double epsilon, std::vector<int>& resultNumbers);

TestSuite::TestSuite() {
	m_pMatrixWriter = new SampleMatrixWriterImpl();
	m_pMatrixFactory = new SimpleDenseMatrixFactoryImpl();
	m_pMatrixOperator = new SampleMatrixOperator(m_pMatrixFactory);

	m_targets.push_back(m_pMatrixFactory->getIdentityMatrix(2));
	double epsilon = 1e-10;
	m_pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	m_pTimer = new CpuTimer();
	m_pSearchSpaceEvaluator = new SearchSpaceTimerEvaluatorImpl<MatrixPtr>(m_targets, epsilon, m_pMatrixDistanceCalculator, m_pMatrixWriter, m_pTimer, std::cout);

	m_pMatrixCombiner = new MultiplierMatrixCombinerImpl(m_pMatrixOperator);
	m_pSearchSpaceConstructor = new SearchSpaceConstructorImpl<MatrixPtr>(m_pMatrixCombiner);
}

TestSuite::~TestSuite() {
	delete m_pSearchSpaceConstructor;
	delete m_pMatrixCombiner;
	delete m_pSearchSpaceEvaluator;
	delete m_pTimer;
	delete m_pMatrixDistanceCalculator;

	for(std::vector<MatrixPtr>::iterator pMatrixIter = m_targets.begin(); pMatrixIter != m_targets.end();) {
		delete *pMatrixIter;
		pMatrixIter = m_targets.erase(pMatrixIter);
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
	testCalculateCoordinatesInSearchSpace();
	testGNATCollectionBuild();
	testGNATSearch();
	freeTestGateCollectionEvaluator();
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

	m_pMatrixWriter->write(pMatrixH, std::cout);

	ComplexValArray arrayT = new ComplexVal[4];
	arrayT[0] = ComplexVal(1,0);
	arrayT[1] = 0.0;
	arrayT[2] = 0.0;
	arrayT[3] = std::exp(ComplexVal(0,1) * M_PI / 4.0);

	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	m_pMatrixWriter->write(pMatrixT, std::cout);

	MatrixPtr pMatrixHT = NullPtr;

	m_pMatrixOperator->multiply(pMatrixH, pMatrixT, pMatrixHT);

	m_pMatrixWriter->write(pMatrixHT, std::cout);

	delete pMatrixHT;
	delete pMatrixT;
	delete pMatrixH;
	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSimpleCollection() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	MatrixCollectionPtr pMatrixCollection = new VectorBasedMatrixCollectionImpl();

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

	MatrixIteratorPtr pResultIter = pMatrixCollection->findNearestNeighbour(pI, m_pMatrixDistanceCalculator, epsilon);
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
	MatrixCollectionPtr pUniversalSet = new VectorBasedMatrixCollectionImpl();
	pUniversalSet->addElement(pMatrixH);
	pUniversalSet->addElement(pMatrixT);
	assert(pUniversalSet->size() == universalSetSize);

	MatrixCollectionPtr pMatrixCollection = new VectorBasedMatrixCollectionImpl();

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

	MatrixIteratorPtr pResultIter = pMatrixCollection->findNearestNeighbour(pHTHTH, m_pMatrixDistanceCalculator, 0);
	assert(pResultIter != NullPtr);
	pResultIter->toBegin();
	assert(!pResultIter->isDone());

	//I = H*H
	MatrixPtr pHH = NullPtr;
	m_pMatrixOperator->multiply(pMatrixH, pMatrixH, pHH);
	pResultIter = pMatrixCollection->findNearestNeighbour(pHH, m_pMatrixDistanceCalculator, 0);
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
	MatrixCollectionPtr pUniversalSet = new VectorBasedMatrixCollectionImpl();
	pUniversalSet->addElement(pMatrixH);
	pUniversalSet->addElement(pMatrixT);
	assert(pUniversalSet->size() == universalSetSize);

	MatrixCollectionPtr pMatrixCollection = new VectorBasedMatrixCollectionImpl();

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
	MatrixSearchSpaceConstructorPtr pSearchSpaceConstructor = new SearchSpaceConstructorImpl<MatrixPtr>(pMatrixCombiner);

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

	MatrixCollectionPtr pUniversalSet = new VectorBasedMatrixCollectionImpl();
	pUniversalSet->addElement(pMatrixH);
	pUniversalSet->addElement(pMatrixT);
	pUniversalSet->addElement(pMatrixInverseH);
	pUniversalSet->addElement(pMatrixInverseT);

	assert(pUniversalSet->size() == universalSetSize);

	MatrixCollectionPtr pMatrixCollection = new VectorBasedMatrixCollectionImpl();
	int maxSequenceLength = 5;
	int expectedSearchSpaceSize = universalSetSize * (std::pow(universalSetSize - 1, maxSequenceLength) - 1) / (universalSetSize - 2);

	pSearchSpaceConstructor->constructSearchSpace(pMatrixCollection, pUniversalSet, maxSequenceLength);

	/*MatrixIteratorPtr pSearchSpaceMatrixIter = pMatrixCollection->getIteratorPtr();

	while(!pSearchSpaceMatrixIter->isDone()) {
		m_pMatrixWriter->write(pSearchSpaceMatrixIter->getObj(), std::cout);
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

void TestSuite::testCalculateCoordinatesInSearchSpace() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	MatrixPtrVector pBasis4;
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, pBasis4);

	assert(pBasis4.size() == 15);

	GateCombinabilityCheckers combinabilityCheckers;
	initTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	GateCollectionPtr pUniversalSet = NullPtr;
	initTwoQubitsGateUniversalSet(m_pMatrixOperator, pUniversalSet);

	GateCollectionPtr pGateCollection = new VectorBasedCollectionImpl<GatePtr>();

	int maxSequenceLength = 4;

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new SearchSpaceConstructorImpl<GatePtr>(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, pUniversalSet, maxSequenceLength);
	CollectionSize_t gateCounts = pGateCollection->size();

	std::cout << "Number of sequence length = " << maxSequenceLength << " constructed by CNOT, H and T: " << gateCounts << std::endl;

	GateWriterPtr pGateWriter = new LabelOnlyGateWriterImpl();
	MatrixRealInnerProductCalculatorPtr pMatrixRealInnerProductCalculator = new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator);
	MatrixRealCoordinateCalculatorPtr pHermitiaRealCoordinateCalculator = new CoordinateOnOrthonormalBasisCalculatorImpl<MatrixPtr, double>(pMatrixRealInnerProductCalculator, pBasis4);
	MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator = new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, pHermitiaRealCoordinateCalculator);

	CoordinateWriterPtr<MatrixPtr, double> pMatrixCoordinateWriter = new SampleRealCoordinateWriterImpl<MatrixPtr>();

	IteratorPtr<GatePtr> pGateIter = pGateCollection->getIteratorPtr();

	int wrongCoordinateCalculateCounter = 0;

	while(!pGateIter->isDone()) {
		//pGateWriter->write(pGateIter->getObj(), std::cout);

		MatrixRealCoordinatePtr pMatrixRealCoordinate = NullPtr;
		pMatrixRealCoordinateCalculator->calulateElementCoordinate(pGateIter->getObj()->getMatrix(), pMatrixRealCoordinate);

		const double COORD_ERR_THRESHOLD = 1e-7;

		//Test if coordinates are calculated correctly
		if(pMatrixRealCoordinate != NullPtr) {

			MatrixPtr pU1 = NullPtr;
			calculateSpecialUnitaryFromTracelessHermitianCoordinates(m_pMatrixOperator, pMatrixRealCoordinate->getCoordinates(), pBasis4, pU1);
			double coordinateCalculatorErr = m_pMatrixDistanceCalculator->distance(pU1, pGateIter->getObj()->getMatrix());

			if(coordinateCalculatorErr > COORD_ERR_THRESHOLD) {

				//Confirm with global phase to make sure incorrect coordinate calculation

				//e^(-i * pi / 2)
				ComplexVal phase1 = std::exp(ComplexVal(0, - M_PI / 2));
				MatrixPtr pGlobalPhaseAdjusted1 = NullPtr;
				m_pMatrixOperator->multiplyScalar(pU1, phase1, pGlobalPhaseAdjusted1);

				//e^(i * pi / 2)
				ComplexVal phase2 = std::exp(ComplexVal(0,  M_PI / 2));
				MatrixPtr pGlobalPhaseAdjusted2 = NullPtr;
				m_pMatrixOperator->multiplyScalar(pU1, phase2, pGlobalPhaseAdjusted2);

				double adjustedError1 = m_pMatrixDistanceCalculator->distance(pGlobalPhaseAdjusted1, pGateIter->getObj()->getMatrix());
				double adjustedError2 = m_pMatrixDistanceCalculator->distance(pGlobalPhaseAdjusted2, pGateIter->getObj()->getMatrix());

				if(adjustedError1 > COORD_ERR_THRESHOLD && adjustedError2 > COORD_ERR_THRESHOLD) {
					wrongCoordinateCalculateCounter++;
				}

				delete pGlobalPhaseAdjusted2;
				delete pGlobalPhaseAdjusted1;

			}

			//assert(coordinateCalculatorErr < COORD_ERR_THRESHOLD);
			_destroy(pU1);
		}

		_destroy(pMatrixRealCoordinate);

		pGateIter->next();
	}

	_destroy(pGateIter);

	delete pMatrixCoordinateWriter;
	delete pMatrixRealCoordinateCalculator;
	delete pHermitiaRealCoordinateCalculator;
	delete pMatrixRealInnerProductCalculator;
	delete pGateWriter;
	delete pGateSearchSpaceConstructor;

	pGateCollection->purge();
	delete pGateCollection;

	releaseTwoQubitsGateUniversalSet(pUniversalSet);
	delete pGateCombiner;

	releaseTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	std::cout << __func__ << " failed coordinate cases: " << wrongCoordinateCalculateCounter << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testGNATCollectionBuild() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	GateCombinabilityCheckers combinabilityCheckers;
	initTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	GateCollectionPtr pUniversalSet = NullPtr;
	initTwoQubitsGateUniversalSet(m_pMatrixOperator, pUniversalSet);

	GateCollectionPtr pGateCollection = new VectorBasedCollectionImpl<GatePtr>();

	int maxSequenceLength = 4;

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new SearchSpaceConstructorImpl<GatePtr>(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, pUniversalSet, maxSequenceLength);

	CollectionSize_t gateCounts = pGateCollection->size();

	GateCollectionPtr pGNATCollection = new GNATCollectionImpl<GatePtr>();
	pGateSearchSpaceConstructor->constructSearchSpace(pGNATCollection, pUniversalSet, maxSequenceLength);
	assert(pGNATCollection->size() == gateCounts);

	GateIteratorPtr pGateIter = pGNATCollection->getIteratorPtr();

	CollectionSize_t gateCountsFromIterator = 0;
	while(!pGateIter->isDone()) {
		gateCountsFromIterator++;
		pGateIter->next();
	}

	assert(gateCountsFromIterator == gateCounts);
	delete pGateIter;

	MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixFowlerDistanceCalculator(m_pMatrixOperator);
	GateDistanceCalculatorPtr pGateDistanceCalculator = new GateDistanceCalculatorByMatrixImpl(pMatrixDistanceCalculator);
	pGNATCollection->rebuildStructure(pGateDistanceCalculator);

	assert(pGNATCollection->size() == gateCounts);

	pGateIter = pGNATCollection->getIteratorPtr();

	gateCountsFromIterator = 0;
	while(!pGateIter->isDone()) {
		gateCountsFromIterator++;
		pGateIter->next();
	}

	assert(gateCountsFromIterator == gateCounts);

	delete pGateIter;

	delete pMatrixDistanceCalculator;
	delete pGateDistanceCalculator;

	delete pGNATCollection;
	delete pGateCollection;

	releaseTwoQubitsGateUniversalSet(pUniversalSet);
	delete pGateCombiner;
	releaseTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	std::cout << __func__ << " passed"  <<  std::endl ;
}

void TestSuite::testGNATSearch() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	MatrixPtrVector pBasis4;
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, pBasis4);

	assert(pBasis4.size() == 15);

	GateCombinabilityCheckers combinabilityCheckers;
	initTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	GateCollectionPtr pUniversalSet = NullPtr;
	initTwoQubitsGateUniversalSet(m_pMatrixOperator, pUniversalSet);

	GateCollectionPtr pGateVectorCollection = new VectorBasedCollectionImpl<GatePtr>();
	GateCollectionPtr pGateGNATCollection = new GNATCollectionImpl<GatePtr>();

	int maxSequenceLength = 5;

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new SearchSpaceConstructorImpl<GatePtr>(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateVectorCollection, pUniversalSet, maxSequenceLength);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateGNATCollection, pUniversalSet, maxSequenceLength);

	double epsilon = 0.5;
	TargetElements<GatePtr> targets;
	getNearIdentityGate(m_pMatrixOperator, pBasis4, targets);

	MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixFowlerDistanceCalculator(m_pMatrixOperator);
	GateDistanceCalculatorPtr pGateDistanceCalculator = new GateDistanceCalculatorByMatrixImpl(pMatrixDistanceCalculator);

	std::vector<int> vectorCollectionResultNumbers;
	std::vector<int> gnatCollectionResultNumbers;

	/* Before constructing data structure */
	getNumberOfNeighbors(pGateVectorCollection, targets, pGateDistanceCalculator, epsilon, vectorCollectionResultNumbers);
	getNumberOfNeighbors(pGateGNATCollection, targets, pGateDistanceCalculator, epsilon, gnatCollectionResultNumbers);

	assert(gnatCollectionResultNumbers.size() == vectorCollectionResultNumbers.size());

	for(unsigned int i = 0; i < gnatCollectionResultNumbers.size(); i++) {
		assert(gnatCollectionResultNumbers[i] == vectorCollectionResultNumbers[i]);
	}

	/* After constructing data structure */
	pGateGNATCollection->rebuildStructure(pGateDistanceCalculator);
	getNumberOfNeighborsFromIterator(pGateGNATCollection, targets, pGateDistanceCalculator, epsilon, gnatCollectionResultNumbers);

	for(unsigned int i = 0; i < gnatCollectionResultNumbers.size(); i++) {
		printf("Number of results for query %d: [GNAT: %d][Vector: %d]\n", i, gnatCollectionResultNumbers[i], vectorCollectionResultNumbers[i]);
		assert(gnatCollectionResultNumbers[i] == vectorCollectionResultNumbers[i]);
	}

	getNumberOfNeighbors(pGateGNATCollection, targets, pGateDistanceCalculator, epsilon, gnatCollectionResultNumbers);
	for(unsigned int i = 0; i < gnatCollectionResultNumbers.size(); i++) {
		assert(gnatCollectionResultNumbers[i] == vectorCollectionResultNumbers[i]);
	}

	delete pMatrixDistanceCalculator;
	delete pGateDistanceCalculator;
	delete pGateSearchSpaceConstructor;

	delete pGateGNATCollection;

	delete pGateVectorCollection;

	releaseTwoQubitsGateUniversalSet(pUniversalSet);
	delete pGateCombiner;
	std::cout << __func__ << " passed"  <<  std::endl ;
}

void TestSuite::freeTestGateCollectionEvaluator() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	MatrixPtrVector pBasis4;
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, pBasis4);

	assert(pBasis4.size() == 15);

	GateCombinabilityCheckers combinabilityCheckers;
	initTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	GateCollectionPtr pUniversalSet = NullPtr;
	initTwoQubitsGateUniversalSet(m_pMatrixOperator, pUniversalSet);

	GateCollectionPtr pGateCollection = new GNATCollectionImpl<GatePtr>();

	int maxSequenceLength = 6;

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new SearchSpaceConstructorImpl<GatePtr>(pGateCombiner);

	pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, pUniversalSet, maxSequenceLength);

	CollectionSize_t gateCounts = pGateCollection->size();

	std::cout << "Number of sequence length = " << maxSequenceLength << " constructed by CNOT, H and T: " << gateCounts << std::endl;

	GateWriterPtr pGateWriter = new FullGateWriterImpl();
	MatrixRealInnerProductCalculatorPtr pMatrixRealInnerProductCalculator = new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator);
	MatrixRealCoordinateCalculatorPtr pHermitiaRealCoordinateCalculator = new CoordinateOnOrthonormalBasisCalculatorImpl<MatrixPtr, double>(pMatrixRealInnerProductCalculator, pBasis4);
	MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator = new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, pHermitiaRealCoordinateCalculator);
	GateRealCoordinateCalculatorPtr pGateRealCoordinateCalculator = new GateCoordinateCalculatorImpl(pMatrixRealCoordinateCalculator);

	CoordinateWriterPtr<MatrixPtr, double> pMatrixCoordinateWriter = new SampleRealCoordinateWriterImpl<MatrixPtr>();

	double epsilon = 0.5;
	TargetElements<GatePtr> targets;
	getNearIdentityGate(m_pMatrixOperator, pBasis4, targets);

	MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixFowlerDistanceCalculator(m_pMatrixOperator);
	GateDistanceCalculatorPtr pGateDistanceCalculator = new GateDistanceCalculatorByMatrixImpl(pMatrixDistanceCalculator);
	GateSearchSpaceEvaluatorPtr pGateSearchSpaceEvaluator = new SearchSpaceTimerEvaluatorImpl<GatePtr>(targets, epsilon, pGateDistanceCalculator, pGateWriter, m_pTimer, std::cout);

	pGateCollection->rebuildStructure(pGateDistanceCalculator);
	pGateSearchSpaceEvaluator->evaluateCollection(pGateCollection);

	delete pGateSearchSpaceEvaluator;
	delete pMatrixDistanceCalculator;
	delete pGateDistanceCalculator;

	delete pMatrixCoordinateWriter;
	delete pGateRealCoordinateCalculator;
	delete pMatrixRealCoordinateCalculator;
	delete pHermitiaRealCoordinateCalculator;
	delete pMatrixRealInnerProductCalculator;
	delete pGateWriter;
	delete pGateSearchSpaceConstructor;

	pGateCollection->purge();
	delete pGateCollection;

	releaseTwoQubitsGateUniversalSet(pUniversalSet);
	delete pGateCombiner;

	releaseTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	std::cout << __func__ << " passed: " << std::endl << "--------------------------"<<  std::endl ;
}

void initTwoQubitsLibGates(MatrixOperatorPtr pMatrixOperator, GatePtr& pCNOT1, GatePtr& pCNOT2, GatePtr& pH1, GatePtr& pH2, GatePtr& pT1, GatePtr& pT2) {
	ComplexVal arrayCNOT1[] = {1.0, 0.0, 0.0, 0.0
			, 0.0, 1.0, 0.0, 0.0
			, 0.0, 0.0, 0.0, 1.0
			, 0.0, 0.0, 1.0, 0.0};

	MatrixPtr pCNOTMat1 = new SimpleDenseMatrixImpl(arrayCNOT1, ROW_SPLICE, 4, 4, "");
	MatrixPtr pSCNOTMat1 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat1, pSCNOTMat1);


	pCNOT1 = new Gate(pSCNOTMat1, 1, "CNOT1");

	ComplexVal arrayCNOT2[] = {0.0, 1.0, 0.0, 0.0
			, 1.0, 0.0, 0.0, 0.0
			, 0.0, 0.0, 1.0, 0.0
			, 0.0, 0.0, 0.0, 1.0};

	MatrixPtr pCNOTMat2 = new SimpleDenseMatrixImpl(arrayCNOT2, ROW_SPLICE, 4, 4, "");
	MatrixPtr pSCNOTMat2 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat2, pSCNOTMat2);

	pCNOT2 = new Gate(pSCNOTMat2, 1, "CNOT2");

	double inverseSQRT2 = 1/sqrt(2);

	ComplexVal arrayH1[] = {ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(inverseSQRT2, 0), 0.0
			, 0.0, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(inverseSQRT2, 0)
			, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(-inverseSQRT2, 0), 0.0
			, 0.0, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH1Mat = new SimpleDenseMatrixImpl(arrayH1, ROW_SPLICE, 4, 4, "");

	pH1 = new Gate(pH1Mat, 4, "H1");

	ComplexVal arrayH2[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0), 0.0, 0.0
			, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0), 0.0, 0.0
			, 0.0, 0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0)
			, 0.0, 0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH2Mat = new SimpleDenseMatrixImpl(arrayH2, ROW_SPLICE, 4, 4, "");

	pH2 = new Gate(pH2Mat, 4, "H2");

	ComplexVal expmPi_8 = std::exp(ComplexVal(0, 1) * M_PI / 8.0);
	ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, -1) * M_PI / 8.0);

	ComplexVal arrayT1[] = {expm_Pi_8, 0.0, 0.0, 0.0
			, 0.0, expm_Pi_8, 0.0, 0.0
			, 0.0, 0.0, expmPi_8, 0.0
			, 0.0, 0.0, 0.0, expmPi_8};

	MatrixPtr pT1Mat = new SimpleDenseMatrixImpl(arrayT1, ROW_SPLICE, 4, 4, "");

	pT1 = new Gate(pT1Mat, 1, "T1");

	ComplexVal arrayT2[] = {expm_Pi_8, 0.0, 0.0, 0.0
			, 0.0, expmPi_8, 0.0, 0.0
			, 0.0, 0.0, expm_Pi_8, 0.0
			, 0.0, 0.0, 0.0, expmPi_8};

	MatrixPtr pT2Mat = new SimpleDenseMatrixImpl(arrayT2, ROW_SPLICE, 4, 4, "");

	pT2 = new Gate(pT2Mat, 1, "T2");
}

void calculateSpecialUnitaryFromTracelessHermitianCoordinates(MatrixOperatorPtr pMatrixOperator, std::vector<double> coordinates, MatrixPtrVector pHermitianBasis, MatrixPtrRef pU) {
	ComplexVal zeroArr[] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	MatrixPtr pH = new SimpleDenseMatrixImpl(zeroArr, ROW_SPLICE, 4, 4, "Hermitian");

	MatrixWriterPtr pMatrixWriter = new SampleMatrixWriterImpl();

	for(int i = 0; i < 15; i++) {
		//pMatrixWriter->write(pHermitianBasis[i], std::cout);

		MatrixPtr pPartialSum = NullPtr;
		pMatrixOperator->multiplyScalar(pHermitianBasis[i], ComplexVal(coordinates[i], 0.0), pPartialSum);
		MatrixPtr pSum = NullPtr;
		pMatrixOperator->add(pH, pPartialSum, pSum);
		delete pPartialSum;
		delete pH;
		pH = pSum;

		//pMatrixWriter->write(pH, std::cout);
	}

	//pMatrixWriter->write(pH, std::cout);

	MatrixPtr pHi = NullPtr;
	pMatrixOperator->multiplyScalar(pH, ComplexVal(0.0,1.0), pHi);

	pMatrixOperator->exponential(pHi, pU);

	//pMatrixWriter->write(pU, std::cout);

	delete pMatrixWriter;

	delete pHi;

	delete pH;
}

void initTwoQubitsGateUniversalSet(MatrixOperatorPtr pMatrixOperator, GateCollectionPtr& pUniversalSet) {
	GatePtr pCNOT1Gate = NullPtr;
	GatePtr pCNOT2Gate = NullPtr;
	GatePtr pH1Gate = NullPtr;
	GatePtr pH2Gate = NullPtr;
	GatePtr pT1Gate = NullPtr;
	GatePtr pT2Gate = NullPtr;

	initTwoQubitsLibGates(pMatrixOperator, pCNOT1Gate,pCNOT2Gate,  pH1Gate, pH2Gate, pT1Gate, pT2Gate);

	pUniversalSet = new VectorBasedCollectionImpl<GatePtr>();
	pUniversalSet->addElement(pCNOT1Gate);
	pUniversalSet->addElement(pCNOT2Gate);
	pUniversalSet->addElement(pH1Gate);
	pUniversalSet->addElement(pH2Gate);
	pUniversalSet->addElement(pT1Gate);
	pUniversalSet->addElement(pT2Gate);
}

void releaseTwoQubitsGateUniversalSet(GateCollectionPtr& pUniversalSet) {

	pUniversalSet->clear();
	delete pUniversalSet;
}


void initTwoQubitsGateCombinabilityCheckers(GateCombinabilityCheckers& combinabilityCheckers) {
	combinabilityCheckers.clear();

	GateCombinabilityCheckerPtr pIdentityCycleChecker = NullPtr;
	getGateIdentityCycleCombinabilityChecker(pIdentityCycleChecker);

	GateCombinabilityCheckerPtr pCombinableChecker = NullPtr;
	getGateSelectiveCombinabilityChecker(pCombinableChecker);

	GateCombinabilityCheckerPtr pCancelationChecker = NullPtr;
	getGateCancelationCombinabilityChecker(pCancelationChecker);

	combinabilityCheckers = {pIdentityCycleChecker, pCombinableChecker, pCancelationChecker};
}

void releaseTwoQubitsGateCombinabilityCheckers(GateCombinabilityCheckers& checkers) {
	for(GateCombinabilityCheckers::iterator cIter = checkers.begin(); cIter != checkers.end();) {
		delete *cIter;
		cIter = checkers.erase(cIter);
	}
}

void getGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker) {
	CombinableGateLabelMap combinableLabelMap;
	combinableLabelMap.insert(CombinableGateLabelPair("T1","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","CNOT1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("H1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","CNOT1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("T2","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","CNOT1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("H2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H2","CNOT1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H2","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT1","CNOT2"));

	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CNOT2","CNOT1"));

	pGateSelectiveCombinabilityChecker = new GateSelectiveCombinabilityCheckerImpl(combinableLabelMap);
}

void getGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker) {
	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair("T1", 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair("T2", 8));

	pGateIdentityCycleCombinabilityChecker = new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap);
}

void getGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker) {
	GateLabelCancelationMap cancelationMap;
	cancelationMap.insert(GateLabelPair("CNOT2", "CNOT1CNOT2CNOT1"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2CNOT1CNOT2"));
	pGateCancelationCombinabilityChecker = new GateCancelationCombinabilityCheckerImpl(cancelationMap);
}


void getNearIdentityGate(MatrixOperatorPtr pMatrixOperator, MatrixPtrVector pHermitianBasis, TargetElements<GatePtr>& targets) {
	/* initialize random seed: */
	srand (time(NULL));

	std::vector<double> randCoords;
	for(unsigned int j = 0; j < pHermitianBasis.size(); j++) {
		randCoords.push_back(my_small_rand(0.0));
	}
	MatrixPtr pU = NullPtr;

	calculateSpecialUnitaryFromTracelessHermitianCoordinates(pMatrixOperator, randCoords, pHermitianBasis, pU);

	//Test add global phase to U
	MatrixPtr pIdentityWithPhasePI_2 = NullPtr;
	MatrixPtr pIdentityWithPhase_PI_2 = NullPtr;
	MatrixPtr pIdentityWithPhasePI = NullPtr;
	MatrixPtr pIdentityWithPhase_PI = NullPtr;

	ComplexVal expmPi_2 = std::exp(ComplexVal(0, 1) * M_PI / 2.0);
	ComplexVal expm_Pi_2 = std::exp(ComplexVal(0, -1) * M_PI / 2.0);
	ComplexVal expmPi = std::exp(ComplexVal(0, 1) * M_PI);
	ComplexVal expm_Pi = std::exp(ComplexVal(0, -1) * M_PI);

	pMatrixOperator->multiplyScalar(pU, expmPi_2, pIdentityWithPhasePI_2);
	pMatrixOperator->multiplyScalar(pU, expm_Pi_2, pIdentityWithPhase_PI_2);
	pMatrixOperator->multiplyScalar(pU, expmPi, pIdentityWithPhasePI);
	pMatrixOperator->multiplyScalar(pU, expm_Pi, pIdentityWithPhase_PI);


	GatePtr pTargetGate = new Gate(pU, 999999, "Unknown");
	GatePtr pTargetGatePI_2 = new Gate(pIdentityWithPhasePI_2, 999999, "Unknown");
	GatePtr pTargetGate_PI_2 = new Gate(pIdentityWithPhase_PI_2, 999999, "Unknown");
	GatePtr pTargetGatePI = new Gate(pIdentityWithPhasePI, 999999, "Unknown");
	GatePtr pTargetGate_PI = new Gate(pIdentityWithPhase_PI, 999999, "Unknown");

	targets.push_back(pTargetGate);
	targets.push_back(pTargetGatePI_2);
	targets.push_back(pTargetGate_PI_2);
	targets.push_back(pTargetGatePI);
	targets.push_back(pTargetGate_PI);
}

void getNumberOfNeighbors(GateCollectionPtr pCollection, const TargetElements<GatePtr>& targets, GateDistanceCalculatorPtr pDistanceCalculator, double epsilon, std::vector<int>& resultNumbers) {
	resultNumbers.clear();
	for(TargetElements<GatePtr>::const_iterator eIter = targets.begin(); eIter != targets.end(); eIter++) {
		GatePtr pQuery = *eIter;
		//printf("----------------------------\n");
		GateIteratorPtr pGateIter = pCollection->findNearestNeighbour(pQuery, pDistanceCalculator, epsilon);
		//printf("----------------------------\n");
		int nbResults = 0;
		while(pGateIter != NullPtr && !pGateIter->isDone()) {
			nbResults++;
			pGateIter->next();
		}
		resultNumbers.push_back(nbResults);
		_destroy(pGateIter);
	}
}

void getNumberOfNeighborsFromIterator(GateCollectionPtr pCollection, const TargetElements<GatePtr>& targets, GateDistanceCalculatorPtr pDistanceCalculator, double epsilon, std::vector<int>& resultNumbers) {
	resultNumbers.clear();
	for(TargetElements<GatePtr>::const_iterator eIter = targets.begin(); eIter != targets.end(); eIter++) {
		GatePtr pQuery = *eIter;

		int nbResults = 0;
		GateIteratorPtr pGateIter = pCollection->getIteratorPtr();
		while(!pGateIter->isDone()) {
			if(pDistanceCalculator->distance(pGateIter->getObj(), pQuery) <= epsilon) {
				nbResults++;
			}
			pGateIter->next();
		}

		resultNumbers.push_back(nbResults);
		_destroy(pGateIter);
	}
}


