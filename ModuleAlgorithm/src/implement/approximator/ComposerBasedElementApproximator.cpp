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
IteratorPtr<T> ComposerBasedElementApproximator<T>::getApproximateElements(CollectionPtr<T> pCoreCollection,
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

	return pFinalResultIter;
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
		IteratorPtr<T> pBuildingBlockIter = getApproximateElementsForPartialQuery(pCoreCollection,
				partialQueries[i],
				pDistanceCalculator,
				m_initialEpsilon);

		//Add found building blocks to the bucket to compose later
		buildingBlockBuckets.push_back(pBuildingBlockIter);
	}

	//TODO Release elements in partialQueries
}

template<typename T>
IteratorPtr<T> ComposerBasedElementApproximator<T>::getApproximateElementsForPartialQuery(CollectionPtr<T> pCoreCollection,
		T pPartialQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon) {
	//In this base class, just return result from core collection.
	//Derived class may override this method to implement other logic
	return pCoreCollection->findNearestNeighbour(pPartialQuery, pDistanceCalculator, epsilon);
}

template<typename T>
void ComposerBasedElementApproximator<T>::releaseBuildingBlocksBuckets(BuildingBlockBuckets<T>& buildingBlockBuckets) {
	for(typename BuildingBlockBuckets<T>::iterator bIter = buildingBlockBuckets.begin(); bIter != buildingBlockBuckets.end();) {
		IteratorPtr<T> pIter = *bIter;
		bIter = buildingBlockBuckets.erase(bIter);
		_destroy(pIter);
	}
}
