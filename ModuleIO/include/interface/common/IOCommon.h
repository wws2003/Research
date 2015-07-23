/*
 * IOCommon.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef IOCOMMON_H_
#define IOCOMMON_H_

#include "MathConceptsCommon.h"

template<typename T>
class IWriter;

typedef IWriter<MatrixPtr> IMatrixWriter;
typedef IWriter<GatePtr> IGateWriter;

template<typename T, typename F>
class ICoordinateWriter;

template<typename T>
using WriterPtr = IWriter<T>*;

typedef IMatrixWriter* MatrixWriterPtr;
typedef IGateWriter* GateWriterPtr;

template<typename T, typename F>
using CoordinateWriterPtr = ICoordinateWriter<T,F>*;

template<typename T>
using RealCoordinateWriter = ICoordinateWriter<T,double>;

template<typename T>
using RealCoordinateWriterPtr = RealCoordinateWriter<T>*;

#endif /* IOCOMMON_H_ */
