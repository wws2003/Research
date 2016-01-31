/*
 * SearchSpaceTimerEvaluatorImpl.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef SEARCHSPACETIMEREVALUATORIMPL_H_
#define SEARCHSPACETIMEREVALUATORIMPL_H_

#include "ISearchSpaceEvaluator.h"
#include "EvaluateCommon.h"
#include "OperatorCommon.h"
#include "IOCommon.h"
#include <ostream>
#include <vector>

#ifndef TargetElements
template<typename T>
using TargetElements = std::vector<T>;
#endif

template<typename T>
class SearchSpaceTimerEvaluatorImpl : public ISearchSpaceEvaluator<T> {
public:
	SearchSpaceTimerEvaluatorImpl(const TargetElements<T>& pTargets,
			mreal_t epsilon,
			mreal_t approximatorEpsilon,
			RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
			RealCoordinateWriterPtr<T> pRealCoordinateWritter,
			WriterPtr<T> pWriter,
			TimerPtr pTimer,
			std::ostream& outputStream);

	virtual ~SearchSpaceTimerEvaluatorImpl(){};

	/**
	 * Override
	 *Evaluate given matrix/gate collection, possibly about running time to find approximation, etc.
	 */
	virtual void evaluateCollection(CollectionPtr<T> pCollection);

	/**
	 * Overide
	 *Evaluate given matrix/gate approximator, possibly about running time to get approximation, etc.
	 */
	virtual void evaluateApproximator(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCoreCollection);

private:
	void releaseResultIter(IteratorPtr<LookupResult<T> >& pFindResultIter);

	TargetElements<T> m_targets;
	mreal_t m_collectionEpsilon;
	mreal_t m_approximatorEpsilon;

	RealCoordinateCalculatorPtr<T> m_pRealCoordinateCalculator;
	RealCoordinateWriterPtr<T> m_pRealCoordinateWritter;
	TimerPtr m_pTimer;
	WriterPtr<T> m_pWriter;

	std::ostream& m_ostream;
};


#endif /* SEARCHSPACETIMEREVALUATORIMPL_H_ */
