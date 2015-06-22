/*
 * FindEquivalentQueryCollectionImpl.hpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef EQUIVALENTQUERIESHANDLERCOLLECTIONIMPL_HPP_
#define EQUIVALENTQUERIESHANDLERCOLLECTIONIMPL_HPP_

#include "AlgoCommon.h"
#include "ICollection.h"
#include "IEquivalentQueryGenerator.h"

/**
 * This class aims to find approximate potential queries theses are equivalent
 * to the query of approximation
 * Main application is to find approximate for a matrix and also for its global-phase
 * equivalent matrices
 */

template<typename T>
class EquivalentQueriesHandlerCollectionImpl: public ICollection<T> {
public:
	EquivalentQueriesHandlerCollectionImpl(CollectionPtr<T> pInnerCollection);

	~EquivalentQueriesHandlerCollectionImpl(){};

	//Add one element to the collection
	void addElement(T element);

	//Clean the collection
	void clear();

	//Deeply clean the collection, i.e. release elements pointer (must be sure elements are pointer type!).
	void purge();

	//Return iterator through a set of element reflecting the changes in the collection
	IteratorPtr<T> getIteratorPtr();

	//Return iterator through a fixed set of elements, regardless the collection
	//should be modified later
	IteratorPtr<T> getReadonlyIteratorPtr();

	//Get collection size
	CollectionSize_t size() const;

	//Find the neighbor elements to the query, given distance calculator
	IteratorPtr<T> findApproxElements(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const;


private:
	CollectionPtr<T> m_pInnerCollection;
};

template<typename T>
EquivalentQueriesHandlerCollectionImpl<T>::EquivalentQueriesHandlerCollectionImpl(CollectionPtr<T> pInnerCollection) {
	m_pInnerCollection = pInnerCollection;
}

template<typename T>
void EquivalentQueriesHandlerCollectionImpl<T>::addElement(T element) {
	m_pInnerCollection->addElement(element);
}

template<typename T>
void EquivalentQueriesHandlerCollectionImpl<T>::clear() {
	m_pInnerCollection->clear();
}

template<typename T>
void EquivalentQueriesHandlerCollectionImpl<T>::purge() {
	m_pInnerCollection->purge();
}

template<typename T>
IteratorPtr<T> EquivalentQueriesHandlerCollectionImpl<T>::getIteratorPtr() {
	return m_pInnerCollection->getIteratorPtr();
}

template<typename T>
IteratorPtr<T> EquivalentQueriesHandlerCollectionImpl<T>::getReadonlyIteratorPtr() {
	return m_pInnerCollection->getReadonlyIteratorPtr();
}

template<typename T>
CollectionSize_t EquivalentQueriesHandlerCollectionImpl<T>::size() const {
	return m_pInnerCollection->size();
}

template<typename T>
IteratorPtr<T> EquivalentQueriesHandlerCollectionImpl<T>::findApproxElements(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const {

}



#endif /* FINDEQUIVALENTQUERYCOLLECTIONIMPL_HPP_ */
