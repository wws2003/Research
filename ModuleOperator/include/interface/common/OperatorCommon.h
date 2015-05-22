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

class IMatrixCoordinateMapper;

template<typename T> class IDistanceCalculator;
typedef IDistanceCalculator<MatrixPtr> IMatrixDistanceCalculator;
typedef IDistanceCalculator<MatrixCoordinatePtr> IMatrixCoordinateDistanceCalculator;

template<typename T>
using DistanceCalculatorPtr = IDistanceCalculator<T>* ;

typedef IMatrixOperator* MatrixOperatorPtr;

typedef IMatrixDistanceCalculator* MatrixDistanceCalculatorPtr;

typedef IMatrixCoordinateDistanceCalculator* MatrixCoordinateDistanceCalculatorPtr;

typedef IMatrixCoordinateMapper* MatrixCoordinateMapperPtr;

#endif /* OPERATORCOMMON_H_ */
