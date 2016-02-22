/*
 * IComposerContainer.h
 *
 *  Created on: Feb 22, 2016
 *      Author: pham
 */

#ifndef ICOMPOSERCONTAINER_H_
#define ICOMPOSERCONTAINER_H_

#include "ApplicationCommon.h"
#include "IComposer.h"
#include "AlgoCommon.h"
#include "Gate.h"

class IComposerContainer {
public:
	virtual ~IComposerContainer(){};

	virtual GateComposerPtr getGateComposer() = 0;
};



#endif /* ICOMPOSERCONTAINER_H_ */
