/*
 * EvaluateCommon.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef EVALUATECOMMON_H_
#define EVALUATECOMMON_H_

#include "MathConceptsCommon.h"

class ITimer;

template<typename T>
class ISearchSpaceConstructor;

typedef ISearchSpaceConstructor<MatrixPtr> MatrixSearchSpaceConstructor;
typedef ISearchSpaceConstructor<GatePtr> GateSearchSpaceConstructor;

template<typename T>
class ISearchSpaceEvaluator;

typedef ISearchSpaceEvaluator<MatrixPtr> MatrixSearchSpaceEvaluator;
typedef ISearchSpaceEvaluator<GatePtr> GateSearchSpaceEvaluator;


typedef ITimer* TimerPtr;

template<typename T>
using SearchSpaceConstructorPtr = ISearchSpaceConstructor<T>*;

template<typename T>
using SearchSpaceEvaluatorPtr = ISearchSpaceEvaluator<T>*;

typedef MatrixSearchSpaceConstructor* MatrixSearchSpaceConstructorPtr;
typedef GateSearchSpaceConstructor* GateSearchSpaceConstructorPtr;

typedef MatrixSearchSpaceEvaluator* MatrixSearchSpaceEvaluatorPtr;
typedef GateSearchSpaceEvaluator* GateSearchSpaceEvaluatorPtr;

#endif /* EVALUATECOMMON_H_ */
