/*
 * AdditionBasedElementComposer.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: pham
 */

#include "AdditionBasedElementComposer.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "IIterator.h"
#include "IDistanceCalculator.h"
#include <algorithm>
#include <exception>
#include <iostream>

//#define ABCE_DEBUG

#ifdef ABCE_DEBUG
#include "Coordinate.hpp"
#endif

template<typename T>
AdditionBasedElementComposer<T>::AdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
		CombinerPtr<T> pCombiner,
		T epsilonElement,
		int maxResultsNumber) : m_wrapperComparator(pElementComparator) {
	m_pCombiner = pCombiner;
	m_epsilonElement = epsilonElement;
	m_maxResultsNumber = maxResultsNumber;
	m_combinationCounter = 0;
}

template<typename T>
IteratorPtr<LookupResult<T> > AdditionBasedElementComposer<T>::composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T target,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		bool toSortResults) {

	SortedVectorArray<T> sortedVectorArray(m_wrapperComparator);
	sortedVectorArray.initByVectors(buildingBlockBuckets);

	SortedVectorArrayList<T> secondarySortedVectorArrays;
	initSecondarySortedVectorArrays(secondarySortedVectorArrays, buildingBlockBuckets);

	std::cout << "Finished pre-process\n";

	std::vector<LookupResult<T> > resultBuffer;
	std::vector<T> partialTermElements;

	int lastVectorIndex = sortedVectorArray.getNbVectors() - 1;
	try {
		findCompositionsInRange(sortedVectorArray,
				secondarySortedVectorArrays,
				lastVectorIndex,
				partialTermElements,
				target,
				target,
				pDistanceCalculator,
				epsilon,
				resultBuffer);
	}
	catch (int e) {
		std::cout << "Enough result\n";
	}

	releaseSecondarySortedVectorArrays(secondarySortedVectorArrays);

	std::cout << "Number of combination checked:" << m_combinationCounter << "\n";
	m_combinationCounter = 0;

	//TODO Filter before/after sort results
	if(toSortResults) {
		std::sort(resultBuffer.begin(), resultBuffer.end(), DistanceComparator<T>());
	}

	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(resultBuffer));
}

template<typename T>
void AdditionBasedElementComposer<T>::initSecondarySortedVectorArrays(SortedVectorArrayList<T>& secondarySortedVectorArrays,
		const BuildingBlockBuckets<T>& buildingBlockBuckets) {
	//Implement in sub-class if neccessary
}

template<typename T>
void AdditionBasedElementComposer<T>::releaseSecondarySortedVectorArrays(SortedVectorArrayList<T>& sortedVectorArrays) {
	//Implement in sub-class if neccessary
}

template<typename T>
void AdditionBasedElementComposer<T>::findCompositionsInRange(const SortedVectorArray<T>& sortedVectorArray,
		const SortedVectorArrayList<T>& secondarySortedVectorArrays,
		int vectorIndex,
		std::vector<T>& partialTermElements,
		const T& partialTarget,
		const T& finalTarget,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& resultBuffer) {

	if(vectorIndex < 0) {
		evaluateCombination(partialTermElements,
				finalTarget,
				pDistanceCalculator,
				epsilon,
				resultBuffer);
		return;
	}

	//Detect bounds [lowerBound, upperBound]
	T min = partialTarget - m_epsilonElement;
	T max = partialTarget + m_epsilonElement;
	if(vectorIndex > 0) {
		min -= sortedVectorArray.getMaxAccumulated(vectorIndex - 1);
		max -= sortedVectorArray.getMinAccumulated(vectorIndex - 1);
	}

	int lowerBound = 0;
	int upperBound = 0;
	sortedVectorArray.getRangeInVector(vectorIndex, min, max, lowerBound, upperBound);

	const std::vector<T>& rightMostVector = sortedVectorArray.getVector(vectorIndex);
	for(int elementIndex = lowerBound; elementIndex < upperBound && elementIndex < rightMostVector.size(); elementIndex++) {
		T lastTerm = rightMostVector[elementIndex];

		//Quick-test to eliminate unpromising combinations, apart from the main filter
		if(!quickEvaluate(secondarySortedVectorArrays, vectorIndex, lastTerm, partialTarget)) {
			continue;
		}
		//Store last term into current partial term elements
		partialTermElements.push_back(lastTerm);

		//Calculate partial target, suppose lastTerm was added to become target
		T subPartialTarget = partialTarget - lastTerm;

		findCompositionsInRange(sortedVectorArray,
				secondarySortedVectorArrays,
				vectorIndex - 1,
				partialTermElements,
				subPartialTarget,
				finalTarget,
				pDistanceCalculator,
				epsilon,
				resultBuffer);

		//Restore partial terms elements
		partialTermElements.pop_back();
	}
}

template<typename T>
bool AdditionBasedElementComposer<T>::quickEvaluate(const SortedVectorArrayList<T>& secondarySortedVectorArrays,
			int vectorIndex,
			const T& rightMostElement,
			const T& partialTarget) const {
	//Implementation-details should be changed in sub-class
	return true;
}

template<typename T>
void AdditionBasedElementComposer<T>::evaluateCombination(const std::vector<T>& partialElements,
		T target,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& resultBuffer) {

	T candidate;
	composeCandidate(partialElements, candidate);
	m_combinationCounter++;

	if(!isValidCandidate(candidate)) {
		return;
	}
	mreal_t distanceToTarget = 0.0;
	if(pDistanceCalculator == NullPtr || (distanceToTarget = pDistanceCalculator->distance(candidate, target)) <= epsilon) {
		resultBuffer.push_back(LookupResult<T>(candidate, distanceToTarget));
		if(m_maxResultsNumber > 0 && resultBuffer.size() >= m_maxResultsNumber) {
			throw (1);
		}
	}
}

template<typename T>
void AdditionBasedElementComposer<T>::composeCandidate(const std::vector<T>& partialElements, T& result) {
	//Note that the vector is in reverse order
	int firstElementIndex = partialElements.size() - 1;
	T combined = partialElements[firstElementIndex];

	for(int i = firstElementIndex - 1; i >= 0; i--) {
		T tmp;
		m_pCombiner->combine(combined, partialElements[i], tmp);
		if(i < firstElementIndex - 1) {
			releaseIntermediateResult(combined);
		}
		combined = tmp;
	}
	result = combined;
}
