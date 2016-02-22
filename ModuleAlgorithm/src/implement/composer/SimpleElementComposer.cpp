/*
 * SimpleElementComposer.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#include "SimpleElementComposer.h"
#include "VectorBasedCollectionImpl.hpp"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "ElementsCombinationVerifyTask.h"
#include <sstream>
#include <iostream>

#define OUTPUT_INTERMEDIATE_RESULT 0

template<typename T>
const std::string SimpleElementComposer<T>::LOG_ROOT_FOLDER = "IntermediateLog";

#define TASK_FUTURE_BUFFER_SIZE (400)

template<typename T>
SimpleElementComposer<T>::SimpleElementComposer(CombinerPtr<T> pCombiner,
		int maxResultsNumber,
		ElementSetLogPtr<T> pElementSetLog,
		TaskExecutorPtr<LookupResult<T> > pTaskExecutor) {

	m_pCombiner = pCombiner;
	m_maxResultsNumber = maxResultsNumber;
	m_pElementSetLog = pElementSetLog;
	m_logFolderCounter = 0;
	m_combinationCounter = 0;
	m_pTaskExecutor = pTaskExecutor;

	pElementSetLog->prepareLogRootFolder(LOG_ROOT_FOLDER);
}

template<typename T>
IteratorPtr<LookupResult<T> > SimpleElementComposer<T>::composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		bool toSortResults) {

	std::vector<T> partialElementsBuffer;
	TaskFutureBuffer taskFutureBuffer(TASK_FUTURE_BUFFER_SIZE, m_maxResultsNumber);

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

	std::cout << "Number of combination checked:" << m_combinationCounter << "\n";
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
void SimpleElementComposer<T>::generateApproximations(std::vector<T>& partialElementsBuffer,
		int bucketIndex,
		const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		TaskFutureBuffer& taskResultBuffer) {

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
		TaskFutureBuffer& taskResultBuffer) {

	TaskPtr<LookupResult<T> > pTask = TaskPtr<LookupResult<T> >(new ElementsCombinationVerifyTask<T>(partialElements,
			m_pCombiner,
			pDistanceCalculator,
			pQuery,
			epsilon));

	TaskFuturePtr<LookupResult<T> > pTaskFuture = m_pTaskExecutor->submitTask(pTask);
	taskResultBuffer.addTaskFuturePair(pTaskFuture, pTask);
}

//-------------------Inner class, private methods-------------------//
template<typename T>
SimpleElementComposer<T>::TaskFutureBuffer::TaskFutureBuffer(size_t maxBufferSize, int maxResultsNumber) {
	m_maxBufferSize = maxBufferSize;
	m_maxResultsNumber = maxResultsNumber;
}

template<typename T>
void SimpleElementComposer<T>::TaskFutureBuffer::addTaskFuturePair(TaskFuturePtr<LookupResult<T> > pTaskFuture,
		TaskPtr<LookupResult<T> > pTask) {
	m_taskFuturesBuffer.push_back(pTaskFuture);
	m_tasks.push_back(pTask);

	if(m_taskFuturesBuffer.size() > m_maxBufferSize) {
		moveResultsFromFutureBuffers();
	}

	if(m_maxResultsNumber > 0 && m_results.size() > m_maxResultsNumber) {
		throw (1);
	}
}

template<typename T>
void SimpleElementComposer<T>::TaskFutureBuffer::collectResults(std::vector<LookupResult<T> >& resultBuffer) {
	//Retrieve results from remaining futures
	moveResultsFromFutureBuffers();
	resultBuffer.reserve(resultBuffer.size() + m_results.size());
	resultBuffer.insert(resultBuffer.end(), m_results.begin(), m_results.end());
	m_results.clear();
}

template<typename T>
void SimpleElementComposer<T>::TaskFutureBuffer::moveResultsFromFutureBuffers() {
	for(unsigned int i = 0; i < m_taskFuturesBuffer.size(); i++) {
		TaskFuturePtr<LookupResult<T> > pTaskFuture = m_taskFuturesBuffer[i];
		TaskPtr<LookupResult<T> > pTask = m_tasks[i];

		//Store verified result
		pTaskFuture->wait();
		TaskResult<LookupResult<T> > verifyTaskResult = pTaskFuture->getResult();
		if(verifyTaskResult.m_resultCode == ElementsCombinationVerifyTask<T>::TRC_POSITIVE) {
			m_results.push_back(verifyTaskResult.m_executeResult);
		}

		//Release memory for future and task
		m_taskFuturesBuffer[i] = NullPtr;
		m_tasks[i] = NullPtr;
		_destroy(pTaskFuture);
		_destroy(pTask);
	}
	m_taskFuturesBuffer.clear();
	m_tasks.clear();
}
