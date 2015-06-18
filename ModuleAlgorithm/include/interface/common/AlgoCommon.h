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
typedef CollectionSize_t MatrixCollectionSize_t;

class IPersistableMatrixCollection;

template<typename T> class IApproximator;
typedef IApproximator<MatrixPtr> IMatrixApproximator;
typedef IApproximator<GatePtr> IGateApproximator;

template<typename T> class ICombiner;
typedef ICombiner<MatrixPtr> IMatrixCombiner;
typedef ICombiner<GatePtr> IGateCombiner;

template<typename T> class ICollection;
template<typename T>
using CollectionPtr = ICollection<T>*;

typedef ICollection<MatrixRealCoordinatePtr> MatrixRealCoordinateCollection;
typedef ICollection<MatrixPtr> IMatrixCollection;

template<typename T> class IIterator;
template<typename T>
using IteratorPtr = IIterator<T>* ;

typedef IIterator<MatrixPtr> MatrixIterator;

typedef MatrixIterator* MatrixIteratorPtr;

typedef IIterator<MatrixRealCoordinatePtr>* MatrixRealCoordinateIteratorPtr;

typedef IMatrixCollection& MatrixCollectionRef;
typedef IMatrixCollection* MatrixCollectionPtr;

typedef IPersistableMatrixCollection* PersitableMatrixCollectionPtr;

typedef IMatrixApproximator* MatrixApproximatorPtr;
typedef IMatrixApproximator& MatrixApproximatorRef;

typedef IGateApproximator* GateApproximatorPtr;
typedef GateApproximatorPtr& GateApproximatorPtrRef;

typedef MatrixRealCoordinateCollection* MatrixRealCoordinateCollectionPtr;

typedef IMatrixCombiner* MatrixCombinerPtr;

#endif /* ALGOCOMMON_H_ */
