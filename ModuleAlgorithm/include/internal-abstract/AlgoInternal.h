/*
 * AlgoInternal.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef ALGOINTERNAL_H_
#define ALGOINTERNAL_H_

#include <vector>

template<typename T> class Bin;
template<typename T>
using BinPtr = Bin<T>*;

typedef Bin<MatrixPtr> MatrixBin;
typedef MatrixBin* MatrixBinPtr;
typedef std::vector<MatrixBinPtr> MatrixBinPtrVector;

typedef Bin<GatePtr> GateBin;
typedef GateBin* GateBinPtr;

typedef IIterator<MatrixBinPtr> MatrixBinIterator;
typedef MatrixBinIterator* MatrixBinIteratorPtr;

typedef std::string BinPattern;
class IMatrixBinCollection;

typedef IMatrixBinCollection* MatrixBinCollectionPtr;


#endif /* ALGOINTERNAL_H_ */
