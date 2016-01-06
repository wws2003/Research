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
class AbstractChainCommand;

class ICollectionContainer;
class IApproximatorContainer;
class IEvaluatorContainer;
class IWriterContainer;

typedef ICommand* CommandPtr;
typedef AbstractChainCommand* AbstractCommandPtr;

typedef ICollectionContainer* CollectionContainerPtr;
typedef IApproximatorContainer* ApproximatorContainerPtr;
typedef IEvaluatorContainer* EvaluatorContainerPtr;
typedef IWriterContainer* WriterContainerPtr;

typedef std::map<std::string, int> LibrarySetNameMap;
typedef std::map<int, std::string> LibrarySetFileNameMap;
typedef std::map<std::string, int> RotationTypeNameMap;

enum CommandCode {
	UNKNOWN_COMMAND,
	GENERATE_NEAR_IDENTITY,
	EVALUATE_COLLECTION_TO_IDENTITY,
	EVALUATE_COLLECTION_APPROXIMATOR_TO_IDENTITY,
	EVALUATE_PERSISTED_COLLECTION_TO_TARGET,
	EVALUATE_CB_APPROXIMATOR_TO_TARGET,
	EVALUATE_SK_APPROXIMATOR_TO_TARGET,
	EVALUATE_SK2_APPROXIMATOR_TO_TARGET,
	EVALUATE_CB_SK2_APPROXIMATOR_TO_TARGET
	//TODO Add more command code
};

#endif /* APPLICATIONCOMMON_H_ */
