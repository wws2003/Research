/*
 * SimpleParallelElementComposer.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#include "SimpleParallelElementComposer.h"
#include "SimpleParallelElementComposer.h"
#include "VectorBasedCollectionImpl.hpp"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "ElementsCombinationVerifyTask.h"
#include <sstream>
#include <iostream>

#define OUTPUT_INTERMEDIATE_RESULT 0

template<typename T>
const std::string SimpleParallelElementComposer<T>::LOG_ROOT_FOLDER = "IntermediateLog";

template<typename T>
SimpleParallelElementComposer<T>::SimpleParallelElementComposer(CombinerPtr<T> pCombiner,
		int taskFutureBufferSize,
		ElementSetLogPtr<T> pElementSetLog,
		TaskExecutorPtr<LookupResult<T> > pTaskExecutor) {

	m_pCombiner = pCombiner;
	m_taskFutureBufferSize = taskFutureBufferSize;
	m_pElementSetLog = pElementSetLog;
	m_logFolderCounter = 0;
	m_combinationCounter = 0;
	m_pTaskExecutor = pTaskExecutor;

	pElementSetLog->prepareLogRootFolder(LOG_ROOT_FOLDER);
}

template<typename T>
IteratorPtr<LookupResult<T> > SimpleParallelElementComposer<T>::composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		bool toSortResults) {

	std::vector<T> partialElementsBuffer;
	TaskFutureBuffer<T> taskFutureBuffer(m_pTaskExecutor, m_taskFutureBufferSize, ElementsCombinationVerifyTask<T>::TRC_POSITIVE);

#if OUTPUT_INTERMEDIATE_RESULT
	m_pElementSetLog->saveElementSets(buildingBlockBuckets);
	m_pElementSetLog->saveQuery(pQuery);
#endif
	m_pTaskExecutor->start();
	try {
		generateApproximations(partialElementsBuffer,
				0,
				buildingBlockBuckets,
				pQuery,
				pDistanceCalculator,
				epsilon,
				taskFutureBuffer);
	}
	catch (int enough) {
		std::cout << "Find enough results\n";
	}

	std::cout << "Number of combination submitted:" << m_combinationCounter << "\n";
	m_combinationCounter = 0;

#if OUTPUT_INTERMEDIATE_RESULT
	std::string logFolderName = getCurrentLogFolderName();
	m_pElementSetLog->flush(logFolderName);
#endif

	//TODO Filter before/after sort results
	std::vector<LookupResult<T> > resultBuffer;
	taskFutureBuffer.collectResults(resultBuffer);
	if(toSortResults) {
		std::sort(resultBuffer.begin(), resultBuffer.end(), DistanceComparator<T>());
	}

	m_pTaskExecutor->shutDown();

	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(resultBuffer));
}

//-------------------Private methods-------------------//
template<typename T>
void SimpleParallelElementComposer<T>::generateApproximations(std::vector<T>& partialElementsBuffer,
		int bucketIndex,
		const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		TaskFutureBuffer<T>& taskResultBuffer) {

	if(bucketIndex >= buildingBlockBuckets.size()) {
		m_combinationCounter++;
		evaluateCombination(partialElementsBuffer,
				pQuery,
				pDistanceCalculator,
				epsilon,
				taskResultBuffer);
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
				taskResultBuffer);

		partialElementsBuffer.pop_back();

		if(!bucketEmpty) {
			pBuildingBlockIter->next();
		}
	}
	while(!pBuildingBlockIter->isDone());

	pBuildingBlockIter->toBegin();
}

template<typename T>
std::string SimpleParallelElementComposer<T>::getCurrentLogFolderName() {
	std::stringstream ss;
	ss << LOG_ROOT_FOLDER  << "/" << ++m_logFolderCounter;
	return ss.str();
}

template<typename T>
void SimpleParallelElementComposer<T>::evaluateCombination(const std::vector<T>& partialElements,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		TaskFutureBuffer<T>& taskResultBuffer) {

	TaskPtr<LookupResult<T> > pTask = TaskPtr<LookupResult<T> >(new ElementsCombinationVerifyTask<T>(partialElements,
			m_pCombiner,
			pDistanceCalculator,
			pQuery,
			epsilon));

	TaskFuturePtr<LookupResult<T> > pTaskFuture = m_pTaskExecutor->submitTask(pTask);
	taskResultBuffer.addTaskFuturePair(pTaskFuture, pTask);
}
