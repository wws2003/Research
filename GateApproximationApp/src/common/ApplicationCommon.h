/*
 * ApplicationCommon.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef APPLICATIONCOMMON_H_
#define APPLICATIONCOMMON_H_

#include "MathConceptsCommon.h"
#include <map>
#include <string>

class ICommand;
class ICollectionContainer;
class IApproximatorContainer;
class IEvaluatorContainer;

typedef ICommand* CommandPtr;
typedef ICollectionContainer* CollectionContainerPtr;
typedef IApproximatorContainer* ApproximatorContainerPtr;
typedef IEvaluatorContainer* EvaluatorContainerPtr;

typedef std::map<std::string, int> LibrarySetNameMap;
typedef std::map<int, std::string> LibrarySetFileNameMap;

enum CommandCode {
	UNKNOWN_COMMAND,
	GENERATE_NEAR_IDENTITY,
	EVALUATE_COLLECTION_TO_IDENTITY,
	EVALUATE_COLLECTION_APPROXIMATOR_TO_IDENTITY,
	//TODO Add more command code
};

#endif /* APPLICATIONCOMMON_H_ */
