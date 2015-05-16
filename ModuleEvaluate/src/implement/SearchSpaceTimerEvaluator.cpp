/*
 * SearchSpaceTimerEvaluator.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#include "SearchSpaceEvaluatorImpl.h"
#include "ITimer.h"
#include "IDistanceCalculator.h"
#include "IMatrixWriter.h"
#include "ScopeTimer.h"
#include "IMatrixCollection.h"
#include "IMatrixApproximator.h"
#include "IIterator.h"
#include <exception>

void findTargetAndMeasureTime(MatrixApproximatorPtr pMatrixApproximator, MatrixCollectionPtr pCollection, MatrixPtr pTarget, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon, MatrixIteratorPtr&  prFindIter, TimerPtr pTimer, double* pSearchTime);

void getClosestApproximationFromFindIterator(MatrixIteratorPtr pFindResultIter, MatrixDistanceCalculatorPtr pDistanceCaculator, MatrixPtr pTarget, MatrixPtrRef prClosestApproximation, double& precision);

void logSearchResult(MatrixPtr pQuery, MatrixPtr pResult, double searchTime, double precision, double epsilon,  MatrixWriterPtr pMatrixWriter, std::ostream& outputStream);

SearchSpaceTimerEvaluatorImpl::SearchSpaceTimerEvaluatorImpl(const TargetMatrices&  pTargets, double epsilon, MatrixDistanceCalculatorPtr pMatrixDistanceCalculator, MatrixWriterPtr pMatrixWriter, TimerPtr pTimer, std::ostream& outputStream) : m_ostream(outputStream) {
	m_targetMatrices.insert(m_targetMatrices.end(), pTargets.begin(), pTargets.end());
	m_epsilon = epsilon;
	m_pMatrixDistanceCalculator = pMatrixDistanceCalculator;
	m_pTimer = pTimer;
	m_pMatrixWriter = pMatrixWriter;
}

//Override
void SearchSpaceTimerEvaluatorImpl::evaluateCollection(MatrixCollectionPtr pMatrixCollection) {
	size_t numberOfCases = m_targetMatrices.size();

	for(size_t i = 0; i < numberOfCases; i++) {
		MatrixPtr pTarget = m_targetMatrices[i];

		double searchTime = 0;
		double precision = -1; //Imply no result found
		MatrixPtr closestApproximation = NullPtr; //Imply no result found
		MatrixIteratorPtr pFindResultIter = NullPtr; //Imply no result found

		findTargetAndMeasureTime(NullPtr, pMatrixCollection, pTarget, m_pMatrixDistanceCalculator, m_epsilon, pFindResultIter, m_pTimer, &searchTime);

		getClosestApproximationFromFindIterator(pFindResultIter, m_pMatrixDistanceCalculator, pTarget, closestApproximation, precision);

		//Write the result to output stream
		logSearchResult(pTarget, closestApproximation, searchTime, precision, m_epsilon, m_pMatrixWriter, m_ostream);
	}
}

void SearchSpaceTimerEvaluatorImpl::evaluateApproximator(MatrixApproximatorPtr pMatrixApproximator, MatrixCollectionPtr pCoreMatrixCollection) {
	size_t numberOfCases = m_targetMatrices.size();

	for(size_t i = 0; i < numberOfCases; i++) {
		MatrixPtr pTarget = m_targetMatrices[i];

		double searchTime = 0;
		double precision = -1; //Imply no result found
		MatrixPtr closestApproximation = NullPtr; //Imply no result found
		MatrixIteratorPtr pFindResultIter = NullPtr; //Imply no result found

		findTargetAndMeasureTime(pMatrixApproximator, pCoreMatrixCollection, pTarget, m_pMatrixDistanceCalculator, m_epsilon, pFindResultIter, m_pTimer, &searchTime);

		getClosestApproximationFromFindIterator(pFindResultIter, m_pMatrixDistanceCalculator, pTarget, closestApproximation, precision);

		//Write the result to output stream
		logSearchResult(pTarget, closestApproximation, searchTime, precision, m_epsilon, m_pMatrixWriter, m_ostream);
	}
}

void findTargetAndMeasureTime(MatrixApproximatorPtr pMatrixApproximator, MatrixCollectionPtr pCollection, MatrixPtr pTarget, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon, MatrixIteratorPtr&  prFindIter, TimerPtr pTimer, double* pSearchTime) {
	//Use try catch and make use of scope timer
	try {
		ScopeTimer scopeTimer(pTimer, pSearchTime);

		//Start the lookup routine for the newly generated target
		if(pMatrixApproximator != NullPtr) {
			prFindIter = pMatrixApproximator->getApproximateMatrices(pCollection, pTarget, pDistanceCalculator, epsilon);
		}
		else {
			prFindIter = pCollection->findApproxMatrices(pTarget, pDistanceCalculator, epsilon);
		}
	}
	catch (std::exception & e) {

	}
}

void getClosestApproximationFromFindIterator(MatrixIteratorPtr pFindResultIter, MatrixDistanceCalculatorPtr pDistanceCaculator, MatrixPtr pTarget, MatrixPtrRef prClosestApproximation, double& precision) {
	if(pFindResultIter != NullPtr) {
		pFindResultIter->toBegin();
		if(!pFindResultIter->isDone()) {
			//Suppose the closest approximation is placed at the beginning of the iterator
			prClosestApproximation = pFindResultIter->getObj();

			precision = pDistanceCaculator->distance(prClosestApproximation, pTarget);
		}
	}
}

void logSearchResult(MatrixPtr pQuery, MatrixPtr pResult, double searchTime, double precision, double epsilon,  MatrixWriterPtr pMatrixWriter, std::ostream& outputStream) {
	const std::string delimeter = ", ";
	const std::string endLine = "\n";

	outputStream << "Query:" << endLine;
	pMatrixWriter->writeMatrix(pQuery, outputStream);
	outputStream << "Epsilon:" << epsilon << endLine;
	outputStream << "Result:" << endLine;
	pMatrixWriter->writeMatrix(pResult, outputStream);
	outputStream << "Info" << endLine;
	outputStream << "Search time: " ;
	outputStream << searchTime << delimeter;
	outputStream << "Precision: ";
	outputStream << precision;
	outputStream << endLine;
}

