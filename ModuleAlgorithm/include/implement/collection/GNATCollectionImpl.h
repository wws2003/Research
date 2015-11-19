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
#include "IIterator.h"
#include <vector>
#include <algorithm>
#include <list>

template<typename T>
using CollectionVector = std::vector<CollectionPtr<T> >;

typedef std::pair<mreal_t, mreal_t> Range;

typedef std::vector<std::vector<Range> > RangeMap;

template<typename T>
using UnstructuredBuffer = std::list<T>;

template<typename T>
using SplitPointSet = std::vector<T>;

/**
 * Geometric-neighbor access tree implementation of collection
 */
template<typename T>
class GNATCollectionImpl : public ICollection<T> {
public:
	GNATCollectionImpl();
	virtual ~GNATCollectionImpl();

	//Override
	virtual void addElement(T pElement) ;

	//Override
	virtual void clear() ;

	//Override
	virtual void purge();

	//Override
	virtual IteratorPtr<T> getIteratorPtr() ;

	//Override
	virtual IteratorPtr<T> getReadonlyIteratorPtr() ;

	//Override
	virtual CollectionSize_t size() const ;

	//(Re)Build the search data structure given distance calculator
	virtual void rebuildStructure(DistanceCalculatorPtr<T> pDistanceCalculator);

	//Find the neighbor elements to the query, given distance calculator
	virtual IteratorPtr<T> findNearestNeighbour(T query, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t epsilon) const ;

protected:
	//Generate instance of a sub-collection. Subject to be overridden in sub-classes
	virtual CollectionPtr<T> generateSubCollection();

	SplitPointSet<T> m_splitPoints;
	CollectionVector<T> m_subCollections;
	UnstructuredBuffer<T> m_unStructeredBuffer;
	RangeMap m_splitPointRanges;

private:

	void filterResults(std::vector<T>& results, DistanceCalculatorPtr<T> pDistanceCalculator) const;

	/*--------------For constructing GNAT data structure--------------*/
	//Re-call the points in sub collection into unstructured buffer to prepare for restructuring
	virtual void recallElements();

	//Choose split points in the buffer
	virtual void initSplitPoints();

	//Create sub collections (considering use abstract factory / make abstract method)
	virtual void createSubCollections(DistanceCalculatorPtr<T> pDistanceCalculator);

	//Calculate ranges between each split point and each sub collection
	virtual void calculateRanges(DistanceCalculatorPtr<T> pDistanceCalculator);

	/*--------------For searching on GNAT data structure--------------*/
	//Get sub collections in those search result can be found
	virtual void getCandidateSubCollections(T query, DistanceCalculatorPtr<T> pDistanceCalculator, mreal_t epsilon, std::vector<int>& rCheckMap) const;

	//Calculate proper number of split points
	virtual int detectNbSplitPoints();

	friend class GNATCollectionIterator<T>;
};


#endif /* GNATCOLLECTIONIMPL_H_ */
