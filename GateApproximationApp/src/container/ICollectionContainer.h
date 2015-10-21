/*
 * ICollectionContainer.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef ICOLLECTIONCONTAINER_H_
#define ICOLLECTIONCONTAINER_H_

#include "ICollection.h"
#include "AlgoCommon.h"
#include "Gate.h"

class ICollectionContainer {
public:
	virtual ~ICollectionContainer(){};

	virtual GateCollectionPtr getGateCollection() = 0;
};


#endif /* ICOLLECTIONCONTAINER_H_ */
