/*
 * ISquareMatrix.h
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#ifndef ISQUAREMATRIX_H_
#define ISQUAREMATRIX_H_

#include "IMatrix.h"

class ISquareMatrix : public IMatrix {
public:

	//Override
	void getSize(int& row, int& columns) final;

	virtual int getSize() = 0;
};


#endif /* ISQUAREMATRIX_H_ */
