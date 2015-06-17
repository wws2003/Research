/*
 * VectorizationMatrixCollectionImpl.cpp
 *
 *  Created on: May 22, 2015
 *      Author: pham
 */

#include "VectorizationMatrixCollectionImpl.h"
#include "OperatorCommon.h"
#include "IDistanceCalculator.h"
#include "Coordinate.hpp"

void matrixVectorFromCoordinateIterator(MatrixRealCoordinateIteratorPtr pMatrixCoordinateIterator, MatrixPtrVector& matrices);

VectorizationMatrixCollectionImpl::VectorizationMatrixCollectionImpl(MatrixRealCoordinateCalculatorPtr pMatrixCoordinateCalculator, MatrixRealCoordinateCollectionPtr pInternalMatrixCoordinateCollection, MatrixRealCoordinateDistanceCalculatorPtr pMatrixCoordinateDistanceCalculator) {
	m_pMatrixCoordinateCalculator = pMatrixCoordinateCalculator;
	m_pInternalMatrixCoordinateCollection = pInternalMatrixCoordinateCollection;
	m_pMatrixCoordinateDistanceCalculator = pMatrixCoordinateDistanceCalculator;
}

void VectorizationMatrixCollectionImpl::addElement(MatrixPtr pm) {
	MatrixRealCoordinatePtr pCoordinate = NullPtr;
	m_pMatrixCoordinateCalculator->calulateElementCoordinate(pm, pCoordinate);
	m_pInternalMatrixCoordinateCollection->addElement(pCoordinate);
}

void VectorizationMatrixCollectionImpl::clear() {
	m_pInternalMatrixCoordinateCollection->clear();
}

MatrixIteratorPtr VectorizationMatrixCollectionImpl::getReadonlyIteratorPtr() {
	MatrixRealCoordinateIteratorPtr pMatrixCoordinateIter = m_pInternalMatrixCoordinateCollection->getReadonlyIteratorPtr();
	MatrixPtrVector pMatrices;
	matrixVectorFromCoordinateIterator(pMatrixCoordinateIter, pMatrices);

	return new InnerVectorMatrixIterator(pMatrices);
}

//Return iterator through a set of matrix reflecting the changes in the collection
MatrixIteratorPtr VectorizationMatrixCollectionImpl::getIteratorPtr() {
	MatrixRealCoordinateIteratorPtr pMatrixCoordinateIter = m_pInternalMatrixCoordinateCollection->getIteratorPtr();
	MatrixPtrVector pMatrices;
	matrixVectorFromCoordinateIterator(pMatrixCoordinateIter, pMatrices);

	return new InnerVectorMatrixIterator(pMatrices);
}

MatrixCollectionSize_t VectorizationMatrixCollectionImpl::size() const {
	return m_pInternalMatrixCoordinateCollection->size();
}

MatrixIteratorPtr VectorizationMatrixCollectionImpl::findApproxElements(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const  {

	MatrixRealCoordinatePtr pTargetCoordinate = NullPtr;
	m_pMatrixCoordinateCalculator->calulateElementCoordinate(pQuery, pTargetCoordinate);
	MatrixRealCoordinateIteratorPtr pApproxMatrixCoordinateIter = m_pInternalMatrixCoordinateCollection->findApproxElements(pTargetCoordinate, m_pMatrixCoordinateDistanceCalculator, epsilon);

	pApproxMatrixCoordinateIter->toBegin();

	MatrixPtrVector pResultMatrices;

	matrixVectorFromCoordinateIterator(pApproxMatrixCoordinateIter, pResultMatrices);

	return new InnerVectorMatrixIterator(pResultMatrices);
}

VectorizationMatrixCollectionImpl::InnerVectorMatrixIterator::InnerVectorMatrixIterator(const MatrixPtrVector& pMatrices) : m_counter(0) {
	m_pMatrices.clear();
	m_pMatrices.insert(m_pMatrices.end(), pMatrices.begin(), pMatrices.end());
}

VectorizationMatrixCollectionImpl::InnerVectorMatrixIterator::~InnerVectorMatrixIterator() {

}

void VectorizationMatrixCollectionImpl::InnerVectorMatrixIterator::toBegin() {
	m_counter = 0;
}

void VectorizationMatrixCollectionImpl::InnerVectorMatrixIterator::next() {
	m_counter++;
}

void VectorizationMatrixCollectionImpl::InnerVectorMatrixIterator::prev() {
	m_counter--;
}

bool VectorizationMatrixCollectionImpl::InnerVectorMatrixIterator::isDone() {
	return m_counter > (counter_t)m_pMatrices.size() - 1;
}

MatrixPtr VectorizationMatrixCollectionImpl::InnerVectorMatrixIterator::getObj() {
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



