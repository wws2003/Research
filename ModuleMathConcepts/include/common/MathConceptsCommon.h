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
class IMatrixFactory;


typedef IMatrix* MatrixPtr;
typedef MatrixPtr& MatrixPtrRef;

typedef IVector* VectorPtr;
typedef VectorPtr& VectorPtrRef;

typedef ISquareMatrix* SquareMatrixPtr;
typedef ISquareMatrix*& SquareMatrixPtrRef;

typedef IIterator<MatrixPtr>* MatrixIteratorPtr;
typedef IIterator<VectorPtr>* VectorItertatorPtr;

typedef IMatrixFactory* MatrixFactoryPtr;


#endif /* COMMON_H_ */
