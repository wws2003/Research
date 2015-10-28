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
#include "GenerateAndStoreApproximationsCommand.h"
#include "EvaluatePersistedCollectionForTargetsCommand.h"
#include "ConfigReader.h"

CommandFactory::CommandFactory() : m_pCollectionContainer(NullPtr), m_pApproximatorContainer(NullPtr), m_pEvaluatorContainer(NullPtr){
}

CommandFactory::~CommandFactory() {
	_destroy(m_pCollectionContainer);
	_destroy(m_pEvaluatorContainer);
}

CommandPtr CommandFactory::getCommand(int commandCode, const CommandParams& commandParams) {
	switch (commandCode) {
	case UNKNOWN_COMMAND:
		return NullPtr;
	case EVALUATE_COLLECTION_TO_IDENTITY:
	{
		return getCollectionEvaluationCommandForIdentity(commandParams[0]);
	}
	case EVALUATE_COLLECTION_APPROXIMATOR_TO_IDENTITY:
	{
		return getApproximatorEvaluationCommandForIdentity(commandParams[0], commandParams[1]);
	}
	case GENERATE_NEAR_IDENTITY: {
		return getGenerateAndStoreApproximationsForIdentity(commandParams[0], commandParams[1], commandParams[2]);
	}
	case EVALUATE_PERSISTED_COLLECTION_TO_TARGET: {
		return getPersistedCollectionEvaluationForTargets(commandParams[0], commandParams[1]);
	}
	default:
		return CommandPtr(new NotAvailableCommand());
	}
}

AbstractCommandPtr CommandFactory::getCollectionEvaluationCommandForIdentity(std::string configFileName) {
	ConfigReader configReader;

	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;

	readCollectionAndEvaluatorConfig(configReader,
			configFileName,
			&collectionConfig,
			&evaluatorConfig);

	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();

	AbstractCommandPtr pCommand = AbstractCommandPtr(new EvaluateCollectionCommand(m_pCollectionContainer->getGateCollection(pGateDistanceCalculator),
			m_pEvaluatorContainer->getGateSearchSpaceEvaluator()));
	return pCommand;
}

AbstractCommandPtr CommandFactory::getApproximatorEvaluationCommandForIdentity(std::string evaluationConfigFile, std::string approximatorConfigFileName) {
	ConfigReader configReader;

	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;
	readCollectionAndEvaluatorConfig(configReader,
			evaluationConfigFile,
			&collectionConfig,
			&evaluatorConfig);

	NearIdentityApproximatorConfig approximatorConfig;
	readApproximatorConfig(configReader,
			approximatorConfigFileName,
			collectionConfig,
			&approximatorConfig);

	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pGateCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pRootCommand = AbstractCommandPtr(new EvaluateCollectionCommand(pGateCollection,
			m_pEvaluatorContainer->getGateSearchSpaceEvaluator()));
	CommandPtr pApproximatorCommand = CommandPtr(new EvaluateApproximatorCommand(m_pApproximatorContainer->getGateApproximator(),
			pGateCollection,
			m_pEvaluatorContainer->getGateSearchSpaceEvaluator()));

	pRootCommand->setSuccessor(pApproximatorCommand);
	return pRootCommand;
}

AbstractCommandPtr CommandFactory::getGenerateAndStoreApproximationsForIdentity(std::string evaluationConfigFile,
		std::string approximatorConfigFileName,
		std::string storeFileName) {

	ConfigReader configReader;

	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;
	readCollectionAndEvaluatorConfig(configReader, evaluationConfigFile, &collectionConfig, &evaluatorConfig);

	NearIdentityApproximatorConfig approximatorConfig;
	readApproximatorConfig(configReader, approximatorConfigFileName, collectionConfig, &approximatorConfig);

	std::vector<GatePtr> targets;
	m_pEvaluatorContainer->getTargetsForEvaluation(targets);
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();

	AbstractCommandPtr pCommand = AbstractCommandPtr(new GenerateAndStoreApproximationsCommand(m_pApproximatorContainer->getGateApproximator(),
			m_pCollectionContainer->getGateCollection(pGateDistanceCalculator),
			pGateDistanceCalculator,
			evaluatorConfig.m_approximatorEpsilon,
			targets,
			m_pCollectionContainer->getPersitableGateCollection(),
			storeFileName));

	return pCommand;
}

AbstractCommandPtr CommandFactory::getPersistedCollectionEvaluationForTargets(std::string storeFileName, std::string targetConfigFile) {
	ConfigReader configReader;

	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;
	readTargetConfig(configReader, targetConfigFile, &collectionConfig, &evaluatorConfig);

	PersitableGateCollectionPtr pPersistableCollection = m_pCollectionContainer->getPersitableGateCollection();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();

	return AbstractCommandPtr(new EvaluatePersistedCollectionForTargetsCommand(pPersistableCollection,
			pEvaluator,
			storeFileName));
}

void CommandFactory::readCollectionAndEvaluatorConfig(ConfigReader configReader, std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig) {
	configReader.readCollectionAndEvaluatorConfig(configFile, pCollectionConfig, pEvaluatorConfig);
	resetCollectionContainer(*pCollectionConfig);
	resetEvaluationContainer(*pEvaluatorConfig, *pCollectionConfig);
}

void CommandFactory::readApproximatorConfig(ConfigReader configReader, std::string configFile, const CollectionConfig&  collectionConfig, NearIdentityApproximatorConfig* pApproximatorConfig) {
	configReader.readApproximatorConfig(configFile, pApproximatorConfig);
	resetApproximatorContainer(*pApproximatorConfig, collectionConfig);
}

void CommandFactory::readTargetConfig(ConfigReader configReader, std::string targetConfigFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig) {
	configReader.readTargetsConfig(targetConfigFile, pCollectionConfig, pEvaluatorConfig);

	resetCollectionContainer(*pCollectionConfig);
	resetEvaluationContainer(*pEvaluatorConfig, *pCollectionConfig);
}

void CommandFactory::resetCollectionContainer(const CollectionConfig& collectionConfig) {
	_destroy(m_pCollectionContainer);
	m_pCollectionContainer = CollectionContainerPtr(new SampleCollectionContainerImpl(collectionConfig));
}

void CommandFactory::resetEvaluationContainer(const EvaluatorConfig& evaluatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pEvaluatorContainer);
	m_pEvaluatorContainer = EvaluatorContainerPtr(new SampleEvaluatorContainerImpl(evaluatorConfig, collectionConfig));
}

void CommandFactory::resetApproximatorContainer(const NearIdentityApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new SampleApproximatorContainerImpl(approximatorConfig, collectionConfig));
}
