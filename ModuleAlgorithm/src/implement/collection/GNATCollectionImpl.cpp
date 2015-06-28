/*
 * GnatCollectionImpl.cpp
 *
 *  Created on: Jun 26, 2015
 *      Author: pham
 */

#include "GNATCollectionImpl.h"
#include "IDistanceCalculator.h"
#include "VectorBasedIteratorImpl.hpp"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "GNATCollectionIterator.hpp"
#include <cstdlib>
#include <ctime>
#include <cassert>

#define MIN_SIZE_TO_BUILD_STRUCTURE 30

template<typename T>
void calculateMaxMinRange(T element, CollectionPtr<T> pCollection, DistanceCalculatorPtr<T> pDistanceCalculator, double& maxDist, double& minDist);

template<typename T>
int findClosestElementIndex(T element, const std::vector<T>& dataSet, DistanceCalculatorPtr<T> pDistanceCalculator);

template<typename T>
void findClosestElementsInUnStructuredBuffer(T query, const UnstructuredBuffer<T>& dataSet, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, std::vector<T>& results);

template<typename T>
void findClosestElementsInSplitPoints(T query, const SplitPointSet<T>& dataSet, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, std::vector<T>& results);

template<typename T>
void selfTest(GNATCollectionImpl<T>* pCollection);

template<typename T>
GNATCollectionImpl<T>::GNATCollectionImpl() {
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

	m_splitPointRanges.clear();
}

template<typename T>
void GNATCollectionImpl<T>::purge() {

	for(typename CollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
		CollectionPtr<T> pCollection = *cIter;
		cIter = m_subCollections.erase(cIter);
		pCollection->purge();
		_destroy(pCollection);
	}

	for(typename UnstructuredBuffer<T>::iterator eIter = m_unStructeredBuffer.begin(); eIter != m_unStructeredBuffer.end(); ) {
		T element = *eIter;
		eIter = m_unStructeredBuffer.erase(eIter);
		_destroy(element);
	}

	for(typename SplitPointSet<T>::iterator eIter = m_splitPoints.begin(); eIter != m_splitPoints.end(); ) {
		T element = *eIter;
		eIter = m_splitPoints.erase(eIter);
		_destroy(element);
	}
}

template<typename T>
IteratorPtr<T> GNATCollectionImpl<T>::getIteratorPtr() {
	return IteratorPtr<T>(new GNATCollectionIterator<T>(this));
}

template<typename T>
IteratorPtr<T> GNATCollectionImpl<T>::getReadonlyIteratorPtr() {
	std::vector<T> buffer;
	IteratorPtr<T> pCurrentIter = getIteratorPtr();

	while(!pCurrentIter->isDone()) {
		buffer.push_back(pCurrentIter->getObj());
	}

	_destroy(pCurrentIter);

	return IteratorPtr<T>(new VectorBasedReadOnlyIteratorImpl<T>(buffer));
}

template<typename T>
CollectionSize_t GNATCollectionImpl<T>::size() const {
	CollectionSize_t size = m_unStructeredBuffer.size() + m_splitPoints.size();

	for(typename CollectionVector<T>::const_iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); cIter++) {
		CollectionPtr<T> pCollection = *cIter;
		size += pCollection->size();
	}

	return size;
}

template<typename T>
void GNATCollectionImpl<T>::rebuildStructure(DistanceCalculatorPtr<T> pDistanceCalculator) {

	if(m_unStructeredBuffer.size() > MIN_SIZE_TO_BUILD_STRUCTURE) {
		//Re-call entire elements passed to sub collections to build from scratch
		recallElements();

		//Find split points
		initSplitPoints();

		//Re-construct sub collection
		createSubCollections(pDistanceCalculator);

		//Calculate range between each split point and each sub collection
		calculateRanges(pDistanceCalculator);

		//Recursively rebuild structure of each sub collections
		for(typename CollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); cIter++) {
			CollectionPtr<T> pSubCollection = *cIter;
			pSubCollection->rebuildStructure(pDistanceCalculator);
		}
	}

	//selfTest(this);
}

template<typename T>
IteratorPtr<T> GNATCollectionImpl<T>::findNearestNeighbour(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const {
	std::vector<T> results;

	//Get results from unstructured buffer first
	findClosestElementsInUnStructuredBuffer(query, m_unStructeredBuffer, pDistanceCalculator, epsilon, results);

	//Then results from split points
	findClosestElementsInSplitPoints(query, m_splitPoints, pDistanceCalculator, epsilon, results);

	//Get results from sub collections
	std::vector<int> candidateSubCollectionIndexes;
	getCandidateSubCollectionIndexes(query, pDistanceCalculator, epsilon, candidateSubCollectionIndexes);

	for(unsigned int i = 0; i < candidateSubCollectionIndexes.size(); i++) {

		CollectionPtr<T> pCandidateSubCollection = m_subCollections[candidateSubCollectionIndexes[i]];

		IteratorPtr<T> pSubResultIter = pCandidateSubCollection->findNearestNeighbour(query, pDistanceCalculator, epsilon);
		while(!pSubResultIter->isDone()) {
			results.push_back(pSubResultIter->getObj());
			pSubResultIter->next();
		}
		_destroy(pSubResultIter);
	}

	return IteratorPtr<T>(new VectorBasedReadOnlyIteratorImpl<T>(results));
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
		pSubCollection->clear();
		_destroy(pSubCollection);
	}
}

template<typename T>
void GNATCollectionImpl<T>::initSplitPoints() {
	m_splitPoints.clear();

	int nbSplitPoints = 6; //FIXME May be modified respecting the data size

	int nbFirstUnstructuredElements = m_unStructeredBuffer.size();

	//Currently "randomly" choose split points
	srand(time(NULL));

	unsigned int nbRemainUnstructuredElements = m_unStructeredBuffer.size();

	for(typename UnstructuredBuffer<T>::iterator eIter = m_unStructeredBuffer.begin(); eIter != m_unStructeredBuffer.end(), nbSplitPoints > 0; ) {
		if (rand() % (nbRemainUnstructuredElements--) < nbSplitPoints) {
			T splitPoint = *eIter;

			//Add new split point
			m_splitPoints.push_back(splitPoint);

			//Extract it from unstructured buffer
			eIter = m_unStructeredBuffer.erase(eIter);

			nbSplitPoints--;
		}
		else {
			eIter++;
		}
	}

	assert(nbFirstUnstructuredElements == m_unStructeredBuffer.size() + m_splitPoints.size());

}

template<typename T>
void GNATCollectionImpl<T>::createSubCollections(DistanceCalculatorPtr<T> pDistanceCalculator) {

	int nbSplitPoints = m_splitPoints.size();

	//Generate sub collections
	for(unsigned int splitPointIndex = 0; splitPointIndex < nbSplitPoints; splitPointIndex++) {
		m_subCollections.push_back(CollectionPtr<T>(new GNATCollectionImpl<T>()));
	}

	//For each element in unstructured buffer, find the closest split point
	//then move to the corresponding sub collection
	for(typename UnstructuredBuffer<T>::iterator eIter = m_unStructeredBuffer.begin(); eIter != m_unStructeredBuffer.end();) {
		T element = *eIter;

		int closedSplitPointIndex = findClosestElementIndex(element, m_splitPoints, pDistanceCalculator);

		m_subCollections[closedSplitPointIndex]->addElement(element);

		eIter = m_unStructeredBuffer.erase(eIter);
	}
}

template<typename T>
void GNATCollectionImpl<T>::calculateRanges(DistanceCalculatorPtr<T> pDistanceCalculator) {
	int nbSplitPoints = m_splitPoints.size();
	m_splitPointRanges = RangeMap(nbSplitPoints ,std::vector<Range>(nbSplitPoints, Range(0,0)));

	for(unsigned int splitPointIndex = 0; splitPointIndex < nbSplitPoints; splitPointIndex++) {

		T splitPoint = m_splitPoints[splitPointIndex];

		for(unsigned int subCollectionIndex = 0; subCollectionIndex < m_subCollections.size(); subCollectionIndex++) {
			double maxRange = 0;
			double minRange = 0;
			calculateMaxMinRange(splitPoint, m_subCollections[subCollectionIndex], pDistanceCalculator, maxRange, minRange);
			m_splitPointRanges[splitPointIndex][subCollectionIndex] = Range(minRange, maxRange);
		}
	}
}

template<typename T>
void GNATCollectionImpl<T>::getCandidateSubCollectionIndexes(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, std::vector<int>& rCandidateIndexes) const {
	rCandidateIndexes.clear();

	unsigned int nbSubCollections = m_subCollections.size();
	std::vector<int> checkMap(nbSubCollections, 1);

	for(unsigned int splitPointIndex = 0; splitPointIndex < nbSubCollections; splitPointIndex++) {

		double dSplitPointQuery = pDistanceCalculator->distance(m_splitPoints[splitPointIndex], query);

		//Calculate range of solution to split point considering the distance from split point to query
		Range solutionDistanceToSplitPointRange(dSplitPointQuery > epsilon ? dSplitPointQuery - epsilon : 0, epsilon + dSplitPointQuery);

		for(unsigned int subCollectionIndex = 0; subCollectionIndex < m_subCollections.size(); subCollectionIndex++) {

			//If range of distance of solution to split point is not in the range of distance between sub collection and split point
			//, then conclude the sub collection can't contain any solution

			Range subCollectionDistanceToSplitPointRange = m_splitPointRanges[splitPointIndex][subCollectionIndex];
			if(solutionDistanceToSplitPointRange.second < subCollectionDistanceToSplitPointRange.first || solutionDistanceToSplitPointRange.first > subCollectionDistanceToSplitPointRange.second) {
				checkMap[subCollectionIndex] = 0;
			}
		}
	}

	for(unsigned int subCollectionIndex = 0; subCollectionIndex < m_subCollections.size(); subCollectionIndex++) {
		if(checkMap[subCollectionIndex] != 0) {
			rCandidateIndexes.push_back(subCollectionIndex);
		}
	}
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

template<typename T>
int findClosestElementIndex(T element, const std::vector<T>& dataSet, DistanceCalculatorPtr<T> pDistanceCalculator) {
	int closestElementIndex = 0;
	double closestDistanceToDataSet = 1e4;

	for(unsigned int elementIndex = 0; elementIndex < dataSet.size(); elementIndex++) {
		double distanceToElement = pDistanceCalculator->distance(element, dataSet[elementIndex]);

		if(distanceToElement < closestDistanceToDataSet) {
			closestDistanceToDataSet = distanceToElement;
			closestElementIndex = elementIndex;
		}
	}

	return closestElementIndex;
}

template<typename T>
void findClosestElementsInUnStructuredBuffer(T query, const UnstructuredBuffer<T>& dataSet, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, std::vector<T>& results) {
	for(typename UnstructuredBuffer<T>::const_iterator eIter = dataSet.begin(); eIter != dataSet.end(); eIter++) {
		T element = *eIter;
		if(pDistanceCalculator->distance(element, query) <= epsilon) {
			results.push_back(element);
		}
	}
}

template<typename T>
void findClosestElementsInSplitPoints(T query, const SplitPointSet<T>& dataSet, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, std::vector<T>& results) {
	for(typename SplitPointSet<T>::const_iterator eIter = dataSet.begin(); eIter != dataSet.end(); eIter++) {
		T element = *eIter;
		if(pDistanceCalculator->distance(element, query) <= epsilon) {
			results.push_back(element);
		}
	}
}

template<typename T>
void selfTest(GNATCollectionImpl<T>* pCollection) {
	CollectionSize_t countsFromIterator = 0;
	IteratorPtr<T> pIter = pCollection->getIteratorPtr();
	while(!pIter->isDone()) {
		countsFromIterator++;
		pIter->next();
	}
	_destroy(pIter);
	assert(countsFromIterator == pCollection->size());
}
