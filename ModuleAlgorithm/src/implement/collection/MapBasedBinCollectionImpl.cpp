/*
 * MapBasedBinCollectionImpl.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#include "MapBasedBinCollectionImpl.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "Bin.hpp"
#include <algorithm>

template<typename T>
MapBasedBinCollectionImpl<T>::MapBasedBinCollectionImpl() {
	m_pInnerBinIterator = BinIteratorPtr<T>(new InnerBinIterator(&m_binMap));
}

template<typename T>
MapBasedBinCollectionImpl<T>::~MapBasedBinCollectionImpl() {
	_destroy(m_pInnerBinIterator);
}

template<typename T>
void MapBasedBinCollectionImpl<T>::addElement(T element, BinPattern binPattern) {
	//This if statement is not really required
	if(m_binMap.find(binPattern) == m_binMap.end()) {
		m_binMap.insert(PatternBinPair<T>(binPattern, BinPtr<T>(new Bin<T>(binPattern))));
	}
	m_binMap[binPattern]->addElement(element);
}

template<typename T>
CollectionSize_t MapBasedBinCollectionImpl<T>::size() {
	return m_binMap.size();
}

template<typename T>
void MapBasedBinCollectionImpl<T>::clear() {
	for(typename BinPatternMap<T>::iterator bIter = m_binMap.begin(); bIter != m_binMap.end(); bIter++) {
		BinPtr<T> pBin = bIter->second;
		_destroy(pBin);
		bIter->second = NullPtr;
	}
	m_binMap.clear();
	m_pInnerBinIterator->toBegin();
}

template<typename T>
BinIteratorPtr<T> MapBasedBinCollectionImpl<T>::getBinIteratorPtr() {
	return m_pInnerBinIterator;
}

template<typename T>
BinIteratorPtr<T> MapBasedBinCollectionImpl<T>::findBinsCloseToBin(BinPtr<T> pOtherBin, int distance) {
	//TODO Thinking of more efficient algorithm than looping

	BinPtrVector<T> resultBins;

	for(typename BinPatternMap<T>::const_iterator bIter = m_binMap.begin(); bIter != m_binMap.end(); bIter++) {
		BinPtr<T> pBin = bIter->second;
		if(pBin->distance(*pOtherBin) <= distance) {
			resultBins.push_back(pBin);
		}
	}
	BinIteratorPtr<T> pBinIter(new VectorBasedReadOnlyIteratorImpl<BinPtr<T> >(resultBins));

	return pBinIter;
}

template<typename T>
void MapBasedBinCollectionImpl<T>::InnerBinIterator::toBegin() {
	m_currentIter = m_pBinMap->begin();
}

template<typename T>
MapBasedBinCollectionImpl<T>::InnerBinIterator::InnerBinIterator(BinPatternMapPtr<T> pBinMap) {
	m_pBinMap = pBinMap;
	toBegin();
}

//Go to next element pointer
template<typename T>
void MapBasedBinCollectionImpl<T>::InnerBinIterator::next() {
	m_currentIter++;
}

//Go to previous pointer
template<typename T>
void MapBasedBinCollectionImpl<T>::InnerBinIterator::prev() {
	m_currentIter--;
}

//Check if the last element has been checked
template<typename T>
bool MapBasedBinCollectionImpl<T>::InnerBinIterator::isDone() {
	return (m_currentIter == m_pBinMap->end());
}

template<typename T>
BinPtr<T> MapBasedBinCollectionImpl<T>::InnerBinIterator::getObj() {
	return m_currentIter->second;
}





