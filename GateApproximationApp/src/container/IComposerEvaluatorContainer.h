/*
 * IComposerEvaluatorContainer.h
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#ifndef ICOMPOSEREVALUATORCONTAINER_H_
#define ICOMPOSEREVALUATORCONTAINER_H_

#include "IComposerEvaluator.h"
#include "IComposer.h"
#include "EvaluateCommon.h"

class IComposerEvaluatorContainer {
public:
	virtual ~IComposerEvaluatorContainer(){};

	virtual GateComposerEvaluatorPtr getGateComposerEvaluator() = 0;

};


#endif /* ICOMPOSEREVALUATORCONTAINER_H_ */
