/*
 * OperatorCommon.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef OPERATORCOMMON_H_
#define OPERATORCOMMON_H_

#include "MathConceptsCommon.h"

class IMatrixOperator;

class ITargetMatrixFactory;

template<typename E, typename Field>
class ICoordinateCalculator;

typedef ICoordinateCalculator<MatrixPtr, double> IMatrixRealCoordinateCalculator;
typedef IMatrixRealCoordinateCalculator* MatrixRealCoordinateCalculatorPtr;

template<typename T> class IDistanceCalculator;
typedef IDistanceCalculator<MatrixPtr> IMatrixDistanceCalculator;
typedef IDistanceCalculator<MatrixRealCoordinatePtr> IMatrixRealCoordinateDistanceCalculator;

template<typename T>
using DistanceCalculatorPtr = IDistanceCalculator<T>* ;

template<typename E, typename Field>
class IInnerProductCalculator;

typedef IMatrixOperator* MatrixOperatorPtr;

typedef IMatrixDistanceCalculator* MatrixDistanceCalculatorPtr;

typedef IMatrixRealCoordinateDistanceCalculator* MatrixRealCoordinateDistanceCalculatorPtr;

typedef IMatrixRealCoordinateCalculator* MatrixRealCoordinateCalculatorPtr;

template<typename E, typename Field>
using InnerProductCalculatorPtr = IInnerProductCalculator<E, Field>*;

typedef IInnerProductCalculator<MatrixPtr, double> IMatrixRealInnerProductCalculator;
typedef IMatrixRealInnerProductCalculator* MatrixRealInnerProductCalculatorPtr;
#endif /* OPERATORCOMMON_H_ */
