/*
 * ComposerEvaluatorImpl.h
 *
 *  Created on: Feb 3, 2016
 *      Author: pham
 */

#ifndef COMPOSEREVALUATORIMPL_H_
#define COMPOSEREVALUATORIMPL_H_

#include "IComposerEvaluator.h"
#include "ITimer.h"
#include "EvaluateCommon.h"
#include "IDecomposer.h"
#include "IDistanceCalculator.h"
#include "IIterator.h"
#include "ICollection.h"
#include "IOCommon.h"
#include "IDecomposer.h"
#include <vector>
#include <ostream>

template<typename T>
class ComposerEvaluatorImpl: public IComposerEvaluator<T> {
public:
	ComposerEvaluatorImpl(const std::vector<T> targets,
			int nbPartialElements,
			int buildingBlocksBucketMaxSize,
			CollectionPtr<T> pCollection,
			CollectionPtr<T> pHelperCollection,
			DecomposerPtr<T> pDecomposer,
			mreal_t epsilon,
			mreal_t initialEpsilon,
			TimerPtr pTimer,
			RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
			RealCoordinateWriterPtr<T> pRealCoordinateWritter,
			WriterPtr<T> pWriter,
			std::ostream& ostream);

	virtual ~ComposerEvaluatorImpl(){};

	//Override
	void evaluateComposers(ComposerPtr<T> pEvaluateComposer,
			ComposerPtr<T> pStandardComposer);

private:
	struct ComposerEvaluateFactors {
		combination_counter_t m_nbResults = 0;
		mreal_t m_bestDistance;
		double m_composeTime;

		void printInfo(std::ostream& ostream, std::string composerName);

		void printCompareToInfo(std::ostream& ostream, const ComposerEvaluateFactors& evalFactors2);
	};

	void logTarget(T target);

	void evaluateComposerForTarget(ComposerPtr<T> pEvaluateComposer,
			ComposerPtr<T> pStandardComposer,
			T target);

	void decomposeQueryIntoBuildingBlocksBuckets(T target,
			BuildingBlockBuckets<T>& buildingBlockBuckets);

	IteratorPtr<T> getExtractedElementIterator(IteratorPtr<LookupResult<T> > pLookupResultIter,
			combination_counter_t* pCombinationCounter = NULL);

	void evaluateComposer(BuildingBlockBuckets<T>& buildingBlockBuckets,
			ComposerPtr<T> pComposer,
			T target,
			ComposerEvaluateFactors* pComposerEvaluateFactor);

	void processFoundResults(IteratorPtr<LookupResult<T> > prFindIter,
			ComposerEvaluateFactors* pComposerEvaluateFactor);

	void logSearchResult(LookupResult<T> result,
			double searchTime);

	void releaseBuildingBlocksBuckets(BuildingBlockBuckets<T>& buildingBlockBuckets);
	void releaseResultIter(IteratorPtr<LookupResult<T> >& pFindResultIter);

	void releaseIter(IteratorPtr<T>& pFindIter);

	std::vector<T> m_targets;
	int m_nbPartialElements;
	int m_buildingBlocksBucketMaxSize;
	CollectionPtr<T> m_pCollection;
	CollectionPtr<T> m_pHelperCollection;
	DecomposerPtr<T> m_pDecomposer;
	mreal_t m_epsilon;
	mreal_t m_initialEpsilon;
	TimerPtr m_pTimer;

	RealCoordinateCalculatorPtr<T> m_pRealCoordinateCalculator;
	RealCoordinateWriterPtr<T> m_pRealCoordinateWriter;
	WriterPtr<T> m_pWriter;
	std::ostream& m_ostream;
};


#endif /* COMPOSEREVALUATORIMPL_H_ */
