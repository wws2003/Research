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
#include "SampleApproximatorContainerImpl.h"
#include "EvaluateCollectionCommand.h"
#include "EvaluateApproximatorCommand.h"
#include "ConfigReader.h"

CommandFactory::CommandFactory() : m_pCollectionContainer(NullPtr), m_pApproximatorContainer(NullPtr), m_pEvaluatorContainer(NullPtr){
}

CommandFactory::~CommandFactory() {
	_destroy(m_pCollectionContainer);
	_destroy(m_pEvaluatorContainer);
}

CommandPtr CommandFactory::getCommand(int commandCode, const CommandParams& commandParams) {
	ConfigReader configReader;

	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;
	NearIdentityApproximatorConfig approximatorConfig;

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
	case EVALUATE_COLLECTION_APPROXIMATOR_TO_IDENTITY:
	{
		configReader.readCollectionAndEvaluatorConfig(commandParams[0], &collectionConfig, &evaluatorConfig);
		configReader.readApproximatorConfig(commandParams[1], &approximatorConfig);
		fillApproximatorConfig(collectionConfig.m_librarySet, collectionConfig.m_nbQubits, &approximatorConfig);

		resetCollectionContainer(collectionConfig);
		resetApproximatorContainer(approximatorConfig);
		resetEvaluationContainer(evaluatorConfig);

		AbstractCommandPtr pRootCommand = AbstractCommandPtr(new EvaluateCollectionCommand(m_pCollectionContainer, m_pEvaluatorContainer));
		CommandPtr pApproximatorCommand = CommandPtr(new EvaluateApproximatorCommand(m_pApproximatorContainer,
				m_pCollectionContainer,
				m_pEvaluatorContainer));
		pRootCommand->setSuccessor(pApproximatorCommand);
		return pRootCommand;
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

void CommandFactory::resetApproximatorContainer(NearIdentityApproximatorConfig approximatorConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new SampleApproximatorContainerImpl(approximatorConfig));
}

void CommandFactory::fillApproximatorConfig(LibrarySet librarySet, int nbQubits, NearIdentityApproximatorConfig* pApproximatorConfig) {
	pApproximatorConfig->m_librarySet = librarySet;
	pApproximatorConfig->m_nbQubits = nbQubits;
}


