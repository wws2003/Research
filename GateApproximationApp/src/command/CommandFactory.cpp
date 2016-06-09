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
#include "EvaluateApproximatorCommand.h"
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
#include "SelingerComposerEvaluatorContainerImpl.h"
#include "ConfigReader.h"
#include <cstdio>

CommandFactory::CommandFactory() : m_pCollectionContainer(NullPtr),
m_pApproximatorContainer(NullPtr),
m_pEvaluatorContainer(NullPtr),
m_pComposerContainer(NullPtr),
m_pEvaluatingComposerContainer(NullPtr),
m_pComposerEvaluatorContainer(NullPtr) {
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
	case EVALUATE_SELINGER_APPROXIMATOR_COMPOSER_TO_TARGET: {
		return getComposerForSelingerApproximatorEvaluationCommandForTarget(commandParams[0], commandParams[1], commandParams[2], commandParams[3]);
	}
	default:
		return CommandPtr(new NotAvailableCommand());
	}
}

AbstractCommandPtr CommandFactory::getCollectionEvaluationCommandForIdentity(std::string configFileName) {
	readCollectionAndEvaluatorConfig(configFileName);

	return generateCollectionEvaluationCommandForIdentity();
}

AbstractCommandPtr CommandFactory::getComposerBasedApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string cbApprxConfigFile, std::string targetConfigFile) {

	readComposerBasedApproximatorConfig(collectionConfigFile,
			cbApprxConfigFile,
			targetConfigFile);

	return generateApproximatorEvaluationCommandForTargets();
}

AbstractCommandPtr CommandFactory::getSKApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string skApprxConfigFile, std::string targetConfigFile) {

	readSKConfig(collectionConfigFile,
			skApprxConfigFile,
			targetConfigFile);

	return generateApproximatorEvaluationCommandForTargets();
}

AbstractCommandPtr CommandFactory::getSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string skApprxConfigFile,
		std::string cadbComposerConfigFile,
		std::string targetConfigFile) {

	readSK2Config(collectionConfigFile,
			cadbComposerConfigFile,
			skApprxConfigFile,
			targetConfigFile);

	return generateApproximatorEvaluationCommandForTargets();
}

AbstractCommandPtr CommandFactory::getComposerBasedSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string skApprxConfigFile,
		std::string cadbConfigFile,
		std::string targetConfigFile) {

	readComposerBasedSK2Config(collectionConfigFile,
			cadbConfigFile,
			skApprxConfigFile,
			cbApprxConfigFile,
			targetConfigFile);

	return generateApproximatorEvaluationCommandForTargets();
}

AbstractCommandPtr CommandFactory::getComposerBasedApproximator2EvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string cadbComposerConfigFile,
		std::string targetConfigFile) {

	readComposerBasedApproximatorConfig(collectionConfigFile,
			cbApprxConfigFile,
			targetConfigFile,
			cadbComposerConfigFile);

	return generateApproximatorEvaluationCommandForTargets();
}

AbstractCommandPtr CommandFactory::getComposerEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string adbComposerConfigFile,
		std::string targetConfigFile,
		bool multiComparatorMode) {

	readComposerEvaluationConfig(collectionConfigFile,
			composerEvalConfigFile,
			targetConfigFile,
			adbComposerConfigFile,
			multiComparatorMode);

	return generateComposerEvaluationCommandForTargets();
}

AbstractCommandPtr CommandFactory::getParallelComposerEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string parallelConfigFile,
		std::string targetConfigFile) {

	readParallelComposerEvaluationConfig(collectionConfigFile,
			cbApprxConfigFile,
			parallelConfigFile,
			targetConfigFile);

	return generateComposerEvaluationCommandForTargets();
}

AbstractCommandPtr CommandFactory::getParallelComposerEvaluationCommandForTargets(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string adbComposerConfigFile,
		std::string parallelConfigFile,
		std::string targetConfigFile) {

	readParallelComposerEvaluationConfig(collectionConfigFile,
			cbApprxConfigFile,
			adbComposerConfigFile,
			parallelConfigFile,
			targetConfigFile);

	return generateComposerEvaluationCommandForTargets();
}

AbstractCommandPtr CommandFactory::getComposerForSelingerApproximatorEvaluationCommandForTarget(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string adbComposerConfigFile,
		std::string targetConfigFile) {

	readSelingerComposerEvaluationConfig(collectionConfigFile, cbApprxConfigFile, targetConfigFile, adbComposerConfigFile);

	return generateComposerEvaluationCommandForTargets();
}

//Below are methods to read config files then instantiate proper containers for dependencies

void CommandFactory::readCollectionConfig(std::string configFile, CollectionConfig* pCollectionConfig) {
	ConfigReader configReader;
	configReader.readCollectionConfig(configFile, pCollectionConfig);
	resetCollectionContainer(*pCollectionConfig);
}

void CommandFactory::readCollectionAndEvaluatorConfig(std::string configFile) {
	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;

	ConfigReader configReader;
	configReader.readCollectionAndEvaluatorConfig(configFile, &collectionConfig, &evaluatorConfig);
	resetCollectionContainer(collectionConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
}

void CommandFactory::readTargetConfig(std::string targetConfigFile) {
	CollectionConfig collectionConfig;
	EvaluatorConfig evaluatorConfig;

	ConfigReader configReader;
	configReader.readTargetsConfig(targetConfigFile, &collectionConfig, &evaluatorConfig);

	resetCollectionContainer(collectionConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
}

void CommandFactory::readComposerBasedApproximatorConfig(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string targetConfigFile) {

	CollectionConfig collectionConfig;
	ComposerBasedApproximatorConfig cbApproximatorConfig;
	EvaluatorConfig evaluatorConfig;

	ConfigReader configReader;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);
	configReader.readEvaluatorConfigFromTargets(targetConfigFile, &evaluatorConfig);
	configReader.readComposerBasedApproximatorConfig(cbApprxConfigFile, &cbApproximatorConfig);

	resetCollectionContainer(collectionConfig);
	resetComposerBasedApproximatorContainer(cbApproximatorConfig, collectionConfig);
	resetEvaluationContainer(evaluatorConfig, collectionConfig);
}

void CommandFactory::readComposerBasedApproximatorConfig(std::string collectionConfigFile,
		std::string cbApprxConfigFile,
		std::string targetConfigFile,
		std::string cadbConfigFile) {

	ConfigReader configReader;

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

void CommandFactory::readComposerEvaluationConfig(std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string targetConfigFile,
		std::string cadbConfigFile,
		bool multiComparatorMode) {

	ConfigReader configReader;

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

void CommandFactory::readParallelComposerEvaluationConfig(std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string parallelConfigFile,
		std::string targetConfigFile) {

	ConfigReader configReader;

	CollectionConfig collectionConfig;
	ComposerEvaluatorConfig composerEvalConfig;
	ParallelConfig parallelConfig;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);

	configReader.readComposerEvaluatorConfig(composerEvalConfigFile, &composerEvalConfig);
	configReader.readComposerEvaluatorConfigFromTargets(targetConfigFile, &composerEvalConfig);
	configReader.readParallelConfig(parallelConfigFile, &parallelConfig);

	resetEvaluatingComposerContainer(parallelConfig);
	resetComposerEvaluatorContainer(composerEvalConfig, collectionConfig);
}

void CommandFactory::readParallelComposerEvaluationConfig(std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string cadbConfigFile,
		std::string parallelConfigFile,
		std::string targetConfigFile) {

	ConfigReader configReader;

	CollectionConfig collectionConfig;
	ComposerEvaluatorConfig composerEvalConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;
	ParallelConfig parallelConfig;

	configReader.readMultiComparatorCoordinateAddtionalBasedComposerConfig(cadbConfigFile, &cadbConfig);
	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);

	configReader.readComposerEvaluatorConfig(composerEvalConfigFile, &composerEvalConfig);
	configReader.readComposerEvaluatorConfigFromTargets(targetConfigFile, &composerEvalConfig);
	configReader.readParallelConfig(parallelConfigFile, &parallelConfig);

	resetEvaluatingComposerContainer(cadbConfig, collectionConfig, parallelConfig);
	resetComposerEvaluatorContainer(composerEvalConfig, collectionConfig);
}

void CommandFactory::readSKConfig(std::string collectionConfigFile,
		std::string skApprxConfigFile,
		std::string targetConfigFile) {

	ConfigReader configReader;

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

void CommandFactory::readSK2Config(std::string collectionConfigFile,
		std::string cadbComposerConfigFile,
		std::string skApprxConfigFile,
		std::string targetConfigFile) {

	ConfigReader configReader;

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

void CommandFactory::readComposerBasedSK2Config(std::string collectionConfigFile,
		std::string cadbComposerConfigFile,
		std::string skApprxConfigFile,
		std::string cbApprxConfigFile,
		std::string targetConfigFile) {

	ConfigReader configReader;

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

void CommandFactory::readSelingerComposerEvaluationConfig(std::string collectionConfigFile,
		std::string composerEvalConfigFile,
		std::string targetConfigFile,
		std::string cadbConfigFile) {

	ConfigReader configReader;

	CollectionConfig collectionConfig;
	CoordinateAdditionalBasedComposerConfig cadbConfig;
	SelingerComposerEvaluatorConfig composerEvalConfig;

	configReader.readCollectionConfig(collectionConfigFile, &collectionConfig);
	configReader.readMultiComparatorCoordinateAddtionalBasedComposerConfig(cadbConfigFile, &cadbConfig);
	configReader.readSelingerComposerEvaluatorConfig(composerEvalConfigFile, &composerEvalConfig);
	configReader.readSelingerComposerEvaluatorConfigFromTargets(targetConfigFile, &composerEvalConfig);

	resetEvaluatingComposerContainer(cadbConfig, collectionConfig);
	resetComposerEvaluatorContainer(composerEvalConfig);
}

//-----------------------------------//
//Reset container for concrete instances of collection, approximator, evaluator...
//-----------------------------------//

void CommandFactory::resetCollectionContainer(const CollectionConfig& collectionConfig) {
	_destroy(m_pCollectionContainer);
	m_pCollectionContainer = CollectionContainerPtr(new SampleCollectionContainerImpl(collectionConfig));
}

void CommandFactory::resetEvaluationContainer(const EvaluatorConfig& evaluatorConfig, const CollectionConfig& collectionConfig) {
	_destroy(m_pEvaluatorContainer);
	m_pEvaluatorContainer = EvaluatorContainerPtr(new SampleEvaluatorContainerImpl(evaluatorConfig, collectionConfig));
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

void CommandFactory::resetEvaluatingComposerContainer(const ParallelConfig& parallelConfig) {
	_destroy(m_pEvaluatingComposerContainer);
	m_pEvaluatingComposerContainer = EvaluatingComposerContainerPtr(new EvaluatingParallelComposerContainerImpl(parallelConfig));
}

void CommandFactory::resetEvaluatingComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig,
		const CollectionConfig& collectionConfig,
		const ParallelConfig& parallelConfig) {
	_destroy(m_pEvaluatingComposerContainer);
	m_pEvaluatingComposerContainer = EvaluatingComposerContainerPtr(new EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl(cabConfig, collectionConfig, parallelConfig));
}

void CommandFactory::resetComposerEvaluatorContainer(const ComposerEvaluatorConfig& composerEvalConfig, const CollectionConfig& collectionCofig) {
	_destroy(m_pComposerEvaluatorContainer);
	m_pComposerEvaluatorContainer = ComposerEvaluatorContainerPtr(new SampleComposerEvaluatorContainerImpl(composerEvalConfig, collectionCofig));
}

void CommandFactory::resetComposerEvaluatorContainer(const SelingerComposerEvaluatorConfig& composerEvalConfig) {
	_destroy(m_pComposerEvaluatorContainer);
	m_pComposerEvaluatorContainer = ComposerEvaluatorContainerPtr(new SelingerComposerEvaluatorContainerImpl(composerEvalConfig));
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

//----------------------------------//
//Generate commands from container
//----------------------------------//

AbstractCommandPtr CommandFactory::generateCollectionEvaluationCommandForIdentity() {
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();

	AbstractCommandPtr pCommand = AbstractCommandPtr(new EvaluateCollectionCommand(m_pCollectionContainer->getGateCollection(pGateDistanceCalculator),
			m_pEvaluatorContainer->getGateSearchSpaceEvaluator()));
	return pCommand;
}

AbstractCommandPtr CommandFactory::generateApproximatorEvaluationCommandForTargets() {
	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	GateDistanceCalculatorPtr pGateDistanceCalculator = m_pEvaluatorContainer->getGateDistanceCalculatorForEvaluation();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection(pGateDistanceCalculator);

	AbstractCommandPtr pApproximatorEvaluationCommand = AbstractCommandPtr(new EvaluateApproximatorCommand(pApproximator,
			pCollection,
			pEvaluator));

	return pApproximatorEvaluationCommand;
}

AbstractCommandPtr CommandFactory::generateComposerEvaluationCommandForTargets() {
	GateComposerEvaluatorPtr pGateComposerEvaluator = m_pComposerEvaluatorContainer->getGateComposerEvaluator();
	GateComposerPtr pEvaluatedComposer = m_pEvaluatingComposerContainer->getEvaluatedGateComposer();
	GateComposerPtr pStandardComposer = m_pEvaluatingComposerContainer->getStandardGateComposer();

	AbstractCommandPtr pEvaluateCommand = AbstractCommandPtr(new EvaluateComposerCommand(pEvaluatedComposer,
			pStandardComposer,
			pGateComposerEvaluator));

	return pEvaluateCommand;
}
