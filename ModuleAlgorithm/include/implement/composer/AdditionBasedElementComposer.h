/*
 * AdditionBasedElementComposer.h
 *
 *  Created on: Dec 6, 2015
 *      Author: pham
 */

#ifndef ADDITIONBASEDELEMENTCOMPOSER_H_
#define ADDITIONBASEDELEMENTCOMPOSER_H_

#include "IComposer.h"
#include "AlgoInternal.h"
#include "AlgoCommon.h"
#include "ICombiner.h"
#include "IElementComparator.h"
#include "SortedVectorArray.hpp"

template<typename T>
class AdditionBasedElementComposer: public IComposer<T> {
public:
	AdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
			CombinerPtr<T> pCombiner,
			T epsilonElement,
			int maxResultsNumber);

	virtual ~AdditionBasedElementComposer(){};

	//Override
	IteratorPtr<LookupResult<T> > composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			bool toSortResults = true);

protected:
	virtual void initSecondarySortedVectorArrays(SortedVectorArrayList<T>& secondarySortedVectorArrays,
			const BuildingBlockBuckets<T>& buildingBlockBuckets);

	void findCompositionsInRange(const SortedVectorArray<T>& sortedVectorArray,
			const SortedVectorArrayList<T>& secondarySortedVectorArrays,
			int vectorIndex,
			std::vector<T>& partialTermElements,
			const T& partialTarget,
			const T& target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			std::vector<LookupResult<T> >& resultBuffer);

	virtual void releaseSecondarySortedVectorArrays(SortedVectorArrayList<T>& sortedVectorArrays);

	virtual bool quickEvaluate(const SortedVectorArrayList<T>& secondarySortedVectorArrays,
			int vectorIndex,
			const T& rightMostElement,
			const T& partialTarget) const;

	void evaluateCombination(const std::vector<T>& partialElements,
			T target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			std::vector<LookupResult<T> >& resultBuffer);

	void composeCandidate(const std::vector<T>& partialElements, T& result);

	virtual void releaseIntermediateResult(T& result) = 0;

	virtual bool isValidCandidate(const T& candidate) = 0;

	WrapperComparator<T> m_wrapperComparator;

	CombinerPtr<T> m_pCombiner;
	int m_maxResultsNumber;

	T m_epsilonElement;
	combination_counter_t m_combinationCounter;
};

#endif /* ADDITIONBASEDELEMENTCOMPOSER_H_ */
