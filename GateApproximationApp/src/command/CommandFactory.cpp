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
#include "SKApproximatorContainerImpl.h"
#include "SK2ApproximatorContainerImpl.h"
#include "ConfigReader.h"

CommandFactory::CommandFactory() : m_pCollectionContainer(NullPtr), m_pApproximatorContainer(NullPtr), m_pEvaluatorContainer(NullPtr){
}

CommandFactory::~CommandFactory() {
	_destroy(m_pCollectionContainer);
	_destroy(m_pEvaluatorContainer);
	_destroy(m_pApproximatorContainer);
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
		return getGenerateAndStoreApproximationsCommandForIdentity(commandParams[0], commandParams[1], commandParams[2]);
	}
	case EVALUATE_PERSISTED_COLLECTION_TO_TARGET: {
		return getPersistedCollectionEvaluationCommandForTargets(commandParams[0], commandParams[1]);
	}
	case EVALUATE_SK_APPROXIMATOR_TO_TARGET: {
		return getSKApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2]);
	}
	case EVALUATE_SK2_APPROXIMATOR_TO_TARGET: {
			return getSK2ApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2]);
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

AbstractCommandPtr CommandFactory::getGenerateAndStoreApproximationsCommandForIdentity(std::string evaluationConfigFile,
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

AbstractCommandPtr CommandFactory::getPersistedCollectionEvaluationCommandForTargets(std::string storeFileName, std::string targetConfigFile) {
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

AbstractCommandPtr CommandFactory::getSKApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string skApprxConfigFile, std::string targetConfigFile) {
	ConfigReader configReader;

	CollectionConfig collectionConfig;
	SKApproximatorConfig skApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	readSKConfig(configReader,
			collectionConfigFile,
			&collectionConfig,
			skApprxConfigFile,
			&skApproximatorConfig,
			targetConfigFile,
			&evaluatorConfig);

	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pApproximatorEvaluationCommand = AbstractCommandPtr(new EvaluateApproximatorCommand(pApproximator,
			pCollection,
			pEvaluator));

	return pApproximatorEvaluationCommand;
}

AbstractCommandPtr CommandFactory::getSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string skApprxConfigFile, std::string targetConfigFile) {
	ConfigReader configReader;

	CollectionConfig collectionConfig;
	SKApproximatorConfig2 skApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	readSK2Config(configReader,
			collectionConfigFile,
			&collectionConfig,
			skApprxConfigFile,
			&skApproximatorConfig,
			targetConfigFile,
			&evaluatorConfig);

	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pApproximatorEvaluationCommand = AbstractCommandPtr(new EvaluateApproximatorCommand(pApproximator,
			pCollection,
			pEvaluator));

	return pApproximatorEvaluationCommand;
}


void CommandFactory::readCollectionConfig(ConfigReader configReader, std::string configFile, CollectionConfig* pCollectionConfig) {
	configReader.readCollectionConfig(configFile, pCollectionConfig);
	resetCollectionContainer(*pCollectionConfig);
}

void CommandFactory::readCollectionAndEvaluatorConfig(ConfigReader configReader, std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig) {
	configReader.readCollectionAndEvaluatorConfig(configFile, pCollectionConfig, pEvaluatorConfig);
	resetCollectionContainer(*pCollectionConfig);
	resetEvaluationContainer(*pEvaluatorConfig, *pCollectionConfig);
}

void CommandFactory::readApproximatorConfig(ConfigReader configReader, std::string configFile, const CollectionConfig&  collectionConfig, NearIdentityApproximatorConfig* pApproximatorConfig) {
	configReader.readNearIdentityApproximatorConfig(configFile, pApproximatorConfig);
	resetApproximatorContainer(*pApproximatorConfig, collectionConfig);
}

void CommandFactory::readTargetConfig(ConfigReader configReader, std::string targetConfigFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig) {
	configReader.readTargetsConfig(targetConfigFile, pCollectionConfig, pEvaluatorConfig);

	resetCollectionContainer(*pCollectionConfig);
	resetEvaluationContainer(*pEvaluatorConfig, *pCollectionConfig);
}

void CommandFactory::readSKConfig(ConfigReader configReader,
		std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
		std::string skApprxConfigFile, SKApproximatorConfig* pApproximatorConfig,
		std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig) {

	configReader.readCollectionConfig(collectionConfigFile, pCollectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, pEvaluatorConfig);
	configReader.readSKApproximatorConfig(skApprxConfigFile, pApproximatorConfig);

	resetCollectionContainer(*pCollectionConfig);
	resetSKApproximatorContainer(*pApproximatorConfig, *pCollectionConfig);
	resetEvaluationContainer(*pEvaluatorConfig, *pCollectionConfig);
}

void CommandFactory::readSK2Config(ConfigReader configReader,
		std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
		std::string skApprxConfigFile, SKApproximatorConfig2* pApproximatorConfig,
		std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig) {

	configReader.readCollectionConfig(collectionConfigFile, pCollectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, pEvaluatorConfig);
	configReader.readSKApproximatorConfig2(skApprxConfigFile, pApproximatorConfig);

	resetCollectionContainer(*pCollectionConfig);
	resetSK2ApproximatorContainer(*pApproximatorConfig, *pCollectionConfig);
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

void CommandFactory::resetSKApproximatorContainer(const SKApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new SKApproximatorContainerImpl(approximatorConfig, collectionConfig));
}

void CommandFactory::resetSK2ApproximatorContainer(const SKApproximatorConfig2& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new SK2ApproximatorContainerImpl(approximatorConfig, collectionConfig));

}
