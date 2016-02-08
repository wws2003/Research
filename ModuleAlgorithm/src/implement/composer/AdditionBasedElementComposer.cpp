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

	SortedVectorArray sortedVectorArray(m_wrapperComparator);
	sortedVectorArray.initByVectors(buildingBlockBuckets);

	std::vector<LookupResult<T> > resultBuffer;
	std::vector<T> partialTermElements;

	int lastVectorIndex = sortedVectorArray.getNbVectors() - 1;
	try {
		findCompositionsInRange(sortedVectorArray,
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

	std::cout << "Number of combination checked:" << m_combinationCounter << "\n";
	m_combinationCounter = 0;

	//TODO Filter before/after sort results
	if(toSortResults) {
		std::sort(resultBuffer.begin(), resultBuffer.end(), DistanceComparator<T>());
	}

	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(resultBuffer));
}

template<typename T>
void AdditionBasedElementComposer<T>::findCompositionsInRange(const SortedVectorArray& sortedVectorArray,
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
	//std::cout << "For vector " <<  vectorIndex << ", bound to investigate:" << lowerBound << " " << upperBound << " oversize " << rightMostVector.size() << "\n";

	for(int elementIndex = lowerBound; elementIndex < upperBound && elementIndex < rightMostVector.size(); elementIndex++) {
		T lastTerm = rightMostVector[elementIndex];

		//Store last term into current partial term elements
		partialTermElements.push_back(lastTerm);

		//Calculate partial target, suppose lastTerm was added to become target
		T subPartialTarget = partialTarget - lastTerm;

		findCompositionsInRange(sortedVectorArray,
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
void AdditionBasedElementComposer<T>::evaluateCombination(const std::vector<T>& partialElements,
		T target,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& resultBuffer) {

	T candidate;
	composeCandidate(partialElements, candidate);

	if(pDistanceCalculator == NullPtr || pDistanceCalculator->distance(candidate, target) <= epsilon) {
		T delta = candidate - target;
		//FIXME This is implementation-dependent !
		resultBuffer.push_back(LookupResult<T>(candidate, delta.getNorm()));
		if(m_maxResultsNumber > 0 && resultBuffer.size() >= m_maxResultsNumber) {
			throw (1);
		}
	}

	m_combinationCounter++;
}

template<typename T>
void AdditionBasedElementComposer<T>::composeCandidate(const std::vector<T>& partialElements, T& result) {
	//Note that the vector is in reverse order
	int firstElementIndex = partialElements.size() - 1;
	T combined = partialElements[firstElementIndex];

	for(int i = firstElementIndex - 1; i >= 0; i--) {
		T tmp;
		m_pCombiner->combine(combined, partialElements[i], tmp);
		combined = tmp;
	}
	result = combined;
}

template<typename T>
void AdditionBasedElementComposer<T>::SortedVectorArray::initByVectors(const BuildingBlockBuckets<T>& buildingBlockBuckets) {
	size_t nbVector = buildingBlockBuckets.size();

	m_vectors.reserve(nbVector);
	m_maxAccumulated.reserve(nbVector);
	m_minAccumulated.reserve(nbVector);

	//Add elements to vectors
	//The counter to calculate how many combinations need to check in the worst case
	combination_counter_t nbMaximumCombination = 1;
	for(unsigned int i = 0; i < nbVector; i++) {
		IteratorPtr<T> pBuildingBlockIter = buildingBlockBuckets[i];
		std::vector<T> buildingBlockVector;

		if(pBuildingBlockIter != NullPtr) {
			while(!pBuildingBlockIter->isDone()) {
				buildingBlockVector.push_back(pBuildingBlockIter->getObj());
				pBuildingBlockIter->next();
			}
			pBuildingBlockIter->toBegin();
		}
		m_vectors.push_back(buildingBlockVector);//FIXME: No way to reserve size ?

		nbMaximumCombination *= buildingBlockVector.size();
	}

	//Sort vectors
	for(unsigned int j = 1; j < nbVector; j++) {
		std::sort(m_vectors[j].begin(), m_vectors[j].end(), m_wrapperComparator);
	}

	//Calculate max, min accumulated
	initMaxMinAccumulatedValues();

	//For debug purpose, print out accumlated max, min
	printDebugInfo();
}

template<typename T>
void AdditionBasedElementComposer<T>::SortedVectorArray::initMaxMinAccumulatedValues() {
	m_maxAccumulated.push_back(m_vectors[0][m_vectors[0].size() - 1]);
	m_minAccumulated.push_back(m_vectors[0][0]);

	size_t nbVectorSet = m_vectors.size();
	for(unsigned int j = 1; j < nbVectorSet; j++) {
		T maxAcc = m_maxAccumulated[j - 1];
		T minAcc = m_minAccumulated[j - 1];
		if(!m_vectors[j].empty()) {
			maxAcc = m_vectors[j][m_vectors[j].size() - 1] + m_maxAccumulated[j - 1];
			minAcc = m_vectors[j][0] + m_minAccumulated[j - 1];
		}
		m_maxAccumulated.push_back(maxAcc);
		m_minAccumulated.push_back(minAcc);
	}
}

template<typename T>
const T& AdditionBasedElementComposer<T>::SortedVectorArray::getMaxAccumulated(int lastVectorIndex) const {
	return m_maxAccumulated[lastVectorIndex];
}

template<typename T>
const T& AdditionBasedElementComposer<T>::SortedVectorArray::getMinAccumulated(int lastVectorIndex) const{
	return m_minAccumulated[lastVectorIndex];
}

template<typename T>
void AdditionBasedElementComposer<T>::SortedVectorArray::getRangeInVector(int vectorIndex,
		T min,
		T max,
		int& lowerBound,
		int& upperBound) const {

	const std::vector<T>& vector = m_vectors[vectorIndex];
	typename std::vector<T>::const_iterator lowerIter = std::lower_bound(vector.begin(), vector.end(), min, m_wrapperComparator);

	if(lowerIter == vector.end()) {
		lowerBound = vector.size();
		upperBound = vector.size();
		return;
	}
	lowerBound = lowerIter - vector.begin();

	typename std::vector<T>::const_iterator upperIter = std::upper_bound(vector.begin(), vector.end(), max, m_wrapperComparator);

	if(upperIter != vector.end()) {
		upperBound = upperIter - vector.begin();
	}
	else {
		upperBound = vector.size();
	}
}

template<typename T>
const std::vector<T>& AdditionBasedElementComposer<T>::SortedVectorArray::getVector(int vectorIndex) const{
	return m_vectors[vectorIndex];
}

template<typename T>
int AdditionBasedElementComposer<T>::SortedVectorArray::getNbVectors() const{
	return m_vectors.size();
}

template<typename T>
void AdditionBasedElementComposer<T>::SortedVectorArray::printDebugInfo() const {
#ifdef ABCE_DEBUG
	size_t nbVectorSet = m_vectors.size();

	auto printCoordFunc = [](real_coordinate_t coords) {
		std::cout << "-Coordinate:\n  ";
		for(mreal_t coord : coords) {
			std::cout << coord << " ";
		}
		std::cout << "\n";
	};
	//Print all vectors in every set
	for(unsigned int i = 0; i < nbVectorSet; i++) {
		std::cout << "Vector set " << i << "\n";
		std::for_each(m_vectors[i].begin(), m_vectors[i].end(), [&printCoordFunc](T coordElement) {
			printCoordFunc(coordElement.getCoordinates());
		}
		);
		std::cout << "--------------------\n";
	}
	for(unsigned int i = 0; i < nbVectorSet; i++) {
		std::cout << "Accumulated value upto vector set " << i << "\n";
		std::cout << "Accumulated max:\n";
		printCoordFunc(m_maxAccumulated[i].getCoordinates());
		std::cout << "Accumulated min:\n";
		printCoordFunc(m_minAccumulated[i].getCoordinates());
		std::cout << "--------------------\n";
	}
#endif
}
