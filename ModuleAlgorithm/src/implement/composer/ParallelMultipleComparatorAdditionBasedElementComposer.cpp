/*
 * ParalleParallelMultipleComparatorAdditionBasedElementComposer.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#include "ParallelMultipleComparatorAdditionBasedElementComposer.h"

template<typename T>
ParallelMultipleComparatorAdditionBasedElementComposer<T>::ParallelMultipleComparatorAdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
		CombinerPtr<T> pCombiner,
		T epsilonElement,
		int maxResultsNumber,
		const std::vector<ComparatorPtr<T> >& secondaryComparators,
		const std::vector<T> secondaryEpsilonElements,
		TaskExecutorPtr<LookupResult<T> > pTaskExecutor) : ParallelAdditionBasedElementComposer<T>(pElementComparator ,
				pCombiner,
				epsilonElement,
				maxResultsNumber,
				pTaskExecutor
		) {
	m_secondaryComparators = secondaryComparators;
	m_secondaryEpsilonElements = secondaryEpsilonElements;
}

template<typename T>
void ParallelMultipleComparatorAdditionBasedElementComposer<T>::initSecondarySortedVectorArrays(SortedVectorArrayList<T>& secondarySortedVectorArrays,
		const BuildingBlockBuckets<T>& buildingBlockBuckets) {

	for(unsigned int i = 0; i < m_secondaryComparators.size(); i++) {
		ComparatorPtr<T> pSecondaryComparator = m_secondaryComparators[i];
		WrapperComparator<T> wrapperSecondaryComparator(pSecondaryComparator);
		SortedVectorArrayPtr<T> pSecondarySortVectorArray = new SortedVectorArray<T>(wrapperSecondaryComparator);
		pSecondarySortVectorArray->initByVectors(buildingBlockBuckets);
		secondarySortedVectorArrays.push_back(pSecondarySortVectorArray);
	}
}

template<typename T>
bool ParallelMultipleComparatorAdditionBasedElementComposer<T>::quickEvaluate(const SortedVectorArrayList<T>& secondarySortedVectorArrays,
		int vectorIndex,
		const T& rightMostElement,
		const T& partialTarget) const {

	for(unsigned int i = 0; i < secondarySortedVectorArrays.size(); i++) {
		SortedVectorArrayPtr<T> pSecondarySortedVectorArray = secondarySortedVectorArrays[i];
		ComparatorPtr<T> pSecondaryComparator = m_secondaryComparators[i];

		T secondaryAccumulatedMax = rightMostElement + m_secondaryEpsilonElements[i];
		T secondaryAccumulatedMin = rightMostElement - m_secondaryEpsilonElements[i];

		if(vectorIndex > 0) {
			T secondaryPrefixMax = pSecondarySortedVectorArray->getMaxAccumulated(vectorIndex - 1);
			T secondaryPrefixMin = pSecondarySortedVectorArray->getMinAccumulated(vectorIndex - 1);
			secondaryAccumulatedMax += secondaryPrefixMax;
			secondaryAccumulatedMin += secondaryPrefixMin;
		}

#ifdef DEBUG_SECONDARY
		std::cout << "-Compare upper for secondary comparator " << i << "-----\n";
		bool isTargetOverUpperBound = pSecondaryComparator->lessThan(secondaryAccumulatedMax, partialTarget);
		std::cout << "-Compare lower for secondary comparator " << i << "-----\n";
		bool isTargetOverLowerBound = pSecondaryComparator->lessThan(partialTarget, secondaryAccumulatedMin);
		std::cout << "--------End of compare-----\n";
		assert(!pSecondaryComparator->lessThan(secondaryPrefixMax, secondaryPrefixMin));
		assert(pSecondaryComparator->lessThan(secondaryAccumulatedMin, secondaryAccumulatedMax));
		std::cout << "--------Pass simple test-----\n\n";
#else
		bool isTargetOverUpperBound = pSecondaryComparator->lessThan(secondaryAccumulatedMax, partialTarget);
		bool isTargetOverLowerBound = pSecondaryComparator->lessThan(partialTarget, secondaryAccumulatedMin);
#endif
		if(isTargetOverUpperBound || isTargetOverLowerBound) {
			return false;
		}
	}
	return true;
}

template<typename T>
void ParallelMultipleComparatorAdditionBasedElementComposer<T>::releaseSecondarySortedVectorArrays(SortedVectorArrayList<T>& sortedVectorArrays) {

	for(unsigned int i = 0; i < sortedVectorArrays.size(); i++) {
		SortedVectorArrayPtr<T> pSortedVectorArray = sortedVectorArrays[i];
		delete pSortedVectorArray;
		sortedVectorArrays[i] = NULL;
	}
	sortedVectorArrays.clear();
}
