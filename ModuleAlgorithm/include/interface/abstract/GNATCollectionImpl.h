/*
 * GNATCollectionImpl.h
 *
 *  Created on: Jun 26, 2015
 *      Author: pham
 */

#ifndef GNATCOLLECTIONIMPL_H_
#define GNATCOLLECTIONIMPL_H_

#include "AlgoCommon.h"
#include "AlgoInternal.h"
#include "ICollection.h"
#include <vector>
#include <algorithm>

template<typename T>
using CollectionVector = std::vector<CollectionPtr<T> >;

typedef std::pair<double, double> Range;

typedef std::vector<std::vector<Range> > RangeMap;

/**
 * Geometric-nearbour access tree implementation of collection
 */
template<typename T>
class GNATCollectionImpl : public ICollection<T> {
public:
	GNATCollectionImpl(int nbSplitPoints);
	virtual ~GNATCollectionImpl();

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

	//(Re)Build the search data structure given distance calculator
	virtual void rebuildStructure(DistanceCalculatorPtr<T> pDistanceCalculator);

	//Find the neighbor elements to the query, given distance calculator
	virtual IteratorPtr<T> findNearestNeighbour(T query, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon) const ;

protected:

	/*--------------For constructing GNAT data structure--------------*/
	//Re-call the points in sub collection into unstructured buffer to prepare for restructuring
	virtual void recallElements();

	//Choose split points in the buffer
	virtual void initSplitPoints();

	//Create sub collections (considering use abstract factory / make abstract method)
	virtual void createSubCollections();

	//Calculate ranges between each split point and each sub collection
	virtual void calculateRanges(DistanceCalculatorPtr<T> pDistanceCalculator);

	/*--------------For searching on GNAT data structure--------------*/
	//Get sub collections in those search result can be found
	virtual void getCandidateSubCollectionIndexes(T query, DistanceCalculatorPtr<T> pDistanceCalculator, std::vector<int>& rCandidateIndexes);


	int m_nbSplitPoints;

	std::vector<T> m_splitPoints;
	CollectionVector<T> m_subCollections;

	std::vector<T> m_unStructeredBuffer;

	RangeMap m_splitPointRanges;

};


#endif /* GNATCOLLECTIONIMPL_H_ */
