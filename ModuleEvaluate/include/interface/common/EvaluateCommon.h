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

template<typename T>
class IComposerEvaluator;

typedef IComposerEvaluator<MatrixPtr> MatrixComposerEvaluator;
typedef IComposerEvaluator<GatePtr> GateComposerEvaluator;

typedef ITimer* TimerPtr;

template<typename T>
using SearchSpaceConstructorPtr = ISearchSpaceConstructor<T>*;

template<typename T>
using SearchSpaceEvaluatorPtr = ISearchSpaceEvaluator<T>*;

template<typename T>
using ComposerEvaluatorPtr = IComposerEvaluator<T>*;

typedef MatrixSearchSpaceConstructor* MatrixSearchSpaceConstructorPtr;
typedef GateSearchSpaceConstructor* GateSearchSpaceConstructorPtr;

typedef MatrixSearchSpaceEvaluator* MatrixSearchSpaceEvaluatorPtr;
typedef GateSearchSpaceEvaluator* GateSearchSpaceEvaluatorPtr;

typedef MatrixComposerEvaluator* MatrixComposerEvaluatorPtr;
typedef GateComposerEvaluator* GateComposerEvaluatorPtr;

#endif /* EVALUATECOMMON_H_ */
