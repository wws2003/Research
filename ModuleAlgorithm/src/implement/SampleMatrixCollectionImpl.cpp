/*
 * SampleMatrixCollectionImpl.cpp
 *
 *  Created on: Apr 28, 2015
 *      Author: pham
 */

#include "SampleMatrixCollectionImpl.h"
#include "IIterator.h"

SampleMatrixCollectionImpl::SampleMatrixCollectionImpl() {
	m_pMatrixIterator = MatrixIteratorPtr(new SampleMatrixCollectionImpl::InnerVectorMatrixIterator(m_pMatrices));
}

SampleMatrixCollectionImpl::~SampleMatrixCollectionImpl() {

}

void SampleMatrixCollectionImpl::addMatrix(MatrixPtr pm) {
	m_pMatrices.push_back(pm);
}

void SampleMatrixCollectionImpl::clear() {
	m_pMatrices.clear();
}

MatrixIteratorPtr SampleMatrixCollectionImpl::getReadonlyIteratorPtr() {
	MatrixPtrVector pResults;
	pResults.insert(pResults.begin(), m_pMatrices.begin(), m_pMatrices.end());
	MatrixIteratorPtr pResultIter = MatrixIteratorPtr(new InnerVectorMatrixIterator(pResults));
	return pResultIter;
}

//Return iterator through a set of matrix reflecting the changes in the collection
MatrixIteratorPtr SampleMatrixCollectionImpl::getIteratorPtr() {
	return m_pMatrixIterator;
}

MatrixCollectionSize_t SampleMatrixCollectionImpl::size() const {
	return (MatrixCollectionSize_t)m_pMatrices.size();
}

MatrixIteratorPtr SampleMatrixCollectionImpl::findApproxMatrices(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const {
	MatrixPtrVector pResults;
	MatrixIteratorPtr pResultIter = MatrixIteratorPtr(new InnerVectorMatrixIterator(pResults));

	size_t collectionSize = m_pMatrices.size();
	for(size_t j = 0; j < collectionSize; j++) {
		MatrixPtr pCandidate = m_pMatrices[j];
		if(pDistanceCalculator->distance(pQuery, pCandidate) <= epsilon) {
			pResults.push_back(pCandidate);
		}
	}

	return pResultIter;
}

SampleMatrixCollectionImpl::InnerVectorMatrixIterator::InnerVectorMatrixIterator(MatrixPtrVector& pMatrices) : m_counter(0), m_pMatrices(pMatrices) {

}

void SampleMatrixCollectionImpl::InnerVectorMatrixIterator::toBegin() {
	m_counter = 0;
}

void SampleMatrixCollectionImpl::InnerVectorMatrixIterator::next() {
	m_counter++;
}

void SampleMatrixCollectionImpl::InnerVectorMatrixIterator::prev() {
	m_counter--;
}

bool SampleMatrixCollectionImpl::InnerVectorMatrixIterator::isDone() {
	return m_counter >= (m_pMatrices.size() - 1);
}

MatrixPtr SampleMatrixCollectionImpl::InnerVectorMatrixIterator::getObj() {
	return m_pMatrices[m_counter];
}


