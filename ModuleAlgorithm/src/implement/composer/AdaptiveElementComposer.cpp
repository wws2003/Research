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
#include "Coordinate.hpp"
#include <iostream>

//#define ACE_DEBUGGING

template<typename T2>
void printTargetDebugInfo(T2 derivedTarget);

template<typename T1, typename T2>
AdaptiveElementComposer<T1, T2>::AdaptiveElementComposer(DistanceCalculatorPtr<T2> pDerivedDistanceCalculator,
		ComposerPtr<T2> pDerivedComposer,
		mreal_t derivedComposerEpsilon,
		ConverterPtr<T1, T2> pConverter,
		bool toVerifyDerivedResults) {

	m_pDerivedDistanceCalculator = pDerivedDistanceCalculator;
	m_pDerivedComposer = pDerivedComposer;
	m_derivedComposerEpsilon = derivedComposerEpsilon;
	m_pConverter = pConverter;
	m_toVerifyDerivedResults = toVerifyDerivedResults;
}

template<typename T1, typename T2>
AdaptiveElementComposer<T1, T2>::AdaptiveElementComposer(DistanceCalculatorPtr<T2> pDerivedDistanceCalculator,
		ComposerPtr<T2> pDerivedComposer,
		ConverterPtr<T1, T2> pConverter,
		bool toVerifyDerivedResults) {

	m_pDerivedDistanceCalculator = pDerivedDistanceCalculator;
	m_pDerivedComposer = pDerivedComposer;
	m_derivedComposerEpsilon = -1.0;
	m_pConverter = pConverter;
	m_toVerifyDerivedResults = toVerifyDerivedResults;
}

template<typename T1, typename T2>
IteratorPtr<LookupResult<T1> > AdaptiveElementComposer<T1, T2>::composeApproximations(const BuildingBlockBuckets<T1>& buildingBlockBuckets,
		T1 target,
		DistanceCalculatorPtr<T1> pDistanceCalculator,
		mreal_t epsilon,
		bool toSortResults) {

	BuildingBlockBuckets<T2> derivedBuildingBlockBuckets;
	convertToDerivedBuildingBlockBuckets(buildingBlockBuckets, derivedBuildingBlockBuckets);

	T2 derivedTarget;
	m_pConverter->convert12(target, derivedTarget);
	printTargetDebugInfo<T2>(derivedTarget);

	IteratorPtr<LookupResult<T2> > pDerivedResultIter = m_pDerivedComposer->composeApproximations(derivedBuildingBlockBuckets,
			derivedTarget,
			m_pDerivedDistanceCalculator,
			m_derivedComposerEpsilon >= 0.0 ? m_derivedComposerEpsilon : epsilon,
			false);

	IteratorPtr<LookupResult<T1> > pResultIter = NullPtr;
	convertToOriginalResultIter(pDerivedResultIter,
			pResultIter,
			target,
			pDistanceCalculator,
			epsilon,
			toSortResults);

	releaseDerivedResultIter(pDerivedResultIter);
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

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::convertToOriginalResultIter(const IteratorPtr<LookupResult<T2> >& pDerivedIter,
		IteratorPtr<LookupResult<T1> >& pOriginalIter,
		T1 target,
		DistanceCalculatorPtr<T1> pDistanceCalculator,
		mreal_t epsilon,
		bool toSortResults) {

	std::vector<LookupResult<T1> > results;

	if(pDerivedIter != NullPtr) {
		while(!pDerivedIter->isDone()) {
			T1 t1;
			m_pConverter->convert21(pDerivedIter->getObj().m_resultElement, t1);

			if(t1 != NullPtr) {
				//Re-check distance to original target to make sure if necessary
				if(m_toVerifyDerivedResults) {
					mreal_t distanceToTarget = pDistanceCalculator->distance(t1, target);
					if(distanceToTarget <= epsilon) {
						results.push_back(LookupResult<T1>(t1->clone(), distanceToTarget));
					}
				}
				else {
					results.push_back(LookupResult<T1>(t1->clone(), pDerivedIter->getObj().m_distanceToTarget));
				}
				//Release later
				/*else {
					_destroy(t1);
				}*/
			}
			else {
				//std::cout << "An abnormal case: Null orginal pointer\n";
			}
			pDerivedIter->next();
		}
		pDerivedIter->toBegin();
	}
	//TODO Filter before/after sort
	if(toSortResults) {
		std::sort(results.begin(), results.end(), DistanceComparator<T1>());
	}
	pOriginalIter = IteratorPtr<LookupResult<T1> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T1> >(results));
}

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::releaseDerivedBuildingBlockBuckets(BuildingBlockBuckets<T2>& derivedBuildingBlockBuckets) {
	for(typename BuildingBlockBuckets<T2>::iterator bIter = derivedBuildingBlockBuckets.begin(); bIter != derivedBuildingBlockBuckets.end();) {
		IteratorPtr<T2> pDerivedIter = *bIter;
		releaseDerivedIter(pDerivedIter);
		_destroy(pDerivedIter);
		bIter = derivedBuildingBlockBuckets.erase(bIter);
	}
}

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::releaseDerivedResultIter(IteratorPtr<LookupResult<T2> >& pDerivedIter) {
	if(pDerivedIter != NullPtr) {
		while(!pDerivedIter->isDone()) {
			T2 derivedElement = pDerivedIter->getObj().m_resultElement;
			releaseDerivedItem(derivedElement);
			pDerivedIter->next();
		}
	}
	_destroy(pDerivedIter);
}

template<typename T1, typename T2>
void AdaptiveElementComposer<T1, T2>::releaseDerivedIter(IteratorPtr<T2>& pDerivedIter) {
	if(pDerivedIter != NullPtr) {
		while(!pDerivedIter->isDone()) {
			T2 derivedElement = pDerivedIter->getObj();
			releaseDerivedItem(derivedElement);
			pDerivedIter->next();
		}
	}
}

template<typename T2>
void printTargetDebugInfo(T2 derivedTarget) {
#ifdef ACE_DEBUGGING
	const real_coordinate_t& vect = derivedTarget.getCoordinates();
	std::cout << "--------------\n";
	std::cout << "Target coordinate\n";
	for(mreal_t coord : vect) {
		std::cout << coord << " ";
	}
	std::cout << "\n";
#endif
}
