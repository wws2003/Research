/*
 * NearIdentityElementBinBasedComposer.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: pham
 */

#include "NearIdentityElementBinBasedComposer.h"
#include "ICoordinateCalculator.h"
#include "Bin.hpp"
#include "IIterator.h"
#include "Coordinate.hpp"
#include "VectorBasedCollectionImpl.hpp"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "ICollection.h"
#include "IBinCollection.h"
#include "ICombiner.h"
#include <iostream>

template<typename T>
void evaluateAproximationCandidate(T element,
		T pQuery,
		mreal_t filterEpsilon,
		mreal_t epsilon,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		CollectionPtr<T> pResultCollection,
		ApprxResultBuffer<LookupResult<T> >& apprxResultBuffer,
		int maxResultsNumber);

template<typename T>
void addApprxResultsToBufferFromCollection(CollectionPtr<T> pCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		ApprxResultBuffer<T>& rResultBuffer,
		int maxResultsNumber);

template<typename T>
NearIdentityElementBinBasedComposer<T>::NearIdentityElementBinBasedComposer(CombinerPtr<T> pCombiner,
		BinCollectionPtr<T> pBinCollection,
		const Config& config) {

	m_pCombiner = pCombiner;
	m_pBinCollection = pBinCollection;
	m_config = config;
}

template<typename T>
IteratorPtr<LookupResult<T> > NearIdentityElementBinBasedComposer<T>::composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		bool toSortResults) {

	//Add query to bin collection as a target
	m_pBinCollection->addTarget(pQuery);

	//This this case, using only the first bucket of building blocks. Buckets should be equivalent
	initBinCollection(buildingBlockBuckets[0],
			pQuery,
			epsilon);

	//If no result found even with initial epsilon
	if(m_pBinCollection->size() == 0) {
		return NullPtr;
	}

	ApprxResultBuffer<LookupResult<T> > apprxResultBuffer;
	generateApproximationsFromBins(pQuery,
			pDistanceCalculator,
			epsilon,
			apprxResultBuffer);

	m_pBinCollection->clear();

	return IteratorPtr<LookupResult<T> >(new VectorBasedReadOnlyIteratorImpl<LookupResult<T> >(apprxResultBuffer)) ;
}


template<typename T>
void NearIdentityElementBinBasedComposer<T>::initBinCollection(IteratorPtr<T> pBuildingBlockIter,
		T pQuery,
		mreal_t epsilon) {

	//Reset bin collection
	m_pBinCollection->clear();

	//Distribute first results into bin collection
	//Note: Do not destroy the building block iterator which given from outside
	distributeResultsToBins(pBuildingBlockIter,
			m_pBinCollection,
			false);
}

template<typename T>
void NearIdentityElementBinBasedComposer<T>::generateApproximationsFromBins(T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		ApprxResultBuffer<LookupResult<T> >& apprxResultBuffer) {

	int maxStep = m_config.m_iterationMaxSteps;
	mreal_t epsilonForMergeCandidate = m_config.m_maxCandidateEpsilon;
	int maxResultNumber = m_config.m_maxResultNumber;

	CollectionPtr<T> pApprxTempCollection(new VectorBasedCollectionImpl<T>(pDistanceCalculator));

	for(int stepCounter = 0; stepCounter < maxStep && apprxResultBuffer.size() < m_config.m_maxResultNumber; stepCounter++) {
		pApprxTempCollection->clear();

		//Thinking of applying SA here

		//TODO Don't fix theses values
		bin_combination_prior_t maxMergeDistance = m_config.m_maxMergedBinDistance;

		std::vector<BinPtrVector<T> > combinableBinSets;
		m_pBinCollection->findBinSetsShouldBeCombined(combinableBinSets, maxMergeDistance);

		for(unsigned int i = 0; i < combinableBinSets.size() && apprxResultBuffer.size() < maxResultNumber; i++) {
			generateApproximationsFromCombinableBins(combinableBinSets[i],
					pQuery,
					pDistanceCalculator,
					epsilonForMergeCandidate,
					epsilon,
					pApprxTempCollection,
					apprxResultBuffer,
					maxResultNumber);
		}

		std::cout << "Number of results so far " << apprxResultBuffer.size() << "\r\n";

		if(stepCounter < maxStep - 1 && apprxResultBuffer.size() < maxResultNumber) {
			//Distribute newly generated matrices to bin collection, for new combination
			distributeResultsToBins(pApprxTempCollection->getIteratorPtr(),
					m_pBinCollection);

			epsilonForMergeCandidate *= m_config.m_maxCandidateEpsilonDecreaseFactor;
		}

		//Re-calculate bins for better structure
		m_pBinCollection->restructureBins();
	}

	_destroy(pApprxTempCollection);
}

template<typename T>
void NearIdentityElementBinBasedComposer<T>::generateApproximationsFromCombinableBins(const BinPtrVector<T>& combinableBins,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilonForMergeCandidate,
		mreal_t approximationEpsilon,
		CollectionPtr<T> pApprxTempCollection,
		ApprxResultBuffer<LookupResult<T> >& apprxResultBuffer,
		int maxResultsNumber) {

	try {
		getApproximationsFromCombinableBins(NullPtr,
				combinableBins,
				0,
				pQuery,
				pDistanceCalculator,
				epsilonForMergeCandidate,
				approximationEpsilon,
				pApprxTempCollection,
				apprxResultBuffer,
				maxResultsNumber);
	}
	catch (int e) {
	}
}

//This implementation is good for memory cost rather than run-time cost
template<typename T>
void NearIdentityElementBinBasedComposer<T>::getApproximationsFromCombinableBins(T element,
		const BinPtrVector<T>& combinableBins,
		int binCounter,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilonForMergeCandidate,
		mreal_t approximationEpsilon,
		CollectionPtr<T> pApprxTempCollection,
		ApprxResultBuffer<LookupResult<T> >& apprxResultBuffer,
		int maxResultsNumber) {

	if(binCounter == combinableBins.size()) {
		evaluateAproximationCandidate(element,
				pQuery,
				epsilonForMergeCandidate,
				approximationEpsilon,
				pDistanceCalculator,
				pApprxTempCollection,
				apprxResultBuffer,
				maxResultsNumber);
	}

	BinPtr<T> currentBin = combinableBins[binCounter];
	for(unsigned int i = 0; i < currentBin->getElements().size(); i++) {
		T nextElementToCombine = currentBin->getElements()[i];
		T nextCombinedElement = NullPtr;
		if(binCounter == 0) {
			nextCombinedElement = nextElementToCombine;
		}
		else {
			m_pCombiner->combine(element, nextElementToCombine, nextCombinedElement);
		}
		//Recursive-call
		getApproximationsFromCombinableBins(nextCombinedElement,
				combinableBins,
				binCounter + 1,
				pQuery,
				pDistanceCalculator,
				epsilonForMergeCandidate,
				approximationEpsilon,
				pApprxTempCollection,
				apprxResultBuffer,
				maxResultsNumber);
	}
}

template<typename T>
void NearIdentityElementBinBasedComposer<T>::distributeResultsToBins(IteratorPtr<T> pApprxIter,
		BinCollectionPtr<T> pBinCollection,
		bool toDestroyApprxIter) {

	pApprxIter->toBegin();

	while(!pApprxIter->isDone()) {
		T apprxElement = pApprxIter->getObj();

		//Add item to bins collection.
		pBinCollection->addElement(apprxElement);

		pApprxIter->next();
	}

	if(toDestroyApprxIter) {
		_destroy(pApprxIter);
	}
}

template<typename T>
void evaluateAproximationCandidate(T element,
		T pQuery,
		mreal_t filterEpsilon,
		mreal_t epsilon,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		CollectionPtr<T> pResultCollection,
		ApprxResultBuffer<LookupResult<T> >& apprxResultBuffer,
		int maxResultsNumber) {

	mreal_t distance = pDistanceCalculator->distance(element, pQuery);

	if(distance <= filterEpsilon || (distance <= epsilon && apprxResultBuffer.size() < maxResultsNumber)) {
		if(distance <= filterEpsilon) {
			pResultCollection->addElement(element);
		}
		if(distance <= epsilon) {
			apprxResultBuffer.push_back(LookupResult<T>(element, distance));
		}
		if(apprxResultBuffer.size() >= maxResultsNumber) {
			throw (1);
		}
	}
	else {
		_destroy(element);
	}
}

template<typename T>
void addApprxResultsToBufferFromCollection(CollectionPtr<T> pCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t epsilon,
		ApprxResultBuffer<T>& rResultBuffer,
		int maxResultsNumber) {
	IteratorPtr<LookupResult<T> > pApprxResultIter = pCollection->findNearestNeighbours(pQuery, pDistanceCalculator, epsilon);
	if(pApprxResultIter != NullPtr) {
		while(!pApprxResultIter->isDone() && rResultBuffer.size() < maxResultsNumber) {
			rResultBuffer.push_back(pApprxResultIter->getObj().m_resultElement);
			pApprxResultIter->next();
		}
	}
}
