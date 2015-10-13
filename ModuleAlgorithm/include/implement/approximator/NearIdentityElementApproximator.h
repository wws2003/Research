/*
 * NearIdentityElementApproximator.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef NEARIDENTITYELEMENTAPPROXIMATOR_H_
#define NEARIDENTITYELEMENTAPPROXIMATOR_H_

#include "OperatorCommon.h"
#include "ICollection.h"
#include "ICombiner.h"
#include "AlgoInternal.h"
#include "IApproximator.h"
#include "OperatorCommon.h"

/**
 * This generic class aims to serve both gate and matrix approximator
 */
typedef std::vector<mreal_t> real_coordinate_t;

template<typename T>
using ApprxResultBuffer = std::vector<T>;

template<typename T>
class NearIdentityElementApproximator: public IApproximator<T> {
public:
	typedef struct Config_ {
		mreal_t m_initialEpsilon;
		int m_maxMergedBinDistance;
		mreal_t m_maxCandidateEpsilon;
		mreal_t m_maxCandidateEpsilonDecreaseFactor;
		int m_iterationMaxSteps;
		int m_maxResultNumber;
	} Config;

	NearIdentityElementApproximator(RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
			CombinerPtr<T> pCombiner,
			BinCollectionPtr<T> pBinCollection,
			const Config& config);

	virtual ~NearIdentityElementApproximator(){};
	virtual IteratorPtr<T> getApproximateElements(CollectionPtr<T> pCoreCollection, T pQuery, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t epsilon);

private:
	void initBinCollection(CollectionPtr<T> pCoreCollection,
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
			BinCollectionPtr<T> pBinCollection);

	void calculateBinPattern(const real_coordinate_t& queryCoordinate,
			const real_coordinate_t& apprxCoordinate,
			BinPattern& binPattern);

	RealCoordinateCalculatorPtr<T> m_pRealCoordinateCalculator;
	CombinerPtr<T> m_pCombiner;
	BinCollectionPtr<T> m_pBinCollection;

	Config m_config;
};

#endif /* NEARIDENTITYELEMENTAPPROXIMATOR_H_ */
