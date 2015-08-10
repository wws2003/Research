/*
 * IMatrix.cpp
 *
 *  Created on: Aug 10, 2015
 *      Author: pham
 */

#include "IMatrix.h"

//Check if 2 matrices have the same number of rows and columns
bool sameSize(IMatrix& lhs, const IMatrix& rhs);

//Check if 2 matrices have the same value for each coordinate
bool sameData(IMatrix& lhs, const IMatrix& rhs);

//Check if 2 matrices have the same label / one or both of them have no label
bool sameLabel(IMatrix& lhs, const IMatrix& rhs);

bool IMatrix::operator==(const IMatrix& rhs) {
	return sameSize(*this, rhs) && sameData(*this, rhs) && sameLabel(*this, rhs);
}

bool sameSize(IMatrix& lhs, const IMatrix& rhs) {
	int nbRows1, nbColumns1, nbRows2, nbColumns2;
	lhs.getSize(nbRows1, nbColumns1);
	rhs.getSize(nbRows2, nbColumns2);
	return nbRows1 == nbRows2 && nbColumns1 == nbColumns2;
}

bool sameData(IMatrix& lhs, const IMatrix& rhs) {
	int nbRows, nbColumns;
	lhs.getSize(nbRows, nbColumns);
	for(int r = 0; r < nbRows; r++) {
		for(int c = 0; c < nbColumns; c++) {
			if(lhs.getValue(r,c) != rhs.getValue(r,c)) {
				return false;
			}
		}
	}
	return true;
}

bool sameLabel(IMatrix& lhs, const IMatrix& rhs) {
	std::string label1 = lhs.getLabel();
	std::string label2 = rhs.getLabel();
	return (label1.empty() || label2.empty() || label1.compare(label2) == 0);
}
