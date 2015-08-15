/*
 * SimpleIdentityMatrixImpl.cpp
 *
 *  Created on: May 16, 2015
 *      Author: pham
 */

#include "SimpleIdentityMatrixImpl.h"

SimpleIdentityMatrixImpl::SimpleIdentityMatrixImpl(int dimension) : m_dimension(dimension) {

}

//Override
void SimpleIdentityMatrixImpl::getSize(int& rows, int& columns) const {
	rows = m_dimension;
	columns = m_dimension;
}

//Override
ComplexVal SimpleIdentityMatrixImpl::getValue(int row, int column) const {
	return (row == column) ? 1.0 : 0.0;
}

//Override
void SimpleIdentityMatrixImpl::toArray(ComplexValArrayRef array) const {
	int arraySize = m_dimension * m_dimension;
	array = new ComplexVal[arraySize];
	for(int i = 0; i < arraySize; i++) {
		int row = i / m_dimension;
		int column = i % m_dimension;
		array[i] = (row == column) ? 1.0 : 0.0;
	}
}

//Override
std::string SimpleIdentityMatrixImpl::getLabel() const {
	return std::string("I");
}

MatrixPtr SimpleIdentityMatrixImpl::clone() const {
	return MatrixPtr(new SimpleIdentityMatrixImpl(m_dimension));
}



