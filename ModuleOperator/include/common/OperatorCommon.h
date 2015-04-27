/*
 * OperatorCommon.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef OPERATORCOMMON_H_
#define OPERATORCOMMON_H_

class IMatrixOperator;

class IMatrixDistanceCalculator;

class ITargetMatrixFactory;

class IMatrixVectorMapper;

typedef IMatrixOperator* MatrixOperatorPtr;

typedef IMatrixDistanceCalculator* MatrixDistanceCalculatorPtr;

typedef IMatrixVectorMapper* MatrixVectorMapperPtr;

#endif /* OPERATORCOMMON_H_ */
