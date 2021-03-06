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
#include "ICollection.h"

template<typename T>
class IApproximator {
public:
	virtual ~IApproximator(){};

	/**
	 * Find in given collection, and moreover, if possible generate from it
		 approximate elements to query (distance smaller than epsilon).
	   Results should be sorted in the ascending order of distance to query
	 */

	virtual IteratorPtr<LookupResult<T> > getApproximateElements(CollectionPtr<T> pCoreCollection,
			T pQuery,
			mreal_t epsilon) = 0;
};

#endif /* IAPPROXIMATOR_H_ */
