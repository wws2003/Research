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
#include "mpreal.h"

#define MPFR_REAL 1

#if MPFR_REAL
typedef mpfr::mpreal mreal_t;
#define REAL_IO_PRECISION 32
#else
typedef double mreal_t;
#endif

typedef std::complex<mreal_t> ComplexVal;

namespace mreal {

inline void initPrecision() {
#if MPFR_REAL
	mpfr::mpreal::set_default_prec(256);
#else
	//DO nothing
#endif
}

inline mreal_t abs(const mreal_t& v) {
#if MPFR_REAL
	return mpfr::abs(v);
#else
	return std::abs(v);
#endif
}

mreal_t norm(const ComplexVal& v);
}
// namespace mreal

typedef ComplexVal* ComplexValArray;
typedef ComplexValArray& ComplexValArrayRef;

#define NullPtr NULL
#define _destroy(a) if(a != NULL)delete a

typedef ComplexVal& ComplexValRef;

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
