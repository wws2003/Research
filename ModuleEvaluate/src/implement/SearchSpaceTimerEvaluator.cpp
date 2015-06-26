/*
 * SearchSpaceTimerEvaluator.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#include "SearchSpaceEvaluatorImpl.h"
#include "ITimer.h"
#include "IDistanceCalculator.h"
#include "IOCommon.h"
#include "ScopeTimer.h"
#include "ICollection.h"
#include "AlgoCommon.h"
#include "IIterator.h"
#include "IApproximator.h"
#include "IWriter.h"
#include <exception>

template<typename T>
void findTargetAndMeasureTime(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCollection, T target, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, IteratorPtr<T>&  prFindIter, TimerPtr pTimer, double* pSearchTime);

template<typename T>
void getClosestApproximationFromFindIterator(IteratorPtr<T> pFindResultIter, DistanceCalculatorPtr<T> pDistanceCaculator, T pTarget, T& prClosestApproximation, double& precision);

template<typename T>
void logSearchResult(T pQuery, T pResult, double searchTime, double precision, double epsilon, WriterPtr<T> pWriter, std::ostream& outputStream);

template<typename T>
SearchSpaceTimerEvaluatorImpl<T>::SearchSpaceTimerEvaluatorImpl(const TargetElements<T>& pTargets, double epsilon, DistanceCalculatorPtr<T> pDistanceCalculator, WriterPtr<T> pWriter, TimerPtr pTimer, std::ostream& outputStream) : m_ostream(outputStream){
	m_targets.insert(m_targets.end(), pTargets.begin(), pTargets.end());
	m_epsilon = epsilon;
	m_pDistanceCalculator = pDistanceCalculator;
	m_pTimer = pTimer;
	m_pWriter = pWriter;
}

//Override
template<typename T>
void SearchSpaceTimerEvaluatorImpl<T>::evaluateCollection(CollectionPtr<T> pCollection) {
	size_t numberOfCases = m_targets.size();

	m_ostream << "Search space size " << pCollection->size() << "\n";

	for(size_t i = 0; i < numberOfCases; i++) {
		T target = m_targets[i];

		double searchTime = 0;
		double precision = -1; //Imply no result found
		T closestApproximation = NullPtr; //Imply no result found
		IteratorPtr<T> pFindResultIter = NullPtr; //Imply no result found

		findTargetAndMeasureTime<T>(NullPtr, pCollection, target, m_pDistanceCalculator, m_epsilon, pFindResultIter, m_pTimer, &searchTime);

		getClosestApproximationFromFindIterator(pFindResultIter, m_pDistanceCalculator, target, closestApproximation, precision);

		//Write the result to output stream
		logSearchResult(target, closestApproximation, searchTime, precision, m_epsilon, m_pWriter, m_ostream);
	}
}

template<typename T>
void SearchSpaceTimerEvaluatorImpl<T>::evaluateApproximator(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCoreCollection) {
	size_t numberOfCases = m_targets.size();

	for(size_t i = 0; i < numberOfCases; i++) {
		T target = m_targets[i];

		double searchTime = 0;
		double precision = -1; //Imply no result found
		T closestApproximation = NullPtr; //Imply no result found
		IteratorPtr<T> pFindResultIter = NullPtr; //Imply no result found

		findTargetAndMeasureTime(pApproximator, pCoreCollection, target, m_pDistanceCalculator, m_epsilon, pFindResultIter, m_pTimer, &searchTime);

		getClosestApproximationFromFindIterator(pFindResultIter, m_pDistanceCalculator, target, closestApproximation, precision);

		//Write the result to output stream
		logSearchResult(target, closestApproximation, searchTime, precision, m_epsilon, m_pWriter, m_ostream);
	}
}

template<typename T>
void findTargetAndMeasureTime(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCollection, T target, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, IteratorPtr<T>&  prFindIter, TimerPtr pTimer, double* pSearchTime) {
	//Use try catch and make use of scope timer
	try {
		ScopeTimer scopeTimer(pTimer, pSearchTime);

		//Start the lookup routine for the newly generated target
		if(pApproximator != NullPtr) {
			prFindIter = pApproximator->getApproximateElements(pCollection, target, pDistanceCalculator, epsilon);
		}
		else {
			prFindIter = pCollection->findNearestNeighbour(target, pDistanceCalculator, epsilon);
		}
	}
	catch (std::exception & e) {

	}
}

template<typename T>
void getClosestApproximationFromFindIterator(IteratorPtr<T> pFindResultIter, DistanceCalculatorPtr<T> pDistanceCaculator, T pTarget, T& prClosestApproximation, double& precision) {
	double minPrecision = 1e4;
	if(pFindResultIter != NullPtr) {
		pFindResultIter->toBegin();
		while(!pFindResultIter->isDone()) {

			precision = pDistanceCaculator->distance(pFindResultIter->getObj(), pTarget);

			if(precision < minPrecision) {
				prClosestApproximation = pFindResultIter->getObj();
				minPrecision = precision;
			}

			pFindResultIter->next();
		}

		precision = minPrecision;
	}
}

template<typename T>
void logSearchResult(T pQuery, T pResult, double searchTime, double precision, double epsilon, WriterPtr<T> pWriter, std::ostream& outputStream) {
	const std::string delimeter = ", ";
	const std::string endLine = "\n";

	outputStream << "Query:" << endLine;
	pWriter->write(pQuery, outputStream);
	outputStream << "Epsilon:" << epsilon << endLine;
	outputStream << "Result:" << endLine;
	if(pResult != NullPtr) {
		pWriter->write(pResult, outputStream);
	}
	else {
		outputStream << "No result found" << endLine;
	}

	outputStream << "Info" << endLine;
	outputStream << "Search time: " ;
	outputStream << searchTime << delimeter;
	outputStream << "Precision: ";
	outputStream << precision;
	outputStream << endLine;
}
