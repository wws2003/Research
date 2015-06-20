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

class ISearchSpaceEvaluator;

typedef ITimer* TimerPtr;

template<typename T>
using SearchSpaceConstructorPtr = ISearchSpaceConstructor<T>*;

typedef ISearchSpaceEvaluator* SearchSpaceEvaluatorPtr;
typedef MatrixSearchSpaceConstructor* MatrixSearchSpaceConstructorPtr;
typedef GateSearchSpaceConstructor* GateSearchSpaceConstructorPtr;

#endif /* EVALUATECOMMON_H_ */
