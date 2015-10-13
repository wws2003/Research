/*
 * VectorizationMatrixCollectionImpl.cpp
 *
 *  Created on: May 22, 2015
 *      Author: pham
 */

#include "CoordinateBasedMatrixCollectionImpl.h"
#include "OperatorCommon.h"
#include "IDistanceCalculator.h"
#include "Coordinate.hpp"

void matrixVectorFromCoordinateIterator(MatrixRealCoordinateIteratorPtr pMatrixCoordinateIterator, MatrixPtrVector& matrices);

CoordinateBasedMatrixCollectionImpl::CoordinateBasedMatrixCollectionImpl(MatrixRealCoordinateCalculatorPtr pMatrixCoordinateCalculator, MatrixRealCoordinateCollectionPtr pInternalMatrixCoordinateCollection, MatrixRealCoordinateDistanceCalculatorPtr pMatrixCoordinateDistanceCalculator) {
	m_pMatrixCoordinateCalculator = pMatrixCoordinateCalculator;
	m_pInternalMatrixCoordinateCollection = pInternalMatrixCoordinateCollection;
	m_pMatrixCoordinateDistanceCalculator = pMatrixCoordinateDistanceCalculator;
}

void CoordinateBasedMatrixCollectionImpl::addElement(MatrixPtr pm) {
	MatrixRealCoordinatePtr pCoordinate = NullPtr;
	m_pMatrixCoordinateCalculator->calulateElementCoordinate(pm, pCoordinate);
	m_pInternalMatrixCoordinateCollection->addElement(pCoordinate);
}

void CoordinateBasedMatrixCollectionImpl::clear() {
	m_pInternalMatrixCoordinateCollection->clear();
}

MatrixIteratorPtr CoordinateBasedMatrixCollectionImpl::getReadonlyIteratorPtr() {
	MatrixRealCoordinateIteratorPtr pMatrixCoordinateIter = m_pInternalMatrixCoordinateCollection->getReadonlyIteratorPtr();
	MatrixPtrVector pMatrices;
	matrixVectorFromCoordinateIterator(pMatrixCoordinateIter, pMatrices);

	return new InnerVectorMatrixIterator(pMatrices);
}

//Return iterator through a set of matrix reflecting the changes in the collection
MatrixIteratorPtr CoordinateBasedMatrixCollectionImpl::getIteratorPtr() {
	MatrixRealCoordinateIteratorPtr pMatrixCoordinateIter = m_pInternalMatrixCoordinateCollection->getIteratorPtr();
	MatrixPtrVector pMatrices;
	matrixVectorFromCoordinateIterator(pMatrixCoordinateIter, pMatrices);

	return new InnerVectorMatrixIterator(pMatrices);
}

MatrixCollectionSize_t CoordinateBasedMatrixCollectionImpl::size() const {
	return m_pInternalMatrixCoordinateCollection->size();
}

MatrixIteratorPtr CoordinateBasedMatrixCollectionImpl::findApproxElements(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, mreal_t epsilon) const  {

	MatrixRealCoordinatePtr pTargetCoordinate = NullPtr;
	m_pMatrixCoordinateCalculator->calulateElementCoordinate(pQuery, pTargetCoordinate);
	MatrixRealCoordinateIteratorPtr pApproxMatrixCoordinateIter = m_pInternalMatrixCoordinateCollection->findNearestNeighbour(pTargetCoordinate, m_pMatrixCoordinateDistanceCalculator, epsilon);

	pApproxMatrixCoordinateIter->toBegin();

	MatrixPtrVector pResultMatrices;

	matrixVectorFromCoordinateIterator(pApproxMatrixCoordinateIter, pResultMatrices);

	return new InnerVectorMatrixIterator(pResultMatrices);
}

CoordinateBasedMatrixCollectionImpl::InnerVectorMatrixIterator::InnerVectorMatrixIterator(const MatrixPtrVector& pMatrices) : m_counter(0) {
	m_pMatrices.clear();
	m_pMatrices.insert(m_pMatrices.end(), pMatrices.begin(), pMatrices.end());
}

CoordinateBasedMatrixCollectionImpl::InnerVectorMatrixIterator::~InnerVectorMatrixIterator() {

}

void CoordinateBasedMatrixCollectionImpl::InnerVectorMatrixIterator::toBegin() {
	m_counter = 0;
}

void CoordinateBasedMatrixCollectionImpl::InnerVectorMatrixIterator::next() {
	m_counter++;
}

void CoordinateBasedMatrixCollectionImpl::InnerVectorMatrixIterator::prev() {
	m_counter--;
}

bool CoordinateBasedMatrixCollectionImpl::InnerVectorMatrixIterator::isDone() {
	return m_counter > (counter_t)m_pMatrices.size() - 1;
}

MatrixPtr CoordinateBasedMatrixCollectionImpl::InnerVectorMatrixIterator::getObj() {
	return m_pMatrices[m_counter];
}

void matrixVectorFromCoordinateIterator(MatrixRealCoordinateIteratorPtr pMatrixCoordinateIterator, MatrixPtrVector& pMatrices) {
	pMatrixCoordinateIterator->toBegin();

	while(!pMatrixCoordinateIterator->isDone()) {
		MatrixRealCoordinatePtr pMatrixCoordinate = pMatrixCoordinateIterator->getObj();

		pMatrices.push_back(pMatrixCoordinate->getElement());

		pMatrixCoordinateIterator->next();
	}

	pMatrixCoordinateIterator->toBegin();
}



