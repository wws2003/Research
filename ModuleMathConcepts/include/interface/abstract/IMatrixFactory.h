/*
 * IMatrixFactory.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXFACTORY_H_
#define IMATRIXFACTORY_H_

#include "IMatrix.h"

class IMatrixFactory {
public:
	virtual ~IMatrixFactory(){};

	virtual MatrixPtr getMatrixFromValues(int row, int columns, const ComplexVal* pVals = NullPtr, const ArraySpliceType arraySpliceType = ROW_SPLICE, const std::string label = "") = 0;

	virtual MatrixPtr getIdentityMatrix(int dimension) = 0;
};

#endif /* IMATRIXFACTORY_H_ */
