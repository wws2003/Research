/*
 * IMatrix.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIX_H_
#define IMATRIX_H_

#include "common/Common.h"
#include "ILabelable.h"
#include "string"

class IMatrix : public ILabelable {
public:
	virtual ~IMatrix(){};

	virtual void getSize(int& row, int& columns) = 0;

	virtual ComplexVal getValue(int row, int column) = 0;

};


#endif /* IMATRIX_H_ */
