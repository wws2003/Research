/*
 * IMatrix.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIX_H_
#define IMATRIX_H_

#include "MathConceptsCommon.h"
#include "ILabelable.h"
#include "string"

class IMatrix : public ILabelable {
public:
	virtual ~IMatrix(){};

	virtual void getSize(int& row, int& columns) const = 0 ;

	virtual ComplexVal getValue(int row, int column)const = 0;

	//Slice the matrix into an array, by matrix row
	virtual void toArray(ComplexValArrayRef array) const = 0;

	//Overload
	bool operator==(const IMatrix& rhs);
};


#endif /* IMATRIX_H_ */
