/*
 * SKElementApproximator.h
 *
 *  Created on: Nov 16, 2015
 *      Author: pham
 */

#ifndef SKELEMENTAPPROXIMATOR_H_
#define SKELEMENTAPPROXIMATOR_H_

#include "AlgoCommon.h"
#include "IApproximator.h"
#include "IComposer.h"
#include "IDecomposer.h"
#include "IApproximator.h"

template<typename T>
using IteratorVector = std::vector<IteratorPtr<T> >;

template<typename T>
class SKElementApproximator : public IApproximator<T> {
public:
	SKElementApproximator(DecomposerPtr<T> pQueryDecomposer,
			ComposerPtr<T> pBuildingBlockComposer,
			mreal_t initialEpsilon,
			int nbCandidates,
			int recursiveLevel);

	virtual ~SKElementApproximator();

	//Override
	virtual IteratorPtr<T> getApproximateElements(CollectionPtr<T> pCoreCollection,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon);

protected:
	virtual void calculateResidual(T apprx, T query, T& residual);

private:
	void initEpsilonForLevels();

	void resetEpsilonForLevels(mreal_t epsilon);

	IteratorPtr<T> skApproximate(CollectionPtr<T> pCoreCollection,
			T pPartialQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			int level);

	IteratorPtr<T> getCandidatesFromRawApprx(T apprx,
			CollectionPtr<T> pCoreCollection,
			T pPartialQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			int level);

	IteratorPtr<T> filterCandidates(const IteratorVector<T>& buildingBlockBuckets,
				T query,
				DistanceCalculatorPtr<T> pDistanceCalculator,
				mreal_t epsilon);

	void addBuildingBlocksBucketsForResidual(T residual,
			CollectionPtr<T> pCoreCollection,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			BuildingBlockBuckets<T>& buildingBlockBuckets,
			int level);

	void releaseIterators(std::vector<IteratorPtr<T> >& buildingBlockBuckets);

	void releaseElementsInIterator(IteratorPtr<T> pIter);

	DecomposerPtr<T> m_pQueryDecomposer;
	ComposerPtr<T> m_pBuildingBlockComposer;
	mreal_t m_initialEpsilon;

	int m_nbCandidates;
	int m_recursiveLevel;
	mreal_t* m_epsilonForLevels;
};



#endif /* SKELEMENTAPPROXIMATOR_H_ */
