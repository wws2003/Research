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
class IEvaluatingComposerContainer;
class IComposerEvaluatorContainer;
class IComposerContainer;

typedef ICommand* CommandPtr;
typedef AbstractChainCommand* AbstractCommandPtr;

typedef ICollectionContainer* CollectionContainerPtr;
typedef IApproximatorContainer* ApproximatorContainerPtr;
typedef IEvaluatorContainer* EvaluatorContainerPtr;
typedef IWriterContainer* WriterContainerPtr;
typedef IEvaluatingComposerContainer* EvaluatingComposerContainerPtr;
typedef IComposerEvaluatorContainer* ComposerEvaluatorContainerPtr;
typedef IComposerContainer* ComposerContainerPtr;

typedef std::map<std::string, int> LibrarySetNameMap;
typedef std::map<std::string, int> RotationTypeNameMap;

enum CommandCode {
	UNKNOWN_COMMAND,
	EVALUATE_COLLECTION_TO_IDENTITY,
	EVALUATE_CB_APPROXIMATOR_TO_TARGET,
	EVALUATE_SK_APPROXIMATOR_TO_TARGET,
	EVALUATE_SK2_APPROXIMATOR_TO_TARGET,
	EVALUATE_CB_SK2_APPROXIMATOR_TO_TARGET,
	EVALUATE_CB2_APPROXIMATOR_TO_TARGET,
	EVALUATE_COMPOSER_TO_TARGET,
	EVALUATE_COMPOSER2_TO_TARGET,
	EVALUATE_PARALLEL_COMPOSER_TO_TARGET,
	EVALUATE_PARALLEL_COMPOSER2_TO_TARGET,
	//TODO Add more command code
};

#endif /* APPLICATIONCOMMON_H_ */
