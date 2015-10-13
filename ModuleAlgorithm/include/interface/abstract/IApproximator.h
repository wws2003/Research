/*
 * IApproximator.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef IAPPROXIMATOR_H_
#define IAPPROXIMATOR_H_

#include "AlgoCommon.h"
#include "OperatorCommon.h"

template<typename T>
class IApproximator {
public:
	virtual ~IApproximator(){};

	/**
	 * Find in given collection, and moreover, if possible generate from it
		 approximate elements to query (distance smaller than epsilon)
	 */
	virtual IteratorPtr<T> getApproximateElements(CollectionPtr<T> pCoreCollection, T pQuery, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t epsilon) = 0;
};

#endif /* IAPPROXIMATOR_H_ */
