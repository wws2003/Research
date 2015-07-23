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
#include "ICoordinateWriter.h"
#include "ICoordinateCalculator.h"
#include <exception>
#include <algorithm>

#define DENOISING 1
#define NOISE_THRESOLD 3e-8
#define LOG_ALL_RESULT 1

template<typename T>
struct ElementWithDistance_ {
	T element;
	double distance;

	bool operator < (const ElementWithDistance_& e) const
	{
		return (distance < e.distance);
	}
};

template<typename T>
using ElementWithDistance = ElementWithDistance_<T>;

template<typename T>
using ElementWithDistances = std::vector<ElementWithDistance<T> >;

template<typename T>
void findTargetAndMeasureTime(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCollection, T target, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, IteratorPtr<T>&  prFindIter, TimerPtr pTimer, double* pSearchTime);

template<typename T>
void logAllSearchResultsFoundIterator(IteratorPtr<T> pFindResultIter,
		DistanceCalculatorPtr<T> pDistanceCaculator,
		RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
		RealCoordinateWriterPtr<T> pRealCoordinateWriter,
		T target,
		double searchTime,
		double epsilon,
		WriterPtr<T> pWriter,
		std::ostream& outputStream);

template<typename T>
void getClosestApproximationFromFoundIterator(IteratorPtr<T> pFindResultIter,
		DistanceCalculatorPtr<T> pDistanceCaculator,
		T pTarget,
		T& prClosestApproximation,
		double& precision);

template<typename T>
void getSortedResults(IteratorPtr<T> pFindResultIter,
		DistanceCalculatorPtr<T> pDistanceCaculator,
		T pTarget,
		ElementWithDistances<T>& sortedResults);

template<typename T>
void logSearchResult(T pQuery,
		T pResult,
		double searchTime,
		double precision,
		double epsilon,
		WriterPtr<T> pWriter,
		std::ostream& outputStream);

template<typename T>
SearchSpaceTimerEvaluatorImpl<T>::SearchSpaceTimerEvaluatorImpl(const TargetElements<T>& pTargets,
		double epsilon,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
		RealCoordinateWriterPtr<T> pRealCoordinateWritter,
		WriterPtr<T> pWriter,
		TimerPtr pTimer,
		std::ostream& outputStream) : m_ostream(outputStream) {

	m_targets.insert(m_targets.end(), pTargets.begin(), pTargets.end());
	m_epsilon = epsilon;
	m_pDistanceCalculator = pDistanceCalculator;
	m_pRealCoordinateCalculator = pRealCoordinateCalculator;
	m_pRealCoordinateWritter = pRealCoordinateWritter;
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
		IteratorPtr<T> pFindResultIter = NullPtr; //Imply no result found

		findTargetAndMeasureTime<T>(NullPtr,
				pCollection,
				target,
				m_pDistanceCalculator,
				m_epsilon,
				pFindResultIter,
				m_pTimer,
				&searchTime);

#ifdef LOG_ALL_RESULT
		logAllSearchResultsFoundIterator(pFindResultIter,
				m_pDistanceCalculator,
				m_pRealCoordinateCalculator,
				m_pRealCoordinateWritter,
				target,
				searchTime,
				m_epsilon,
				m_pWriter,
				m_ostream);

#else
		double precision = -1; //Imply no result found
		T closestApproximation = NullPtr; //Imply no result found

		//Get result closest to query
		getClosestApproximationFromFoundIterator(pFindResultIter,
				m_pDistanceCalculator,
				target,
				closestApproximation,
				precision);

		//Write the result to output stream
		logSearchResult(target,
				closestApproximation,
				searchTime,
				precision,
				m_epsilon,
				m_pWriter,
				m_ostream);
#endif
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

		//Get result closest to query
		getClosestApproximationFromFoundIterator(pFindResultIter, m_pDistanceCalculator, target, closestApproximation, precision);

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

//Sort found results by precision (distance to query) then output
template<typename T>
void logAllSearchResultsFoundIterator(IteratorPtr<T> pFindResultIter,
		DistanceCalculatorPtr<T> pDistanceCaculator,
		RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
		RealCoordinateWriterPtr<T> pRealCoordinateWriter,
		T target,
		double searchTime,
		double epsilon,
		WriterPtr<T> pWriter,
		std::ostream& outputStream) {

	if(pFindResultIter == NullPtr) {
		return;
	}

	//Collect all results into a vector
	ElementWithDistances<T> results;
	getSortedResults(pFindResultIter, pDistanceCaculator, target, results);

	//Output some general information
	outputStream << "--------------------------" << "\n";
	outputStream << "Number of results:" << results.size() << "\n";
	CoordinatePtr<T, double> pQueryCoordinate = NullPtr;
	if(pRealCoordinateCalculator != NullPtr) {
		pRealCoordinateCalculator->calulateElementCoordinate(target, pQueryCoordinate);
		outputStream << "Query coordinate:" << "\n";
		pRealCoordinateWriter->writeCoordinate(*pQueryCoordinate, outputStream);
	}

	//Output (log) each result
	for(unsigned int i = 0; i < results.size(); i++) {
		T result = results[i].element;
		double precision = results[i].distance;

		outputStream << "Result " << i << "\n";

		//Output element (gate or matrix or anything)
		logSearchResult(target,
				result,
				searchTime,
				precision,
				epsilon,
				pWriter,
				outputStream);

		//Output element coordinate
		CoordinatePtr<T, double> pCoordinate = NullPtr;
		if(pRealCoordinateCalculator != NullPtr) {
			pRealCoordinateCalculator->calulateElementCoordinate(result, pCoordinate);
			pRealCoordinateWriter->writeCoordinate(*pCoordinate, outputStream);
		}
	}
}

template<typename T>
void getClosestApproximationFromFoundIterator(IteratorPtr<T> pFindResultIter, DistanceCalculatorPtr<T> pDistanceCaculator, T pTarget, T& prClosestApproximation, double& precision) {
	double minPrecision = 1e4;

	if(pFindResultIter != NullPtr) {
		return;
	}

	pFindResultIter->toBegin();
	while(!pFindResultIter->isDone()) {
		precision = pDistanceCaculator->distance(pFindResultIter->getObj(), pTarget);

#ifdef DENOISING
		if(precision < NOISE_THRESOLD) {
			precision = 1;
		}
#endif
		if(precision < minPrecision) {
			prClosestApproximation = pFindResultIter->getObj();
			minPrecision = precision;
		}
		pFindResultIter->next();
	}
	precision = minPrecision;
}

template<typename T>
void getSortedResults(IteratorPtr<T> pFindResultIter,
		DistanceCalculatorPtr<T> pDistanceCaculator,
		T pTarget,
		ElementWithDistances<T>& sortedResults) {

	pFindResultIter->toBegin();

	//Traverse iterator to get element and push to results vector
	while(!pFindResultIter->isDone()) {
		T element = pFindResultIter->getObj();
		double precision = pDistanceCaculator->distance(element, pTarget);

		ElementWithDistance<T> result;
		result.element = element;
		result.distance = precision;

		sortedResults.push_back(result);

		pFindResultIter->next();
	}

	//Wind-back the iterator for future purpose
	pFindResultIter->toBegin();

	//Sort the results vector by precision
	std::sort(sortedResults.begin(), sortedResults.end());
}

template<typename T>
void logSearchResult(T pQuery, T pResult, double searchTime, double precision, double epsilon, WriterPtr<T> pWriter, std::ostream& outputStream) {
	const std::string delimeter = ", ";
	const std::string endLine = "\n";

	outputStream << "Query:" << endLine;
	pWriter->write(pQuery, outputStream);
	outputStream << "Epsilon:" << epsilon << endLine;

	outputStream << "Info" << endLine;
	outputStream << "Search time: " ;
	outputStream << searchTime << delimeter;
	outputStream << "Precision: ";
	outputStream << precision;
	outputStream << endLine;
	outputStream << "Result:" << endLine;

	if(pResult != NullPtr) {
		pWriter->write(pResult, outputStream);
	}
	else {
		outputStream << "No result found" << endLine;
	}
}
