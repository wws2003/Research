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

template<typename T>
class IReader;

typedef IWriter<MatrixPtr> IMatrixWriter;
typedef IWriter<GatePtr> IGateWriter;

typedef IReader<MatrixPtr> IMatrixReader;
typedef IReader<GatePtr> IGateReader;

template<typename T, typename F>
class ICoordinateWriter;

template<typename T>
using WriterPtr = IWriter<T>*;

template<typename T>
using ReaderPtr = IReader<T>*;

typedef IMatrixWriter* MatrixWriterPtr;
typedef IGateWriter* GateWriterPtr;

typedef IMatrixReader* MatrixReaderPtr;
typedef IGateReader* GateReaderPtr;

template<typename T, typename F>
using CoordinateWriterPtr = ICoordinateWriter<T,F>*;

template<typename T>
using RealCoordinateWriter = ICoordinateWriter<T, mreal_t>;

template<typename T>
using RealCoordinateWriterPtr = RealCoordinateWriter<T>*;

#endif /* IOCOMMON_H_ */
