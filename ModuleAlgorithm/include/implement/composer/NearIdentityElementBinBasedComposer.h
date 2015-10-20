/*
 * NearIdentityElementComposer.h
 *
 *  Created on: Oct 18, 2015
 *      Author: pham
 */

#ifndef NEARIDENTITYELEMENTBINBASEDCOMPOSER_H_
#define NEARIDENTITYELEMENTBINBASEDCOMPOSER_H_

#include "IComposer.h"
#include "Bin.hpp"
#include "IIterator.h"

typedef std::vector<mreal_t> real_coordinate_t;

template<typename T>
using ApprxResultBuffer = std::vector<T>;

template<typename T>
class NearIdentityElementBinBasedComposer: public IComposer<T> {
public:
	typedef struct Config_ {
		int m_maxMergedBinDistance;
		mreal_t m_maxCandidateEpsilon;
		mreal_t m_maxCandidateEpsilonDecreaseFactor;
		int m_iterationMaxSteps;
		int m_maxResultNumber;
	} Config;

	NearIdentityElementBinBasedComposer(RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
			CombinerPtr<T> pCombiner,
			BinCollectionPtr<T> pBinCollection,
			const Config& config);

	virtual ~NearIdentityElementBinBasedComposer(){};

	//Override
	virtual IteratorPtr<T> composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon);

private:
	void initBinCollection(IteratorPtr<T> pBuildingBlockIter,
			T pQuery,
			const real_coordinate_t& queryCoordinate,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon);

	void generateApproximationsFromBins(T pQuery,
			const real_coordinate_t& queryCoordinate,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			ApprxResultBuffer<T>& apprxResultBuffer);

	void generateApproximationsPrefixedFromBins(BinPtr<T> pBin,
			int maxBinDistance,
			T pQuery,
			const real_coordinate_t& queryCoordinate,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilonForMergeCandidate,
			mreal_t approximationEpsilon,
			CollectionPtr<T> pApprxTempCollection,
			ApprxResultBuffer<T>& apprxResultBuffer,
			int maxResultsNumber);

	void distributeResultsToBins(const real_coordinate_t& queryCoordinate,
			IteratorPtr<T> pApprxIter,
			RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
			BinCollectionPtr<T> pBinCollection,
			bool toDestroyApprxIter = true);

	void calculateBinPattern(const real_coordinate_t& queryCoordinate,
			const real_coordinate_t& apprxCoordinate,
			BinPattern& binPattern);

	RealCoordinateCalculatorPtr<T> m_pRealCoordinateCalculator;
	CombinerPtr<T> m_pCombiner;
	BinCollectionPtr<T> m_pBinCollection;

	Config m_config;
};


#endif /* NEARIDENTITYELEMENTCOMPOSER_H_ */
