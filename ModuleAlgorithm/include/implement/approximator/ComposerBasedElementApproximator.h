/*
 * ComposerBasedElementApproximator.h
 *
 *  Created on: Oct 18, 2015
 *      Author: pham
 */

#ifndef COMPOSERBASEDELEMENTAPPROXIMATOR_H_
#define COMPOSERBASEDELEMENTAPPROXIMATOR_H_

#include "AlgoCommon.h"
#include "IApproximator.h"
#include "IComposer.h"
#include "IDecomposer.h"

template<typename T>
class ComposerBasedElementApproximator : public IApproximator<T> {
public:
	ComposerBasedElementApproximator(DecomposerPtr<T> pQueryDecomposer,
			int nbPartialQueries,
			ComposerPtr<T> pBuildingBlockComposer,
			mreal_t initialEpsilon);

	virtual ~ComposerBasedElementApproximator(){};

	//Override
	virtual IteratorPtr<T> getApproximateElements(CollectionPtr<T> pCoreCollection,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon);

private:
	void decomposeQueryIntoBuildingBlocksBuckets(CollectionPtr<T> pCoreCollection,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			BuildingBlockBuckets<T>& buildingBlockBuckets);

	IteratorPtr<T> getApproximateElementsForPartialQuery(CollectionPtr<T> pCoreCollection,
			T pPartialQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon);

	void releaseBuildingBlocksBuckets(BuildingBlockBuckets<T>& buildingBlockBuckets);

	DecomposerPtr<T> m_pQueryDecomposer;
	int m_nbPartialQueries;
	ComposerPtr<T> m_pBuildingBlockComposer;
	mreal_t m_initialEpsilon;
};



#endif /* COMPOSERBASEDELEMENTAPPROXIMATOR_H_ */
