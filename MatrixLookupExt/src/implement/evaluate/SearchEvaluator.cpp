/*
 * SearchEvaluator.cpp
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#include <string>
#include <exception>

#include "implement/evaluate/SearchEvaluator.h"
#include "implement/timer/ScopeTimer.h"
#include "abstract/inout/IMatrixWriter.h"
#include "abstract/factory/ITargetMatrixFactory.h"
#include "abstract/basic/algo/IMatrixCollection.h"
#include "abstract/basic/algo/IMatrixDistanceCalculator.h"
#include "abstract/basic/algo/IMatrixIterator.h"

void findTargetAndMeasureTime(MatrixCollectionPtr pCollection, MatrixPtr pTarget, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon, MatrixIteratorPtr&  prFindIter, TimerPtr pTimer, double* pSearchTime);

void getClosestApproximationFromFindIterator(MatrixIteratorPtr pFindResultIter, MatrixDistanceCalculatorPtr pDistanceCaculator, MatrixPtr pTarget, MatrixPtrRef prClosestApproximation, double& precision);

void logSearchResult(MatrixPtr pQuery, MatrixPtr pResult, double searchTime, double precision, double epsilon,  MatrixWriterPtr pMatrixWriter, std::ostream& outputStream);


SearchEvaluator::SearchEvaluator(MatrixCollectionPtr pMatrixCollection, MatrixDistanceCalculatorPtr pMatrixDistanceCalculator, MatrixWriterPtr pMatrixWriter):
								m_pMatrixCollection(pMatrixCollection),
								m_pMatrixDistanceCalculator(pMatrixDistanceCalculator),
								m_pMatrixWriter(pMatrixWriter) {

}

void SearchEvaluator::evaluate(int numberOfCases, double epsilon, TargetMatrixFactoryPtr pTargetMatrixFactory, TimerPtr pTimer, std::ostream& outputStream) {
	for(int i = 0; i < numberOfCases; i++) {
		MatrixPtr pTarget = pTargetMatrixFactory->generateTargetMatrixForSearch(NULL);

		double searchTime = 0;
		double precision = -1; //Imply no result found
		MatrixPtr closestApproximation = NullPtr; //Imply no result found
		MatrixIteratorPtr pFindResultIter = NullPtr; //Imply no result found

		findTargetAndMeasureTime(m_pMatrixCollection, pTarget, m_pMatrixDistanceCalculator, epsilon, pFindResultIter, pTimer, &searchTime);

		getClosestApproximationFromFindIterator(pFindResultIter, m_pMatrixDistanceCalculator, pTarget, closestApproximation, precision);

		//Write the result to output stream
		logSearchResult(pTarget, closestApproximation, searchTime, precision, epsilon, m_pMatrixWriter, outputStream);

		_destroy(pTarget);
	}
}

void findTargetAndMeasureTime(MatrixCollectionPtr pCollection, MatrixPtr pTarget, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon, MatrixIteratorPtr&  prFindIter, TimerPtr pTimer, double* pSearchTime) {
	//Use try catch and make use of scope timer
	try {
		ScopeTimer scopeTimer(pTimer, pSearchTime);

		//Start the lookup routine for the newly generated target
		prFindIter = pCollection->findApproxMatrices(pTarget, pDistanceCalculator, epsilon);
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
	const std::string delimeter = ",";
	const std::string endLine = "\n";

	pMatrixWriter->writeMatrix(pQuery, outputStream);
	outputStream << delimeter;
	outputStream << epsilon;
	outputStream << delimeter;
	pMatrixWriter->writeMatrix(pResult, outputStream);
	outputStream << delimeter;
	outputStream << searchTime;
	outputStream << delimeter;
	outputStream << precision;
	outputStream << endLine;
}
