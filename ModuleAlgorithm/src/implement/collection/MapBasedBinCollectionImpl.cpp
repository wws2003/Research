/*
 * MapBasedBinCollectionImpl.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#include "MapBasedBinCollectionImpl.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "Bin.hpp"
#include "Coordinate.hpp"
#include <algorithm>

template<typename T>
MapBasedBinCollectionImpl<T>::MapBasedBinCollectionImpl(RealCoordinateCalculatorPtr<T> pCoordinateCalculator) {
	m_pRealCoordinateCalculator = pCoordinateCalculator;
	m_pInnerBinIterator = BinIteratorPtr<T>(new InnerBinIterator(&m_binMap));
}

template<typename T>
MapBasedBinCollectionImpl<T>::~MapBasedBinCollectionImpl() {
	_destroy(m_pInnerBinIterator);
}

template<typename T>
void MapBasedBinCollectionImpl<T>::addTarget(T target) {
	//FIXME This class only accept one target?
	m_targets.clear();
	m_targets.push_back(target);
	m_targetCoords.clear();

	RealCoordinatePtr<T> pTargetRealCoordinate;
	m_pRealCoordinateCalculator->calulateElementCoordinate(target, pTargetRealCoordinate);
	m_targetCoords.push_back(pTargetRealCoordinate->getCoordinates());
	_destroy(pTargetRealCoordinate);
}

template<typename T>
void MapBasedBinCollectionImpl<T>::addElement(T element, int targetIndex) {
	BinBinaryPattern binPattern  = calculateBinPattern(element, targetIndex);
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
	for(typename BinBinaryPatternMap<T>::iterator bIter = m_binMap.begin(); bIter != m_binMap.end(); bIter++) {
		BinPtr<T> pBin = bIter->second;
		_destroy(pBin);
		bIter->second = NullPtr;
	}
	m_binMap.clear();
	m_pInnerBinIterator->toBegin();
}

template<typename T>
void MapBasedBinCollectionImpl<T>::restructureBins() {
	//Do nothing as an element never changes its own bin, which solely depends on it coordinate
}

template<typename T>
BinIteratorPtr<T> MapBasedBinCollectionImpl<T>::getBinIteratorPtr() {
	return m_pInnerBinIterator;
}

template<typename T>
void MapBasedBinCollectionImpl<T>::findBinSetsShouldBeCombined(std::vector<BinPtrVector<T> >& binSets, bin_combination_prior_t prior_threshold) {
	//Only handle 2 bins ?
	//TODO Use more efficient algorithm than looping

	binSets.clear();

	for(typename BinBinaryPatternMap<T>::const_iterator lIter = m_binMap.begin(); lIter != m_binMap.end(); lIter++) {
		BinPtr<T> pBin1 = lIter->second;
		for(typename BinBinaryPatternMap<T>::const_iterator rIter = m_binMap.begin(); rIter != m_binMap.end(); rIter++) {
			BinPtr<T> pBin2 = rIter->second;
			BinPtrVector<T> bins = {pBin1, pBin2};
			if(calculateBinCombinationPriority(bins) < prior_threshold) {
				binSets.push_back(bins);
			}
		}
	}
}

/*
 * Protected methods follow, focusing how to calculate bin identifier (i.e. bin pattern) and how to use
 * the identifiers to calculate the likelihood of bin combination
 */
template<typename T>
bin_combination_prior_t MapBasedBinCollectionImpl<T>::calculateBinCombinationPriority(BinPtrVector<T> bins) {
	//FIXME  Only handle 2 bins ?
	BinPtr<T> pBin1 = bins[0];
	BinPtr<T> pBin2 = bins[1];

	int binPatternSize = pBin1->getPattern().size();
	BinBinaryPattern pattern1 = pBin1->getPattern();
	BinBinaryPattern pattern2 = pBin2->getPattern();

	bin_combination_prior_t priority = binPatternSize;

	for(unsigned int i = 0; i < binPatternSize; i++) {
		if(pattern1[i] != pattern2[i]) {
			priority--;
		}
	}
	return priority;
}

template<typename T>
BinBinaryPattern MapBasedBinCollectionImpl<T>::calculateBinPattern(T element, int targetIndex) {
	real_coordinate_t targetCoordinate = m_targetCoords[targetIndex];

	//Calculate element coordinate
	RealCoordinatePtr<T> pElementRealCoordinate;
	m_pRealCoordinateCalculator->calulateElementCoordinate(element, pElementRealCoordinate);
	real_coordinate_t elementCoordinate = pElementRealCoordinate->getCoordinates();
	_destroy(pElementRealCoordinate);

	unsigned int nbCoordinates = targetCoordinate.size();

	BinBinaryPattern binPattern(nbCoordinates, '0');
	for(unsigned int i = 0; i < nbCoordinates; i++) {
		if(elementCoordinate[i] < targetCoordinate[i] / 2) {
			binPattern[i] = '1';
		}
		else {
			binPattern[i] = '0';
		}
	}
	return binPattern;
}

template<typename T>
void MapBasedBinCollectionImpl<T>::InnerBinIterator::toBegin() {
	m_currentIter = m_pBinMap->begin();
}

template<typename T>
MapBasedBinCollectionImpl<T>::InnerBinIterator::InnerBinIterator(BinBinaryPatternMapPtr<T> pBinMap) {
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





