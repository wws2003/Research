/*
 * SimpleDenseMatrixImpl.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#include "SimpleDenseMatrixImpl.h"

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
	int index = (m_arraySpliceType == ROW_SPLICE) ? (row * m_nbColumns + column) : (column * m_nbRows + row);
	return m_array[index];
}

//Override
void SimpleDenseMatrixImpl::toArray(ComplexValArrayRef array) const {
	int arraySize = m_nbRows * m_nbColumns;
	array = new ComplexVal[arraySize];
	for(int i = 0; i < arraySize; i++) {
		array[i] = m_array[i];
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
	m_nbColumns = nbColumns;
	m_nbRows = nbRows;
}
