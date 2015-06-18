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

/*template<typename T>
typedef ICoordinateCalculator<T, double> IRealCoordinateCalculator;*/

typedef ICoordinateCalculator<MatrixPtr, double> IMatrixRealCoordinateCalculator;
typedef IMatrixRealCoordinateCalculator* MatrixRealCoordinateCalculatorPtr;
typedef ICoordinateCalculator<GatePtr, double> IGateRealCoordinateCalculator;
typedef IGateRealCoordinateCalculator* IGateRealCoordinateCalculatorPtr;

template<typename T> class IDistanceCalculator;
typedef IDistanceCalculator<MatrixPtr> IMatrixDistanceCalculator;
typedef IDistanceCalculator<MatrixRealCoordinatePtr> IMatrixRealCoordinateDistanceCalculator;
typedef IDistanceCalculator<GatePtr> IGateDistanceCalculator;
typedef IDistanceCalculator<GateRealCoordinatePtr> IGateRealCoordinateDistanceCalculator;

template<typename T>
using DistanceCalculatorPtr = IDistanceCalculator<T>* ;

template<typename E, typename Field>
class IInnerProductCalculator;

typedef IMatrixOperator* MatrixOperatorPtr;

typedef IMatrixDistanceCalculator* MatrixDistanceCalculatorPtr;

typedef IMatrixRealCoordinateDistanceCalculator* MatrixRealCoordinateDistanceCalculatorPtr;

template<typename E, typename Field>
using InnerProductCalculatorPtr = IInnerProductCalculator<E, Field>*;

/*template<typename T>
using RealCoordinateCalculatorPtr = IRealCoordinateCalculator<T>*;*/

typedef IInnerProductCalculator<MatrixPtr, double> IMatrixRealInnerProductCalculator;
typedef IMatrixRealInnerProductCalculator* MatrixRealInnerProductCalculatorPtr;
#endif /* OPERATORCOMMON_H_ */
