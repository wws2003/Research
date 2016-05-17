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
#include "IPersistableCollection.h"
#include "IDistanceCalculator.h"
#include "Gate.h"

class ICollectionContainer {
public:
	virtual ~ICollectionContainer(){};

	virtual GateCollectionPtr getGateCollection(GateDistanceCalculatorPtr pGateDistanceCalculator) = 0;
};


#endif /* ICOLLECTIONCONTAINER_H_ */
