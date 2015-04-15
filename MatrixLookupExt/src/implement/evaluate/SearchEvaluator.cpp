/*
 * SearchEvaluator.cpp
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#include <string>
#include "implement/evaluate/SearchEvaluator.h"
#include "abstract/inout/IMatrixWriter.h"
#include "abstract/factory/ITargetMatrixFactory.h"
#include "abstract/basic/IMatrixCollection.h"
#include "abstract/basic/IMatrixDistanceCalculator.h"
#include "abstract/basic/IMatrixIterator.h"
#include "abstract/timer/ITimer.h"

void logSearchResult(MatrixPtr pQuery, MatrixPtr pResult, double searchTime, double precision, double epsilon,  MatrixWriterPtr pMatrixWriter, std::ostream& outputStream);

SearchEvaluator::SearchEvaluator(MatrixCollectionPtr pMatrixCollection, MatrixDistanceCalculatorPtr pMatrixDistanceCalculator, MatrixWriterPtr pMatrixWriter):
m_pMatrixCollection(pMatrixCollection),
m_pMatrixDistanceCalculator(pMatrixDistanceCalculator),
m_pMatrixWriter(pMatrixWriter) {

}

void SearchEvaluator::evaluate(int numberOfCases, double epsilon, TargetMatrixFactoryPtr pTargetMatrixFactory, TimerPtr pTimer, std::ostream& outputStream) {
	for(int i = 0; i < numberOfCases; i++) {
		MatrixPtr pTarget = pTargetMatrixFactory->generateTargetMatrixForSearch(NULL);

		//Start the lookup routine for the newly generated target
		double startTime = pTimer->getTime();
		MatrixIteratorPtr pIter = m_pMatrixCollection->findApproxMatrices(pTarget, m_pMatrixDistanceCalculator, epsilon);
		double endTime = pTimer->getTime();

		double searchTime = endTime - startTime;
		double precision = -1; //Imply no result found

		pIter->toBegin();
		if(!pIter->isDone()) {
			//Suppose the closest approximation is placed at the beginning of the iterator
			MatrixPtr closestApproximation = pIter->getMatrixObj();

			precision = m_pMatrixDistanceCalculator->distance(closestApproximation, pTarget);

			logSearchResult(pTarget, closestApproximation, searchTime, precision, epsilon, m_pMatrixWriter, outputStream);
		}
		else {
			logSearchResult(pTarget, NullPtr, searchTime, precision, epsilon, m_pMatrixWriter, outputStream);
		}

		_destroy(pTarget);
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
