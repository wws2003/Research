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

template<typename T>
void findApprxByMerge2Bins(BinPtr<T> pBin1, BinPtr<T> pBin2, T pQuery, double epsilon, CombinerPtr<T> pCombiner, DistanceCalculatorPtr<T> pDistanceCalculator, CollectionPtr<T> pResultCollection);

template<typename T>
void addApprxResultsToBufferFromCollection(CollectionPtr<T> pCollection, T pQuery, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, std::vector<T>& rResultBuffer);

template<typename T>
NearIdentityElementApproximator<T>::NearIdentityElementApproximator(RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator, CombinerPtr<T> pCombiner, BinCollectionPtr<T> pBinCollection) {
	m_pRealCoordinateCalculator = pRealCoordinateCalculator;
	m_pCombiner = pCombiner;
	m_pBinCollection = pBinCollection;
}

template<typename T>
IteratorPtr<T> NearIdentityElementApproximator<T>::getApproximateElements(CollectionPtr<T> pCoreCollection, T pQuery, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) {

	//Find first round results
	double firstRoundEpsilon = 10 * epsilon;
	IteratorPtr<T> pFirstRoundApprxIter = pCoreCollection->findApproxElements(pQuery, pDistanceCalculator, firstRoundEpsilon);

	if(pFirstRoundApprxIter == NullPtr || pFirstRoundApprxIter->isDone()) {
		return pFirstRoundApprxIter;
	}

	//Calculate coordinate of query
	RealCoordinatePtr<T> pQueryCoordinate;
	m_pRealCoordinateCalculator->calulateElementCoordinate(pQuery, pQueryCoordinate);
	real_coordinate_t queryCoordinate = pQueryCoordinate->getCoordinates();

	//Reset bin collection
	m_pBinCollection->clear();

	//Distribute first results into bin collection
	distributeResultsToBins(queryCoordinate, pFirstRoundApprxIter, m_pRealCoordinateCalculator, m_pBinCollection);

	int stepCounter = 0;
	int maxStep = 15;

	CollectionPtr<T> pApprxTempCollection(new VectorBasedCollectionImpl<T>());
	std::vector<T> apprxResultBuffer;

	while(++stepCounter <= maxStep) {
		pApprxTempCollection->clear();

		//Thinking of applying SA here

		//TODO Don't fix theses values
		int maxMergeDistance = 5;
		double epsilonForMergeCandidate = 3.0 * epsilon;

		BinIteratorPtr<T> pBinIter = m_pBinCollection->getBinIteratorPtr();

		while(!pBinIter->isDone()) {
			BinPtr<T> pBin = pBinIter->getObj();
			BinIteratorPtr<T> pMergableBinIter = m_pBinCollection->findBinsCloseToBin(pBin, maxMergeDistance);

			while(!pMergableBinIter->isDone()) {
				BinIteratorPtr<T> pMergableBin = pMergableBinIter->getObj();
				findApprxByMerge2Bins(pBin, pMergableBin, pQuery, epsilonForMergeCandidate, m_pCombiner, pDistanceCalculator, pApprxTempCollection);
				pMergableBinIter->next();
			}

			pBinIter->next();
		}

		//Distribute newly generated matrices to bin collection
		distributeResultsToBins(queryCoordinate, pApprxTempCollection->getIteratorPtr(), m_pRealCoordinateCalculator, m_pBinCollection);

		//Collect new results
		addApprxResultsToBufferFromCollection(pApprxTempCollection, pQuery, pDistanceCalculator, epsilon, apprxResultBuffer);
	}

	_destroy(pApprxTempCollection);

	m_pBinCollection->clear();

	return IteratorPtr<T>(new VectorBasedReadOnlyIteratorImpl<T>(apprxResultBuffer)) ;
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
void findApprxByMerge2Bins(BinPtr<T> pBin1, BinPtr<T> pBin2, T pQuery, double epsilon, CombinerPtr<T> pCombiner, DistanceCalculatorPtr<T> pDistanceCalculator, CollectionPtr<T> pResultCollection) {
	unsigned int sizeBin1 = pBin1->getElements().size();
	unsigned int sizeBin2 = pBin2->getElements().size();

	for(unsigned int i = 0; i < sizeBin1; i++) {
		for(unsigned int j = 0; j < sizeBin2; j++) {

			T pProduct1 = NullPtr;
			pCombiner->combine(pBin1->getElements()[i], pBin2->getElements()[j], pProduct1);

			if(pProduct1 != NullPtr && pDistanceCalculator->distance(pProduct1, pQuery) <= epsilon) {
				pResultCollection->addElement(pProduct1);
			}
			else {
				_destroy(pProduct1);
			}
		}
	}
}

template<typename T>
void addApprxResultsToBufferFromCollection(CollectionPtr<T> pCollection, T pQuery, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon, std::vector<T>& rResultBuffer) {
	IteratorPtr<T> pApprxResultIter = pCollection->findApproxElements(pQuery, pDistanceCalculator, epsilon);
	if(pApprxResultIter != NullPtr) {
		while(!pApprxResultIter->isDone()) {
			rResultBuffer.push_back(pApprxResultIter->getObj());
			pApprxResultIter->next();
		}
	}
}




