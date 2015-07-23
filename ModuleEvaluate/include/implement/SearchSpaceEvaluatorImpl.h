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
			double epsilon,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
			RealCoordinateWriterPtr<T> pRealCoordinateWritter,
			WriterPtr<T> pWriter,
			TimerPtr pTimer,
			std::ostream& outputStream);

	virtual ~SearchSpaceTimerEvaluatorImpl(){};

	/**
	 *Evaluate given matrix/gate collection, possibly about running time to find approximation, etc.
	 */
	virtual void evaluateCollection(CollectionPtr<T> pCollection);

	/**
	 *Evaluate given matrix/gate approximator, possibly about running time to get approximation, etc.
	 */
	virtual void evaluateApproximator(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCoreCollection);

private:
	TargetElements<T> m_targets;
	double m_epsilon;

	DistanceCalculatorPtr<T> m_pDistanceCalculator;
	RealCoordinateCalculatorPtr<T> m_pRealCoordinateCalculator;
	RealCoordinateWriterPtr<T> m_pRealCoordinateWritter;
	TimerPtr m_pTimer;
	WriterPtr<T> m_pWriter;

	std::ostream& m_ostream;
};


#endif /* SEARCHSPACETIMEREVALUATORIMPL_H_ */
