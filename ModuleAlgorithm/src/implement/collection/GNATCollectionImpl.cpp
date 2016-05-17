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
#include <algorithm>
#include <ctime>
#include <cassert>
#include <iostream>

//#define DEBUGGING
//#define DEBUGGING_SP

#define MIN_SIZE_TO_BUILD_STRUCTURE 30

template<typename T>
void calculateMaxMinRange(T element, CollectionPtr<T> pCollection, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t& maxDist, mreal_t& minDist);

template<typename T>
int findClosestElementIndex(T element, const std::vector<T>& dataSet, DistanceCalculatorPtr<T> pDistanceCalculator);

template<typename T>
void findClosestElementsInUnStructuredBuffer2(T query,
		const UnstructuredBuffer<T>& dataSet,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& results);

template<typename T>
void findClosestElementsInSplitPoints2(T query,
		const SplitPointSet<T>& dataSet,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& results);

template<typename T>
void findClosestElementsInUnStructuredBuffer3(T query,
		const UnstructuredBuffer<T>& dataSet,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		LookupResultProcessorPtr<T> pLookupResultProcessor);

template<typename T>
void findClosestElementsInSplitPoints3(T query,
		const SplitPointSet<T>& dataSet,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		LookupResultProcessorPtr<T> pLookupResultProcessor);

template<typename T>
void selfTest(GNATCollectionImpl<T>* pCollection);

template<typename T>
GNATCollectionImpl<T>::GNATCollectionImpl(DistanceCalculatorPtr<T> pDistanceCalculator,
		LookupResultProcessorPtr<T> pLookupResultProcessor,
		bool toCloneFilteredResults) : ICollection<T>(pDistanceCalculator)
		{
	m_pLookupResultProcessor = pLookupResultProcessor;
	m_toCloneFilteredResults = toCloneFilteredResults;
		}

template<typename T>
GNATCollectionImpl<T>::~GNATCollectionImpl() {
	if(m_pLookupResultProcessor != NullPtr) {
		m_pLookupResultProcessor->reset();
	}
	purge();
}

template<typename T>
void GNATCollectionImpl<T>::addElement(T element) {
	m_unStructeredBuffer.push_back(element);
}

template<typename T>
void GNATCollectionImpl<T>::clear() {

	for(typename GNATCollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
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

	for(typename GNATCollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
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
		pCurrentIter->next();
	}

	_destroy(pCurrentIter);

	return IteratorPtr<T>(new VectorBasedReadOnlyIteratorImpl<T>(buffer));
}

template<typename T>
CollectionSize_t GNATCollectionImpl<T>::size() const {
	CollectionSize_t size = m_unStructeredBuffer.size() + m_splitPoints.size();

	for(typename GNATCollectionVector<T>::const_iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); cIter++) {
		CollectionPtr<T> pCollection = *cIter;
		size += pCollection->size();
	}

	return size;
}

template<typename T>
void GNATCollectionImpl<T>::rebuildStructure() {
	DistanceCalculatorPtr<T> pDistanceCalculator = ICollection<T>::getDistanceCalculator();

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
		for(typename GNATCollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); cIter++) {
			CollectionPtr<T> pSubCollection = *cIter;
			pSubCollection->rebuildStructure();
		}
	}

	selfTest(this);
}

template<typename T>
IteratorPtr<LookupResult<T> > GNATCollectionImpl<T>::findNearestNeighbours(T query,
		mreal_t epsilon,
		bool toSortResults) const {

	std::vector<LookupResult<T> > results ;
	results.reserve(128); //Tekitou-ni

	findAndProcessNearestNeighbours(query,
			ICollection<T>::getDistanceCalculator(),
			epsilon,
			m_pLookupResultProcessor,
			results);

	if(m_pLookupResultProcessor != NullPtr) {
		//Final check and collect results
		m_pLookupResultProcessor->reset();
		m_pLookupResultProcessor->addLookupResultsBatch(results);

		results.clear();
		m_pLookupResultProcessor->retrieveProcessedLookupResults(results, toSortResults);

		//Reset again to maintain post condition
		m_pLookupResultProcessor->reset();
	}
	else {
		if(toSortResults) {
			std::sort(results.begin(), results.end(), DistanceComparator<T>());
		}
	}

	if(m_toCloneFilteredResults) {
		for(unsigned int i = 0; i < results.size(); i++) {
			results[i] = LookupResult<T>(results[i].m_resultElement->clone(), results[i].m_distanceToTarget);
		}
	}
	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(results));
}

template<typename T>
void GNATCollectionImpl<T>::findAndProcessNearestNeighbours(T query,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		LookupResultProcessorPtr<T> pLookupResultProcessor,
		std::vector<LookupResult<T> >& finalResults) const {

	std::vector<LookupResult<T> > tmpResults;

	if(pLookupResultProcessor != NullPtr) {
		//Get results from unstructured buffer first
		findClosestElementsInUnStructuredBuffer3(query,
				m_unStructeredBuffer,
				pDistanceCalculator,
				epsilon,
				pLookupResultProcessor);

		findClosestElementsInSplitPoints3(query,
				m_splitPoints,
				pDistanceCalculator,
				epsilon,
				pLookupResultProcessor);

		//Retrieve temporary results
		pLookupResultProcessor->retrieveProcessedLookupResults(tmpResults, false);

		//Reset processor
		pLookupResultProcessor->reset();
	}
	else {
		//Get results from unstructured buffer first
		findClosestElementsInUnStructuredBuffer2(query,
				m_unStructeredBuffer,
				pDistanceCalculator,
				epsilon,
				finalResults);

		findClosestElementsInSplitPoints2(query,
				m_splitPoints,
				pDistanceCalculator,
				epsilon,
				finalResults);
	}

	//Merge temporary results to final results
	finalResults.insert(finalResults.end(), tmpResults.begin(), tmpResults.end());

	//Get results from sub collections
	int nbSubCollections = m_subCollections.size();
	std::vector<int> subCollectionsCheckMap(nbSubCollections, 1);

	getCandidateSubCollections(query,
			pDistanceCalculator,
			epsilon,
			subCollectionsCheckMap);

	for(unsigned int i = 0; i < nbSubCollections; i++) {
		if(subCollectionsCheckMap[i] == 0) {
			continue;
		}
		GNATCollectionImplPtr<T> pCandidateSubCollection = m_subCollections[i];
		pCandidateSubCollection->findAndProcessNearestNeighbours(query,
				pDistanceCalculator,
				epsilon,
				pLookupResultProcessor,
				finalResults);
	}
}

template<typename T>
void GNATCollectionImpl<T>::recallElements() {
	for(typename GNATCollectionVector<T>::iterator cIter = m_subCollections.begin(); cIter != m_subCollections.end(); ) {
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
GNATCollectionImplPtr<T> GNATCollectionImpl<T>::generateSubCollection() {
	return GNATCollectionImplPtr<T>(new GNATCollectionImpl<T>(ICollection<T>::getDistanceCalculator(),
			NullPtr,
			false));
}

template<typename T>
void GNATCollectionImpl<T>::initSplitPoints() {
	m_splitPoints.clear();

	int nbSplitPoints = detectNbSplitPoints();

#ifdef DEBUGGING_SP
	//For debugging purpose, only take first elements from unstructured buffer to split points
	int nbSplitPointsCounter = 0;
	typename UnstructuredBuffer<T>::iterator firstIter = m_unStructeredBuffer.begin();
	firstIter++;	//Force case for UT error to happen.
	for(typename UnstructuredBuffer<T>::iterator eIter = firstIter; eIter != m_unStructeredBuffer.end(), nbSplitPointsCounter < nbSplitPoints; nbSplitPointsCounter++) {
		T splitPoint = *eIter;
		m_splitPoints.push_back(splitPoint);
		eIter = m_unStructeredBuffer.erase(eIter);
	}

#else
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
#endif
}

template<typename T>
void GNATCollectionImpl<T>::createSubCollections(DistanceCalculatorPtr<T> pDistanceCalculator) {

	int nbSplitPoints = m_splitPoints.size();

	//Generate sub collections
	for(unsigned int splitPointIndex = 0; splitPointIndex < nbSplitPoints; splitPointIndex++) {
		m_subCollections.push_back(generateSubCollection());
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
			mreal_t maxRange = 0;
			mreal_t minRange = 0;
			calculateMaxMinRange(splitPoint, m_subCollections[subCollectionIndex], pDistanceCalculator, maxRange, minRange);
			m_splitPointRanges[splitPointIndex][subCollectionIndex] = Range(minRange, maxRange);
		}
	}
}

template<typename T>
void GNATCollectionImpl<T>::getCandidateSubCollections(T query, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t epsilon, std::vector<int>& rCheckMap) const {

	unsigned int nbSubCollections = m_subCollections.size();

	for(unsigned int splitPointIndex = 0; splitPointIndex < nbSubCollections; splitPointIndex++) {

		mreal_t dSplitPointQuery = pDistanceCalculator->distance(m_splitPoints[splitPointIndex], query);

		//Calculate range of solution to split point considering the distance from split point to query
		Range solutionDistanceToSplitPointRange(dSplitPointQuery > epsilon ? dSplitPointQuery - epsilon : 0, epsilon + dSplitPointQuery);

		for(unsigned int subCollectionIndex = 0; subCollectionIndex < m_subCollections.size(); subCollectionIndex++) {
			if(rCheckMap[subCollectionIndex]) {
				//If range of distance of solution to split point is not in the range of distance between sub collection and split point
				//, then conclude the sub collection can't contain any solution
				Range subCollectionDistanceToSplitPointRange = m_splitPointRanges[splitPointIndex][subCollectionIndex];
				if(solutionDistanceToSplitPointRange.second < subCollectionDistanceToSplitPointRange.first || solutionDistanceToSplitPointRange.first > subCollectionDistanceToSplitPointRange.second) {
					rCheckMap[subCollectionIndex] = 0;
				}
			}
		}
	}
}

template<typename T>
int GNATCollectionImpl<T>::detectNbSplitPoints() {
	//FIXME More sophisticated solution
	return m_unStructeredBuffer.size() > 100000 ? 3 : 6;
}

template<typename T>
void calculateMaxMinRange(T element, CollectionPtr<T> pCollection, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t& maxDist, mreal_t& minDist) {
	maxDist = 0;
	minDist = 1E9;
	IteratorPtr<T> cIter = pCollection->getIteratorPtr();
	while(!cIter->isDone()) {
		mreal_t d = pDistanceCalculator->distance(cIter->getObj(), element);
		maxDist = std::max(d, maxDist);
		minDist = std::min(d, minDist);
		cIter->next();
	}
	_destroy(cIter);
}

template<typename T>
int findClosestElementIndex(T element, const std::vector<T>& dataSet, DistanceCalculatorPtr<T> pDistanceCalculator) {
	int closestElementIndex = 0;
	mreal_t closestDistanceToDataSet = 1e4;

	for(unsigned int elementIndex = 0; elementIndex < dataSet.size(); elementIndex++) {
		mreal_t distanceToElement = pDistanceCalculator->distance(element, dataSet[elementIndex]);

		if(distanceToElement < closestDistanceToDataSet) {
			closestDistanceToDataSet = distanceToElement;
			closestElementIndex = elementIndex;
		}
	}

	return closestElementIndex;
}

template<typename T>
void findClosestElementsInUnStructuredBuffer2(T query,
		const UnstructuredBuffer<T>& dataSet,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& results) {
	for(typename UnstructuredBuffer<T>::const_iterator eIter = dataSet.begin(); eIter != dataSet.end(); eIter++) {
		T element = *eIter;
		mreal_t distance = pDistanceCalculator->distance(element, query);
		if(distance <= epsilon) {
			results.push_back(LookupResult<T>(element, distance));
		}
	}
}

template<typename T>
void findClosestElementsInSplitPoints2(T query,
		const SplitPointSet<T>& dataSet,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		std::vector<LookupResult<T> >& results) {
	for(typename SplitPointSet<T>::const_iterator eIter = dataSet.begin(); eIter != dataSet.end(); eIter++) {
		T element = *eIter;
		mreal_t distance = pDistanceCalculator->distance(element, query);
		if(distance <= epsilon) {
			results.push_back(LookupResult<T>(element, distance));
		}
	}
}

template<typename T>
void findClosestElementsInUnStructuredBuffer3(T query,
		const UnstructuredBuffer<T>& dataSet,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		LookupResultProcessorPtr<T> pLookupResultProcessor) {
	for(typename UnstructuredBuffer<T>::const_iterator eIter = dataSet.begin(); eIter != dataSet.end(); eIter++) {
		T element = *eIter;
		mreal_t distance = pDistanceCalculator->distance(element, query);
		if(distance <= epsilon && pLookupResultProcessor != NullPtr) {
			pLookupResultProcessor->addLookupResult(LookupResult<T>(element, distance));
		}
	}
}

template<typename T>
void findClosestElementsInSplitPoints3(T query,
		const SplitPointSet<T>& dataSet,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		LookupResultProcessorPtr<T> pLookupResultProcessor) {
	for(typename SplitPointSet<T>::const_iterator eIter = dataSet.begin(); eIter != dataSet.end(); eIter++) {
		T element = *eIter;
		mreal_t distance = pDistanceCalculator->distance(element, query);
		if(distance <= epsilon && pLookupResultProcessor != NullPtr) {
			pLookupResultProcessor->addLookupResult(LookupResult<T>(element, distance));
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
