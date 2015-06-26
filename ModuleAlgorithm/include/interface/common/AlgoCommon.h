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
template<typename T>
using CombinerPtr = ICombiner<T>*;

template<typename T> class ICollection;
template<typename T>
using CollectionPtr = ICollection<T>*;

template<typename T>
using ApproximatorPtr = IApproximator<T>*;

typedef ICollection<MatrixRealCoordinatePtr> MatrixRealCoordinateCollection;
typedef ICollection<GateRealCoordinatePtr> GateRealCoordinateCollection;
typedef ICollection<MatrixPtr> IMatrixCollection;
typedef ICollection<GatePtr> IGateCollection;

template<typename T> class IIterator;
template<typename T>
using IteratorPtr = IIterator<T>* ;

typedef IIterator<MatrixPtr> MatrixIterator;
typedef IIterator<GatePtr> GateIterator;

typedef MatrixIterator* MatrixIteratorPtr;
typedef GateIterator* GateIteratorPtr;

typedef IIterator<MatrixRealCoordinatePtr>* MatrixRealCoordinateIteratorPtr;

typedef IMatrixCollection& MatrixCollectionRef;
typedef IMatrixCollection* MatrixCollectionPtr;

typedef IGateCollection& GateCollectionRef;
typedef IGateCollection* GateCollectionPtr;

typedef IPersistableMatrixCollection* PersitableMatrixCollectionPtr;

typedef IMatrixApproximator* MatrixApproximatorPtr;
typedef IMatrixApproximator& MatrixApproximatorRef;

typedef IGateApproximator* GateApproximatorPtr;
typedef GateApproximatorPtr& GateApproximatorPtrRef;

typedef MatrixRealCoordinateCollection* MatrixRealCoordinateCollectionPtr;

typedef IMatrixCombiner* MatrixCombinerPtr;

#endif /* ALGOCOMMON_H_ */
