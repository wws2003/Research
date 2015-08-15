/*
 * IAppContainer.h
 *
 *  Created on: Aug 15, 2015
 *      Author: pham
 */

#ifndef IAPPCONTAINER_H_
#define IAPPCONTAINER_H_

#include "Gate.h"
#include "ICollection.h"
#include "AlgoCommon.h"
#include "ISearchSpaceEvaluator.h"
#include "EvaluateCommon.h"

/**
 * Returns concrete instances for collection, evaluator...
 */
class IAppContainer {
public:
	virtual ~IAppContainer(){};

	//TODO May add some user config params / config file name
	virtual GateCollectionPtr getGateCollection() = 0;

	virtual GateSearchSpaceEvaluatorPtr getGateSearchSpaceEvaluator() = 0;

	virtual void recycle(GateCollectionPtr& rpGateCollection) = 0;

	virtual void recycle(GateSearchSpaceEvaluatorPtr& rpGateSearchSpaceEvaluator) = 0;
};

typedef IAppContainer* AppContainerPtr;

#endif /* IAPPCONTAINER_H_ */
