/*
 * SKElementApproximator.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: pham
 */

#include "SKElementApproximator.h"
#include "ICollection.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "VectorBasedCollectionImpl.hpp"
#include <iostream>

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
IteratorPtr<LookupResult<T> > SKElementApproximator<T>::getApproximateElements(CollectionPtr<T> pCoreCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon) {

	resetEpsilonForLevels(epsilon);

	IteratorPtr<LookupResult<T> > pFullResultIter = skApproximate(pCoreCollection,
			pQuery,
			pDistanceCalculator,
			m_recursiveLevel);

	return pFullResultIter;
}

template<typename T>
IteratorPtr<LookupResult<T> > SKElementApproximator<T>::skApproximate(CollectionPtr<T> pCoreCollection,
		T query,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		int level) {

	mreal_t epsilonForCurrentLevel = m_epsilonForLevels[level];

	if(level == 0) {
		return pCoreCollection->findNearestNeighbours(query,
				pDistanceCalculator,
				epsilonForCurrentLevel,
				true);
	}

	IteratorPtr<LookupResult<T> > pRawApprxIter = skApproximate(pCoreCollection,
			query,
			pDistanceCalculator,
			level - 1);

	IteratorVector<T> apprxCandidatesIters;

	int candidateCounter = 0;

	while((candidateCounter++ < m_nbCandidates) && (pRawApprxIter != NullPtr && !pRawApprxIter->isDone())) {
		T rawApprx = pRawApprxIter->getObj().m_resultElement;

		IteratorPtr<T> pCandidatesIter = getCandidatesFromRawApprx(rawApprx,
				pCoreCollection,
				query,
				pDistanceCalculator,
				epsilonForCurrentLevel,
				level);

		if(pCandidatesIter != NullPtr && !pCandidatesIter->isDone()) {
			apprxCandidatesIters.push_back(pCandidatesIter);;
		}
		else {
			_destroy(pCandidatesIter);
		}

		pRawApprxIter->next();
	}

	IteratorPtr<LookupResult<T> > pApprxIter = filterCandidates(apprxCandidatesIters,
			query,
			pDistanceCalculator,
			epsilonForCurrentLevel);

	//Release apprximation candidates
	releaseIterators(apprxCandidatesIters);

	//Release raw approximations
	if(pRawApprxIter) {
		pRawApprxIter->toBegin();
		releaseElementsInResultsIterator(pRawApprxIter);
		_destroy(pRawApprxIter);
	}

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
IteratorPtr<LookupResult<T> > SKElementApproximator<T>::filterCandidates(const IteratorVector<T>& buildingBlockBuckets,
		T query,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon) {

	std::vector<LookupResult<T> > results;
	try {
		for(unsigned int i = 0; i < buildingBlockBuckets.size(); i++) {
			IteratorPtr<T> pCandidateIterator = buildingBlockBuckets[i];

			while(pCandidateIterator != NullPtr && !pCandidateIterator->isDone()) {
				T candidate = pCandidateIterator->getObj();
				mreal_t distance = pDistanceCalculator->distance(candidate, query);

				if(distance <= epsilon) {
					results.push_back(LookupResult<T>(candidate->clone(), distance));
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
	std::sort(results.begin(), results.end(), DistanceComparator<T>());
	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(results));
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
		IteratorPtr<LookupResult<T> > pPartialApprx = skApproximate(pCoreCollection,
				residualPartitions[i],
				pDistanceCalculator,
				level - 1);

		buildingBlockBuckets.push_back(getExtractedElementIterator(pPartialApprx));

		_destroy(pPartialApprx);
	}

	//Destroy partial targets
	for(unsigned int i = 0; i < residualPartitions.size(); i++) {
		_destroy(residualPartitions[i]);
	}
}

template<typename T>
IteratorPtr<T> SKElementApproximator<T>::getExtractedElementIterator(IteratorPtr<LookupResult<T> > pLookupResultIter) {
	std::vector<T> elements;
	int elementCounter = 0;
	while(!pLookupResultIter->isDone()) {
		if(++elementCounter <= m_nbCandidates) {
			elements.push_back(pLookupResultIter->getObj().m_resultElement);
		}
		else {
			_destroy(pLookupResultIter->getObj().m_resultElement);
		}
		pLookupResultIter->next();
	}

	std::cout << "Nb Sub-Results: " << elements.size() << "\n";
	return IteratorPtr<T>(new VectorBasedReadOnlyIteratorImpl<T>(elements));
}

template<typename T>
void SKElementApproximator<T>::releaseIterators(std::vector<IteratorPtr<T> >& buildingBlockBuckets) {
	for(typename BuildingBlockBuckets<T>::iterator bIter = buildingBlockBuckets.begin(); bIter != buildingBlockBuckets.end();) {
		IteratorPtr<T> pIter = *bIter;
		releaseElementsInIterator(pIter);
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

template<typename T>
void SKElementApproximator<T>::releaseElementsInResultsIterator(IteratorPtr<LookupResult<T> > pIter) {
	while(pIter != NullPtr && !pIter->isDone()) {
		T element = pIter->getObj().m_resultElement;
		pIter->next();
		_destroy(element);
	}
}

