/*
 * FreeTester.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: pham
 */

#include "IntegratingTester.h"
#include "FullGateWriterImpl.h"
#include "LabelOnlyGateWriterImpl.h"
#include "IIterator.h"
#include "AlgoCommon.h"
#include "Gate.h"
#include "ICollection.h"
#include "SampleMatrixOperator.h"
#include "MatrixTraceDistanceCalculator.h"
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "SearchSpaceEvaluatorImpl.h"
#include "CpuTimer.h"
#include <iostream>

IntegratingTester::IntegratingTester() {


}

IntegratingTester::~IntegratingTester() {

}

//Write out elements of gate search space (gate collection)
void IntegratingTester::freeTestShowGateSearchSpace(int nbQubits, int maxSequenceLength) {
	AssemblerUtilPtr pUtil = new AssemblerUtil();

	CollectionPtr<GatePtr> pSearchSpaceCollection = NullPtr;
	pUtil->getSampleGateSearchSpaceCollection(pSearchSpaceCollection,
			NullPtr,
			nbQubits,
			maxSequenceLength);

	GateWriterPtr pGateWriter = new LabelOnlyGateWriterImpl();

	std::cout << "Collection size " << pSearchSpaceCollection->size() << std::endl;

	GateIteratorPtr pGateIter = pSearchSpaceCollection->getIteratorPtr();
	while(!pGateIter->isDone()) {
		pGateWriter->write(pGateIter->getObj(), std::cout);
		pGateIter->next();
	}

	delete pGateWriter;
	delete pSearchSpaceCollection;
	delete pUtil;
}

//Evaluate gate search space (gate collection) by some simple query
void IntegratingTester::freeTestEvaluateGateSearchSpace(int nbQubits, int maxSeqLength, double epsilon) {
	AssemblerUtilPtr pUtil = new AssemblerUtil();

	GatePtrVector queries;
	pUtil->getSampleQueryGate(queries, nbQubits);

	MatrixOperatorPtr pMatrixOperator = NullPtr;
	pUtil->getMatrixOperator(pMatrixOperator);

	MatrixDistanceCalculatorPtr pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(pMatrixOperator);
	GateDistanceCalculatorPtr pGateDistanceCalculator = new GateDistanceCalculatorByMatrixImpl(pMatrixDistanceCalculator);

	CollectionPtr<GatePtr> pSearchSpaceCollection = NullPtr;
	pUtil->getSampleGateSearchSpaceCollection(pSearchSpaceCollection,
			pGateDistanceCalculator,
			nbQubits,
			maxSeqLength);

	TimerPtr pTimer = new CpuTimer();
	GateWriterPtr pGateWriter = new FullGateWriterImpl();

	GateSearchSpaceEvaluatorPtr pGateSeachSpaceEvaluator = new SearchSpaceTimerEvaluatorImpl<GatePtr>(queries,
			epsilon,
			epsilon,
			NullPtr,
			NullPtr,
			pGateWriter,
			pTimer,
			std::cout);
	pGateSeachSpaceEvaluator->evaluateCollection(pSearchSpaceCollection);

	delete pGateSeachSpaceEvaluator;
	delete pGateWriter;
	delete pTimer;

	delete pGateDistanceCalculator;
	delete pMatrixDistanceCalculator;

	delete pSearchSpaceCollection;
	delete pUtil;
}
