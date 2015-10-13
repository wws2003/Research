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
#define NOISE_THRESOLD 9e-8

#define END_LINE "\r\n"

template<typename T>
struct ElementWithDistance_ {
	T element;
	mreal_t distance;

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
void findTargetAndMeasureTime(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCollection, T target, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t epsilon, IteratorPtr<T>&  prFindIter, TimerPtr pTimer, double* pSearchTime);

template<typename T>
void logAllSearchResultsFoundIterator(IteratorPtr<T> pFindResultIter,
		DistanceCalculatorPtr<T> pDistanceCaculator,
		RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
		RealCoordinateWriterPtr<T> pRealCoordinateWriter,
		T target,
		double searchTime,
		mreal_t epsilon,
		WriterPtr<T> pWriter,
		std::ostream& outputStream);

template<typename T>
void getSortedResults(IteratorPtr<T> pFindResultIter,
		DistanceCalculatorPtr<T> pDistanceCaculator,
		T pTarget,
		ElementWithDistances<T>& sortedResults);

template<typename T>
void logSearchResult(T pQuery,
		T pResult,
		double searchTime,
		mreal_t precision,
		mreal_t epsilon,
		WriterPtr<T> pWriter,
		CoordinatePtr<T, mreal_t> pCoordinate,
		RealCoordinateWriterPtr<T> pRealCoordinateWriter,
		std::ostream& outputStream);

template<typename T>
SearchSpaceTimerEvaluatorImpl<T>::SearchSpaceTimerEvaluatorImpl(const TargetElements<T>& pTargets,
		mreal_t collectionEpsilon,
		mreal_t approximatorEpsilon,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
		RealCoordinateWriterPtr<T> pRealCoordinateWritter,
		WriterPtr<T> pWriter,
		TimerPtr pTimer,
		std::ostream& outputStream) : m_ostream(outputStream) {

	m_targets.insert(m_targets.end(), pTargets.begin(), pTargets.end());
	m_collectionEpsilon = collectionEpsilon;
	m_approximatorEpsilon = approximatorEpsilon;
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

	m_ostream << "Search space size " << pCollection->size() << END_LINE;

	for(size_t i = 0; i < numberOfCases; i++) {
		T target = m_targets[i];
		double searchTime = 0;
		IteratorPtr<T> pFindResultIter = NullPtr; //Imply no result found

		findTargetAndMeasureTime<T>(NullPtr,
				pCollection,
				target,
				m_pDistanceCalculator,
				m_collectionEpsilon,
				pFindResultIter,
				m_pTimer,
				&searchTime);

		logAllSearchResultsFoundIterator(pFindResultIter,
				m_pDistanceCalculator,
				m_pRealCoordinateCalculator,
				m_pRealCoordinateWritter,
				target,
				searchTime,
				m_collectionEpsilon,
				m_pWriter,
				m_ostream);
	}
}

template<typename T>
void SearchSpaceTimerEvaluatorImpl<T>::evaluateApproximator(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCoreCollection) {
	size_t numberOfCases = m_targets.size();

	for(size_t i = 0; i < numberOfCases; i++) {
		T target = m_targets[i];

		double searchTime = 0;
		IteratorPtr<T> pFindResultIter = NullPtr; //Imply no result found

		findTargetAndMeasureTime(pApproximator,
				pCoreCollection,
				target,
				m_pDistanceCalculator,
				m_approximatorEpsilon,
				pFindResultIter,
				m_pTimer,
				&searchTime);

		logAllSearchResultsFoundIterator(pFindResultIter,
				m_pDistanceCalculator,
				m_pRealCoordinateCalculator,
				m_pRealCoordinateWritter,
				target,
				searchTime,
				m_approximatorEpsilon,
				m_pWriter,
				m_ostream);
	}
}

template<typename T>
void findTargetAndMeasureTime(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCollection, T target, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t epsilon, IteratorPtr<T>&  prFindIter, TimerPtr pTimer, double* pSearchTime) {
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
		mreal_t epsilon,
		WriterPtr<T> pWriter,
		std::ostream& outputStream) {

	if(pFindResultIter == NullPtr) {
		return;
	}

	//Collect all results into a vector
	ElementWithDistances<T> results;
	getSortedResults(pFindResultIter, pDistanceCaculator, target, results);

	//Output some general information
	outputStream << "--------------------------" << END_LINE;
	outputStream << "Number of results:" << results.size() << END_LINE;

	CoordinatePtr<T, mreal_t> pQueryCoordinate = NullPtr;
	if(pRealCoordinateCalculator != NullPtr) {
		pRealCoordinateCalculator->calulateElementCoordinate(target, pQueryCoordinate);
		outputStream << "Query coordinate:" << END_LINE;
		pRealCoordinateWriter->writeCoordinate(*pQueryCoordinate, outputStream);
		outputStream << END_LINE;
	}

	//Output (log) each result
	for(unsigned int i = 0; i < results.size(); i++) {
		T result = results[i].element;
		mreal_t precision = results[i].distance;

		//outputStream << "Result " << i << END_LINE;

		//Calculate result coordinate
		CoordinatePtr<T, mreal_t> pCoordinate = NullPtr;
		if(pRealCoordinateCalculator != NullPtr) {
			pRealCoordinateCalculator->calulateElementCoordinate(result, pCoordinate);
		}

		//Log result (gate or matrix or anything)
		logSearchResult(target,
				result,
				searchTime,
				precision,
				epsilon,
				pWriter,
				pCoordinate,
				pRealCoordinateWriter,
				outputStream);
	}
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
		mreal_t precision = pDistanceCaculator->distance(element, pTarget);

#ifdef DENOISING
		if(precision >= NOISE_THRESOLD) {
			ElementWithDistance<T> result;
			result.element = element;
			result.distance = precision;

			sortedResults.push_back(result);
		}
#endif

		pFindResultIter->next();
	}

	//Wind-back the iterator for future purpose
	pFindResultIter->toBegin();

	//Sort the results vector by precision
	std::sort(sortedResults.begin(), sortedResults.end());
}

template<typename T>
void logSearchResult(T pQuery,
		T pResult,
		double searchTime,
		mreal_t precision,
		mreal_t epsilon,
		WriterPtr<T> pWriter,
		CoordinatePtr<T, mreal_t> pCoordinate,
		RealCoordinateWriterPtr<T> pRealCoordinateWriter,
		std::ostream& outputStream) {

	const std::string delimeter = ", ";

	//Temporally output nothing but the result 's coordinate and approximation precision

	/*outputStream << "Query:" << END_LINE;
	pWriter->write(pQuery, outputStream);
	outputStream << "Epsilon:" << epsilon << END_LINE;

	outputStream << "Info" << END_LINE;
	outputStream << "Search time: " ;
	outputStream << searchTime << delimeter;
	outputStream << "Result:" << END_LINE;*/

	if(pResult != NullPtr) {
		pWriter->write(pResult, outputStream);
		if(pCoordinate != NullPtr) {
			pRealCoordinateWriter->writeCoordinate(*pCoordinate, outputStream);
		}
	}

	outputStream << "Distance to target:";
	outputStream << precision;
	outputStream << END_LINE;
}
