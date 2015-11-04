/*
 * Common.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <complex>
#include <vector>
#include "Mreal.h"

typedef ComplexVal* ComplexValArray;
typedef ComplexValArray& ComplexValArrayRef;

#define NullPtr NULL
#define _destroy(a) if(a != NULL)delete a

typedef ComplexVal& ComplexValRef;

typedef std::vector<std::string> LabelSeq;

class IMatrix;
class IMatrixFactory;
class Gate;

enum ArraySpliceType {
	ROW_SPLICE,
	COLUMN_SPLICE
};

template<typename T,typename F> class Coordinate;

typedef IMatrix* MatrixPtr;
typedef MatrixPtr& MatrixPtrRef;

typedef std::vector<ComplexVal> ComplexVector;
typedef ComplexVector& ComplexVectorRef;

typedef IMatrixFactory* MatrixFactoryPtr;

template<typename T>
using RealCoordinate = Coordinate<T, mreal_t>;

typedef std::vector<mreal_t> real_coordinate_t;

template<typename T>
using RealCoordinatePtr = RealCoordinate<T>*;

typedef Coordinate<MatrixPtr, mreal_t> MatrixRealCoordinate;
typedef MatrixRealCoordinate* MatrixRealCoordinatePtr;
typedef MatrixRealCoordinatePtr& MatrixCoordinatePtrRef;

typedef std::vector<MatrixPtr> MatrixPtrVector;
typedef MatrixPtrVector* MatrixPtrVectorPtr;

template<typename T,typename F>
using CoordinatePtr = Coordinate<T, F>*;

template<typename T,typename F>
using CoordinatePtrRef = CoordinatePtr<T, F>&;

typedef Gate* GatePtr;
typedef Coordinate<GatePtr, mreal_t> GateRealCoordinate;
typedef GateRealCoordinate* GateRealCoordinatePtr;
typedef GateRealCoordinatePtr& GateRealCoordinatePtrRef;

typedef std::vector<GatePtr> GatePtrVector;
typedef GatePtrVector* GatePtrVectorPtr;

#endif /* COMMON_H_ */
