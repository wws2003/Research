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

//Only for furture purpose. Generic level of current implementation is not enough for this

template<typename T>
class ICollection {
public:
	virtual ~ICollection();

	//Add one element to the collection
	virtual void addElement(T pElement) = 0;

	//Clean the collection
	virtual void clear() = 0;

	//Return iterator through a set of element reflecting the changes in the collection
	virtual IteratorPtr<T> getIterator() = 0;

	//Return iterator through a fixed set of elements, regardless the collection
	//should be modified later
	virtual IteratorPtr<T> getReadonlyIteratorPtr() = 0;

	//Get collection size
	virtual CollectionSize_t size() = 0;

	//Find the neighbour elements to the query, given distance calculator
	virtual IteratorPtr<T> findApproxElements(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const = 0;

};


#endif /* ICOLLECTION_H_ */
