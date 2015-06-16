/*
 * SampleMatrixCollectionImpl.cpp
 *
 *  Created on: Apr 28, 2015
 *      Author: pham
 */

#include "SampleMatrixCollectionImpl.h"
#include "VectorBasedReadOnlyIteratorImpl.hpp"
#include "VectorBasedIteratorImpl.hpp"
#include "IIterator.h"

SampleMatrixCollectionImpl::SampleMatrixCollectionImpl() {
	m_pMatrixIterator = MatrixIteratorPtr(new VectorBasedIteratorImpl<MatrixPtr>(&m_matrices));
}

SampleMatrixCollectionImpl::~SampleMatrixCollectionImpl() {
	_destroy(m_pMatrixIterator);
}

void SampleMatrixCollectionImpl::addMatrix(MatrixPtr pm) {
	m_matrices.push_back(pm);
}

void SampleMatrixCollectionImpl::clear() {
	m_matrices.clear();
}

MatrixIteratorPtr SampleMatrixCollectionImpl::getReadonlyIteratorPtr() {
	MatrixPtrVector pResults;
	pResults.insert(pResults.begin(), m_matrices.begin(), m_matrices.end());
	MatrixIteratorPtr pResultIter = MatrixIteratorPtr(new VectorBasedReadOnlyIteratorImpl<MatrixPtr>(pResults));
	return pResultIter;
}

//Return iterator through a set of matrix reflecting the changes in the collection
MatrixIteratorPtr SampleMatrixCollectionImpl::getIteratorPtr() {
	return m_pMatrixIterator;
}

MatrixCollectionSize_t SampleMatrixCollectionImpl::size() const {
	return (MatrixCollectionSize_t)m_matrices.size();
}

MatrixIteratorPtr SampleMatrixCollectionImpl::findApproxMatrices(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const {
	MatrixPtrVector pResults;

	size_t collectionSize = m_matrices.size();
	for(size_t j = 0; j < collectionSize; j++) {
		MatrixPtr pCandidate = m_matrices[j];
		if(pDistanceCalculator->distance(pQuery, pCandidate) <= epsilon) {
			pResults.push_back(pCandidate);
		}
	}

	MatrixIteratorPtr pResultIter = MatrixIteratorPtr(new VectorBasedReadOnlyIteratorImpl<MatrixPtr>(pResults));
	return pResultIter;
}

