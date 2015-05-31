/*
 * AlgoCommon.h
 *
 *  Created on: Apr 28, 2015
 *      Author: pham
 */

#ifndef ALGOCOMMON_H_
#define ALGOCOMMON_H_

#include "MathConceptsCommon.h"

typedef long long counter_t;
typedef long long CollectionSize_t;
typedef long long MatrixCollectionSize_t;

class IMatrixCollection;
class IPersistableMatrixCollection;
class IMatrixApproximator;

template<typename T> class ICollection;
template<typename T>
using CollectionPtr = ICollection<T>*;

typedef ICollection<MatrixRealCoordinatePtr> MatrixRealCoordinateCollection;

template<typename T> class IIterator;
template<typename T>
using IteratorPtr = IIterator<T>* ;

typedef IIterator<MatrixPtr> MatrixIterator;
typedef IIterator<VectorPtr> VectorIterator;

typedef MatrixIterator* MatrixIteratorPtr;
typedef VectorIterator* VectorItertatorPtr;

typedef IIterator<MatrixRealCoordinatePtr>* MatrixRealCoordinateIteratorPtr;

typedef IMatrixCollection& MatrixCollectionRef;
typedef IMatrixCollection* MatrixCollectionPtr;

typedef IPersistableMatrixCollection* PersitableMatrixCollectionPtr;

typedef IMatrixApproximator* MatrixApproximatorPtr;
typedef IMatrixApproximator& MatrixApproximatorRef;

typedef MatrixRealCoordinateCollection* MatrixRealCoordinateCollectionPtr;

#endif /* ALGOCOMMON_H_ */
