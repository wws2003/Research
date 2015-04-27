/*
 * ISquareMatrix.cpp
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#include "ISquareMatrix.h"

void ISquareMatrix::getSize(int& row, int& columns) {
	int size = getSize();

	row = size;
	columns = size;
}


