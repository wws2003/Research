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
		int maxResultsNumber,
		TaskExecutorPtr<LookupResult<T> > pTaskExecutor) : m_wrapperComparator(pElementComparator) {
	m_pCombiner = pCombiner;
	m_pTaskExecutor = pTaskExecutor;
	m_epsilonElement = epsilonElement;
	m_maxResultsNumber = maxResultsNumber;
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
	TaskFutureBuffer taskFutureBuffer;
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

	std::vector<LookupResult<T> > resultBuffer;
	m_pTaskExecutor->executeAllRemaining();
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
		TaskFutureBuffer& taskFutureBuffer) {

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
		TaskFutureBuffer& taskFutureBuffer) {

	TaskPtr<LookupResult<T> > pCombiningTask = generateCombiningTask(partialElements);
	TaskFuturePtr<LookupResult<T> > pTaskFuture = m_pTaskExecutor->submitTask(pCombiningTask);
	taskFutureBuffer.addTaskFuturePair(pTaskFuture, pCombiningTask);
	m_combinationCounter++;
}

//-------------------Inner class-------------------//
template<typename T>
void ParallelAdditionBasedElementComposer<T>::TaskFutureBuffer::addTaskFuturePair(TaskFuturePtr<LookupResult<T> > pTaskFuture,
		TaskPtr<LookupResult<T> > pTask) {
	m_taskFuturesBuffer.push_back(pTaskFuture);
	m_tasks.push_back(pTask);
}

template<typename T>
void ParallelAdditionBasedElementComposer<T>::TaskFutureBuffer::collectResults(std::vector<LookupResult<T> >& resultBuffer) {
	for(unsigned int i = 0; i < m_taskFuturesBuffer.size(); i++) {
		TaskFuturePtr<LookupResult<T> > pTaskFuture = m_taskFuturesBuffer[i];
		TaskPtr<LookupResult<T> > pTask = m_tasks[i];

		//Store verified result
		pTaskFuture->wait();
		TaskResult<LookupResult<T> > taskResult = pTaskFuture->getResult();
		resultBuffer.push_back(taskResult.m_executeResult);

		//Release memory for future and task
		m_taskFuturesBuffer[i] = NullPtr;
		m_tasks[i] = NullPtr;
		_destroy(pTaskFuture);
		_destroy(pTask);
	}
	m_taskFuturesBuffer.clear();
	m_tasks.clear();
}


