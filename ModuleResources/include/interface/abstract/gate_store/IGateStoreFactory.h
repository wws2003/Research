/*
 * IGateStoreFactory.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pham
 */

#ifndef IGATESTOREFACTORY_H_
#define IGATESTOREFACTORY_H_

#include "ResourcesCommon.h"
#include "IGateStore.h"

class IGateStoreFactory {
public:
	virtual ~IGateStoreFactory(){};

	virtual GateStorePtr getGateStore(int nbQubits) = 0;
};



#endif /* IGATESTOREFACTORY_H_ */
