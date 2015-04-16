/*
 * Common.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <complex>

/*typedef struct ComplexVal {
	double real;
	double imf;
}
ComplexVal;*/

typedef std::complex<double> ComplexVal;
typedef ComplexVal* ComplexValArray;
typedef ComplexValArray& ComplexValArrayRef;

#define NullPtr NULL
#define _destroy(a) delete a

typedef ComplexVal& ComplexValRef;

class IMatrix;
class IVector;
class ISquareMatrix;
template<typename T> class IIterator;

class IMatrixOperator;

class IMatrixDistanceCalculator;

class IMatrixCollection;
class IMatrixCollectionFactory;

class IPersistableMatrixCollection;
class IPersistableMatrixCollectionFactory;

class ITargetMatrixFactory;

class IMatrixWriter;

class ITimer;

class IMatrixVectorMapper;

typedef IMatrix* MatrixPtr;
typedef MatrixPtr& MatrixPtrRef;

typedef IVector* VectorPtr;
typedef VectorPtr& VectorPtrRef;

typedef ISquareMatrix* SquareMatrixPtr;
typedef ISquareMatrix& SquareMatrixPtrRef;

typedef IIterator<MatrixPtr>* MatrixIteratorPtr;
typedef IIterator<VectorPtr>* VectorItertatorPtr;

typedef IMatrixOperator* MatrixOperatorPtr;

typedef IMatrixDistanceCalculator* MatrixDistanceCalculatorPtr;

typedef IMatrixCollection* MatrixCollectionPtr;

typedef IPersistableMatrixCollection* PersistableMatrixCollectionPtr;

typedef long long MatrixCollectionSize_t;

typedef IPersistableMatrixCollectionFactory* PersistableMatrixCollectionFactoryPtr;

typedef IMatrixCollectionFactory* MatrixCollectionFactoryPtr;

typedef ITargetMatrixFactory* TargetMatrixFactoryPtr;

typedef IMatrixWriter* MatrixWriterPtr;

typedef ITimer* TimerPtr;

typedef IMatrixVectorMapper* MatrixVectorMapperPtr;

#endif /* COMMON_H_ */
