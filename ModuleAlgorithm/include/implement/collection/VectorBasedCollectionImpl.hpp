/*
 * VectorBasedCollectionImpl.hpp
 *
 *  Created on: Jun 17, 2015
 *      Author: pham
 */

#ifndef VECTORBASEDCOLLECTIONIMPL_HPP_
#define VECTORBASEDCOLLECTIONIMPL_HPP_

#include "ICollection.h"
#include "VectorBasedIteratorImpl.hpp"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "IDistanceCalculator.h"
#include <vector>

template<typename T>
class VectorBasedCollectionImpl : public ICollection<T> {
public:
	VectorBasedCollectionImpl();
	virtual ~VectorBasedCollectionImpl(){};

	//Add one element to the collection
	virtual void addElement(T pElement) ;

	//Clean the collection
	virtual void clear() ;

	//Deeply clean the collection, i.e. release elements pointer (must be sure elements are pointer type!).
	virtual void purge();

	//Return iterator through a set of element reflecting the changes in the collection
	virtual IteratorPtr<T> getIteratorPtr() ;

	//Return iterator through a fixed set of elements, regardless the collection
	//should be modified later
	virtual IteratorPtr<T> getReadonlyIteratorPtr() ;

	//Get collection size
	virtual CollectionSize_t size() const ;

	//Find the neighbor elements to the query, given distance calculator
	virtual IteratorPtr<T> findNearestNeighbour(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const ;

private:
	std::vector<T> m_elements;
	IteratorPtr<T> m_pIterator;
};

template<typename T>
VectorBasedCollectionImpl<T>::VectorBasedCollectionImpl() {
	m_pIterator = IteratorPtr<T>(new VectorBasedIteratorImpl<T>(&m_elements));
}

template<typename T>
void VectorBasedCollectionImpl<T>::addElement(T element) {
	m_elements.push_back(element);
}

template<typename T>
void VectorBasedCollectionImpl<T>::clear() {
	m_elements.clear();
}

template<typename T>
void VectorBasedCollectionImpl<T>::purge() {
	for(typename std::vector<T>::iterator eIter = m_elements.begin(); eIter != m_elements.end(); ) {
		T element = *eIter;
		eIter = m_elements.erase(eIter);
		_destroy(element);
	}
}

template<typename T>
IteratorPtr<T> VectorBasedCollectionImpl<T>::getIteratorPtr() {
	return m_pIterator;
}

template<typename T>
IteratorPtr<T> VectorBasedCollectionImpl<T>::getReadonlyIteratorPtr() {
	IteratorPtr<T> pResultIter(new VectorBasedReadOnlyIteratorImpl<T>(m_elements));
	return pResultIter;
}

template<typename T>
CollectionSize_t VectorBasedCollectionImpl<T>::size() const {
	return m_elements.size();
}

template<typename T>
IteratorPtr<T> VectorBasedCollectionImpl<T>::findNearestNeighbour(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const {
	std::vector<T> results;
	CollectionSize_t collectionSize = m_elements.size();
	for(unsigned int i = 0; i < collectionSize; i++) {
		T element = m_elements[i];
		if(pDistanceCalculator->distance(element, query) <= epsilon) {
			results.push_back(element);
		}
	}
	IteratorPtr<T> pResultIter(new VectorBasedReadOnlyIteratorImpl<T>(results));
	return pResultIter;
}



#endif /* VECTORBASEDCOLLECTIONIMPL_HPP_ */
