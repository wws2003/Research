/*
 * AlgoInternal.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef ALGOINTERNAL_H_
#define ALGOINTERNAL_H_

#include <vector>
#include "AlgoCommon.h"

template<typename T> class Bin;
template<typename T>
using BinPtr = Bin<T>*;

typedef Bin<MatrixPtr> MatrixBin;
typedef MatrixBin* MatrixBinPtr;
typedef std::vector<MatrixBinPtr> MatrixBinPtrVector;

template<typename T>
using BinPtrVector = std::vector<BinPtr<T> >;

typedef Bin<GatePtr> GateBin;
typedef GateBin* GateBinPtr;

template<typename T> class IBinCollection;
typedef IBinCollection<MatrixPtr> IMatrixBinCollection;

template<typename T>
using BinCollectionPtr = IBinCollection<T>*;

template<typename T>
class IEquivalentQueryGenerator;

typedef IEquivalentQueryGenerator<GatePtr> IEquivalentGateQueryGenerator;
typedef IEquivalentQueryGenerator<MatrixPtr> IEquivalentMatrixQueryGenerator;

class IGateCombinabilityChecker;

template<typename T>
using BinIteratorPtr =  IIterator<BinPtr<T> >*;

typedef IIterator<MatrixBinPtr> MatrixBinIterator;
typedef MatrixBinIterator* MatrixBinIteratorPtr;

typedef IMatrixBinCollection* MatrixBinCollectionPtr;

template<typename T>
using EquivalentQueryGeneratorPtr = IEquivalentQueryGenerator<T>*;

typedef IEquivalentGateQueryGenerator* EquivalentGateQueryGeneratorPtr;
typedef IEquivalentMatrixQueryGenerator* EquivalentMatrixQueryGeneratorPtr;

typedef IGateCombinabilityChecker* GateCombinabilityCheckerPtr;
typedef std::vector<GateCombinabilityCheckerPtr> GateCombinabilityCheckers;

template<typename T>
class GNATCollectionIterator;

template<typename T>
class PersistableGNATCollectionImpl;

template<typename T>
using PersistableGNATCollectionImplPtr = PersistableGNATCollectionImpl<T>*;

template<typename T>
class IElementSetLog;

template<typename T>
using ElementSetLogPtr = IElementSetLog<T>*;

template<typename T> class ILookupResultFilter;

template<typename T>
using LookupResultFilterPtr = ILookupResultFilter<T>*;

typedef LookupResultFilterPtr<GatePtr> GateLookupResultFilterPtr;
typedef LookupResultFilterPtr<MatrixPtr> MatrixLookupResultFilterPtr;

template<typename T>
class IElementComparator;

template<typename T>
using ComparatorPtr = IElementComparator<T>*;

template<typename T1, typename T2>
class IConverter;

template<typename T1, typename T2>
using ConverterPtr = IConverter<T1, T2>*;

#endif /* ALGOINTERNAL_H_ */
