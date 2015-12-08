/*
 * TestSuite.cpp
 *
 *  Created on: May 16, 2015
 *      Author: pham
 */

#include "FullTestSuite.h"
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
#include "SearchSpaceConstructorImpl.h"
#include "MatrixSearchSpaceConstructorImpl.h"
#include "GateSearchSpaceConstructorImpl.h"
#include "AlwaysTrueMultiplierMatrixCombinerImpl.h"
#include "InverseCancelationMultiplierMatrixCombinerImpl.h"
#include "MapBasedMatrixBinCollectionImpl.h"
#include "AlgoInternal.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "ISearchSpaceEvaluator.h"
#include "MatrixSearchSpaceTimerEvaluatorImpl.h"
#include "GateSearchSpaceTimerEvaluatorImpl.h"
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
#include "GNATGateCollectionImpl.h"
#include "SampleRealCoordinateWriterImpl.hpp"
#include "MatrixFowlerDistanceCalculator.h"
#include "PersistableGNATGateCollectionImpl.h"
#include "DuplicateGateLookupResultFilterImpl.h"
#include "BinaryGateWriterImpl.h"
#include "BinaryGateReaderImpl.h"
#include "BinaryMatrixReaderImpl.h"
#include "BinaryMatrixWriterImpl.h"
#include "CoordinateAdditionBasedGateComposer.h"
#include "DictionaryOrderCoordinateComparator.hpp"
#include "VectorBasedIteratorImpl.hpp"
#include "GateCoordinateCombinerImpl.h"
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cassert>
#include <cstdlib>

#define DEBUGGING 1

#define EXPERIMENT_BINNING 1

const std::string FullTestSuite::GNAT_COLLECTION_PERSIST_FILE = "gnat2.dat";

//#define IGNORE_PHASE 1

#define my_small_rand(factor) (double)(rand() - (RAND_MAX / 2)) / (double)RAND_MAX * factor

void initTwoQubitsLibGates(MatrixOperatorPtr pMatrixOperator, GatePtr& pCNOT1, GatePtr& pCNOT2, GatePtr& pH1, GatePtr& pH2, GatePtr& pT1, GatePtr& pT2);

void calculateSpecialUnitaryFromTracelessHermitianCoordinates(MatrixOperatorPtr pMatrixOperator, std::vector<mreal_t> coordinates, MatrixPtrVector pHermitianBasis, MatrixPtrRef pU);

void initTwoQubitsGateUniversalSet(MatrixOperatorPtr pMatrixOperator, GateCollectionPtr& pUniversalSet);

void releaseTwoQubitsGateUniversalSet(GateCollectionPtr& pUniversalSet);

void initTwoQubitsGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void releaseTwoQubitsGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

void getGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker);

void getGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

void getGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);

void getNearIdentityGate(MatrixOperatorPtr pMatrixOperator, MatrixPtrVector pHermitianBasis, TargetElements<GatePtr>& targets);

void evalFindNeighbours(GateCollectionPtr pCollection,
		const TargetElements<GatePtr>& targets,
		GateDistanceCalculatorPtr pDistanceCalculator,
		double epsilon,
		std::vector<int>& resultNumbers);

void evalFilteredFindNeighbours(GateCollectionPtr pCollection,
		const TargetElements<GatePtr>& targets,
		GateDistanceCalculatorPtr pDistanceCalculator,
		double epsilon);

void getNumberOfNeighborsFromIterator(GateCollectionPtr pCollection, const TargetElements<GatePtr>& targets, GateDistanceCalculatorPtr pDistanceCalculator, double epsilon, std::vector<int>& resultNumbers);

FullTestSuite::FullTestSuite() {
	m_pMatrixWriter = new SampleMatrixWriterImpl();
	m_pMatrixFactory = new SimpleDenseMatrixFactoryImpl();
	m_pMatrixOperator = new SampleMatrixOperator(m_pMatrixFactory);

	m_targets.push_back(m_pMatrixFactory->getIdentityMatrix(2));
	mreal_t epsilon = 1e-10;
	m_pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	m_pTimer = new CpuTimer();
	m_pSearchSpaceEvaluator = new MatrixSearchSpaceTimerEvaluatorImpl(m_targets,
			epsilon,
			epsilon,
			m_pMatrixDistanceCalculator,
			NullPtr,
			NullPtr,
			m_pMatrixWriter,
			m_pTimer,
			std::cout);

	m_pMatrixCombiner = new MultiplierMatrixCombinerImpl(m_pMatrixOperator);
	m_pSearchSpaceConstructor = new MatrixSearchSpaceConstructorImpl(m_pMatrixCombiner);
}

FullTestSuite::~FullTestSuite() {
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

	std::remove(GNAT_COLLECTION_PERSIST_FILE.c_str());
}
void FullTestSuite::test(){
	//testSimpleWriter();
	testSimpleCollection();
	testSimpleSearchSpaceConstructor();
	testSimpleEvaluator();
	testInverseCancelingSearchSpaceConstructor();
	testSampleMatrixBinCollection();
	/*testCalculateCoordinatesInSearchSpace();
	testGNATCollectionBuild();
	testGNATCollectionPersistence();
	testGNATSearch();
	testFilteredGNATSearch();*/
	testAddtionBasedCoordinateComposer();
	//freeTestGateCollectionEvaluator();
}

void FullTestSuite::testSimpleWriter() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray arrayH = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	arrayH[0] = ComplexVal(inverSqrt2,0);
	arrayH[1] = ComplexVal(inverSqrt2,0);
	arrayH[2] = ComplexVal(inverSqrt2,0);
	arrayH[3] = ComplexVal(-inverSqrt2,0);

	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	m_pMatrixWriter->write(pMatrixH, std::cout);

	ComplexValArray arrayT = new ComplexVal[4];
	arrayT[0] = ComplexVal(1,0);
	arrayT[1] = (ComplexVal)0.0;
	arrayT[2] = (ComplexVal)0.0;
	arrayT[3] = std::exp(ComplexVal(0, M_PI / 4.0));

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

void FullTestSuite::testSimpleCollection() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	MatrixCollectionPtr pMatrixCollection = new VectorBasedMatrixCollectionImpl();

	double inverSqrt2 = 1 / sqrt(2);
	ComplexVal arrayH[] = {ComplexVal(inverSqrt2,0), ComplexVal(inverSqrt2,0),
			ComplexVal(inverSqrt2,0), ComplexVal(-inverSqrt2,0)};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), (ComplexVal)0.0,
			(ComplexVal)0.0, std::exp(ComplexVal(0, M_PI / 4.0))};
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	MatrixPtr pMatrixHT = NullPtr;

	m_pMatrixOperator->multiply(pMatrixH, pMatrixT, pMatrixHT);

	pMatrixCollection->addElement(pMatrixH);
	pMatrixCollection->addElement(pMatrixT);

	assert(pMatrixCollection->size() == 2);

	MatrixPtr pI = m_pMatrixFactory->getIdentityMatrix(2);
	double epsilon = 1;

	IteratorPtr<LookupResult<MatrixPtr> > pResultIter = pMatrixCollection->findNearestNeighbours(pI, m_pMatrixDistanceCalculator, epsilon);
	pResultIter->toBegin();

	//Must find T because d(T,I) ~ 0.76537 < epsilon = 1
	//Must not find H because d(H,I) = 2 > epsilon = 1

	assert(!pResultIter->isDone());

	MatrixPtr pResult = pResultIter->getObj().m_resultElement;
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

void FullTestSuite::testSimpleSearchSpaceConstructor() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	double inverSqrt2 = 1 / sqrt(2);
	ComplexVal arrayH[] = {ComplexVal(inverSqrt2,0), ComplexVal(inverSqrt2,0),
			ComplexVal(inverSqrt2,0), ComplexVal(-inverSqrt2,0)};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), (ComplexVal)0.0,
			(ComplexVal)0.0, std::exp(ComplexVal(0, M_PI / 4.0))};
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

	MatrixLookupResultIteratorPtr pResultIter = pMatrixCollection->findNearestNeighbours(pHTHTH, m_pMatrixDistanceCalculator, 0);
	assert(pResultIter != NullPtr);
	pResultIter->toBegin();
	assert(!pResultIter->isDone());

	//I = H*H
	MatrixPtr pHH = NullPtr;
	m_pMatrixOperator->multiply(pMatrixH, pMatrixH, pHH);
	pResultIter = pMatrixCollection->findNearestNeighbours(pHH, m_pMatrixDistanceCalculator, 0);
	assert(pResultIter != NullPtr);
	pResultIter->toBegin();
	assert(!pResultIter->isDone());

	MatrixPtr pI = m_pMatrixFactory->getIdentityMatrix(2);
	mreal_t dI_HH = m_pMatrixDistanceCalculator->distance(pI, pHH);
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

void FullTestSuite::testSimpleEvaluator() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	double inverSqrt2 = 1 / sqrt(2);
	ComplexVal arrayH[] = {ComplexVal(inverSqrt2,0), ComplexVal(inverSqrt2,0),
			ComplexVal(inverSqrt2,0), ComplexVal(-inverSqrt2,0)};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), (ComplexVal)0.0,
			(ComplexVal)0.0, std::exp(ComplexVal(0, M_PI / 4.0))};
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

void FullTestSuite::testInverseCancelingSearchSpaceConstructor() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	MatrixCombinerPtr pMatrixCombiner = new InverseCancelationMultiplierMatrixCombinerImpl(m_pMatrixOperator);
	MatrixSearchSpaceConstructorPtr pSearchSpaceConstructor = new MatrixSearchSpaceConstructorImpl(pMatrixCombiner);

	double inverSqrt2 = 1 / sqrt(2);

	ComplexVal arrayH[] = {ComplexVal(inverSqrt2,0), ComplexVal(inverSqrt2,0),
			ComplexVal(inverSqrt2,0), ComplexVal(-inverSqrt2,0)};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), (ComplexVal)0.0,
			(ComplexVal)0.0, std::exp(ComplexVal(0, M_PI / 4.0))};
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

void FullTestSuite::testSampleMatrixBinCollection() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

#if !EXPERIMENT_BINNING
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
	ComplexVal arrayT[] = {ComplexVal(1,0), (ComplexVal)0.0,
			(ComplexVal)0.0, std::exp(ComplexVal(0, M_PI / 4.0))};
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
#endif
}

void FullTestSuite::testCalculateCoordinatesInSearchSpace() {
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

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new GateSearchSpaceConstructorImpl(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, pUniversalSet, maxSequenceLength);
	CollectionSize_t gateCounts = pGateCollection->size();

	std::cout << "Number of sequence length = " << maxSequenceLength << " constructed by CNOT, H and T: " << gateCounts << std::endl;

	GateWriterPtr pGateWriter = new LabelOnlyGateWriterImpl();
	MatrixRealInnerProductCalculatorPtr pMatrixRealInnerProductCalculator = new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator);
	MatrixRealCoordinateCalculatorPtr pHermitiaRealCoordinateCalculator = new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(pMatrixRealInnerProductCalculator, pBasis4);
	MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator = new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, pHermitiaRealCoordinateCalculator);

	CoordinateWriterPtr<MatrixPtr, mreal_t> pMatrixCoordinateWriter = new SampleRealCoordinateWriterImpl<MatrixPtr>();

	IteratorPtr<GatePtr> pGateIter = pGateCollection->getIteratorPtr();

	int wrongCoordinateCalculateCounter = 0;

	//Iterate through the whole collection
	while(!pGateIter->isDone()) {
		//pGateWriter->write(pGateIter->getObj(), std::cout);

		MatrixRealCoordinatePtr pMatrixRealCoordinate = NullPtr;
		pMatrixRealCoordinateCalculator->calulateElementCoordinate(pGateIter->getObj()->getMatrix(),
				pMatrixRealCoordinate);

		const double COORD_ERR_THRESHOLD = 1e-7;

		//Test if coordinates are calculated correctly
		if(pMatrixRealCoordinate != NullPtr) {

			MatrixPtr pU1 = NullPtr;

			//Calculate back matrix from coordinate
			calculateSpecialUnitaryFromTracelessHermitianCoordinates(m_pMatrixOperator,
					pMatrixRealCoordinate->getCoordinates(),
					pBasis4,
					pU1);
			mreal_t coordinateCalculatorErr = m_pMatrixDistanceCalculator->distance(pU1, pGateIter->getObj()->getMatrix());

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

				mreal_t adjustedError1 = m_pMatrixDistanceCalculator->distance(pGlobalPhaseAdjusted1, pGateIter->getObj()->getMatrix());
				mreal_t adjustedError2 = m_pMatrixDistanceCalculator->distance(pGlobalPhaseAdjusted2, pGateIter->getObj()->getMatrix());

				if(adjustedError1 > COORD_ERR_THRESHOLD && adjustedError2 > COORD_ERR_THRESHOLD) {
					wrongCoordinateCalculateCounter++;
#if DEBUGGING
					std::cout << "Abnormal case:" << std::endl;
					pGateWriter->write(pGateIter->getObj(), std::cout);
					m_pMatrixWriter->write(pGateIter->getObj()->getMatrix(), std::cout);
					m_pMatrixWriter->write(pU1, std::cout);
					pMatrixCoordinateWriter->writeCoordinate(*pMatrixRealCoordinate, std::cout);
					std::cout << coordinateCalculatorErr << " " << adjustedError1 << " " << adjustedError2 << std::endl;
#endif
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

void FullTestSuite::testGNATCollectionBuild() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	GateCombinabilityCheckers combinabilityCheckers;
	initTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	GateCollectionPtr pUniversalSet = NullPtr;
	initTwoQubitsGateUniversalSet(m_pMatrixOperator, pUniversalSet);

	GateCollectionPtr pGateCollection = new VectorBasedCollectionImpl<GatePtr>();

	int maxSequenceLength = 4;

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new GateSearchSpaceConstructorImpl(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, pUniversalSet, maxSequenceLength);

	CollectionSize_t gateCounts = pGateCollection->size();

	GateCollectionPtr pGNATCollection = new GNATGateCollectionImpl();
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
	//MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
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

void FullTestSuite::testGNATCollectionPersistence() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	GateCombinabilityCheckers combinabilityCheckers;
	initTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	GateCollectionPtr pUniversalSet = NullPtr;
	initTwoQubitsGateUniversalSet(m_pMatrixOperator, pUniversalSet);

	//Instantiate gate writer, reader
	MatrixWriterPtr pMatrixWriter = new BinaryMatrixWriterImpl();
	GateWriterPtr pGateWriter = new BinaryGateWriterImpl(pMatrixWriter);
	MatrixReaderPtr pMatrixReader = new BinaryMatrixReaderImpl(m_pMatrixFactory);
	GateReaderPtr pGateReader = new BinaryGateReaderImpl(pMatrixReader);

	//Instantiate persistable GNAT collection
	PersistableGNATGateCollectionImpl* pGateCollection = new PersistableGNATGateCollectionImpl(pGateWriter, pGateReader);

	//Construct collection
	int maxSequenceLength = 5;
	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new GateSearchSpaceConstructorImpl(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, pUniversalSet, maxSequenceLength);
	std::cout << "Number of sequence length = " << maxSequenceLength << " constructed by CNOT, H and T: " << pGateCollection->size() << std::endl;

	//Re-structured collection respecting distance function
	MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixFowlerDistanceCalculator(m_pMatrixOperator);
	//MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	GateDistanceCalculatorPtr pGateDistanceCalculator = new GateDistanceCalculatorByMatrixImpl(pMatrixDistanceCalculator);
	pGateCollection->rebuildStructure(pGateDistanceCalculator);
	std::cout << "Finish re-constructing" << std::endl;

	//Save collection
	pGateCollection->save(GNAT_COLLECTION_PERSIST_FILE);
	std::cout << "Finish saving" << std::endl;

	//Load collection
	PersistableGNATGateCollectionImpl* pReadGateCollection = new PersistableGNATGateCollectionImpl(pGateWriter, pGateReader);
	pReadGateCollection->load(GNAT_COLLECTION_PERSIST_FILE);
	std::cout << "Finish loading" << std::endl;

	//Assert read back OK
	assert(*pReadGateCollection == *pGateCollection);

	delete pReadGateCollection;

	delete pGateDistanceCalculator;
	delete pMatrixDistanceCalculator;

	delete pGateCollection;

	delete pGateReader;
	delete pMatrixReader;
	delete pGateWriter;
	delete pMatrixWriter;

	releaseTwoQubitsGateUniversalSet(pUniversalSet);
	delete pGateCombiner;
	releaseTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	std::cout << __func__ << " passed"  <<  std::endl ;
}

void FullTestSuite::testGNATSearch() {
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
	GateCollectionPtr pGateGNATCollection = new GNATGateCollectionImpl();

	int maxSequenceLength = 5;

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new GateSearchSpaceConstructorImpl(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateVectorCollection, pUniversalSet, maxSequenceLength);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateGNATCollection, pUniversalSet, maxSequenceLength);

	double epsilon = 0.5;
	TargetElements<GatePtr> targets;
	getNearIdentityGate(m_pMatrixOperator, pBasis4, targets);

	MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixFowlerDistanceCalculator(m_pMatrixOperator);
	//MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	GateDistanceCalculatorPtr pGateDistanceCalculator = new GateDistanceCalculatorByMatrixImpl(pMatrixDistanceCalculator);

	std::vector<int> vectorCollectionResultNumbers;
	std::vector<int> gnatCollectionResultNumbers;

	/* Before constructing data structure */
	evalFindNeighbours(pGateVectorCollection, targets, pGateDistanceCalculator, epsilon, vectorCollectionResultNumbers);
	evalFindNeighbours(pGateGNATCollection, targets, pGateDistanceCalculator, epsilon, gnatCollectionResultNumbers);

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

	evalFindNeighbours(pGateGNATCollection, targets, pGateDistanceCalculator, epsilon, gnatCollectionResultNumbers);
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

void FullTestSuite::testFilteredGNATSearch() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	MatrixPtrVector pBasis4;
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, pBasis4);

	assert(pBasis4.size() == 15);

	GateCombinabilityCheckers combinabilityCheckers;
	initTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	GateCollectionPtr pUniversalSet = NullPtr;
	initTwoQubitsGateUniversalSet(m_pMatrixOperator, pUniversalSet);

	LookupResultFilterPtr<GatePtr> pGateLookupResultFilter = new DuplicateGateLookupResultFilterImpl();
	GateCollectionPtr pGateGNATCollection = new GNATGateCollectionImpl(pGateLookupResultFilter);

	int maxSequenceLength = 5;

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new GateSearchSpaceConstructorImpl(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateGNATCollection, pUniversalSet, maxSequenceLength);

	double epsilon = 0.5;
	TargetElements<GatePtr> targets;
	getNearIdentityGate(m_pMatrixOperator, pBasis4, targets);

	MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixFowlerDistanceCalculator(m_pMatrixOperator);
	//MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	GateDistanceCalculatorPtr pGateDistanceCalculator = new GateDistanceCalculatorByMatrixImpl(pMatrixDistanceCalculator);

	evalFilteredFindNeighbours(pGateGNATCollection, targets, pGateDistanceCalculator, epsilon);

	/* After constructing data structure */
	pGateGNATCollection->rebuildStructure(pGateDistanceCalculator);
	evalFilteredFindNeighbours(pGateGNATCollection, targets, pGateDistanceCalculator, epsilon);

	delete pMatrixDistanceCalculator;
	delete pGateDistanceCalculator;
	delete pGateSearchSpaceConstructor;

	delete pGateGNATCollection;
	delete pGateLookupResultFilter;

	releaseTwoQubitsGateUniversalSet(pUniversalSet);
	delete pGateCombiner;
	std::cout << __func__ << " passed"  <<  std::endl ;
}

void FullTestSuite::testAddtionBasedCoordinateComposer() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	BuildingBlockBuckets<GateRealCoordinate> buckets;

	//Bucket1
	//(0,0), (0,1), (0,2), (0,3), (0,4),
	std::vector<GateRealCoordinate> buildingBlocks1;
	for(int i = 0; i <= 4; i++) {
		GateRealCoordinate c(NullPtr, {0.0, i + 0.0});
		buildingBlocks1.push_back(c);
	}
	IteratorPtr<GateRealCoordinate> pGateCoordIter1 = new VectorBasedIteratorImpl<GateRealCoordinate>(&buildingBlocks1);
	buckets.push_back(pGateCoordIter1);

	//Bucket2
	//(1,0), (1,1), (1,2), (1,3), (1,4),
	std::vector<GateRealCoordinate> buildingBlocks2;
	for(int i = 0; i <= 4; i++) {
		GateRealCoordinate c(NullPtr, {1.0, i + 0.0});
		buildingBlocks2.push_back(c);
	}
	IteratorPtr<GateRealCoordinate> pGateCoordIter2 = new VectorBasedIteratorImpl<GateRealCoordinate>(&buildingBlocks2);
	buckets.push_back(pGateCoordIter2);

	GateRealCoordinate zeroEpsilon(NullPtr, {0.0,0.0});

	GateRealCoordinate target(NullPtr, {1.0, 5.0});

	ComparatorPtr<GateRealCoordinate> pCoordinateComparator = new DictionaryOrderCoordinateComparator<GatePtr>();
	CombinerPtr<GateRealCoordinate> pCoordinateCombiner = new GateCoordinateCombinerImpl(NullPtr);

	int maxResultsNumber = 5;

	CoordinateAdditionBasedGateComposer additionBasedComposer(pCoordinateComparator,
			pCoordinateCombiner,
			zeroEpsilon,
			maxResultsNumber);

	IteratorPtr<GateRealCoordinate> pComposeIter = additionBasedComposer.composeApproximations(buckets, target, NullPtr, 0.1);

	assert(pComposeIter != NullPtr && !pComposeIter->isDone());

	int nbResults = 0;
	int expectedNbResults = 4;

	while(!pComposeIter->isDone()) {
		/*Print result to review*/
		GateRealCoordinate gateCoord = pComposeIter->getObj();
		printf("Result [%d] 's coordinate \n---[", nbResults + 1);
		for(unsigned int i = 0; i < gateCoord.getCoordinates().size(); i++) {
			printf("%f,", mreal::toDouble(gateCoord.getCoordinates()[i]));
		}
		printf("]\n");
		nbResults++;
		pComposeIter->next();
	}

	assert(nbResults == expectedNbResults);

	delete pComposeIter;

	delete pCoordinateCombiner;
	delete pCoordinateComparator;

	delete pGateCoordIter2;
	delete pGateCoordIter1;

	std::cout << __func__ << " passed"  <<  std::endl ;
}


void FullTestSuite::freeTestGateCollectionEvaluator() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	MatrixPtrVector pBasis4;
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, pBasis4);

	assert(pBasis4.size() == 15);

	GateCombinabilityCheckers combinabilityCheckers;
	initTwoQubitsGateCombinabilityCheckers(combinabilityCheckers);

	CombinerPtr<GatePtr> pGateCombiner = new GateCombinerImpl(combinabilityCheckers, m_pMatrixOperator);

	GateCollectionPtr pUniversalSet = NullPtr;
	initTwoQubitsGateUniversalSet(m_pMatrixOperator, pUniversalSet);

	GateCollectionPtr pGateCollection = new GNATGateCollectionImpl();
	int maxSequenceLength = 7;

	GateSearchSpaceConstructorPtr pGateSearchSpaceConstructor = new GateSearchSpaceConstructorImpl(pGateCombiner);
	pGateSearchSpaceConstructor->constructSearchSpace(pGateCollection, pUniversalSet, maxSequenceLength);
	CollectionSize_t gateCounts = pGateCollection->size();
	std::cout << "Number of sequence length = " << maxSequenceLength << " constructed by CNOT, H and T: " << gateCounts << std::endl;

	GateWriterPtr pGateWriter = new LabelOnlyGateWriterImpl(",");
	MatrixRealInnerProductCalculatorPtr pMatrixRealInnerProductCalculator = new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator);
	MatrixRealCoordinateCalculatorPtr pHermitiaRealCoordinateCalculator = new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(pMatrixRealInnerProductCalculator, pBasis4);
	MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator = new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, pHermitiaRealCoordinateCalculator);
	GateRealCoordinateCalculatorPtr pGateRealCoordinateCalculator = new GateCoordinateCalculatorImpl(pMatrixRealCoordinateCalculator);
	RealCoordinateWriterPtr<GatePtr> pCoordinateWriter = new SampleRealCoordinateWriterImpl<GatePtr>();

	double epsilon = 0.4;
	TargetElements<GatePtr> targets;
	getNearIdentityGate(m_pMatrixOperator, pBasis4, targets);

	MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixFowlerDistanceCalculator(m_pMatrixOperator);
	//MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	GateDistanceCalculatorPtr pGateDistanceCalculator = new GateDistanceCalculatorByMatrixImpl(pMatrixDistanceCalculator);
	GateSearchSpaceEvaluatorPtr pGateSearchSpaceEvaluator = new GateSearchSpaceTimerEvaluatorImpl(targets,
			epsilon,
			epsilon,
			pGateDistanceCalculator,
			pGateRealCoordinateCalculator,
			pCoordinateWriter,
			pGateWriter,
			m_pTimer,
			std::cout);

	pGateCollection->rebuildStructure(pGateDistanceCalculator);
	pGateSearchSpaceEvaluator->evaluateCollection(pGateCollection);

	delete pGateSearchSpaceEvaluator;
	delete pMatrixDistanceCalculator;
	delete pGateDistanceCalculator;

	delete pCoordinateWriter;
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
	ComplexVal arrayCNOT1[] = {(ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0};

	MatrixPtr pCNOTMat1 = new SimpleDenseMatrixImpl(arrayCNOT1, ROW_SPLICE, 4, 4, "");
	MatrixPtr pSCNOTMat1 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat1, pSCNOTMat1);


	pCNOT1 = new Gate(pSCNOTMat1, 1, "CNOT1");

	ComplexVal arrayCNOT2[] = {(ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0};

	MatrixPtr pCNOTMat2 = new SimpleDenseMatrixImpl(arrayCNOT2, ROW_SPLICE, 4, 4, "");
	MatrixPtr pSCNOTMat2 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat2, pSCNOTMat2);

	pCNOT2 = new Gate(pSCNOTMat2, 1, "CNOT2");

	double inverseSQRT2 = 1/sqrt(2);

	ComplexVal arrayH1[] = {ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0
			, (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0)
			, ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, ComplexVal(-inverseSQRT2, 0), (ComplexVal)0.0
			, (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH1Mat = new SimpleDenseMatrixImpl(arrayH1, ROW_SPLICE, 4, 4, "");

	pH1 = new Gate(pH1Mat, 4, "H1");

	ComplexVal arrayH2[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, (ComplexVal)0.0
			, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0), (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0)
			, (ComplexVal)0.0, (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH2Mat = new SimpleDenseMatrixImpl(arrayH2, ROW_SPLICE, 4, 4, "");

	pH2 = new Gate(pH2Mat, 4, "H2");

	ComplexVal expmPi_8 = std::exp(ComplexVal(0, M_PI / 8.0));
	ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, -M_PI / 8.0));

	ComplexVal arrayT1[] = {expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8};

	MatrixPtr pT1Mat = new SimpleDenseMatrixImpl(arrayT1, ROW_SPLICE, 4, 4, "");

	pT1 = new Gate(pT1Mat, 1, "T1");

	ComplexVal arrayT2[] = {expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, expmPi_8, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, expm_Pi_8, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8};

	MatrixPtr pT2Mat = new SimpleDenseMatrixImpl(arrayT2, ROW_SPLICE, 4, 4, "");

	pT2 = new Gate(pT2Mat, 1, "T2");
}

void calculateSpecialUnitaryFromTracelessHermitianCoordinates(MatrixOperatorPtr pMatrixOperator, std::vector<mreal_t> coordinates, MatrixPtrVector pHermitianBasis, MatrixPtrRef pU) {
	ComplexVal zeroArr[] = {(mreal_t)0.0, (mreal_t)0.0 ,(mreal_t)0.0 ,(mreal_t)0.0 ,
			(mreal_t)0.0 ,(mreal_t)0.0 ,(mreal_t)0.0 ,(mreal_t)0.0 ,
			(mreal_t)0.0 ,(mreal_t)0.0 ,(mreal_t)0.0 ,(mreal_t)0.0 ,
			(mreal_t)0.0 ,(mreal_t)0.0 ,(mreal_t)0.0 ,(mreal_t)0.0};
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
	cancelationMap.insert(GateLabelPair("T2", "CNOT1CNOT2T2CNOT1CNOT2"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2H1CNOT1CNOT2H1"));
	cancelationMap.insert(GateLabelPair("H1", "CNOT1CNOT2H1CNOT1CNOT2"));
	cancelationMap.insert(GateLabelPair("CNOT1", "CNOT2H1CNOT2CNOT1H1"));

	pGateCancelationCombinabilityChecker = new GateCancelationCombinabilityCheckerImpl(cancelationMap);
}


void getNearIdentityGate(MatrixOperatorPtr pMatrixOperator, MatrixPtrVector pHermitianBasis, TargetElements<GatePtr>& targets) {
	/* initialize random seed: */
	srand (time(NULL));

	std::vector<mreal_t> randCoords;
	for(unsigned int j = 0; j < pHermitianBasis.size(); j++) {
		randCoords.push_back(my_small_rand(0.0));
	}
	MatrixPtr pU = NullPtr;

	calculateSpecialUnitaryFromTracelessHermitianCoordinates(pMatrixOperator, randCoords, pHermitianBasis, pU);
	GatePtr pTargetGate = new Gate(pU, 999999, "Unknown");
	targets.push_back(pTargetGate);

#ifndef IGNORE_PHASE
	//Test add global phase to U
	MatrixPtr pIdentityWithPhasePI_2 = NullPtr;
	MatrixPtr pIdentityWithPhase_PI_2 = NullPtr;
	MatrixPtr pIdentityWithPhasePI = NullPtr;
	MatrixPtr pIdentityWithPhase_PI = NullPtr;

	ComplexVal expmPi_2 = std::exp(ComplexVal(0,  M_PI / 2.0));
	ComplexVal expm_Pi_2 = std::exp(ComplexVal(0, -M_PI / 2.0));
	ComplexVal expmPi = std::exp(ComplexVal(0, M_PI / 2.0));
	ComplexVal expm_Pi = std::exp(ComplexVal(0, - M_PI));

	pMatrixOperator->multiplyScalar(pU, expmPi_2, pIdentityWithPhasePI_2);
	pMatrixOperator->multiplyScalar(pU, expm_Pi_2, pIdentityWithPhase_PI_2);
	pMatrixOperator->multiplyScalar(pU, expmPi, pIdentityWithPhasePI);
	pMatrixOperator->multiplyScalar(pU, expm_Pi, pIdentityWithPhase_PI);

	GatePtr pTargetGatePI_2 = new Gate(pIdentityWithPhasePI_2, 999999, "Unknown");
	GatePtr pTargetGate_PI_2 = new Gate(pIdentityWithPhase_PI_2, 999999, "Unknown");
	GatePtr pTargetGatePI = new Gate(pIdentityWithPhasePI, 999999, "Unknown");
	GatePtr pTargetGate_PI = new Gate(pIdentityWithPhase_PI, 999999, "Unknown");

	targets.push_back(pTargetGatePI_2);
	targets.push_back(pTargetGate_PI_2);
	targets.push_back(pTargetGatePI);
	targets.push_back(pTargetGate_PI);
#endif

}

void evalFindNeighbours(GateCollectionPtr pCollection, const TargetElements<GatePtr>& targets, GateDistanceCalculatorPtr pDistanceCalculator, double epsilon, std::vector<int>& resultNumbers) {
	resultNumbers.clear();
	for(TargetElements<GatePtr>::const_iterator eIter = targets.begin(); eIter != targets.end(); eIter++) {
		GatePtr pQuery = *eIter;

		//printf("----------------------------\n");
		GateLookupResultIteratorPtr pGateResultIter = pCollection->findNearestNeighbours(pQuery,
				pDistanceCalculator,
				epsilon,
				true);

		//printf("----------------------------\n");
		int nbResults = 0;

		//Also test order of results
		mreal_t prevDistance = -0.001;
		while(pGateResultIter != NullPtr && !pGateResultIter->isDone()) {
			nbResults++;

			mreal_t currentDistance = pGateResultIter->getObj().m_distanceToTarget;
			assert(currentDistance >= prevDistance);
			prevDistance = currentDistance;

			pGateResultIter->next();
		}
		resultNumbers.push_back(nbResults);
		_destroy(pGateResultIter);
	}
}

void evalFilteredFindNeighbours(GateCollectionPtr pCollection,
		const TargetElements<GatePtr>& targets,
		GateDistanceCalculatorPtr pGateDistanceCalculator,
		double epsilon) {

	for(unsigned int i = 0; i < targets.size(); i++) {
		GateLookupResultIteratorPtr pLookupResultIter = pCollection->findNearestNeighbours(targets[i],
				pGateDistanceCalculator,
				epsilon,
				true);

		//Just a simple check, may not enough but detect simple errors
		//But assume the results are sorted, the check should be considered correct
		if(pLookupResultIter != NullPtr) {
			GatePtr pPrevResult = NullPtr;
			const mreal_t maxDistanceToConsdiredOne = 1e-7;

			int nbResults = 0;
			while(!pLookupResultIter->isDone()) {
				GatePtr pCurrentResult = pLookupResultIter->getObj().m_resultElement;

				if(pPrevResult != NullPtr) {
					mreal_t distanceBetweenResult = pGateDistanceCalculator->distance(pCurrentResult, pPrevResult);
					assert(distanceBetweenResult >= maxDistanceToConsdiredOne);
				}
				pPrevResult = pCurrentResult;

				pLookupResultIter->next();

				nbResults++;
			}
			printf("Number of results for query %d: [Filtered GNAT: %d]\n", i, nbResults);
		}
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


