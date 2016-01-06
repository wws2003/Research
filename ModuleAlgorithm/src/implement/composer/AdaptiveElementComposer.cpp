/*
 * AdaptiveElementComposer.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "AdaptiveElementComposer.h"
#include "IIterator.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "IDistanceCalculator.h"
#include <iostream>

template<typename T1, typename T2>
AdaptiveElementComposer<T1, T2>::AdaptiveElementComposer(DistanceCalculatorPtr<T2> pDerivedDistanceCalculator,
		ComposerPtr<T2> pDerivedComposer,
		mreal_t derivedComposerEpsilon,
		ConverterPtr<T1, T2> pConverter) {

	m_pDerivedDistanceCalculator = pDerivedDistanceCalculator;
	m_pDerivedComposer = pDerivedComposer;
	m_derivedComposerEpsilon = derivedComposerEpsilon;
	m_pConverter = pConverter;
}

template<typename T1, typename T2>
IteratorPtr<T1> AdaptiveElementComposer<T1, T2>::composeApproximations(const BuildingBlockBuckets<T1>& buildingBlockBuckets,
		T1 target,
		DistanceCalculatorPtr<T1> pDistanceCalculator,
		mreal_t epsilon) {

	BuildingBlockBuckets<T2> derivedBuildingBlockBuckets;
	convertToDerivedBuildingBlockBuckets(buildingBlockBuckets, derivedBuildingBlockBuckets);

	T2 derivedTarget;
	m_pConverter->convert12(target, derivedTarget);

	IteratorPtr<T2> pDerivedResultIter = m_pDerivedComposer->composeApproximations(derivedBuildingBlockBuckets, derivedTarget, m_pDerivedDistanceCalculator, m_derivedComposerEpsilon);

	IteratorPtr<T1> pResultIter = NullPtr;

	convertToOriginalIter(pDerivedResultIter, pResultIter, target, pDistanceCalculator, epsilon);

	releaseDerivedIter(pDerivedResultIter);

	releaseDerivedBuildingBlockBuckets(derivedBuildingBlockBuckets);

	return pResultIter;
}

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::convertToDerivedBuildingBlockBuckets(const BuildingBlockBuckets<T1>& buildingBlockBuckets, BuildingBlockBuckets<T2>& derivedBuildingBlockBuckets) {
	derivedBuildingBlockBuckets.clear();
	derivedBuildingBlockBuckets.reserve(buildingBlockBuckets.size());

	for(unsigned int i = 0; i < buildingBlockBuckets.size(); i++) {
		IteratorPtr<T2> pDerivedIter;
		convertToDerivedIter(buildingBlockBuckets[i], pDerivedIter);
		derivedBuildingBlockBuckets.push_back(pDerivedIter);
	}
}

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::releaseDerivedBuildingBlockBuckets(BuildingBlockBuckets<T2>& derivedBuildingBlockBuckets) {
	for(typename BuildingBlockBuckets<T2>::iterator bIter = derivedBuildingBlockBuckets.begin(); bIter != derivedBuildingBlockBuckets.end();) {
		IteratorPtr<T2> pDerivedIter = *bIter;
		releaseDerivedIter(pDerivedIter);
		bIter = derivedBuildingBlockBuckets.erase(bIter);
	}
}

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::releaseDerivedIter(IteratorPtr<T2>& pDerivedIter) {
	_destroy(pDerivedIter);
}

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::convertToOriginalIter(const IteratorPtr<T2>& pDerivedIter,
		IteratorPtr<T1>& pOriginalIter,
		T1 target,
		DistanceCalculatorPtr<T1> pDistanceCalculator,
		mreal_t epsilon) {
	std::vector<T1> results;

	if(pDerivedIter != NullPtr) {
		while(!pDerivedIter->isDone()) {
			T1 t1;
			m_pConverter->convert21(pDerivedIter->getObj(), t1);

			if(t1 != NullPtr) {
				//Re-check distance to original target to make sure
				if(pDistanceCalculator->distance(t1, target) <= epsilon) {
					results.push_back(t1);
				}
				else {
					_destroy(t1);
				}
			}
			else {
				//std::cout << "An abnormal case: Null orginal pointer\n";
			}

			pDerivedIter->next();
		}
		pDerivedIter->toBegin();
	}
	pOriginalIter = IteratorPtr<T1>(new VectorBasedReadOnlyIteratorImpl<T1>(results));
}

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::convertToDerivedIter(const IteratorPtr<T1>& pOriginalIter,
		IteratorPtr<T2>& pDerivedIter) {

	std::vector<T2> derivedElements;

	if(pOriginalIter != NullPtr) {
		while(!pOriginalIter->isDone()) {
			T2 derivedElement;
			m_pConverter->convert12(pOriginalIter->getObj(), derivedElement);
			derivedElements.push_back(derivedElement);
			pOriginalIter->next();
		}
		pOriginalIter->toBegin();
	}

	pDerivedIter = IteratorPtr<T2>(new VectorBasedReadOnlyIteratorImpl<T2>(derivedElements));
}

