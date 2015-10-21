/*
 * ApplicationCommon.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef APPLICATIONCOMMON_H_
#define APPLICATIONCOMMON_H_

#include "MathConceptsCommon.h"

class ICommandExecutor;
class ICollectionContainer;
class IApproximatorContainer;
class IEvaluatorContainer;

typedef ICommandExecutor* CommandExecutorPtr;
typedef ICollectionContainer* CollectionContainerPtr;
typedef IApproximatorContainer* ApproximatorContainerPtr;
typedef IEvaluatorContainer* EvaluatorContainerPtr;

#endif /* APPLICATIONCOMMON_H_ */
