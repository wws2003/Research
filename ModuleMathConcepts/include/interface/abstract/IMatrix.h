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

	virtual void toArray(ComplexValArrayRef array) const = 0;

};


#endif /* IMATRIX_H_ */
