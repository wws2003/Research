/*
 * ITargetMatrixFactory.h
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#ifndef ITARGETMATRIXFACTORY_H_
#define ITARGETMATRIXFACTORY_H_

#include "common/Common.h"

class ITargetMatrixFactory {
public:
	virtual ~ITargetMatrixFactory(){};

	virtual MatrixPtr generateTargetMatrixForSearch(void* pArgument) = 0;
};

#endif /* ITARGETMATRIXFACTORY_H_ */
