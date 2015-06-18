/*
 * IMatrixBinCollection.h
 *
 *  Created on: Jun 15, 2015
 *      Author: pham
 */

#ifndef IMATRIXBINCOLLECTION_H_
#define IMATRIXBINCOLLECTION_H_

#include "IIterator.h"
#include "AlgoCommon.h"
#include "AlgoInternal.h"

class IMatrixBinCollection {
public:
	virtual ~IMatrixBinCollection(){};

	virtual void addMatrix(MatrixPtr pMatrix, BinPattern binPattern) = 0;

	virtual void clear() = 0;

	virtual CollectionSize_t size() = 0;

	virtual MatrixBinIteratorPtr getMatrixBinIteratorPtr() = 0;

	virtual MatrixBinIteratorPtr findBinsCloseToBin(MatrixBinPtr pMatrixBin, int distance) = 0;
};


#endif /* IMATRIXBINCOLLECTION_H_ */
