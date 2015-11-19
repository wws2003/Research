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

template<typename T>class IComposer;

template<typename T>
using ComposerPtr = IComposer<T>*;

template<typename T>class IDecomposer;

template<typename T>
using DecomposerPtr = IDecomposer<T>*;

template<typename T>
class IPersistableCollection;

template<typename T>
using PersistableCollectionPtr = IPersistableCollection<T>*;

typedef ICollection<MatrixRealCoordinatePtr> MatrixRealCoordinateCollection;
typedef ICollection<GateRealCoordinatePtr> GateRealCoordinateCollection;
typedef ICollection<MatrixPtr> IMatrixCollection;
typedef ICollection<GatePtr> IGateCollection;
typedef IComposer<GatePtr> IGateComposer;
typedef IComposer<MatrixPtr> IMatrixComposer;
typedef IDecomposer<GatePtr> IGateDecomposer;
typedef IDecomposer<MatrixPtr> IMatrixDecomposer;
typedef IPersistableCollection<MatrixPtr> IPersistableMatrixCollection;
typedef IPersistableCollection<GatePtr> IPersistableGateCollection;

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

typedef IMatrixApproximator* MatrixApproximatorPtr;
typedef IMatrixApproximator& MatrixApproximatorRef;

typedef IMatrixComposer* MatrixComposerPtr;
typedef MatrixComposerPtr& MatrixComposerPtrRef;

typedef IGateApproximator* GateApproximatorPtr;
typedef GateApproximatorPtr& GateApproximatorPtrRef;

typedef IGateComposer* GateComposerPtr;
typedef GateComposerPtr& GateComposerPtrRef;

typedef IGateDecomposer* GateDecomposerPtr;
typedef GateDecomposerPtr& GateDecomposerPtrRef;

typedef MatrixRealCoordinateCollection* MatrixRealCoordinateCollectionPtr;

typedef IMatrixCombiner* MatrixCombinerPtr;

typedef IPersistableMatrixCollection* PersistableMatrixCollectionPtr;
typedef IPersistableGateCollection* PersitableGateCollectionPtr;

#endif /* ALGOCOMMON_H_ */
