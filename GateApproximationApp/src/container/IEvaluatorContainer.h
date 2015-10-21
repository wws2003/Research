/*
 * IEvaluatorContainer.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef IEVALUATORCONTAINER_H_
#define IEVALUATORCONTAINER_H_

#include "AlgoCommon.h"
#include "EvaluateCommon.h"
#include "Gate.h"

class IEvaluatorContainer {
public:
	virtual ~IEvaluatorContainer(){};

	virtual GateSearchSpaceEvaluatorPtr getGateSearchSpaceEvaluator() = 0;
};



#endif /* IEVALUATORCONTAINER_H_ */
