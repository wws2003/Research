/*
 * IGateCombinabilityCheckerFactory.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pham
 */

#ifndef IGATECOMBINABILITYCHECKERFACTORY_H_
#define IGATECOMBINABILITYCHECKERFACTORY_H_

#include "ResourcesCommon.h"
#include "IGateCombinabilityChecker.h"

class IGateCombinabilityCheckerFactory {
public:
	virtual ~IGateCombinabilityCheckerFactory(){};

	virtual void getGateCombinabilityCheckers(R_LibrarySet librarySet, GateCombinabilityCheckers& checkers) = 0;
};


#endif /* IGATECOMBINABILITYCHECKERFACTORY_H_ */
