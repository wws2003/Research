/*
 * ICollection.h
 *
 *  Created on: May 12, 2015
 *      Author: pham
 */

#ifndef ICOLLECTION_H_
#define ICOLLECTION_H_

#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "MathConceptsCommon.h"
#include "IIterator.h"

template<typename T>
struct LookupResult{
	LookupResult(T resultElement, mreal_t distanceToTarget){m_resultElement = resultElement; m_distanceToTarget = distanceToTarget;};
	T m_resultElement;
	mreal_t m_distanceToTarget;
};

template<typename T>
struct _DistanceComparator {
	bool operator()(const LookupResult<T>& r1, const LookupResult<T>& r2)
	{
		return r1.m_distanceToTarget < r2.m_distanceToTarget;
	}
};

template<typename T>
using DistanceComparator = struct _DistanceComparator<T>;

template<typename T>
class ICollection {
public:
	virtual ~ICollection(){};

	//Add one element to the collection
	virtual void addElement(T pElement) = 0;

	//Clean the collection
	virtual void clear() = 0;

	//Deeply clean the collection, i.e. release elements pointer (must be sure elements are pointer type!).
	virtual void purge() = 0;

	//Return iterator through a set of element reflecting the changes in the collection
	virtual IteratorPtr<T> getIteratorPtr() = 0;

	//Return iterator through a fixed set of elements, regardless the collection
	//should be modified later
	virtual IteratorPtr<T> getReadonlyIteratorPtr() = 0;

	//Get collection size
	virtual CollectionSize_t size() const = 0;

	//(Re)Build the search data structure given distance calculator
	virtual void rebuildStructure(DistanceCalculatorPtr<T> pDistanceCalculator) = 0;

	//Advanced lookup function, with distance stored in results
	virtual IteratorPtr<LookupResult<T> > findNearestNeighbours(T query,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			bool toSortResults = false) const = 0;
};


#endif /* ICOLLECTION_H_ */
