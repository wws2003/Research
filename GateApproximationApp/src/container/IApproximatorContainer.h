/*
 * IApproximatorContainer.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef IAPPROXIMATORCONTAINER_H_
#define IAPPROXIMATORCONTAINER_H_

#include "AlgoCommon.h"
#include "Gate.h"

class IApproximatorContainer {
public:
	virtual ~IApproximatorContainer(){};

	virtual GateApproximatorPtr getGateApproximator() = 0;
};



#endif /* IAPPROXIMATORCONTAINER_H_ */
