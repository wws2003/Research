/*
 * IComposerContainer.h
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#ifndef ICOMPOSERCONTAINER_H_
#define ICOMPOSERCONTAINER_H_

#include "IComposer.h"
#include "Gate.h"
#include "AlgoCommon.h"

class IComposerContainer {
public:
	virtual ~IComposerContainer(){};

	virtual GateComposerPtr getStandardGateComposer() = 0;

	virtual GateComposerPtr getEvaluatedGateComposer() = 0;
};


#endif /* ICOMPOSERCONTAINER_H_ */
