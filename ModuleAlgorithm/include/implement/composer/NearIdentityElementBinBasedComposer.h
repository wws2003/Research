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

	NearIdentityElementBinBasedComposer(CombinerPtr<T> pCombiner,
			BinCollectionPtr<T> pBinCollection,
			const Config& config);

	virtual ~NearIdentityElementBinBasedComposer(){};

	//Override
	virtual IteratorPtr<LookupResult<T> > composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			bool toSortResults = true);

private:
	void initBinCollection(IteratorPtr<T> pBuildingBlockIter,
			T pQuery,
			mreal_t epsilon);

	void generateApproximationsFromBins(T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			ApprxResultBuffer<LookupResult<T> >& apprxResultBuffer);

	void generateApproximationsFromCombinableBins(const BinPtrVector<T>&  combinableBins,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilonForMergeCandidate,
			mreal_t approximationEpsilon,
			CollectionPtr<T> pApprxTempCollection,
			ApprxResultBuffer<LookupResult<T> >& apprxResultBuffer,
			int maxResultsNumber);

	void getApproximationsFromCombinableBins(T element,
			const BinPtrVector<T>& combinableBins,
			int binCounter,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilonForMergeCandidate,
			mreal_t approximationEpsilon,
			CollectionPtr<T> pApprxTempCollection,
			ApprxResultBuffer<LookupResult<T> >& apprxResultBuffer,
			int maxResultsNumber);

	void distributeResultsToBins(IteratorPtr<T> pApprxIter,
			BinCollectionPtr<T> pBinCollection,
			bool toDestroyApprxIter = true);

	CombinerPtr<T> m_pCombiner;
	BinCollectionPtr<T> m_pBinCollection;

	Config m_config;
};


#endif /* NEARIDENTITYELEMENTCOMPOSER_H_ */
