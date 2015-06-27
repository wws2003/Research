/*
 * GnatCollectionImpl.cpp
 *
 *  Created on: Jun 26, 2015
 *      Author: pham
 */

#include "GNATCollectionImpl.h"
#include "IDistanceCalculator.h"
#include "VectorBasedIteratorImpl.hpp"

#define MIN_SIZE_TO_BUILD_STRUCTURE 50

template<typename T>
void calculateMaxMinRange(T element, CollectionPtr<T> pCollection, DistanceCalculatorPtr<T> pDistanceCalculator, double& maxDist, double& minDist);

template<typename T>
GNATCollectionImpl<T>::GNATCollectionImpl(int nbSplitPoints) : m_splitPointRanges(RangeMap(nbSplitPoints ,std::vector<Range>(nbSplitPoints, Range(0,0)))){
	m_nbSplitPoints = nbSplitPoints;
}

template<typename T>
GNATCollectionImpl<T>::~GNATCollectionImpl() {
	purge();
}

template<typename T>
void GNATCollectionImpl<T>::addElement(T element) {
	m_unStructeredBuffer.push_back(element);
}

template<typename T>
void GNATCollectionImpl<T>::clear() {

	for(typename CollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
		CollectionPtr<T> pCollection = *cIter;
		cIter = m_subCollections.erase(cIter);
		pCollection->clear();
		_destroy(pCollection);
	}

	m_unStructeredBuffer.clear();

	m_splitPoints.clear();
}

template<typename T>
void GNATCollectionImpl<T>::purge() {

	for(typename CollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
		CollectionPtr<T> pCollection = *cIter;
		cIter = m_subCollections.erase(cIter);
		pCollection->purge();
		_destroy(pCollection);
	}

	for(typename std::vector<T>::iterator eIter = m_unStructeredBuffer.begin(); eIter != m_unStructeredBuffer.end(); ) {
		T element = *eIter;
		eIter = m_subCollections.erase(eIter);
		_destroy(element);
	}
}

template<typename T>
IteratorPtr<T> GNATCollectionImpl<T>::getIteratorPtr() {
	//TODO Implement
}

template<typename T>
IteratorPtr<T> GNATCollectionImpl<T>::getReadonlyIteratorPtr() {
	//TODO Implement
}

template<typename T>
CollectionSize_t GNATCollectionImpl<T>::size() const {
	CollectionSize_t size = m_unStructeredBuffer.size();

	for(typename CollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
		CollectionPtr<T> pCollection = *cIter;
		size += pCollection->size();
	}

	return size;
}

template<typename T>
void GNATCollectionImpl<T>::rebuildStructure(DistanceCalculatorPtr<T> pDistanceCalculator) {

	if(m_unStructeredBuffer.size() > MIN_SIZE_TO_BUILD_STRUCTURE) {
		//Re-call entire elements passed to sub collectiions to build from scratch
		recallElements();

		//Find split points
		initSplitPoints();

		//Re-construct sub collection
		m_subCollections.clear();
		createSubCollections();

		//Calculate range between each split point and each sub collection
		calculateRanges(pDistanceCalculator);

		//Recursively rebuild structure of each sub collections
		for(typename CollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
			CollectionPtr<T> pSubCollection = *cIter;
			pSubCollection->rebuildStructure(pDistanceCalculator);
		}

		//Clear unstructured buffer as all the elements has been structured
		m_unStructeredBuffer.clear();
	}
}

template<typename T>
IteratorPtr<T> GNATCollectionImpl<T>::findNearestNeighbour(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const {
	std::vector<T> results;

	//Get results from unstructured buffer first
	for(typename std::vector<T>::const_iterator eIter = m_unStructeredBuffer.begin(); eIter != m_unStructeredBuffer.end(); ) {
		T element = *eIter;
		if(pDistanceCalculator->distance(element, query) < epsilon) {
			results.push_back(element);
		}
	}

	//Get result from sub collections
	std::vector<int> candidateSubCollectionIndexes;
	getCandidateSubCollectionIndexes(query, pDistanceCalculator, candidateSubCollectionIndexes);

	for(unsigned int i = 0; i < candidateSubCollectionIndexes.size(); i++) {

		CollectionPtr<T> pCandidateSubCollection = m_subCollections[candidateSubCollectionIndexes[i]];
		pCandidateSubCollection->rebuildStructure();

		IteratorPtr<T> pSubResultIter = pCandidateSubCollection->findNearestNeighbour(query, pDistanceCalculator, epsilon);
		while(!pSubResultIter->isDone()) {
			results.push_back(*pSubResultIter);
			pSubResultIter->next();
		}
	}

	return IteratorPtr<T>(new VectorBasedIteratorImpl<T>(results));
}

template<typename T>
void GNATCollectionImpl<T>::recallElements() {
	for(typename CollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
		CollectionPtr<T> pSubCollection = *cIter;
		IteratorPtr<T> pSubCollectionElementIter = pSubCollection->getIteratorPtr();

		while(!pSubCollectionElementIter->isDone()) {
			m_unStructeredBuffer.push_back(pSubCollectionElementIter->getObj());
			pSubCollectionElementIter->next();
		}

		_destroy(pSubCollectionElementIter);

		cIter = m_subCollections.erase(cIter);
		pSubCollection->purge();
		_destroy(pSubCollection);
	}
}

template<typename T>
void GNATCollectionImpl<T>::initSplitPoints() {
	//TODO Implement
}

template<typename T>
void GNATCollectionImpl<T>::createSubCollections() {
	//TODO Create new sub collection instance

}

template<typename T>
void GNATCollectionImpl<T>::calculateRanges(DistanceCalculatorPtr<T> pDistanceCalculator) {
	for(unsigned int splitPointIndex = 0; splitPointIndex < m_nbSplitPoints; splitPointIndex++) {

		T splitPoint = m_splitPoints[splitPointIndex];

		for(unsigned int subCollectionIndex = 0; subCollectionIndex < m_subCollections.size(); subCollectionIndex++) {
			double maxRange = 0;
			double minRange = 0;
			calculateMaxMinRange(splitPoint, m_subCollections[subCollectionIndex], pDistanceCalculator, maxRange, minRange);
			m_splitPointRanges[splitPointIndex][subCollectionIndex] = Range(maxRange, minRange);
		}
	}
}

template<typename T>
void GNATCollectionImpl<T>::getCandidateSubCollectionIndexes(T query, DistanceCalculatorPtr<T> pDistanceCalculator, std::vector<int>& rCandidateIndexes) {
	//TODO Implement
}

template<typename T>
void calculateMaxMinRange(T element, CollectionPtr<T> pCollection, DistanceCalculatorPtr<T> pDistanceCalculator, double& maxDist, double& minDist) {
	maxDist = 0;
	minDist = 1E9;
	IteratorPtr<T> cIter = pCollection->getIteratorPtr();
	while(!cIter->isDone()) {
		double d = pDistanceCalculator->distance(cIter->getObj(), element);
		maxDist = std::max(d, maxDist);
		minDist = std::min(d, minDist);
		cIter->next();
	}
	_destroy(cIter);
}
