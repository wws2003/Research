/*
 * ILibraryMatrixStore.h
 *
 *  Created on: Oct 20, 2015
 *      Author: pham
 */

#ifndef ILIBRARYMATRIXSTORE_H_
#define ILIBRARYMATRIXSTORE_H_

#include "MathConceptsCommon.h"
#include "OperatorCommon.h"
#include "IMatrix.h"

class ILibaryMatrixMatrixStore {
public:
	virtual ~ILibaryMatrixMatrixStore(){};

	virtual MatrixPtr getLibraryMatrixFromLabel(std::string label) = 0;
};



#endif /* IMATRIXSTORE_H_ */
