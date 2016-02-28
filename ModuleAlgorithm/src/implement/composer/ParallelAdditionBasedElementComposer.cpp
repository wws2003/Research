/*
 * ParallelParallelAdditionBasedElementComposer.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#include "ParallelAdditionBasedElementComposer.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "IIterator.h"
#include "IDistanceCalculator.h"
#include <algorithm>
#include <exception>
#include <iostream>

template<typename T>
ParallelAdditionBasedElementComposer<T>::ParallelAdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
		CombinerPtr<T> pCombiner,
		T epsilonElement,
		int taskFutureBufferSize,
		TaskExecutorPtr<LookupResult<T> > pTaskExecutor) : m_wrapperComparator(pElementComparator) {

	m_pCombiner = pCombiner;
	m_pTaskExecutor = pTaskExecutor;
	m_epsilonElement = epsilonElement;
	m_taskFutureBufferSize = taskFutureBufferSize;
	m_combinationCounter = 0;
}

template<typename T>
IteratorPtr<LookupResult<T> > ParallelAdditionBasedElementComposer<T>::composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T target,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		bool toSortResults) {

	SortedVectorArray<T> sortedVectorArray(m_wrapperComparator);
	sortedVectorArray.initByVectors(buildingBlockBuckets);

	SortedVectorArrayList<T> secondarySortedVectorArrays;
	initSecondarySortedVectorArrays(secondarySortedVectorArrays, buildingBlockBuckets);

	std::cout << "Finished pre-process\n";

	std::vector<T> partialTermElements;

	int lastVectorIndex = sortedVectorArray.getNbVectors() - 1;
	TaskFutureBuffer<T> taskFutureBuffer(m_pTaskExecutor, m_taskFutureBufferSize, 1);

	m_pTaskExecutor->start();
	findCompositionsInRange(sortedVectorArray,
			secondarySortedVectorArrays,
			lastVectorIndex,
			partialTermElements,
			target,
			target,
			pDistanceCalculator,
			epsilon,
			taskFutureBuffer);

	std::cout << "Number of combination submitted:" << m_combinationCounter << "\n";
	m_combinationCounter = 0;

	m_pTaskExecutor->executeAllRemaining();
	std::vector<LookupResult<T> > resultBuffer;
	taskFutureBuffer.collectResults(resultBuffer);
	m_pTaskExecutor->shutDown();

	//TODO Filter before/after sort results
	if(toSortResults) {
		std::sort(resultBuffer.begin(), resultBuffer.end(), DistanceComparator<T>());
	}

	releaseSecondarySortedVectorArrays(secondarySortedVectorArrays);

	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(resultBuffer));
}

template<typename T>
void ParallelAdditionBasedElementComposer<T>::initSecondarySortedVectorArrays(SortedVectorArrayList<T>& secondarySortedVectorArrays,
		const BuildingBlockBuckets<T>& buildingBlockBuckets) {
	//Implement in sub-class if neccessary
}

template<typename T>
void ParallelAdditionBasedElementComposer<T>::releaseSecondarySortedVectorArrays(SortedVectorArrayList<T>& sortedVectorArrays) {
	//Implement in sub-class if neccessary
}

template<typename T>
void ParallelAdditionBasedElementComposer<T>::findCompositionsInRange(const SortedVectorArray<T>& sortedVectorArray,
		const SortedVectorArrayList<T>& secondarySortedVectorArrays,
		int vectorIndex,
		std::vector<T>& partialTermElements,
		const T& partialTarget,
		const T& finalTarget,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		TaskFutureBuffer<T>& taskFutureBuffer) {

	if(vectorIndex < 0) {
		evaluateCombination(partialTermElements,
				finalTarget,
				pDistanceCalculator,
				epsilon,
				taskFutureBuffer);
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
				taskFutureBuffer);

		//Restore partial terms elements
		partialTermElements.pop_back();
	}
}

template<typename T>
bool ParallelAdditionBasedElementComposer<T>::quickEvaluate(const SortedVectorArrayList<T>& secondarySortedVectorArrays,
		int vectorIndex,
		const T& rightMostElement,
		const T& partialTarget) const {
	//Implementation-details should be changed in sub-class
	return true;
}

template<typename T>
void ParallelAdditionBasedElementComposer<T>::evaluateCombination(const std::vector<T>& partialElements,
		T target,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		TaskFutureBuffer<T>& taskFutureBuffer) {

	TaskPtr<LookupResult<T> > pCombiningTask = generateCombiningTask(partialElements, target, pDistanceCalculator, epsilon);
	TaskFuturePtr<LookupResult<T> > pTaskFuture = m_pTaskExecutor->submitTask(pCombiningTask);
	taskFutureBuffer.addTaskFuturePair(pTaskFuture, pCombiningTask);
	m_combinationCounter++;
}
