/*
 * MultipleComparatorAdditionBasedElementComposer.cpp
 *
 *  Created on: Feb 12, 2016
 *      Author: pham
 */

#include "MultipleComparatorAdditionBasedElementComposer.h"
#include <iostream>
#include <cassert>

//#define DEBUG_SECONDARY

template<typename T>
MultipleComparatorAdditionBasedElementComposer<T>::MultipleComparatorAdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
		CombinerPtr<T> pCombiner,
		T epsilonElement,
		int maxResultsNumber,
		const std::vector<ComparatorPtr<T> >& secondaryComparators,
		const std::vector<T> secondaryEpsilonElements) :AdditionBasedElementComposer<T>(pElementComparator ,
				pCombiner,
				epsilonElement,
				maxResultsNumber
		) {
	m_secondaryComparators = secondaryComparators;
	m_secondaryEpsilonElements = secondaryEpsilonElements;
}

template<typename T>
void MultipleComparatorAdditionBasedElementComposer<T>::initSecondarySortedVectorArrays(typename AdditionBasedElementComposer<T>::SortedVectorArrayList& secondarySortedVectorArrays,
		const BuildingBlockBuckets<T>& buildingBlockBuckets) {

	typedef typename AdditionBasedElementComposer<T>::SortedVectorArray SecondarySortedVectorArray;
	typedef typename AdditionBasedElementComposer<T>::SortedVectorArrayPtr SecondarySortedVectorArrayPtr;
	typedef typename AdditionBasedElementComposer<T>::WrapperComparator SecondaryWrapperComparator;

	for(unsigned int i = 0; i < m_secondaryComparators.size(); i++) {
		ComparatorPtr<T> pSecondaryComparator = m_secondaryComparators[i];
		SecondaryWrapperComparator wrapperSecondaryComparator(pSecondaryComparator);
		SecondarySortedVectorArrayPtr pSecondarySortVectorArray = new SecondarySortedVectorArray(wrapperSecondaryComparator);
		pSecondarySortVectorArray->initByVectors(buildingBlockBuckets);
		secondarySortedVectorArrays.push_back(pSecondarySortVectorArray);
	}
}

template<typename T>
bool MultipleComparatorAdditionBasedElementComposer<T>::quickEvaluate(const typename AdditionBasedElementComposer<T>::SortedVectorArrayList& secondarySortedVectorArrays,
		int vectorIndex,
		const T& rightMostElement,
		const T& partialTarget) const {
	if(vectorIndex == 0) {
		return true;
	}
	typedef typename AdditionBasedElementComposer<T>::SortedVectorArrayPtr SecondarySortedVectorArrayPtr;

	for(unsigned int i = 0; i < secondarySortedVectorArrays.size(); i++) {
		SecondarySortedVectorArrayPtr pSecondarySortedVectorArray = secondarySortedVectorArrays[i];
		ComparatorPtr<T> pSecondaryComparator = m_secondaryComparators[i];

		T secondaryPrefixMax = pSecondarySortedVectorArray->getMaxAccumulated(vectorIndex - 1);
		T secondaryPrefixMin = pSecondarySortedVectorArray->getMinAccumulated(vectorIndex - 1);

		T secondaryAccumulatedMax = secondaryPrefixMax + rightMostElement + m_secondaryEpsilonElements[i];
		T secondaryAccumulatedMin = secondaryPrefixMin + rightMostElement - m_secondaryEpsilonElements[i];

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
void MultipleComparatorAdditionBasedElementComposer<T>::releaseSecondarySortedVectorArrays(typename AdditionBasedElementComposer<T>::SortedVectorArrayList& sortedVectorArrays) {
	typedef typename AdditionBasedElementComposer<T>::SortedVectorArrayPtr SecondarySortedVectorArrayPtr;

	for(unsigned int i = 0; i < sortedVectorArrays.size(); i++) {
		SecondarySortedVectorArrayPtr pSortedVectorArray = sortedVectorArrays[i];
		delete pSortedVectorArray;
		sortedVectorArrays[i] = NULL;
	}
	sortedVectorArrays.clear();
}
