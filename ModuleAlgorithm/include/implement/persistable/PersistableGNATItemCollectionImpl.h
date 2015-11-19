/*
 * PersistableGNATItemCollectionImpl.h
 *
 *  Created on: Oct 27, 2015
 *      Author: pham
 */

#ifndef PERSISTABLEGNATITEMCOLLECTIONIMPL_H_
#define PERSISTABLEGNATITEMCOLLECTIONIMPL_H_

#include "PersistableGNATCollectionImpl.h"
#include "MathConceptsCommon.h"
#include "AlgoInternal.h"

//Just a wrapper for implemenation of the interface IPersistableCollection
//This class effectively delegate on an instance of PersistableGNATCollection,
//which UNFORTUNATELY can't be used as a derived of IPersistableCollection

template<typename T>
class PersistableGNATItemCollectionImpl : public IPersistableCollection<T> {
public:
	PersistableGNATItemCollectionImpl(WriterPtr<T> pWriter, ReaderPtr<T> pReader){
		m_pInternal = new PersistableGNATCollectionImpl<T>(pWriter, pReader);
	};

	virtual ~PersistableGNATItemCollectionImpl(){_destroy(m_pInternal);};

	//Implementation for ICollection
	//Add one element to the collection
	void addElement(T pElement){m_pInternal->addElement(pElement);};

	//Clean the collection
	void clear(){m_pInternal->clear();};

	//Deeply clean the collection, i.e. release elements pointer (must be sure elements are pointer type!).
	void purge(){m_pInternal->purge();};

	//Return iterator through a set of element reflecting the changes in the collection
	IteratorPtr<T> getIteratorPtr(){return m_pInternal->getIteratorPtr();};

	//Return iterator through a fixed set of elements, regardless the collection
	//should be modified later
	IteratorPtr<T> getReadonlyIteratorPtr(){return m_pInternal->getReadonlyIteratorPtr();};

	//Get collection size
	CollectionSize_t size() const {return m_pInternal->size();};

	//(Re)Build the search data structure given distance calculator
	void rebuildStructure(DistanceCalculatorPtr<GatePtr> pDistanceCalculator){m_pInternal->rebuildStructure(pDistanceCalculator);};

	//Find the neighbor elements to the query, given distance calculator
	IteratorPtr<GatePtr> findNearestNeighbour(GatePtr query, DistanceCalculatorPtr<GatePtr> pDistanceCalculator, mreal_t epsilon) const  {;
	return m_pInternal->findNearestNeighbour(query, pDistanceCalculator, epsilon);
	}

	//Implementation for IPersistable
	void save(std::string fileName){m_pInternal->save(fileName);};

	void load(std::string fileName){m_pInternal->load(fileName);};

	//Overloading
	bool operator==(const PersistableGNATItemCollectionImpl<T>& rhs) {
		return *m_pInternal == *rhs.m_pInternal;
	}

	//The following 2 methods are made public solely for test purpose

	//Explicitly set data fields
	void init(const SplitPointSet<T>& splitPoints,
			const UnstructuredBuffer<T>& unStructeredBuffer,
			const RangeMap& splitPointRanges) {
		m_pInternal->init(splitPoints, unStructeredBuffer, splitPointRanges);
	}

	//Add sub-collection
	void addSubCollection(CollectionPtr<T> pSubCollection) {
		m_pInternal->addSubCollection(pSubCollection);
	}

private:
	PersistableGNATCollectionImpl<T>* m_pInternal;
};





#endif /* PERSISTABLEGNATITEMCOLLECTIONIMPL_H_ */
