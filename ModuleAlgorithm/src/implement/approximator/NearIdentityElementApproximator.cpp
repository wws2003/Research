/*
 * NearIdentityElementApproximator.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#include "NearIdentityElementApproximator.h"

#include "ICoordinateCalculator.h"
#include "Bin.hpp"
#include "IIterator.h"
#include "Coordinate.hpp"
#include "VectorBasedCollectionImpl.hpp"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "ICollection.h"
#include "IBinCollection.h"
#include <iostream>

template<typename T>
void findApprxByMerge2Bins(BinPtr<T> pBin1,
		BinPtr<T> pBin2,
		T pQuery,
		double filterEpsilon,
		double epsilon,
		CombinerPtr<T> pCombiner,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		CollectionPtr<T> pResultCollection,
		ApprxResultBuffer<T>& apprxResultBuffer,
		int maxResultsNumber);

template<typename T>
void addApprxResultsToBufferFromCollection(CollectionPtr<T> pCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		double epsilon,
		ApprxResultBuffer<T>& rResultBuffer,
		int maxResultsNumber);

template<typename T>
NearIdentityElementApproximator<T>::NearIdentityElementApproximator(RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
		CombinerPtr<T> pCombiner,
		BinCollectionPtr<T> pBinCollection,
		const Config& config) {

	m_pRealCoordinateCalculator = pRealCoordinateCalculator;
	m_pCombiner = pCombiner;
	m_pBinCollection = pBinCollection;
	m_config = config;
}

template<typename T>
IteratorPtr<T> NearIdentityElementApproximator<T>::getApproximateElements(CollectionPtr<T> pCoreCollection,
		T pQuery,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		double epsilon) {

	//Calculate coordinate of query
	RealCoordinatePtr<T> pQueryCoordinate;
	m_pRealCoordinateCalculator->calulateElementCoordinate(pQuery, pQueryCoordinate);
	real_coordinate_t queryCoordinate = pQueryCoordinate->getCoordinates();

	initBinCollection(pCoreCollection,
			pQuery,
			pQueryCoordinate->getCoordinates(),
			pDistanceCalculator,
			epsilon);

	//If no result found even with initial epsilon
	if(m_pBinCollection->size() == 0) {
		return NullPtr;
	}

	ApprxResultBuffer<T> apprxResultBuffer;
	generateApproximationsFromBins(pQuery,
			pQueryCoordinate->getCoordinates(),
			pDistanceCalculator,
			epsilon,
			apprxResultBuffer);

	m_pBinCollection->clear();

	return IteratorPtr<T>(new VectorBasedReadOnlyIteratorImpl<T>(apprxResultBuffer)) ;
}

template<typename T>
void NearIdentityElementApproximator<T>::initBinCollection(CollectionPtr<T> pCoreCollection,
		T pQuery,
		const real_coordinate_t& queryCoordinate,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		double epsilon) {

	//Find first round results
	double firstRoundEpsilon = m_config.m_initialEpsilon;
	IteratorPtr<T> pFirstRoundApprxIter = pCoreCollection->findNearestNeighbour(pQuery, pDistanceCalculator, firstRoundEpsilon);

	if(pFirstRoundApprxIter == NullPtr || pFirstRoundApprxIter->isDone()) {
		return;
	}

	//Reset bin collection
	m_pBinCollection->clear();

	//Distribute first results into bin collection
	distributeResultsToBins(queryCoordinate,
			pFirstRoundApprxIter,
			m_pRealCoordinateCalculator,
			m_pBinCollection);
}

template<typename T>
void NearIdentityElementApproximator<T>::generateApproximationsFromBins(T pQuery,
		const real_coordinate_t& queryCoordinate,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		double epsilon,
		ApprxResultBuffer<T>& apprxResultBuffer) {

	int maxStep = m_config.m_iterationMaxSteps;
	double epsilonForMergeCandidate = m_config.m_maxCandidateEpsilon;
	int maxResultNumber = m_config.m_maxResultNumber;

	CollectionPtr<T> pApprxTempCollection(new VectorBasedCollectionImpl<T>());

	for(int stepCounter = 0; stepCounter < maxStep && apprxResultBuffer.size() < m_config.m_maxResultNumber; stepCounter++) {
		pApprxTempCollection->clear();

		//Thinking of applying SA here

		//TODO Don't fix theses values
		int maxMergeDistance = m_config.m_maxMergedBinDistance;

		BinIteratorPtr<T> pBinIter = m_pBinCollection->getBinIteratorPtr();
		pBinIter->toBegin();

		while(!pBinIter->isDone() && apprxResultBuffer.size() < maxResultNumber) {
			BinPtr<T> pBin = pBinIter->getObj();
			generateApproximationsPrefixedFromBins(pBin,
					maxMergeDistance,
					pQuery,
					queryCoordinate,
					pDistanceCalculator,
					epsilonForMergeCandidate,
					epsilon,
					pApprxTempCollection,
					apprxResultBuffer,
					maxResultNumber);

			pBinIter->next();
		}

		std::cout << "Number of results so far " << apprxResultBuffer.size() << "\r\n";

		if(stepCounter < maxStep - 1 && apprxResultBuffer.size() < maxResultNumber) {
			//Distribute newly generated matrices to bin collection, for new combination
			distributeResultsToBins(queryCoordinate,
					pApprxTempCollection->getIteratorPtr(),
					m_pRealCoordinateCalculator,
					m_pBinCollection);

			epsilonForMergeCandidate *= m_config.m_maxCandidateEpsilonDecreaseFactor;
		}
	}

	_destroy(pApprxTempCollection);
}

template<typename T>
void NearIdentityElementApproximator<T>::generateApproximationsPrefixedFromBins(BinPtr<T> pBin,
		int maxBinDistance,
		T pQuery,
		const real_coordinate_t& queryCoordinate,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		double epsilonForMergeCandidate,
		double approximationEpsilon,
		CollectionPtr<T> pApprxTempCollection,
		ApprxResultBuffer<T>& apprxResultBuffer,
		int maxResultsNumber) {

	BinIteratorPtr<T> pMergableBinIter = m_pBinCollection->findBinsCloseToBin(pBin, maxBinDistance);

	while(!pMergableBinIter->isDone()) {
		BinPtr<T> pMergableBin = pMergableBinIter->getObj();
		findApprxByMerge2Bins(pBin,
				pMergableBin,
				pQuery,
				epsilonForMergeCandidate,
				approximationEpsilon,
				m_pCombiner,
				pDistanceCalculator,
				pApprxTempCollection,
				apprxResultBuffer,
				maxResultsNumber);

		pMergableBinIter->next();
	}

	_destroy(pMergableBinIter);
}

template<typename T>
void NearIdentityElementApproximator<T>::distributeResultsToBins(const real_coordinate_t& queryCoordinate, IteratorPtr<T> pApprxIter, RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator, BinCollectionPtr<T> pBinCollection) {
	unsigned int nbCoordinates = queryCoordinate.size();
	BinPattern binPattern(nbCoordinates, '0');

	pApprxIter->toBegin();

	while(!pApprxIter->isDone()) {
		T apprxElement = pApprxIter->getObj();

		//Calculate real coordinate of approximation
		RealCoordinatePtr<T> pApprxCoord = NullPtr;
		pRealCoordinateCalculator->calulateElementCoordinate(apprxElement, pApprxCoord);

		//Calculate bin pattern for the approximation
		calculateBinPattern(queryCoordinate, pApprxCoord->getCoordinates(), binPattern);

		//Add to bin collection (may generate a new bin or add to existed one)
		pBinCollection->addElement(apprxElement, binPattern);

		_destroy(pApprxCoord);
		pApprxIter->next();
	}

	_destroy(pApprxIter);
}

template<typename T>
void NearIdentityElementApproximator<T>::calculateBinPattern(const real_coordinate_t& queryCoordinate, const real_coordinate_t& apprxCoordinate, BinPattern& binPattern) {
	unsigned int nbCoordinates = queryCoordinate.size();
	for(unsigned int i = 0; i < nbCoordinates; i++) {
		if(queryCoordinate[i] < apprxCoordinate[i] / 2) {
			binPattern[i] = '1';
		}
		else {
			binPattern[i] = '0';
		}
	}
}

template<typename T>
void findApprxByMerge2Bins(BinPtr<T> pBin1,
		BinPtr<T> pBin2,
		T pQuery,
		double filterEpsilon,
		double epsilon,
		CombinerPtr<T> pCombiner,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		CollectionPtr<T> pResultCollection,
		ApprxResultBuffer<T>& apprxResultBuffer,
		int maxResultsNumber) {

	unsigned int sizeBin1 = pBin1->getElements().size();
	unsigned int sizeBin2 = pBin2->getElements().size();

	std::cout << "Number of combination to check:" << (sizeBin1 * sizeBin2) << "\r\n";
	for(unsigned int i = 0; i < sizeBin1; i++) {
		for(unsigned int j = 0; j < sizeBin2; j++) {

			T pProduct1 = NullPtr;
			pCombiner->combine(pBin1->getElements()[i], pBin2->getElements()[j], pProduct1);

			if(pProduct1 != NullPtr) {
				double distance = pDistanceCalculator->distance(pProduct1, pQuery);

				if(distance <= filterEpsilon || (distance <= epsilon && apprxResultBuffer.size() < maxResultsNumber)) {
					if(distance <= filterEpsilon) {
						pResultCollection->addElement(pProduct1);
					}
					if(distance <= epsilon && apprxResultBuffer.size() < maxResultsNumber) {
						apprxResultBuffer.push_back(pProduct1);
					}
				}
				else {
					_destroy(pProduct1);
				}
			}
		}
	}
}

template<typename T>
void addApprxResultsToBufferFromCollection(CollectionPtr<T> pCollection, T pQuery, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, ApprxResultBuffer<T>& rResultBuffer, int maxResultsNumber) {
	IteratorPtr<T> pApprxResultIter = pCollection->findNearestNeighbour(pQuery, pDistanceCalculator, epsilon);
	if(pApprxResultIter != NullPtr) {
		while(!pApprxResultIter->isDone() && rResultBuffer.size() < maxResultsNumber) {
			rResultBuffer.push_back(pApprxResultIter->getObj());
			pApprxResultIter->next();
		}
	}
}




