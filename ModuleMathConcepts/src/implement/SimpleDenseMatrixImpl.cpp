/*
 * SimpleDenseMatrixImpl.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#include "SimpleDenseMatrixImpl.h"

SimpleDenseMatrixImpl::SimpleDenseMatrixImpl(const ComplexVal* array, int nbRows, int nbColumns, std::string label): m_label(label) {
	initArray(array, nbRows, nbColumns);
}

SimpleDenseMatrixImpl::~SimpleDenseMatrixImpl() {
	delete m_array;
}

//Override
void SimpleDenseMatrixImpl::getSize(int& rows, int& columns) {
	rows = m_nbRows;
	columns = m_nbColumns;
}

//Override
ComplexVal SimpleDenseMatrixImpl::getValue(int row, int column) {
	return m_array[row * m_nbColumns + column];
}

//Override
void SimpleDenseMatrixImpl::toArray(ComplexValArrayRef array) {
	int arraySize = m_nbRows * m_nbColumns;
	array = new ComplexVal[arraySize];
	for(int i = 0; i < arraySize; i++) {
		array[i] = m_array[i];
	}
}

std::string SimpleDenseMatrixImpl::getLabel() {
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
