/*
 * SimpleParallelElementComposer.h
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#ifndef SIMPLEPARALLELELEMENTCOMPOSER_H_
#define SIMPLEPARALLELELEMENTCOMPOSER_H_

#include "IComposer.h"
#include "ICombiner.h"
#include "ICollection.h"
#include "IElementSetLog.h"
#include "ITaskExecutor.h"
#include "TaskCommon.h"
#include "TaskFutureBuffer.hpp"

template<typename T>
class SimpleParallelElementComposer: public IComposer<T> {
public:
	SimpleParallelElementComposer(CombinerPtr<T> pCombiner,
			int taskFutureBufferSize,
			ElementSetLogPtr<T> pElementSetLog,
			TaskExecutorPtr<LookupResult<T> > pTaskExecutor);

	virtual ~SimpleParallelElementComposer(){};

	//Override
	virtual IteratorPtr<LookupResult<T> > composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			bool toSortResults);

private:
	void generateApproximations(std::vector<T>& partialElementsBuffer,
			int bucketIndex,
			const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			TaskFutureBuffer<T>& taskResultBuffer);

	std::string getCurrentLogFolderName();

	void evaluateCombination(const std::vector<T>& partialElements,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			TaskFutureBuffer<T>& taskResultBuffer);

	static const std::string LOG_ROOT_FOLDER;

	ElementSetLogPtr<T> m_pElementSetLog;
	CombinerPtr<T> m_pCombiner;
	int m_taskFutureBufferSize;

	combination_counter_t m_combinationCounter;

	int m_logFolderCounter;

	TaskExecutorPtr<LookupResult<T> > m_pTaskExecutor;
};

#endif /* SIMPLEPARALLELELEMENTCOMPOSER_H_ */
