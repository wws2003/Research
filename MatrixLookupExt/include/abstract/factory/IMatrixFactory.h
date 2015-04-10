/*
 * IMatrixFactory.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXFACTORY_H_
#define IMATRIXFACTORY_H_

#include "abstract/basic/IMatrix.h"

class IMatrixFactory {
public:
	virtual ~IMatrixFactory(){};

	virtual MatrixPtr getMatrix(int row, int columns, const ComplexVal** ppVals) = 0;

	virtual MatrixPtr getIdentityMatrix(int dimension) = 0;
};

#endif /* IMATRIXFACTORY_H_ */
