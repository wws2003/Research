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

template<typename T>
ComposerBasedElementApproximator<T>::ComposerBasedElementApproximator(DecomposerPtr<T> pQueryDecomposer,
		int nbPartialQueries,
		ComposerPtr<T> pBuildingBlockComposer,
		mreal_t initialEpsilon) {

	m_pQueryDecomposer = pQueryDecomposer;
	m_nbPartialQueries = nbPartialQueries;
	m_pBuildingBlockComposer = pBuildingBlockComposer;
	m_initialEpsilon = initialEpsilon;
}

template<typename T>
IteratorPtr<LookupResult<T> > ComposerBasedElementApproximator<T>::getApproximateElements(CollectionPtr<T> pCoreCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon) {

	BuildingBlockBuckets<T> buildingBlockBuckets;
	decomposeQueryIntoBuildingBlocksBuckets(pCoreCollection,
			pQuery,
			pDistanceCalculator,
			buildingBlockBuckets);

	IteratorPtr<T> pFinalResultIter = m_pBuildingBlockComposer->composeApproximations(buildingBlockBuckets, pQuery, pDistanceCalculator, epsilon);

	releaseBuildingBlocksBuckets(buildingBlockBuckets);

	IteratorPtr<LookupResult<T> > pFullResultIter = getFullResultIterator(pFinalResultIter, pQuery, pDistanceCalculator);
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

	for(unsigned int i = 0; i < partialQueries.size(); i++) {
		//Get buiding block list for partial query
		IteratorPtr<LookupResult<T> > pLookupIter = getApproximateElementsForPartialQuery(pCoreCollection,
				partialQueries[i],
				pDistanceCalculator,
				m_initialEpsilon);

		//Add found building blocks to the bucket to compose later
		buildingBlockBuckets.push_back(getExtractedElementIterator(pLookupIter));

		_destroy(pLookupIter);
	}

	//TODO Release elements in partialQueries
}

template<typename T>
IteratorPtr<LookupResult<T> > ComposerBasedElementApproximator<T>::getApproximateElementsForPartialQuery(CollectionPtr<T> pCoreCollection,
		T pPartialQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon) {
	//In this base class, just return result from core collection.
	//Derived class may override this method to implement other logic
	return pCoreCollection->findNearestNeighbours(pPartialQuery,
			pDistanceCalculator,
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
IteratorPtr<T> ComposerBasedElementApproximator<T>::getExtractedElementIterator(IteratorPtr<LookupResult<T> > pLookupResultIter) {
	std::vector<T> elements;
	while(!pLookupResultIter->isDone()) {
		elements.push_back(pLookupResultIter->getObj().m_resultElement);
		pLookupResultIter->next();
	}
	return IteratorPtr<T>(new VectorBasedReadOnlyIteratorImpl<T>(elements));
}

template<typename T>
IteratorPtr<LookupResult<T> > ComposerBasedElementApproximator<T>::getFullResultIterator(IteratorPtr<T> pResultIter, T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator) {
	std::vector<LookupResult<T> > fullResults;

	if(pResultIter != NullPtr) {
		while(!pResultIter->isDone()) {
			T resultElement = pResultIter->getObj();
			mreal_t distance = pDistanceCalculator->distance(resultElement, pQuery);
			fullResults.push_back(LookupResult<T>(resultElement, distance));
			pResultIter->next();
		}
		pResultIter->toBegin();
		std::sort(fullResults.begin(), fullResults.end(), DistanceComparator<T>());
	}
	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(fullResults));
}
