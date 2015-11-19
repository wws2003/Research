/*
 * SKElementApproximator.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: pham
 */

#include "SKElementApproximator.h"
#include "ICollection.h"
#include "VectorBasedCollectionImpl.hpp"

template<typename T>
SKElementApproximator<T>::SKElementApproximator(DecomposerPtr<T> pQueryDecomposer,
		ComposerPtr<T> pBuildingBlockComposer,
		mreal_t initialEpsilon,
		int nbCandidates,
		int recursiveLevel) {
	m_pBuildingBlockComposer = pBuildingBlockComposer;
	m_pQueryDecomposer = pQueryDecomposer;
	m_nbCandidates = nbCandidates;
	m_initialEpsilon = initialEpsilon;
	m_recursiveLevel = recursiveLevel;
}

template<typename T>
IteratorPtr<T> SKElementApproximator<T>::getApproximateElements(CollectionPtr<T> pCoreCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon) {
	return skApproximate(pCoreCollection,
			pQuery,
			pDistanceCalculator,
			epsilon,
			m_recursiveLevel);
}

template<typename T>
IteratorPtr<T> SKElementApproximator<T>::skApproximate(CollectionPtr<T> pCoreCollection,
		T query,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		int level) {

	if(level == 0) {
		return pCoreCollection->findNearestNeighbour(query, pDistanceCalculator, m_initialEpsilon);
	}

	mreal_t epsilonIncreseFactor = 4.0;
	mreal_t lowerLevelEpsilon = epsilon * epsilonIncreseFactor;

	IteratorPtr<T> pRawApprxIter = skApproximate(pCoreCollection,
			query,
			pDistanceCalculator,
			lowerLevelEpsilon,
			level - 1);

	IteratorVector<T> apprxCandidatesIters;
	while(pRawApprxIter != NullPtr && !pRawApprxIter->isDone()) {
		T rawApprx = pRawApprxIter->getObj();

		IteratorPtr<T> pCandidatesIter = getCandidatesFromRawApprx(rawApprx,
				pCoreCollection,
				query,
				pDistanceCalculator,
				lowerLevelEpsilon,
				level);

		apprxCandidatesIters.push_back(pCandidatesIter);
		pRawApprxIter->next();

		//Break to test
		if(level == 1) {
			break;
		}
	}
	IteratorPtr<T> pApprxIter = filterCandidates(apprxCandidatesIters,
			query,
			pDistanceCalculator,
			epsilon);

	releaseIterators(apprxCandidatesIters);
	_destroy(pRawApprxIter);

	return pApprxIter;
}

template<typename T>
IteratorPtr<T> SKElementApproximator<T>::getCandidatesFromRawApprx(T apprx,
		CollectionPtr<T> pCoreCollection,
		T query,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		int level) {

	BuildingBlockBuckets<T> buildingBlocksBuckets;

	//Add apprx to buiding blocks as the first one
	VectorBasedCollectionImpl<T> apprxVector;
	apprxVector.addElement(apprx->clone());
	buildingBlocksBuckets.push_back(apprxVector.getIteratorPtr());

	T residual;
	calculateResidual(apprx, query, residual);

	//Add partial approximations of residual to buiding blocks
	addBuildingBlocksBucketsForResidual(residual,
			pCoreCollection,
			pDistanceCalculator,
			epsilon,
			buildingBlocksBuckets,
			level);

	//Compose building blocks to generare approximations for query
	IteratorPtr<T> pResultIter = m_pBuildingBlockComposer->composeApproximations(buildingBlocksBuckets,
			query,
			pDistanceCalculator,
			epsilon);

	_destroy(residual);

	//Release building blocks, also release elements inside them (suppose composer generate clone for result elements)
	releaseIterators(buildingBlocksBuckets);

	return pResultIter;
}

template<typename T>
IteratorPtr<T> SKElementApproximator<T>::filterCandidates(const IteratorVector<T>& buildingBlockBuckets,
		T query,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon) {

	VectorBasedCollectionImpl<T> results;
	try {
		for(unsigned int i = 0; i < buildingBlockBuckets.size(); i++) {
			IteratorPtr<T> pCandidateIterator = buildingBlockBuckets[i];

			while(pCandidateIterator != NullPtr && !pCandidateIterator->isDone()) {
				T candidate = pCandidateIterator->getObj();

				if(pDistanceCalculator->distance(candidate, query) <= epsilon) {
					results.addElement(candidate->clone());
					if(m_nbCandidates > 0 && results.size() >= m_nbCandidates) {
						throw (1);
					}
				}

				pCandidateIterator->next();
			}
		}
	}
	catch (int e) {
	}
	return results.getReadonlyIteratorPtr();
}

template<typename T>
void SKElementApproximator<T>::calculateResidual(T apprx,
		T query,
		T& residual) {
	// Implement in sub-class. This may be only a temporary workaround
}

template<typename T>
void SKElementApproximator<T>::addBuildingBlocksBucketsForResidual(T residual,
		CollectionPtr<T> pCoreCollection,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		BuildingBlockBuckets<T>& buildingBlockBuckets,
		int level) {

	std::vector<T> residualPartitions;
	m_pQueryDecomposer->decomposeElement(residual, residualPartitions);

	for(unsigned int i = 0; i < residualPartitions.size(); i++) {
		IteratorPtr<T> pPartialApprx = skApproximate(pCoreCollection,
				residualPartitions[i],
				pDistanceCalculator,
				epsilon,
				level - 1);
		buildingBlockBuckets.push_back(pPartialApprx);
	}
}

template<typename T>
void SKElementApproximator<T>::releaseIterators(std::vector<IteratorPtr<T> >& buildingBlockBuckets) {
	for(typename BuildingBlockBuckets<T>::iterator bIter = buildingBlockBuckets.begin(); bIter != buildingBlockBuckets.end();) {
		IteratorPtr<T> pIter = *bIter;
		//releaseElementsInIterator(pIter);
		bIter = buildingBlockBuckets.erase(bIter);
		_destroy(pIter);
	}
}

template<typename T>
void SKElementApproximator<T>::releaseElementsInIterator(IteratorPtr<T> pIter) {
	while(pIter != NullPtr && !pIter->isDone()) {
		T element = pIter->getObj();
		pIter->next();
		_destroy(element);
	}
}
