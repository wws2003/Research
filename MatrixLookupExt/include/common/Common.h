/*
 * Common.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef COMMON_H_
#define COMMON_H_

typedef struct ComplexVal {
	double real;
	double imf;
}
ComplexVal;

#define NullPtr NULL
#define _destroy(a) delete a

typedef ComplexVal& ComplexValRef;

class IMatrix;

typedef IMatrix* MatrixPtr;
typedef MatrixPtr& MatrixPtrRef;

class ISquareMatrix;

typedef ISquareMatrix* SquareMatrixPtr;
typedef ISquareMatrix& SquareMatrixPtrRef;

class IMatrixIterator;

typedef IMatrixIterator* MatrixIteratorPtr;

class IMatrixOperator;

typedef IMatrixOperator* MatrixOperatorPtr;

class IMatrixDistanceCalculator;

typedef IMatrixDistanceCalculator* MatrixDistanceCalculatorPtr;

class IMatrixCollection;

typedef IMatrixCollection* MatrixCollectionPtr;

class IPersistableMatrixCollection;

typedef IPersistableMatrixCollection* PersistableMatrixCollectionPtr;

typedef long long MatrixCollectionSize_t;

class IPersistableMatrixCollectionFactory;

typedef IPersistableMatrixCollectionFactory* PersistableMatrixCollectionFactoryPtr;

class IMatrixCollectionFactory;

typedef IMatrixCollectionFactory* MatrixCollectionFactoryPtr;

class ITargetMatrixFactory;
typedef ITargetMatrixFactory* TargetMatrixFactoryPtr;

class IMatrixWriter;
typedef IMatrixWriter* MatrixWriterPtr;

class ITimer;
typedef ITimer* TimerPtr;

#endif /* COMMON_H_ */
