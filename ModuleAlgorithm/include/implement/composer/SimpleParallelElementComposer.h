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

template<typename T>
class SimpleParallelElementComposer: public IComposer<T> {
public:
	SimpleParallelElementComposer(CombinerPtr<T> pCombiner,
			int maxResultsNumber,
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
	class TaskFutureBuffer {
	public:
		TaskFutureBuffer(size_t maxBufferSize, int maxResultsNumber);
		virtual ~TaskFutureBuffer(){};

		void addTaskFuturePair(TaskFuturePtr<LookupResult<T> > pTaskFuture, TaskPtr<LookupResult<T> > pTask);
		void collectResults(std::vector<LookupResult<T> >& resultBuffer);

	private:
		void moveResultsFromFutureBuffers();

		size_t m_maxBufferSize;
		int m_maxResultsNumber;
		std::vector<LookupResult<T> > m_results;
		std::vector<TaskFuturePtr<LookupResult<T> > > m_taskFuturesBuffer;
		std::vector<TaskPtr<LookupResult<T> > > m_tasks;
	};

	void generateApproximations(std::vector<T>& partialElementsBuffer,
			int bucketIndex,
			const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			TaskFutureBuffer& taskResultBuffer);

	std::string getCurrentLogFolderName();

	void evaluateCombination(const std::vector<T>& partialElements,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			TaskFutureBuffer& taskResultBuffer);

	static const std::string LOG_ROOT_FOLDER;

	ElementSetLogPtr<T> m_pElementSetLog;
	CombinerPtr<T> m_pCombiner;
	int m_maxResultsNumber;

	combination_counter_t m_combinationCounter;

	int m_logFolderCounter;

	TaskExecutorPtr<LookupResult<T> > m_pTaskExecutor;
};

#endif /* SIMPLEPARALLELELEMENTCOMPOSER_H_ */
