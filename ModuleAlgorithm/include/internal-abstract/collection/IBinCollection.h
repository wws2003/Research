/*
 * IBinCollection.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef IBINCOLLECTION_H_
#define IBINCOLLECTION_H_

#include "Bin.hpp"
#include "AlgoCommon.h"
#include "AlgoInternal.h"

template<typename T>
class IBinCollection {
public:
	virtual ~IBinCollection(){};

	virtual void addElement(T element, BinPattern binPattern) = 0;

	virtual void clear() = 0;

	virtual CollectionSize_t size() = 0;

	virtual IteratorPtr<BinPtr<T> > getBinIteratorPtr() = 0;

	virtual IteratorPtr<BinPtr<T> > findBinsCloseToBin(BinPtr<T> pMatrixBin, int distance) = 0;

};




#endif /* IBINCOLLECTION_H_ */
