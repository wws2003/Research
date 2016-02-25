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
#include "EvaluateComposerCommand.h"
#include "ComposerBasedApproximatorContainer.h"
#include "IComposer.h"
#include "SKApproximatorContainerImpl.h"
#include "SK2ApproximatorContainerImpl.h"
#include "ComposerBasedSK2ApproximatorContainerImpl.h"
#include "SampleEvaluatingComposerContainerImpl.h"
#include "SimpleComposerContainerImpl.h"
#include "GateCoordinateAdditionBasedComposerContainerImpl.h"
#include "SampleComposerEvaluatorContainerImpl.h"
#include "EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl.h"
#include "EvaluatingParallelComposerContainerImpl.h"
#include "ConfigReader.h"
#include <cstdio>

CommandFactory::CommandFactory() : m_pCollectionContainer(NullPtr),
m_pApproximatorContainer(NullPtr),
m_pEvaluatorContainer(NullPtr),
m_pComposerContainer(NullPtr),
m_pEvaluatingComposerContainer(NullPtr),
m_pComposerEvaluatorContainer(NullPtr){
}

CommandFactory::~CommandFactory() {
	_destroy(m_pCollectionContainer);
	_destroy(m_pEvaluatorContainer);
	_destroy(m_pComposerContainer);
	_destroy(m_pApproximatorContainer);
	_destroy(m_pEvaluatingComposerContainer);
	_destroy(m_pComposerEvaluatorContainer);
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
	case EVALUATE_CB_APPROXIMATOR_TO_TARGET: {
		return getComposerBasedApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2]);
	}
	case EVALUATE_CB2_APPROXIMATOR_TO_TARGET: {
		return getComposerBasedApproximator2EvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3]);
	}
	case EVALUATE_CB_SK2_APPROXIMATOR_TO_TARGET: {
		return getComposerBasedSK2ApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3], commandParams[4]);
	}
	case EVALUATE_SK_APPROXIMATOR_TO_TARGET: {
		return getSKApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2]);
	}
	case EVALUATE_SK2_APPROXIMATOR_TO_TARGET: {
		return getSK2ApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3]);
	}
	case EVALUATE_COMPOSER_TO_TARGET: {
		return getComposerEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3], false);
	}
	case EVALUATE_COMPOSER2_TO_TARGET: {
		return getComposerEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3], true);
	}
	case EVALUATE_PARALLEL_COMPOSER_TO_TARGET: {
		return getParallelComposerEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3]);
	}
	case EVALUATE_PARALLEL_COMPOSER2_TO_TARGET: {
		return getParallelComposerEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3], commandParams[4]);
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

	readTargetConfig(configReader, targetConfigFile);

	PersitableGateCollectionPtr pPersistableCollection = m_pCollectionContainer->getPersitableGateCollection();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();

	return AbstractCommandPtr(new EvaluatePersistedCollectionForTargetsCommand(pPersistableCollection,
			pEvaluator,
			storeFileName));
}

AbstractCommandPtr CommandFactory::getComposerBasedApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string cbApprxConfigFile, std::string targetConfigFile) {
	ConfigReader configReader;

	readComposerBasedApproximatorConfig(configReader,
			collectionConfigFile,
			cbApprxConfigFile,
			targetConfigFile);

	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pApproximatorEvaluationCommand = AbstractCommandPtr(new EvaluateApproximatorCommand(pApproximator,
			pCollection,
			pEvaluator));

	return pApproximatorEvaluationCommand;
}

AbstractCommandPtr CommandFactory::getSKApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string skApprxConfigFile, std::string targetConfigFile) {
	ConfigReader configReader;

	readSKConfig(configReader,
			collectionConfigFile,
			skApprxConfigFile,
			targetConfigFile);

	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pApproximatorEvaluationCommand = AbstractCommandPtr(new EvaluateApproximatorCommand(pApproximator,
			pCollection,
			pEvaluator));

	return pApproximatorEvaluationCommand;
}

AbstractCommandPtr CommandFactory::getSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string skApprxConfigFile,
		std::string cadbComposerConfigFile,
		std::string targetConfigFile) {

	ConfigReader configReader;

	readSK2Config(configReader,
			collectionConfigFile,
			cadbComposerConfigFile,
			skApprxConfigFile,
			targetConfigFile);

	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pApproximatorEvaluationCommand = AbstractCommandPtr(new EvaluateApproximatorCommand(pApproximator,
			pCollection,
			pEvaluator));

	return pApproximatorEvaluationCommand;
}

AbstractCommandPtr CommandFactory::getComposerBasedSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string skApprxConfigFile,
		std::string cadbConfigFile,
		std::string targetConfigFile) {

	ConfigReader configReader;

	readComposerBasedSK2Config(configReader,
			collectionConfigFile,
			cadbConfigFile,
			skApprxConfigFile,
			cbApprxConfigFile,
			targetConfigFile);

	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pApproximatorEvaluationCommand = AbstractCommandPtr(new EvaluateApproximatorCommand(pApproximator,
			pCollection,
			pEvaluator));

	return pApproximatorEvaluationCommand;
}

AbstractCommandPtr CommandFactory::getComposerBasedApproximator2EvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string cadbComposerConfigFile,
		std::string targetConfigFile) {
	ConfigReader configReader;

	readComposerBasedApproximatorConfig(configReader,
			collectionConfigFile,
			cbApprxConfigFile,
			targetConfigFile,
			cadbComposerConfigFile);

	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pApproximatorEvaluationCommand = AbstractCommandPtr(new EvaluateApproximatorCommand(pApproximator,
			pCollection,
			pEvaluator));

	return pApproximatorEvaluationCommand;
}

AbstractCommandPtr CommandFactory::getComposerEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string adbComposerConfigFile,
		std::string targetConfigFile,
		bool multiComparatorMode) {

	ConfigReader configReader;

	readComposerEvaluationConfig(configReader,
			collectionConfigFile,
			composerEvalConfigFile,
			targetConfigFile,
			adbComposerConfigFile,
			multiComparatorMode);

	GateComposerEvaluatorPtr pGateComposerEvaluator = m_pComposerEvaluatorContainer->getGateComposerEvaluator();
	GateComposerPtr pEvaluatedComposer = m_pEvaluatingComposerContainer->getEvaluatedGateComposer();
	GateComposerPtr pStandardComposer = m_pEvaluatingComposerContainer->getStandardGateComposer();

	AbstractCommandPtr pEvaluateCommand = AbstractCommandPtr(new EvaluateComposerCommand(pEvaluatedComposer,
			pStandardComposer,
			pGateComposerEvaluator));

	return pEvaluateCommand;
}

AbstractCommandPtr CommandFactory::getParallelComposerEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string nbThreadStr,
		std::string targetConfigFile) {
	ConfigReader configReader;

	readParallelComposerEvaluationConfig(configReader,
			collectionConfigFile,
			cbApprxConfigFile,
			nbThreadStr,
			targetConfigFile);

	GateComposerEvaluatorPtr pGateComposerEvaluator = m_pComposerEvaluatorContainer->getGateComposerEvaluator();
	GateComposerPtr pEvaluatedComposer = m_pEvaluatingComposerContainer->getEvaluatedGateComposer();
	GateComposerPtr pStandardComposer = m_pEvaluatingComposerContainer->getStandardGateComposer();

	AbstractCommandPtr pEvaluateCommand = AbstractCommandPtr(new EvaluateComposerCommand(pEvaluatedComposer,
			pStandardComposer,
			pGateComposerEvaluator));

	return pEvaluateCommand;
}

AbstractCommandPtr CommandFactory::getParallelComposerEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string adbComposerConfigFile,
		std::string nbThreadStr,
		std::string targetConfigFile) {

	ConfigReader configReader;

	readParallelComposerEvaluationConfig(configReader,
			collectionConfigFile,
			cbApprxConfigFile,
			adbComposerConfigFile,
			nbThreadStr,
			targetConfigFile);

	GateComposerEvaluatorPtr pGateComposerEvaluator = m_pComposerEvaluatorContainer->getGateComposerEvaluator();
	GateComposerPtr pEvaluatedComposer = m_pEvaluatingComposerContainer->getEvaluatedGateComposer();
	GateComposerPtr pStandardComposer = m_pEvaluatingComposerContainer->getStandardGateComposer();

	AbstractCommandPtr pEvaluateCommand = AbstractCommandPtr(new EvaluateComposerCommand(pEvaluatedComposer,
			pStandardComposer,
			pGateComposerEvaluator));

	return pEvaluateCommand;
}

//Below are methods to read config files then instantiate proper containers for dependencies

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

void CommandFactory::readTargetConfig(ConfigReader configReader, std::string targetConfigFile) {
	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;

	configReader.readTargetsConfig(targetConfigFile, &collectionConfig, &evaluatorConfig);

	resetCollectionContainer(collectionConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
}

void CommandFactory::readComposerBasedApproximatorConfig(ConfigReader configReader,
		std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string targetConfigFile) {

	CollectionConfig collectionConfig;
	ComposerBasedApproximatorConfig cbApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, &evaluatorConfig);
	configReader.readComposerBasedApproximatorConfig(cbApprxConfigFile, &cbApproximatorConfig);

	resetCollectionContainer(collectionConfig);
	resetComposerBasedApproximatorContainer(cbApproximatorConfig, collectionConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
}

void CommandFactory::readComposerBasedApproximatorConfig(ConfigReader configReader,
		std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string targetConfigFile,
		std::string cadbConfigFile) {

	CollectionConfig collectionConfig;
	ComposerBasedApproximatorConfig cbApproximatorConfig;
	EvaluatorConfig evaluatorConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, &evaluatorConfig);
	configReader.readComposerBasedApproximatorConfig(cbApprxConfigFile, &cbApproximatorConfig);
	configReader.readCoordinateAddtionalBasedComposerConfig(cadbConfigFile, &cadbConfig);

	resetCollectionContainer(collectionConfig);
	resetComposerBasedApproximatorContainer(cbApproximatorConfig, cadbConfig, collectionConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
}

void CommandFactory::readComposerEvaluationConfig(ConfigReader configReader,
		std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string targetConfigFile,
		std::string cadbConfigFile,
		bool multiComparatorMode) {

	CollectionConfig collectionConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;
	ComposerEvaluatorConfig composerEvalConfig;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);
	if(multiComparatorMode) {
		configReader.readMultiComparatorCoordinateAddtionalBasedComposerConfig(cadbConfigFile, &cadbConfig);
	}
	else {
		configReader.readCoordinateAddtionalBasedComposerConfig(cadbConfigFile, &cadbConfig);
	}
	configReader.readComposerEvaluatorConfig(composerEvalConfigFile, &composerEvalConfig);
	configReader.readComposerEvaluatorConfigFromTargets(targetConfigFile, &composerEvalConfig);

	resetEvaluatingComposerContainer(cadbConfig, collectionConfig);
	resetComposerEvaluatorContainer(composerEvalConfig, collectionConfig);
}

void CommandFactory::readParallelComposerEvaluationConfig(ConfigReader configReader,
		std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string nbThreadStr,
		std::string targetConfigFile) {

	CollectionConfig collectionConfig;
	ComposerEvaluatorConfig composerEvalConfig;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);

	configReader.readComposerEvaluatorConfig(composerEvalConfigFile, &composerEvalConfig);
	configReader.readComposerEvaluatorConfigFromTargets(targetConfigFile, &composerEvalConfig);

	int nbThreads = 2;
	sscanf(nbThreadStr.data(), "%d", &nbThreads);
	resetEvaluatingComposerContainer(nbThreads);
	resetComposerEvaluatorContainer(composerEvalConfig, collectionConfig);
}

void CommandFactory::readParallelComposerEvaluationConfig(ConfigReader configReader,
		std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string cadbConfigFile,
		std::string nbThreadStr,
		std::string targetConfigFile) {

	CollectionConfig collectionConfig;
	ComposerEvaluatorConfig composerEvalConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;

	configReader.readMultiComparatorCoordinateAddtionalBasedComposerConfig(cadbConfigFile, &cadbConfig);
	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);

	configReader.readComposerEvaluatorConfig(composerEvalConfigFile, &composerEvalConfig);
	configReader.readComposerEvaluatorConfigFromTargets(targetConfigFile, &composerEvalConfig);

	int nbThreads = 2;
	sscanf(nbThreadStr.data(), "%d", &nbThreads);
	resetEvaluatingComposerContainer(cadbConfig, collectionConfig, nbThreads);
	resetComposerEvaluatorContainer(composerEvalConfig, collectionConfig);
}

void CommandFactory::readSKConfig(ConfigReader configReader,
		std::string collectionConfigFile,
		std::string skApprxConfigFile,
		std::string targetConfigFile) {

	CollectionConfig collectionConfig;
	SKApproximatorConfig skApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, &evaluatorConfig);
	configReader.readSKApproximatorConfig(skApprxConfigFile, &skApproximatorConfig);

	resetCollectionContainer(collectionConfig);
	resetSKApproximatorContainer(skApproximatorConfig, collectionConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
}

void CommandFactory::readSK2Config(ConfigReader configReader,
		std::string collectionConfigFile,
		std::string cadbComposerConfigFile,
		std::string skApprxConfigFile,
		std::string targetConfigFile) {

	CollectionConfig collectionConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;
	SKApproximatorConfig skApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	configReader.readMultiComparatorCoordinateAddtionalBasedComposerConfig(cadbComposerConfigFile, &cadbConfig);
	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, &evaluatorConfig);
	configReader.readSKApproximatorConfig(skApprxConfigFile, &skApproximatorConfig);

	resetCollectionContainer(collectionConfig);
	resetSK2ApproximatorContainer(skApproximatorConfig, collectionConfig, cadbConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
}

void CommandFactory::readComposerBasedSK2Config(ConfigReader configReader,
		std::string collectionConfigFile,
		std::string cadbComposerConfigFile,
		std::string skApprxConfigFile,
		std::string cbApprxConfigFile,
		std::string targetConfigFile) {

	CollectionConfig collectionConfig;
	ComposerBasedApproximatorConfig cbApproximatorConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;
	SKApproximatorConfig skApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);
	configReader.readMultiComparatorCoordinateAddtionalBasedComposerConfig(cadbComposerConfigFile, &cadbConfig);
	configReader.readComposerBasedApproximatorConfig(cbApprxConfigFile, &cbApproximatorConfig);
	configReader.readSKApproximatorConfig(skApprxConfigFile, &skApproximatorConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, &evaluatorConfig);

	resetCollectionContainer(collectionConfig);
	resetComposerBasedSK2ApproximatorContainer(skApproximatorConfig, cbApproximatorConfig, collectionConfig, cadbConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
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

void CommandFactory::resetComposerContainer() {
	_destroy(m_pComposerContainer);
	m_pComposerContainer = ComposerContainerPtr(new SimpleComposerContainerImpl());
}

void CommandFactory::resetComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig) {
	_destroy(m_pComposerContainer);
	m_pComposerContainer = ComposerContainerPtr(new GateCoordinateAdditionBasedComposerContainerImpl(cabConfig, collectionConfig));
}

void CommandFactory::resetComposerBasedApproximatorContainer(const ComposerBasedApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new ComposerBasedApproximatorContainer(approximatorConfig, collectionConfig));
}

void CommandFactory::resetComposerBasedApproximatorContainer(const ComposerBasedApproximatorConfig& approximatorConfig, const CoordinateAdditionalBasedComposerConfig& cadbConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new ComposerBasedApproximatorContainer(approximatorConfig, cadbConfig, collectionConfig));
}

void CommandFactory::resetEvaluatingComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pEvaluatingComposerContainer);
	m_pEvaluatingComposerContainer = EvaluatingComposerContainerPtr(new SampleEvaluatingComposerContainerImpl(cabConfig, collectionConfig));
}

void CommandFactory::resetEvaluatingComposerContainer(int nbThreads) {
	_destroy(m_pEvaluatingComposerContainer);
	m_pEvaluatingComposerContainer = EvaluatingComposerContainerPtr(new EvaluatingParallelComposerContainerImpl(nbThreads));
}

void CommandFactory::resetEvaluatingComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig,
		int nbThreads) {
	_destroy(m_pEvaluatingComposerContainer);
	m_pEvaluatingComposerContainer = EvaluatingComposerContainerPtr(new EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl(cabConfig, collectionConfig, nbThreads));
}

void CommandFactory::resetComposerEvaluatorContainer(const ComposerEvaluatorConfig& composerEvalConfig, const CollectionConfig& collectionCofig) {
	_destroy(m_pComposerEvaluatorContainer);
	m_pComposerEvaluatorContainer = ComposerEvaluatorContainerPtr(new SampleComposerEvaluatorContainerImpl(composerEvalConfig, collectionCofig));
}

void CommandFactory::resetSKApproximatorContainer(const SKApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new SKApproximatorContainerImpl(approximatorConfig, collectionConfig));
}

void CommandFactory::resetSK2ApproximatorContainer(const SKApproximatorConfig& approximatorConfig,
		const CollectionConfig& collectionConfig,
		const CoordinateAdditionalBasedComposerConfig& cadbConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new SK2ApproximatorContainerImpl(approximatorConfig, collectionConfig, cadbConfig));
}

void CommandFactory::resetComposerBasedSK2ApproximatorContainer(const SKApproximatorConfig& skApproximatorConfig,
		const ComposerBasedApproximatorConfig& approximatorConfig,
		const CollectionConfig& collectionConfig,
		const CoordinateAdditionalBasedComposerConfig& cadbConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new ComposerBasedSK2ApproximatorContainerImpl(skApproximatorConfig, collectionConfig, cadbConfig, approximatorConfig));
}
