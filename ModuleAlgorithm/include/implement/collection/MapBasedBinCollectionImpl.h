/*
 * MapBasedBinCollectionImpl.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef MAPBASEDBINCOLLECTIONIMPL_H_
#define MAPBASEDBINCOLLECTIONIMPL_H_

#include "AlgoInternal.h"
#include "IBinCollection.h"
#include <map>
#include <algorithm>

template<typename T>
using BinPatternMap = std::map<BinPattern, BinPtr<T> >;

template<typename T>
using BinPatternMapPtr = BinPatternMap<T>* ;

template<typename T>
using PatternBinPair = std::pair<BinPattern, BinPtr<T> > ;

template<typename T>
class MapBasedBinCollectionImpl : public IBinCollection<T> {
public:

	MapBasedBinCollectionImpl();

	virtual ~MapBasedBinCollectionImpl();

	//Override
	virtual void addTarget(T target);

	//Override
	virtual void addElement(T element);

	//Override
	virtual CollectionSize_t size();

	//Override
	virtual void clear();

	//Override
	virtual BinIteratorPtr<T> getBinIteratorPtr();

	//Override
	virtual BinIteratorPtr<T> findBinsCloseToBin(BinPtr<T> pOtherBin, int distance);

private:
	BinPatternMap<T> m_binMap;

	class InnerBinIterator : IIterator<BinPtr<T> > {
	public:
		InnerBinIterator(BinPatternMapPtr<T> pBinMap);

		//Go to the beginning (point to first element)
		virtual void toBegin();

		//Go to next element pointer
		virtual void next();

		//Go to previous pointer
		virtual void prev();

		//Check if the last element has been checked
		virtual bool isDone();

		virtual BinPtr<T> getObj();

	private:
		typename BinPatternMap<T>::iterator m_currentIter;
		BinPatternMapPtr<T> m_pBinMap;
	};

	BinIteratorPtr<T> m_pInnerBinIterator;
};



#endif /* MAPBASEDBINCOLLECTIONIMPL_H_ */
