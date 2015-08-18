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
typedef std::vector<double> real_coordinate_t;

template<typename T>
class NearIdentityElementApproximator: public IApproximator<T> {
public:
	typedef struct Config_ {
		double m_initialEpsilon;
		int m_maxMergedBinDistance;
		double m_maxCandidateEpsilon;
		int m_iterationMaxSteps;
	} Config;

	NearIdentityElementApproximator(RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
			CombinerPtr<T> pCombiner,
			BinCollectionPtr<T> pBinCollection,
			const Config& config);

	virtual ~NearIdentityElementApproximator(){};
	virtual IteratorPtr<T> getApproximateElements(CollectionPtr<T> pCoreCollection, T pQuery, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon);

private:
	void initBinCollection(CollectionPtr<T> pCoreCollection,
			T pQuery,
			const real_coordinate_t& queryCoordinate,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			double epsilon);

	void generateApproximationsFromBins(T pQuery,
			const real_coordinate_t& queryCoordinate,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			double epsilon,
			std::vector<T>& apprxResultBuffer);

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
