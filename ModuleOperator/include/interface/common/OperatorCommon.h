/*
 * OperatorCommon.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef OPERATORCOMMON_H_
#define OPERATORCOMMON_H_

class IMatrixOperator;

class ITargetMatrixFactory;

class IMatrixVectorMapper;

template<typename T> class IDistanceCalculator;
typedef IDistanceCalculator<MatrixPtr> IMatrixDistanceCalculator;

template<typename T>
using DistanceCalculatorPtr = IDistanceCalculator<T>* ;

typedef IMatrixOperator* MatrixOperatorPtr;

typedef IMatrixDistanceCalculator* MatrixDistanceCalculatorPtr;

typedef IMatrixVectorMapper* MatrixVectorMapperPtr;

#endif /* OPERATORCOMMON_H_ */
