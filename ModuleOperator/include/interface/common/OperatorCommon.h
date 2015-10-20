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

class ILibaryMatrixMatrixStore;

template<typename E, typename Field>
class ICoordinateCalculator;

template<typename T>
using IRealCoordinateCalculator = ICoordinateCalculator<T, mreal_t> ;

typedef ICoordinateCalculator<MatrixPtr, mreal_t> IMatrixRealCoordinateCalculator;
typedef IMatrixRealCoordinateCalculator* MatrixRealCoordinateCalculatorPtr;
typedef ICoordinateCalculator<GatePtr, mreal_t> IGateRealCoordinateCalculator;
typedef IGateRealCoordinateCalculator* GateRealCoordinateCalculatorPtr;

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
typedef IGateDistanceCalculator* GateDistanceCalculatorPtr;
typedef IMatrixRealCoordinateDistanceCalculator* MatrixRealCoordinateDistanceCalculatorPtr;

template<typename E, typename Field>
using InnerProductCalculatorPtr = IInnerProductCalculator<E, Field>*;

template<typename T>
using RealCoordinateCalculatorPtr = IRealCoordinateCalculator<T>*;

typedef IInnerProductCalculator<MatrixPtr, mreal_t> IMatrixRealInnerProductCalculator;
typedef IMatrixRealInnerProductCalculator* MatrixRealInnerProductCalculatorPtr;

typedef ILibaryMatrixMatrixStore* LibraryMatrixStorePtr;

#endif /* OPERATORCOMMON_H_ */
