/*
 * ComposerBasedElementApproximator.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: pham
 */

#include "ComposerBasedElementApproximator.h"
#include "IIterator.h"
#include "ICollection.h"
#include "IComposer.h"
#include "IDistanceCalculator.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include <iostream>

template<typename T>
ComposerBasedElementApproximator<T>::ComposerBasedElementApproximator(DecomposerPtr<T> pQueryDecomposer,
		int nbPartialQueries,
		int buildingBlocksBucketMaxSize,
		ComposerPtr<T> pBuildingBlockComposer,
		mreal_t initialEpsilon,
		LookupResultFilterPtr<T> pLookupResultFilter) {

	m_pQueryDecomposer = pQueryDecomposer;
	m_nbPartialQueries = nbPartialQueries;
	m_buildingBlocksBucketMaxSize = buildingBlocksBucketMaxSize;
	m_pBuildingBlockComposer = pBuildingBlockComposer;
	m_initialEpsilon = initialEpsilon;
	m_pLookupResultFilter = pLookupResultFilter;
}

template<typename T>
IteratorPtr<LookupResult<T> > ComposerBasedElementApproximator<T>::getApproximateElements(CollectionPtr<T> pCoreCollection,
		T pQuery,
		mreal_t epsilon) {
	DistanceCalculatorPtr<T> pDistanceCalculator = pCoreCollection->getDistanceCalculator();

	BuildingBlockBuckets<T> buildingBlockBuckets;
	decomposeQueryIntoBuildingBlocksBuckets(pCoreCollection,
			pQuery,
			pDistanceCalculator,
			buildingBlockBuckets);

	IteratorPtr<LookupResult<T> > pFinalResultIter = m_pBuildingBlockComposer->composeApproximations(buildingBlockBuckets,
			pQuery,
			pDistanceCalculator,
			epsilon,
			true);

	releaseBuildingBlocksBuckets(buildingBlockBuckets);

	IteratorPtr<LookupResult<T> > pFullResultIter = getFullResultIterator(pFinalResultIter, pDistanceCalculator);
	_destroy(pFinalResultIter);

	return pFullResultIter;
}

template<typename T>
void ComposerBasedElementApproximator<T>::decomposeQueryIntoBuildingBlocksBuckets(CollectionPtr<T> pCoreCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		BuildingBlockBuckets<T>& buildingBlockBuckets) {
	buildingBlockBuckets.clear();

	std::vector<T> partialQueries;
	m_pQueryDecomposer->decomposeElement(pQuery,
			partialQueries,
			m_nbPartialQueries);

	combination_counter_t maximumNbCombinations = 1;
	for(unsigned int i = 0; i < partialQueries.size(); i++) {
		//Get buiding block list for partial query
		IteratorPtr<LookupResult<T> > pLookupIter = getApproximateElementsForPartialQuery(pCoreCollection,
				partialQueries[i],
				m_initialEpsilon);

		//Add found building blocks to the bucket to compose later
		buildingBlockBuckets.push_back(getExtractedElementIterator(pLookupIter,
				&maximumNbCombinations));

		_destroy(pLookupIter);
	}
	std::cout << "Maximum number of combinations " << maximumNbCombinations << "\n";

	//TODO Release elements in partialQueries
}

template<typename T>
IteratorPtr<LookupResult<T> > ComposerBasedElementApproximator<T>::getApproximateElementsForPartialQuery(CollectionPtr<T> pCoreCollection,
		T pPartialQuery,
		mreal_t epsilon) {
	//In this base class, just return result from core collection.
	//Derived class may override this method to implement other logic
	return pCoreCollection->findNearestNeighbours(pPartialQuery,
			epsilon);
}

template<typename T>
void ComposerBasedElementApproximator<T>::releaseBuildingBlocksBuckets(BuildingBlockBuckets<T>& buildingBlockBuckets) {
	for(typename BuildingBlockBuckets<T>::iterator bIter = buildingBlockBuckets.begin(); bIter != buildingBlockBuckets.end();) {
		IteratorPtr<T> pIter = *bIter;
		bIter = buildingBlockBuckets.erase(bIter);
		_destroy(pIter);
	}
}

template<typename T>
IteratorPtr<T> ComposerBasedElementApproximator<T>::getExtractedElementIterator(IteratorPtr<LookupResult<T> > pLookupResultIter,
		combination_counter_t* pCombinationCounter) {
	std::vector<T> elements;
	int count = 0;
	while(!pLookupResultIter->isDone() && (m_buildingBlocksBucketMaxSize == 0 || ++count <= m_buildingBlocksBucketMaxSize)) {
		elements.push_back(pLookupResultIter->getObj().m_resultElement);
		pLookupResultIter->next();
	}

	//Side-effect: Count maximum possible number of combinations
	std::cout << "Size of subset: " << elements.size() << "\n";
	if(pCombinationCounter != NULL) {
		*pCombinationCounter *= elements.size();
	}

	return IteratorPtr<T>(new VectorBasedReadOnlyIteratorImpl<T>(elements));
}

template<typename T>
IteratorPtr<LookupResult<T> > ComposerBasedElementApproximator<T>::getFullResultIterator(IteratorPtr<LookupResult<T> > pResultIter,
		DistanceCalculatorPtr<T> pDistanceCalculator) {

	std::vector<LookupResult<T> > fullResults;

	if(pResultIter != NullPtr) {
		while(!pResultIter->isDone()) {
			LookupResult<T> resultObj = pResultIter->getObj();
			fullResults.push_back(resultObj);
			pResultIter->next();
		}
		pResultIter->toBegin();

		//Filter results before sort
		if(m_pLookupResultFilter != NullPtr) {
			m_pLookupResultFilter->filterLookupResults(fullResults, pDistanceCalculator, false);
		}

		std::sort(fullResults.begin(), fullResults.end(), DistanceComparator<T>());
	}
	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(fullResults));
}
