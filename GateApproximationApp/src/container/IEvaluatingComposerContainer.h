/*
 * IEvaluatingComposerContainer.h
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#ifndef IEVALUATINGCOMPOSERCONTAINER_H_
#define IEVALUATINGCOMPOSERCONTAINER_H_

#include "IComposer.h"
#include "Gate.h"
#include "AlgoCommon.h"

class IEvaluatingComposerContainer {
public:
	virtual ~IEvaluatingComposerContainer(){};

	virtual GateComposerPtr getStandardGateComposer() = 0;

	virtual GateComposerPtr getEvaluatedGateComposer() = 0;
};


#endif /* ICOMPOSERCONTAINER_H_ */
