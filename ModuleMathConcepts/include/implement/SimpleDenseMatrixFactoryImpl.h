/*
 * SimpleDenseMatrixFactoryImpl.h
 *
 *  Created on: May 13, 2015
 *      Author: pham
 */

#ifndef SIMPLEDENSEMATRIXFACTORYIMPL_H_
#define SIMPLEDENSEMATRIXFACTORYIMPL_H_

#include "IMatrixFactory.h"

class SimpleDenseMatrixFactoryImpl: public IMatrixFactory {
public:
	virtual MatrixPtr getMatrixFromValues(int row, int columns, const ComplexVal* pVals = NullPtr, const ArraySpliceType arraySpliceType = ROW_SPLICE, const std::string label = "");

	virtual MatrixPtr getIdentityMatrix(int dimension);
};


#endif /* SIMPLEDENSEMATRIXFACTORYIMPL_H_ */
