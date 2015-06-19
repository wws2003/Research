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
using BinIteratorPtr =  IIterator<BinPtr<T> >*;

typedef IIterator<MatrixBinPtr> MatrixBinIterator;
typedef MatrixBinIterator* MatrixBinIteratorPtr;

typedef std::string BinPattern;

typedef IMatrixBinCollection* MatrixBinCollectionPtr;



#endif /* ALGOINTERNAL_H_ */
