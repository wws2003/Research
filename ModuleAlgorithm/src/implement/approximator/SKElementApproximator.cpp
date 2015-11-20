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
	initEpsilonForLevels();
}

template<typename T>
SKElementApproximator<T>::~SKElementApproximator() {
	delete[] m_epsilonForLevels;
}

template<typename T>
IteratorPtr<T> SKElementApproximator<T>::getApproximateElements(CollectionPtr<T> pCoreCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon) {
	resetEpsilonForLevels(epsilon);

	return skApproximate(pCoreCollection,
			pQuery,
			pDistanceCalculator,
			m_recursiveLevel);
}

template<typename T>
IteratorPtr<T> SKElementApproximator<T>::skApproximate(CollectionPtr<T> pCoreCollection,
		T query,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		int level) {

	mreal_t epsilonForCurrentLevel = m_epsilonForLevels[level];

	if(level == 0) {
		return pCoreCollection->findNearestNeighbour(query,
				pDistanceCalculator,
				epsilonForCurrentLevel);
	}

	IteratorPtr<T> pRawApprxIter = skApproximate(pCoreCollection,
			query,
			pDistanceCalculator,
			level - 1);

	IteratorVector<T> apprxCandidatesIters;
	int nbc = m_nbCandidates;

	while(pRawApprxIter != NullPtr && !pRawApprxIter->isDone()) {
		T rawApprx = pRawApprxIter->getObj();

		IteratorPtr<T> pCandidatesIter = getCandidatesFromRawApprx(rawApprx,
				pCoreCollection,
				query,
				pDistanceCalculator,
				epsilonForCurrentLevel,
				level);

		apprxCandidatesIters.push_back(pCandidatesIter);
		pRawApprxIter->next();

		//FIXME Break to test
		if(--nbc <= 0 || (pCandidatesIter != NullPtr && !pCandidatesIter->isDone())) {
			break;
		}
	}

	IteratorPtr<T> pApprxIter = filterCandidates(apprxCandidatesIters,
			query,
			pDistanceCalculator,
			epsilonForCurrentLevel);

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
void SKElementApproximator<T>::initEpsilonForLevels() {
	int nbEpsilonForLevels = m_recursiveLevel + 1;
	m_epsilonForLevels = new mreal_t[nbEpsilonForLevels];
	for(int i = 0; i < nbEpsilonForLevels; i++) {
		m_epsilonForLevels[i] = m_initialEpsilon;
	}
}

template<typename T>
void SKElementApproximator<T>::resetEpsilonForLevels(mreal_t epsilon) {
	//TODO Update lower-level epsilons properly
	m_epsilonForLevels[m_recursiveLevel] = epsilon;
	for(int j = m_recursiveLevel - 1; j >= 0; j--) {
		m_epsilonForLevels[j] = m_epsilonForLevels[j + 1] / 0.9;
	}
	//Temporaly fix level zero epsilon
	m_epsilonForLevels[0] = m_initialEpsilon;
}

template<typename T>
void SKElementApproximator<T>::addBuildingBlocksBucketsForResidual(T residual,
		CollectionPtr<T> pCoreCollection,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		BuildingBlockBuckets<T>& buildingBlockBuckets,
		int level) {

	std::vector<T> residualPartitions;
	m_pQueryDecomposer->decomposeElement(residual, residualPartitions);

	for(unsigned int i = 0; i < residualPartitions.size(); i++) {
		IteratorPtr<T> pPartialApprx = skApproximate(pCoreCollection,
				residualPartitions[i],
				pDistanceCalculator,
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
