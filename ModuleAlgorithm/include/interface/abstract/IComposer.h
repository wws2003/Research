/*
 * IComposer.h
 *
 *  Created on: Oct 18, 2015
 *      Author: pham
 */

#ifndef ICOMPOSER_H_
#define ICOMPOSER_H_

#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "ICollection.h"
#include <vector>

template<typename T>
using BuildingBlockBuckets = std::vector<IteratorPtr<T> > ;

typedef unsigned long combination_counter_t;

template<typename T>
class IComposer {
public:
	virtual ~IComposer(){};

	/**
	 * Compose building blocks in buckets to generate list of approximation
	 * candidates for the target
	 */
	virtual IteratorPtr<LookupResult<T> > composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			bool toSortResults = true) = 0;
};


#endif /* ICOMPOSER_H_ */
