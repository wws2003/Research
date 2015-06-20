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

//Only for future purpose. Generic level of current implementation is not enough for this

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

	//Find the neighbor elements to the query, given distance calculator
	virtual IteratorPtr<T> findApproxElements(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const = 0;

};


#endif /* ICOLLECTION_H_ */
