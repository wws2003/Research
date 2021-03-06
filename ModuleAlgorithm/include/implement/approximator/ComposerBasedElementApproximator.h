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
#include "ILookupResultFilter.h"

template<typename T>
class ComposerBasedElementApproximator : public IApproximator<T> {
public:
	ComposerBasedElementApproximator(DecomposerPtr<T> pQueryDecomposer,
			int nbPartialQueries,
			int buildingBlocksBucketMaxSize,
			ComposerPtr<T> pBuildingBlockComposer,
			mreal_t initialEpsilon,
			LookupResultFilterPtr<T> pLookupResultFilter);

	virtual ~ComposerBasedElementApproximator(){};

	//Override
	virtual IteratorPtr<LookupResult<T> > getApproximateElements(CollectionPtr<T> pCoreCollection,
			T pQuery,
			mreal_t epsilon);

private:
	void decomposeQueryIntoBuildingBlocksBuckets(CollectionPtr<T> pCoreCollection,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			BuildingBlockBuckets<T>& buildingBlockBuckets);

	IteratorPtr<LookupResult<T> > getApproximateElementsForPartialQuery(CollectionPtr<T> pCoreCollection,
			T pPartialQuery,
			mreal_t epsilon);

	void releaseBuildingBlocksBuckets(BuildingBlockBuckets<T>& buildingBlockBuckets);

	IteratorPtr<T> getExtractedElementIterator(IteratorPtr<LookupResult<T> > pLookupResultIter,
			combination_counter_t* pCombinationCounter = NULL);

	IteratorPtr<LookupResult<T> > getFullResultIterator(IteratorPtr<LookupResult<T> > pResultIter, DistanceCalculatorPtr<T> pDistanceCalculator);

	DecomposerPtr<T> m_pQueryDecomposer;
	int m_nbPartialQueries;
	int m_buildingBlocksBucketMaxSize;
	ComposerPtr<T> m_pBuildingBlockComposer;
	mreal_t m_initialEpsilon;
	LookupResultFilterPtr<T> m_pLookupResultFilter;
};



#endif /* COMPOSERBASEDELEMENTAPPROXIMATOR_H_ */
