/*
 * IVector.h
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#ifndef IVECTOR_H_
#define IVECTOR_H_

#include "common/Common.h"
#include "ILabelable.h"

class IVector: public ILabelable {
public:
	virtual ~IVector(){};

	virtual int getSize() = 0;

	virtual ComplexVal &operator[] (int index) = 0;

	virtual void toArray(ComplexVal* array) = 0;
};



#endif /* IVECTOR_H_ */
