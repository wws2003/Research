/*
 * IOCommon.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef IOCOMMON_H_
#define IOCOMMON_H_

class IMatrixWriter;
class IGateWriter;

template<typename T, typename F>
class ICoordinateWriter;

typedef IMatrixWriter* MatrixWriterPtr;
typedef IGateWriter* GateWriterPtr;

template<typename T, typename F>
using  CoordinateWriterPtr = ICoordinateWriter<T,F>*;

#endif /* IOCOMMON_H_ */
