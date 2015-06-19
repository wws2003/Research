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

/*typedef struct ComplexVal {
	double real;
	double imf;
}
ComplexVal;*/

typedef std::complex<double> ComplexVal;
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
using RealCoordinate = Coordinate<T, double>;

template<typename T>
using RealCoordinatePtr = RealCoordinate<T>*;

typedef Coordinate<MatrixPtr, double> MatrixRealCoordinate;
typedef MatrixRealCoordinate* MatrixRealCoordinatePtr;
typedef MatrixRealCoordinatePtr& MatrixCoordinatePtrRef;

typedef std::vector<MatrixPtr> MatrixPtrVector;
typedef MatrixPtrVector* MatrixPtrVectorPtr;

template<typename T,typename F>
using CoordinatePtr = Coordinate<T, F>*;

template<typename T,typename F>
using CoordinatePtrRef = CoordinatePtr<T, F>&;

typedef Gate* GatePtr;
typedef Coordinate<GatePtr, double> GateRealCoordinate;
typedef GateRealCoordinate* GateRealCoordinatePtr;

typedef std::vector<GatePtr> GatePtrVector;
typedef GatePtrVector* GatePtrVectorPtr;

#endif /* COMMON_H_ */
