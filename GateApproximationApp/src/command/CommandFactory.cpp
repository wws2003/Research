/*
 * CommandFactory.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#include "CommandFactory.h"
#include "NotAvailableCommand.h"
#include "SampleCollectionContainerImpl.h"
#include "SampleEvaluatorContainerImpl.h"
#include "EvaluateCollectionCommand.h"
#include "ConfigReader.h"

CommandFactory::CommandFactory()  : m_pCollectionContainer(NullPtr), m_pEvaluatorContainer(NullPtr){
}

CommandFactory::~CommandFactory() {
	_destroy(m_pCollectionContainer);
	_destroy(m_pEvaluatorContainer);
}

CommandPtr CommandFactory::getCommand(int commandCode, const CommandParams& commandParams) {
	ConfigReader configReader;
	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;

	switch (commandCode) {
	case UNKNOWN_COMMAND:
		return NullPtr;

	case EVALUATE_COLLECTION_TO_IDENTITY:
	{
		configReader.readCollectionAndEvaluatorConfig(commandParams[0], &collectionConfig, &evaluatorConfig);
		resetCollectionContainer(collectionConfig);
		resetEvaluationContainer(evaluatorConfig);
		CommandPtr pCommand = CommandPtr(new EvaluateCollectionCommand(m_pCollectionContainer, m_pEvaluatorContainer));
		return pCommand;
	}
	default:
		return CommandPtr(new NotAvailableCommand());
	}
}

void CommandFactory::resetCollectionContainer(CollectionConfig collectionConfig) {
	_destroy(m_pCollectionContainer);
	m_pCollectionContainer = CollectionContainerPtr(new SampleCollectionContainerImpl(collectionConfig));
}

void CommandFactory::resetEvaluationContainer(EvaluatorConfig evaluatorConfig) {
	_destroy(m_pEvaluatorContainer);
	m_pEvaluatorContainer = EvaluatorContainerPtr(new SampleEvaluatorContainerImpl(evaluatorConfig));
}
