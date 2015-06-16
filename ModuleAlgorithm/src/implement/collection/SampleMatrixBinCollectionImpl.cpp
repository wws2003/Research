/*
 * SampleMatrixBinCollectionImpl.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: pham
 */

#include "SampleMatrixBinCollectionImpl.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include <algorithm>

SampleMatrixBinCollectionImpl::SampleMatrixBinCollectionImpl() {
	m_pInnerMatrixBinIterator = MatrixBinIteratorPtr(new InnerMatrixBinIterator(&m_binMap));
}

SampleMatrixBinCollectionImpl::~SampleMatrixBinCollectionImpl() {
	_destroy(m_pInnerMatrixBinIterator);
}


void SampleMatrixBinCollectionImpl::addMatrix(MatrixPtr pMatrix, BinPattern binPattern) {
	//This if statement is not really required
	if(m_binMap.find(binPattern) == m_binMap.end()) {
		m_binMap.insert(PatternBinPair(binPattern, new MatrixBin(binPattern)));
	}
	m_binMap[binPattern]->addMatrix(pMatrix);
}

void SampleMatrixBinCollectionImpl::clear() {
	for(BinPatternMap::iterator bIter = m_binMap.begin(); bIter != m_binMap.end(); bIter++) {
		MatrixBinPtr pBin = bIter->second;
		_destroy(pBin);
		bIter->second = NullPtr;
	}
	m_binMap.clear();
}

MatrixBinIteratorPtr SampleMatrixBinCollectionImpl::getMatrixBinIteratorPtr() {
	return m_pInnerMatrixBinIterator;
}

MatrixBinIteratorPtr SampleMatrixBinCollectionImpl::findBinsCloseToBin(MatrixBinPtr pMatrixBin, int distance) {
	//TODO Thinking of more efficient algorithm than looping

	MatrixBinPtrVector resultBins;

	for(BinPatternMap::const_iterator bIter = m_binMap.begin(); bIter != m_binMap.end(); bIter++) {
		MatrixBinPtr pBin = bIter->second;
		if(pBin->distance(*pMatrixBin) <= distance) {
			resultBins.push_back(pBin);
		}
	}
	MatrixBinIteratorPtr pMatrixBinIter(new VectorBasedReadOnlyIteratorImpl<MatrixBinPtr>(resultBins));

	return pMatrixBinIter;
}

void SampleMatrixBinCollectionImpl::InnerMatrixBinIterator::toBegin() {
	m_currentIter = m_pBinMap->begin();
}

SampleMatrixBinCollectionImpl::InnerMatrixBinIterator::InnerMatrixBinIterator(BinPatternMapPtr pBinMap) {
	m_pBinMap = pBinMap;
	toBegin();
}

//Go to next element pointer
void SampleMatrixBinCollectionImpl::InnerMatrixBinIterator::next() {
	m_currentIter++;
}

//Go to previous pointer
void SampleMatrixBinCollectionImpl::InnerMatrixBinIterator::prev() {
	m_currentIter--;
}

//Check if the last element has been checked
bool SampleMatrixBinCollectionImpl::InnerMatrixBinIterator::isDone() {
	return (m_currentIter == m_pBinMap->end());
}

MatrixBinPtr SampleMatrixBinCollectionImpl::InnerMatrixBinIterator::getObj() {
	return m_currentIter->second;
}



