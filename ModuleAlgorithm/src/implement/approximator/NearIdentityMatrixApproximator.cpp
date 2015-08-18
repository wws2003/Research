/*
 * NearIdentityMatrixApproximator.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: pham
 */

#include "NearIdentityMatrixApproximator.h"

#if USE_NEAR_IDENTITY_TEMPLATE

#include "NearIdentityElementApproximator.cpp"
#include "VectorBasedMatrixCollectionImpl.h"
#include "IMatrix.h"

template class NearIdentityElementApproximator<MatrixPtr>;

#else
#include "ICoordinateCalculator.h"
#include "Bin.hpp"
#include "IIterator.h"
#include "Coordinate.hpp"
#include "VectorBasedMatrixCollectionImpl.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"

typedef std::vector<double> real_coordinate_t;

void distributeResultsToBins(const real_coordinate_t& queryCoordinate, MatrixIteratorPtr pApprxMatrixIter, MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator, MatrixBinCollectionPtr pMatrixBinCollection);

void calculateBinsPattern(const real_coordinate_t& queryCoordinate, const real_coordinate_t& apprxCoordinate, BinPattern& binPattern);

void findApprxByMerge2Bins(MatrixBinPtr pBin1, MatrixBinPtr pBin2, MatrixPtr pQuery, double epsilon, MatrixCombinerPtr pMatrixCombiner, MatrixDistanceCalculatorPtr pDistanceCalculator, MatrixCollectionPtr pResultCollection);

void addApprxResultsToBufferFromCollection(MatrixCollectionPtr pCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon, MatrixPtrVector& rResultBuffer);

NearIdentityMatrixApproximator::NearIdentityMatrixApproximator(MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator, MatrixCombinerPtr pMatrixCombiner, MatrixBinCollectionPtr pMatrixBinCollections) {
	m_pMatrixRealCoordinateCalculator = pMatrixRealCoordinateCalculator;
	m_pMatrixCombiner = pMatrixCombiner;
	m_pMatrixBinCollection = pMatrixBinCollections;
}

MatrixIteratorPtr NearIdentityMatrixApproximator::getApproximateElements(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) {
	//Find first round results
	double firstRoundEpsilon = 10 * epsilon;
	MatrixIteratorPtr pFirstRoundApprxMatrixIter = pCoreCollection->findApproxElements(pQuery, pDistanceCalculator, firstRoundEpsilon);

	if(pFirstRoundApprxMatrixIter == NullPtr || pFirstRoundApprxMatrixIter->isDone()) {
		return pFirstRoundApprxMatrixIter;
	}

	//Calculate coordinate of query
	MatrixRealCoordinatePtr pQueryCoordinate;
	m_pMatrixRealCoordinateCalculator->calulateElementCoordinate(pQuery, pQueryCoordinate);
	real_coordinate_t queryCoordinate = pQueryCoordinate->getCoordinates();

	//Reset bin collection
	m_pMatrixBinCollection->clear();

	//Distribute first results into bin collection
	distributeResultsToBins(queryCoordinate, pFirstRoundApprxMatrixIter, m_pMatrixRealCoordinateCalculator, m_pMatrixBinCollection);

	int stepCounter = 0;
	int maxStep = 15;

	MatrixCollectionPtr pApprxTempCollection(new VectorBasedMatrixCollectionImpl());
	MatrixPtrVector apprxResultBuffer;

	while(++stepCounter <= maxStep) {
		pApprxTempCollection->clear();

		//Thinking of applying SA here

		//TODO Don't fix theses values
		int maxMergeDistance = 5;
		double epsilonForMergeCandidate = 3.0 * epsilon;

		MatrixBinIteratorPtr pBinIter = m_pMatrixBinCollection->getBinIteratorPtr();

		while(!pBinIter->isDone()) {
			MatrixBinPtr pBin = pBinIter->getObj();
			MatrixBinIteratorPtr pMergableBinIter = m_pMatrixBinCollection->findBinsCloseToBin(pBin, maxMergeDistance);

			while(!pMergableBinIter->isDone()) {
				MatrixBinPtr pMergableBin = pMergableBinIter->getObj();
				findApprxByMerge2Bins(pBin, pMergableBin, pQuery, epsilonForMergeCandidate, m_pMatrixCombiner, pDistanceCalculator, pApprxTempCollection);
				pMergableBinIter->next();
			}

			pBinIter->next();
		}

		//Distribute newly generated matrices to bin collection
		distributeResultsToBins(queryCoordinate, pApprxTempCollection->getIteratorPtr(), m_pMatrixRealCoordinateCalculator, m_pMatrixBinCollection);

		//Collect new results
		addApprxResultsToBufferFromCollection(pApprxTempCollection, pQuery, pDistanceCalculator, epsilon, apprxResultBuffer);
	}

	_destroy(pApprxTempCollection);

	m_pMatrixBinCollection->clear();

	return MatrixIteratorPtr(new VectorBasedReadOnlyIteratorImpl<MatrixPtr>(apprxResultBuffer)) ;
}

void distributeResultsToBins(const real_coordinate_t& queryCoordinate, MatrixIteratorPtr pApprxMatrixIter, MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator, MatrixBinCollectionPtr pMatrixBinCollection) {
	unsigned int nbCoordinates = queryCoordinate.size();
	BinPattern binPattern(nbCoordinates, '0');

	pApprxMatrixIter->toBegin();

	while(!pApprxMatrixIter->isDone()) {
		MatrixPtr pApprxMatrix = pApprxMatrixIter->getObj();

		//Calculate real coordinate of approximation matrix
		MatrixRealCoordinatePtr pApprxCoord = NullPtr;
		pMatrixRealCoordinateCalculator->calulateElementCoordinate(pApprxMatrix, pApprxCoord);

		//Calculate bin pattern for the approximation matrix
		calculateBinPattern(queryCoordinate, pApprxCoord->getCoordinates(), binPattern);

		//Add matrix to bin collection (may generate a new bin or add to existed one)
		pMatrixBinCollection->addElement(pApprxMatrix, binPattern);

		_destroy(pApprxCoord);
		pApprxMatrixIter->next();
	}

	pApprxMatrixIter->toBegin();
}

void calculateBinsPattern(const real_coordinate_t& queryCoordinate, const real_coordinate_t& apprxCoordinate, BinPattern& binPattern) {
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

void findApprxByMerge2Bins(MatrixBinPtr pBin1, MatrixBinPtr pBin2, MatrixPtr pQuery, double epsilon, MatrixCombinerPtr pMatrixCombiner, MatrixDistanceCalculatorPtr pDistanceCalculator, MatrixCollectionPtr pResultCollection) {
	unsigned int sizeBin1 = pBin1->getElements().size();
	unsigned int sizeBin2 = pBin2->getElements().size();

	for(unsigned int i = 0; i < sizeBin1; i++) {
		for(unsigned int j = 0; j < sizeBin2; j++) {

			MatrixPtr pProduct1 = NullPtr;
			pMatrixCombiner->combine(pBin1->getElements()[i], pBin2->getElements()[j], pProduct1);

			if(pProduct1 != NullPtr && pDistanceCalculator->distance(pProduct1, pQuery) <= epsilon) {
				pResultCollection->addElement(pProduct1);
			}
			else {
				_destroy(pProduct1);
			}
		}
	}
}

void addApprxResultsToBufferFromCollection(MatrixCollectionPtr pCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon, MatrixPtrVector& rResultBuffer) {
	MatrixIteratorPtr pApprxResultIter = pCollection->findApproxElements(pQuery, pDistanceCalculator, epsilon);
	if(pApprxResultIter != NullPtr) {
		while(!pApprxResultIter->isDone()) {
			rResultBuffer.push_back(pApprxResultIter->getObj());
			pApprxResultIter->next();
		}
	}
}

#endif
