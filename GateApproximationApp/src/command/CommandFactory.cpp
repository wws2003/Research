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
#include "SampleComposerContainerImpl.h"
#include "SampleComposerEvaluatorContainerImpl.h"
#include "ConfigReader.h"

CommandFactory::CommandFactory() : m_pCollectionContainer(NullPtr),
m_pApproximatorContainer(NullPtr),
m_pEvaluatorContainer(NullPtr),
m_pComposerContainer(NullPtr),
m_pComposerEvaluatorContainer(NullPtr){
}

CommandFactory::~CommandFactory() {
	_destroy(m_pCollectionContainer);
	_destroy(m_pEvaluatorContainer);
	_destroy(m_pApproximatorContainer);
	_destroy(m_pComposerContainer);
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
	case EVALUATE_SK_APPROXIMATOR_TO_TARGET: {
		return getSKApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2]);
	}
	case EVALUATE_SK2_APPROXIMATOR_TO_TARGET: {
		return getSK2ApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2]);
	}
	case EVALUATE_CB_SK2_APPROXIMATOR_TO_TARGET: {
		return getComposerBasedSK2ApproximatorEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3]);
	}
	case EVALUATE_CB2_APPROXIMATOR_TO_TARGET: {
		return getComposerBasedApproximator2EvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3]);
	}
	case EVALUATE_COMPOSER_TO_TARGET: {
		return getComposerEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3], false);
	}
	case EVALUATE_COMPOSER2_TO_TARGET: {
		return getComposerEvaluationCommandForTargets(commandParams[0], commandParams[1], commandParams[2], commandParams[3], true);
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

AbstractCommandPtr CommandFactory::getComposerBasedApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string cbApprxConfigFile, std::string targetConfigFile) {
	ConfigReader configReader;

	CollectionConfig collectionConfig;
	ComposerBasedApproximatorConfig cbApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	readComposerBasedApproximatorConfig(configReader,
			collectionConfigFile,
			&collectionConfig,
			cbApprxConfigFile,
			&cbApproximatorConfig,
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

AbstractCommandPtr CommandFactory::getComposerBasedSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string skApprxConfigFile,
		std::string targetConfigFile) {
	ConfigReader configReader;

	//Reset collection container, helper approximator container
	CollectionConfig collectionConfig;
	ComposerBasedApproximatorConfig cbApproximatorConfig;
	SKApproximatorConfig2 skApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	readComposerBasedSK2Config(configReader,
			collectionConfigFile,
			&collectionConfig,
			skApprxConfigFile,
			&skApproximatorConfig,
			cbApprxConfigFile,
			&cbApproximatorConfig,
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

AbstractCommandPtr CommandFactory::getComposerBasedApproximator2EvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string cadbComposerConfigFile,
		std::string targetConfigFile) {
	ConfigReader configReader;

	CollectionConfig collectionConfig;
	ComposerBasedApproximatorConfig cbApproximatorConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;
	EvaluatorConfig evaluatorConfig;

	readComposerBasedApproximatorConfig(configReader,
			collectionConfigFile,
			&collectionConfig,
			cbApprxConfigFile,
			&cbApproximatorConfig,
			targetConfigFile,
			&evaluatorConfig,
			cadbComposerConfigFile,
			&cadbConfig);

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

	CollectionConfig collectionConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;
	ComposerEvaluatorConfig composerEvalConfig;

	readComposerEvaluationConfig(configReader,
			collectionConfigFile,
			&collectionConfig,
			composerEvalConfigFile,
			targetConfigFile,
			&composerEvalConfig,
			adbComposerConfigFile,
			&cadbConfig,
			multiComparatorMode);

	GateComposerEvaluatorPtr pGateComposerEvaluator = m_pComposerEvaluatorContainer->getGateComposerEvaluator();
	GateComposerPtr pEvaluatedComposer = m_pComposerContainer->getEvaluatedGateComposer();
	GateComposerPtr pStandardComposer = m_pComposerContainer->getStandardGateComposer();

	AbstractCommandPtr pEvaluateCommand = AbstractCommandPtr(new EvaluateComposerCommand(pEvaluatedComposer,
			pStandardComposer,
			pGateComposerEvaluator));

	return pEvaluateCommand;
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

void CommandFactory::readComposerBasedApproximatorConfig(ConfigReader configReader,
		std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
		std::string cbApprxConfigFile, ComposerBasedApproximatorConfig* pApproximatorConfig,
		std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig) {

	configReader.readCollectionConfig(collectionConfigFile, pCollectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, pEvaluatorConfig);
	configReader.readComposerBasedApproximatorConfig(cbApprxConfigFile, pApproximatorConfig);

	resetCollectionContainer(*pCollectionConfig);
	resetComposerBasedApproximatorContainer(*pApproximatorConfig, *pCollectionConfig);
	resetEvaluationContainer(*pEvaluatorConfig, *pCollectionConfig);
}

void CommandFactory::readComposerBasedApproximatorConfig(ConfigReader configReader,
		std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
		std::string cbApprxConfigFile, ComposerBasedApproximatorConfig* pApproximatorConfig,
		std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig,
		std::string cadbConfigFile, CoordinateAdditionalBasedComposerConfig* pCadbConfig) {

	configReader.readCollectionConfig(collectionConfigFile, pCollectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, pEvaluatorConfig);
	configReader.readComposerBasedApproximatorConfig(cbApprxConfigFile, pApproximatorConfig);
	configReader.readCoordinateAddtionalBasedComposerConfig(cadbConfigFile, pCadbConfig);

	resetCollectionContainer(*pCollectionConfig);
	resetComposerBasedApproximatorContainer(*pApproximatorConfig, *pCadbConfig, *pCollectionConfig);
	resetEvaluationContainer(*pEvaluatorConfig, *pCollectionConfig);
}

void CommandFactory::readComposerEvaluationConfig(ConfigReader configReader,
					std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
					std::string composerEvalConfigFile,
					std::string targetConfigFile, ComposerEvaluatorConfig* pComposerEvalConfig,
					std::string cadbConfigFile, CoordinateAdditionalBasedComposerConfig* pCadbConfig, bool multiComparatorMode) {

	configReader.readCollectionConfig(collectionConfigFile, pCollectionConfig);
	if(multiComparatorMode) {
		configReader.readMultiComparatorCoordinateAddtionalBasedComposerConfig(cadbConfigFile, pCadbConfig);
	}
	else {
		configReader.readCoordinateAddtionalBasedComposerConfig(cadbConfigFile, pCadbConfig);
	}
	configReader.readComposerEvaluatorConfig(composerEvalConfigFile, pComposerEvalConfig);
	configReader.readComposerEvaluatorConfigFromTargets(targetConfigFile, pComposerEvalConfig);

	resetComposerContainer(*pCadbConfig, *pCollectionConfig);
	resetComposerEvaluatorContainer(*pComposerEvalConfig, *pCollectionConfig);
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

void CommandFactory::readComposerBasedSK2Config(ConfigReader configReader,
		std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
		std::string skApprxConfigFile, SKApproximatorConfig2* pSkApproximatorConfig,
		std::string cbApprxConfigFile, ComposerBasedApproximatorConfig* pCbApproximatorConfig,
		std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig) {

	configReader.readCollectionConfig(collectionConfigFile, pCollectionConfig);
	configReader.readComposerBasedApproximatorConfig(cbApprxConfigFile, pCbApproximatorConfig);
	configReader.readSKApproximatorConfig2(skApprxConfigFile, pSkApproximatorConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, pEvaluatorConfig);

	resetCollectionContainer(*pCollectionConfig);
	resetComposerBasedSK2ApproximatorContainer(*pSkApproximatorConfig, *pCbApproximatorConfig, *pCollectionConfig);
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

void CommandFactory::resetComposerBasedApproximatorContainer(const ComposerBasedApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new ComposerBasedApproximatorContainer(approximatorConfig, collectionConfig));
}

void CommandFactory::resetComposerBasedApproximatorContainer(const ComposerBasedApproximatorConfig& approximatorConfig, const CoordinateAdditionalBasedComposerConfig& cadbConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new ComposerBasedApproximatorContainer(approximatorConfig, cadbConfig, collectionConfig));
}

void CommandFactory::resetComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pComposerContainer);
	m_pComposerContainer = ComposerContainerPtr(new SampleComposerContainerImpl(cabConfig, collectionConfig));
}

void CommandFactory::resetComposerEvaluatorContainer(const ComposerEvaluatorConfig& composerEvalConfig, const CollectionConfig& collectionCofig) {
	_destroy(m_pComposerEvaluatorContainer);
	m_pComposerEvaluatorContainer = ComposerEvaluatorContainerPtr(new SampleComposerEvaluatorContainerImpl(composerEvalConfig, collectionCofig));
}


void CommandFactory::resetSKApproximatorContainer(const SKApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new SKApproximatorContainerImpl(approximatorConfig, collectionConfig));
}

void CommandFactory::resetSK2ApproximatorContainer(const SKApproximatorConfig2& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new SK2ApproximatorContainerImpl(approximatorConfig, collectionConfig));
}

void CommandFactory::resetComposerBasedSK2ApproximatorContainer(const SKApproximatorConfig2& skApproximatorConfig, const ComposerBasedApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pApproximatorContainer);
	m_pApproximatorContainer = ApproximatorContainerPtr(new ComposerBasedSK2ApproximatorContainerImpl(skApproximatorConfig, collectionConfig, approximatorConfig));
}
