/*
 * SimpleDenseMatrixImpl.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#include "SimpleDenseMatrixImpl.h"

#define NOISE_THRESHOLD 1e-16

//Erase noised zero value such as 1e-16, 1e-17
void cleanNoise(ComplexVal* array, int length);

SimpleDenseMatrixImpl::SimpleDenseMatrixImpl(const ComplexVal* array, ArraySpliceType arraySpliceType, int nbRows, int nbColumns, std::string label): m_label(label) {
	initArray(array, nbRows, nbColumns);
	m_arraySpliceType = arraySpliceType;
}

SimpleDenseMatrixImpl::~SimpleDenseMatrixImpl() {
	delete[] m_array;
}

//Override
void SimpleDenseMatrixImpl::getSize(int& rows, int& columns) const {
	rows = m_nbRows;
	columns = m_nbColumns;
}

//Override
ComplexVal SimpleDenseMatrixImpl::getValue(int row, int column) const {
	int index = getInternalIndex(row, column);
	return m_array[index];
}

//Override
void SimpleDenseMatrixImpl::toArray(ComplexValArrayRef array) const {
	int arraySize = m_nbRows * m_nbColumns;
	array = new ComplexVal[arraySize];
	for(int i = 0; i < arraySize; i++) {
		int row = i / m_nbColumns;
		int column = i % m_nbColumns;
		int internalIndex = getInternalIndex(row, column);
		array[i] = m_array[internalIndex];
	}
}

std::string SimpleDenseMatrixImpl::getLabel() const {
	return m_label;
}

void SimpleDenseMatrixImpl::initArray(const ComplexVal* array, int nbRows, int nbColumns) {
	int arraySize = nbRows * nbColumns;
	m_array = new ComplexVal[arraySize];
	for(int i = 0; i < arraySize; i++) {
		m_array[i] = array[i];
	}
	cleanNoise(m_array, arraySize);
	m_nbColumns = nbColumns;
	m_nbRows = nbRows;
}

inline int SimpleDenseMatrixImpl::getInternalIndex(int row, int column) const {
	return (m_arraySpliceType == ROW_SPLICE) ? (row * m_nbColumns + column) : (column * m_nbRows + row);
}

void cleanNoise(ComplexVal* array, int length) {
	for(int i = 0; i < length; i++) {
		if(std::abs(array[i].real()) < NOISE_THRESHOLD) {
			array[i].real(0.0);
		}
		if(std::abs(array[i].imag()) < NOISE_THRESHOLD) {
			array[i].imag(0.0);
		}
	}
}
