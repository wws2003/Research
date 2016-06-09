/*
 * FullComposerEvaluatorImpl.h
 *
 *  Created on: Jun 7, 2016
 *      Author: pham
 */

#ifndef SELINGERCOMPOSEREVALUATORIMPL_H_
#define SELINGERCOMPOSEREVALUATORIMPL_H_

#include "IComposerEvaluator.h"
#include "ITimer.h"
#include "EvaluateCommon.h"
#include "IDecomposer.h"
#include "IDistanceCalculator.h"
#include "IIterator.h"
#include "ICollection.h"
#include "IOCommon.h"
#include "ICombiner.h"
#include "AlgoCommon.h"
#include <vector>
#include <ostream>

template<typename T>
class SelingerComposerEvaluatorImpl: public IComposerEvaluator<T> {
public:
	SelingerComposerEvaluatorImpl(const std::vector<std::vector<T> > partialTargets,
			const std::vector<T> modifiers,
			int buildingBlocksBucketMaxSize,
			mreal_t epsilon,
			mreal_t initialEpsilon,
			TimerPtr pTimer,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			CombinerPtr<T> pCombiner,
			RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
			RealCoordinateWriterPtr<T> pRealCoordinateWritter,
			WriterPtr<T> pWriter,
			std::ostream& ostream);

	virtual ~SelingerComposerEvaluatorImpl(){};

	//Override
	void evaluateComposers(ComposerPtr<T> pEvaluateComposer,
			ComposerPtr<T> pStandardComposer);

protected:
	void applyModifier(T element, T modifier, T& result);

	virtual void generateBuildingBlockBuckets(const std::vector<T>& partials,
			T modifier,
			BuildingBlockBuckets<T>& buildingBlockBuckets) = 0;

	mreal_t m_initialEpsilon;
	int m_buildingBlocksBucketMaxSize;

private:
	struct ComposerEvaluateFactors {
		combination_counter_t m_nbResults = 0;
		mreal_t m_bestDistance;
		double m_composeTime;

		void printInfo(std::ostream& ostream, std::string composerName);

		void printCompareToInfo(std::ostream& ostream, const ComposerEvaluateFactors& evalFactors2);
	};

	void generateTarget(const std::vector<T>& partials, T modifier, T& target);

	void logTarget(T target);

	void evaluateBestEffortPartialResults(BuildingBlockBuckets<T>& buildingBlockBuckets, T modifier, T target);

	void evaluateComposerForTarget(ComposerPtr<T> pEvaluateComposer,
			ComposerPtr<T> pStandardComposer,
			BuildingBlockBuckets<T>& buildingBlockBuckets,
			T target);

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

	std::vector<std::vector<T> > m_partialTargets;
	std::vector<T> m_modifiers;

	mreal_t m_epsilon;

	TimerPtr m_pTimer;

	DistanceCalculatorPtr<T> m_pDistanceCalculator;
	CombinerPtr<T> m_pCombiner;
	RealCoordinateCalculatorPtr<T> m_pRealCoordinateCalculator;
	RealCoordinateWriterPtr<T> m_pRealCoordinateWriter;
	WriterPtr<T> m_pWriter;

	std::ostream& m_ostream;
};




#endif /* FULLCOMPOSEREVALUATORIMPL_H_ */
