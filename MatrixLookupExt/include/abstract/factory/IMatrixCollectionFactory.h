/*
 * IMatrixCollectionFactory.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXCOLLECTIONFACTORY_H_
#define IMATRIXCOLLECTIONFACTORY_H_

#include "abstract/basic/IMatrixCollection.h"

class IMatrixCollectionFactory {
public:
	virtual ~IMatrixCollectionFactory(){};

	virtual MatrixCollectionPtr generateEmptyMatrixCollectionPtr() = 0;

	virtual void recycle(MatrixCollectionPtr pCollection);
};


#endif /* IMATRIXCOLLECTIONFACTORY_H_ */
