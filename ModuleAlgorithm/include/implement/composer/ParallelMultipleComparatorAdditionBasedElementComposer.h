/*
 * ParallelMultiComparatorAdditionBasedElementComposer.h
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#ifndef PARALLELMULTICOMPARATORADDITIONBASEDELEMENTCOMPOSER_H_
#define PARALLELMULTICOMPARATORADDITIONBASEDELEMENTCOMPOSER_H_

#include "ParallelAdditionBasedElementComposer.h"

template<typename T>
class ParallelMultipleComparatorAdditionBasedElementComposer: public ParallelAdditionBasedElementComposer<T> {
public:
	ParallelMultipleComparatorAdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
			CombinerPtr<T> pCombiner,
			T epsilonElement,
			int maxResultsNumber,
			const std::vector<ComparatorPtr<T> >& secondaryComparators,
			const std::vector<T> secondaryEpsilonElements,
			TaskExecutorPtr<LookupResult<T> > pTaskExecutor);

	virtual ~ParallelMultipleComparatorAdditionBasedElementComposer(){};

protected:
	//Override
	void initSecondarySortedVectorArrays(SortedVectorArrayList<T>& secondarySortedVectorArrays,
			const BuildingBlockBuckets<T>& buildingBlockBuckets);

	//Override
	bool quickEvaluate(const SortedVectorArrayList<T>& secondarySortedVectorArrays,
			int vectorIndex,
			const T& rightMostElement,
			const T& partialTarget) const;

	//Override
	void releaseSecondarySortedVectorArrays(SortedVectorArrayList<T>& sortedVectorArrays);

private:
	std::vector<ComparatorPtr<T> > m_secondaryComparators;
	std::vector<T> m_secondaryEpsilonElements;
};




#endif /* PARALLELMULTICOMPARATORADDITIONBASEDELEMENTCOMPOSER_H_ */
