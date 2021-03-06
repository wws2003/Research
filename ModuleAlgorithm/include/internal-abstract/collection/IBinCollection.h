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

typedef int bin_combination_prior_t;

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

	//The following method is normally used when the results are composed from more than 2 bins
	virtual void findBinSetsShouldBeCombined(std::vector<BinPtrVector<T> >& binSets, bin_combination_prior_t prior_threshold) = 0;

protected:
	//Calculate the likelihood of bins combination to compose good result
	//For convinience, the smaller the value the better
	virtual bin_combination_prior_t calculateBinCombinationPriority(BinPtrVector<T> bins) = 0;
};

#endif /* IBINCOLLECTION_H_ */
