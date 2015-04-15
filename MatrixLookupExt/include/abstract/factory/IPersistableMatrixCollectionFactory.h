/*
 * IPersistableMatrixCollectionFactory.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IPERSISTABLEMATRIXCOLLECTIONFACTORY_H_
#define IPERSISTABLEMATRIXCOLLECTIONFACTORY_H_

#include "abstract/basic/algo/IPersistableMatrixCollection.h"

class IPersistableMatrixCollectionFactory {
public:
	virtual ~IPersistableMatrixCollectionFactory(){};

	virtual PersistableMatrixCollectionPtr generateEmptyPersistableMatrixCollectionPtr() = 0;

	virtual void recycle(PersistableMatrixCollectionPtr pCollection);
};


#endif /* IPERSISTABLEMATRIXCOLLECTIONFACTORY_H_ */
