/*
 * SimpleElementComposer.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#include "SimpleElementComposer.h"
#include "VectorBasedCollectionImpl.hpp"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include <sstream>
#include <iostream>

#define OUTPUT_INTERMEDIATE_RESULT 0

template<typename T>
const std::string SimpleElementComposer<T>::LOG_ROOT_FOLDER = "IntermediateLog";

template<typename T>
SimpleElementComposer<T>::SimpleElementComposer(CombinerPtr<T> pCombiner,
		int maxResultsNumber,
		ElementSetLogPtr<T> pElementSetLog) {

	m_pCombiner = pCombiner;
	m_maxResultsNumber = maxResultsNumber;
	m_pElementSetLog = pElementSetLog;
	m_logFolderCounter = 0;
	m_combinationCounter = 0;

	pElementSetLog->prepareLogRootFolder(LOG_ROOT_FOLDER);
}

template<typename T>
IteratorPtr<LookupResult<T> > SimpleElementComposer<T>::composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		bool toSortResults) {

	std::vector<T> partialElementsBuffer;
	std::vector<LookupResult<T> > resultBuffer;

#if OUTPUT_INTERMEDIATE_RESULT
	m_pElementSetLog->saveElementSets(buildingBlockBuckets);
	m_pElementSetLog->saveQuery(pQuery);
#endif

	try {
		generateApproximations(partialElementsBuffer,
				0,
				buildingBlockBuckets,
				pQuery,
				pDistanceCalculator,
				epsilon,
				resultBuffer);
	}
	catch (int enough) {
		std::cout << "Find enough results\n";
	}

	std::cout << "Number of combination checked:" << m_combinationCounter << "\n";
	m_combinationCounter = 0;

#if OUTPUT_INTERMEDIATE_RESULT
	std::string logFolderName = getCurrentLogFolderName();
	m_pElementSetLog->flush(logFolderName);
#endif

	//TODO Filter before/after sort results
	if(toSortResults) {
		std::sort(resultBuffer.begin(), resultBuffer.end(), DistanceComparator<T>());
	}
	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(resultBuffer));
}

template<typename T>
void SimpleElementComposer<T>::generateApproximations(std::vector<T>& partialElementsBuffer,
		int bucketIndex,
		const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& resultBuffer) {

	if(bucketIndex >= buildingBlockBuckets.size()) {
		m_combinationCounter++;
		evaluateCombination(partialElementsBuffer,
				pQuery,
				pDistanceCalculator,
				epsilon,
				resultBuffer);
		return;
	}

	IteratorPtr<T> pBuildingBlockIter = buildingBlockBuckets[bucketIndex];
	do {
		bool bucketEmpty = pBuildingBlockIter->isDone();
		T nextPartialElement = bucketEmpty ? NullPtr : pBuildingBlockIter->getObj();

		partialElementsBuffer.push_back(nextPartialElement);

		generateApproximations(partialElementsBuffer,
				bucketIndex + 1,
				buildingBlockBuckets,
				pQuery,
				pDistanceCalculator,
				epsilon,
				resultBuffer);

		partialElementsBuffer.pop_back();

		if(!bucketEmpty) {
			pBuildingBlockIter->next();
		}
	}
	while(!pBuildingBlockIter->isDone());

	pBuildingBlockIter->toBegin();
}

template<typename T>
std::string SimpleElementComposer<T>::getCurrentLogFolderName() {
	std::stringstream ss;
	ss << LOG_ROOT_FOLDER  << "/" << ++m_logFolderCounter;
	return ss.str();
}

template<typename T>
void SimpleElementComposer<T>::evaluateCombination(const std::vector<T>& partialElements,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& resultBuffer) {

	T candidate;
	composeCandidate(partialElements, candidate);

	if(candidate != NullPtr) {
		mreal_t distanceToTarget = pDistanceCalculator->distance(candidate, pQuery);
		if(distanceToTarget <= epsilon) {
			resultBuffer.push_back(LookupResult<T>(candidate, distanceToTarget));
			if(m_maxResultsNumber > 0 && resultBuffer.size() >= m_maxResultsNumber) {
				throw (1);
			}
		}
		else {
			_destroy(candidate);
		}
	}
}

template<typename T>
void SimpleElementComposer<T>::composeCandidate(const std::vector<T>& partialElements, T& result) {
	T combined = partialElements.empty() ? NullPtr : partialElements[0]->clone();
	for(unsigned int i = 1; i < partialElements.size() && combined != NullPtr; i++) {
		if(partialElements[i] != NullPtr) {
			T tmp = NullPtr;
			m_pCombiner->combine(combined, partialElements[i], tmp);
			_destroy(combined);
			combined = tmp;
		}
	}
	result = combined;
}
