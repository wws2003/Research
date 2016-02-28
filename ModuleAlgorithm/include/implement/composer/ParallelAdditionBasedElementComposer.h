/*
 * ParallelAdditionBasedElementComposer.h
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#ifndef PARALLELADDITIONBASEDELEMENTCOMPOSER_H_
#define PARALLELADDITIONBASEDELEMENTCOMPOSER_H_

#include "IComposer.h"
#include "AlgoInternal.h"
#include "AlgoCommon.h"
#include "ICombiner.h"
#include "IElementComparator.h"
#include "ITaskExecutor.h"
#include "ICombiner.h"
#include "IElementComparator.h"
#include "SortedVectorArray.hpp"

template<typename T>
class ParallelAdditionBasedElementComposer: public IComposer<T>  {
public:
	ParallelAdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
			CombinerPtr<T> pCombiner,
			T epsilonElement,
			int maxResultsNumber,
			TaskExecutorPtr<LookupResult<T> > pTaskExecutor);

	virtual ~ParallelAdditionBasedElementComposer(){};

	//Override
	IteratorPtr<LookupResult<T> > composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			bool toSortResults = true);

protected:
	virtual void initSecondarySortedVectorArrays(SortedVectorArrayList<T>& secondarySortedVectorArrays,
			const BuildingBlockBuckets<T>& buildingBlockBuckets);

	class TaskFutureBuffer {
	public:
		void addTaskFuturePair(TaskFuturePtr<LookupResult<T> > pTaskFuture, TaskPtr<LookupResult<T> > pTask);
		void collectResults(std::vector<LookupResult<T> >& resultBuffer);

	private:
		std::vector<TaskFuturePtr<LookupResult<T> > > m_taskFuturesBuffer;
		std::vector<TaskPtr<LookupResult<T> > > m_tasks;
	};

	void findCompositionsInRange(const SortedVectorArray<T>& sortedVectorArray,
			const SortedVectorArrayList<T>& secondarySortedVectorArrays,
			int vectorIndex,
			std::vector<T>& partialTermElements,
			const T& partialTarget,
			const T& target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			TaskFutureBuffer& taskFutureBuffer);

	virtual void releaseSecondarySortedVectorArrays(SortedVectorArrayList<T>& sortedVectorArrays);

	virtual bool quickEvaluate(const SortedVectorArrayList<T>& secondarySortedVectorArrays,
			int vectorIndex,
			const T& rightMostElement,
			const T& partialTarget) const;

	void evaluateCombination(const std::vector<T>& partialElements,
			T target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			TaskFutureBuffer& taskFutureBuffer);

	virtual TaskPtr<LookupResult<T> > generateCombiningTask(const std::vector<T>& partialElements,
			T target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon) = 0;

	TaskExecutorPtr<LookupResult<T> > m_pTaskExecutor;
	CombinerPtr<T> m_pCombiner;

	WrapperComparator<T> m_wrapperComparator;
	int m_maxResultsNumber;

	T m_epsilonElement;
	combination_counter_t m_combinationCounter;
};

#endif /* PARALLELADDITIONBASEDELEMENTCOMPOSER_H_ */
