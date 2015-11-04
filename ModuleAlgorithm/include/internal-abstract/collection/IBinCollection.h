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

	//Provide a target to construct bins around it
	virtual void addTarget(T target) = 0;

	//The following method is deprecated since bin pattern should be hidden from clients
	//virtual void addElement(T element, BinPattern binPattern) = 0;

	//The following method is used to hide bin pattern details from client of bin collection
	//, for more dynamic implementation of binning algorithms
	//Should specified target also ?
	virtual void addElement(T element, int targetIndex = 0) = 0;

	//Clear all bin structures
	virtual void clear() = 0;

	virtual CollectionSize_t size() = 0;

	//Re-bin the elements in collection
	virtual void restructureBins() = 0;

	virtual IteratorPtr<BinPtr<T> > getBinIteratorPtr() = 0;

	//The following method is normally used when the results are composed from 2 bins
	//and bin distance is clear to user
	virtual IteratorPtr<BinPtr<T> > findBinsCloseToBin(BinPtr<T> pMatrixBin, int distance) = 0;

	//The following method is normally used when the results are composed from more than 2 bins
	virtual void findBinSetsShouldBeCombined(std::vector<BinPtrVector<T> >& binSets) = 0;
};

#endif /* IBINCOLLECTION_H_ */
