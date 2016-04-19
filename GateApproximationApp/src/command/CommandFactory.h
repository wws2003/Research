/*
 * CommandFactory.h
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#ifndef COMMANDFACTORY_H_
#define COMMANDFACTORY_H_

#include "ICommand.h"
#include "IEvaluatorContainer.h"
#include "IApproximatorContainer.h"
#include "ICollectionContainer.h"
#include "IComposerEvaluator.h"
#include "IComposerEvaluatorContainer.h"
#include "IEvaluatingComposerContainer.h"
#include "Config.h"
#include "ApplicationCommon.h"
#include "ConfigReader.h"

class CommandFactory {
public:
	CommandFactory();
	virtual ~CommandFactory();

	virtual CommandPtr getCommand(int commandCode, const CommandParams& commandParams);

private:
	//----------------------------------//
	//(High-level) Methods to generate command from configuration file
	//----------------------------------//
	AbstractCommandPtr getCollectionEvaluationCommandForIdentity(std::string configFileName);

	AbstractCommandPtr getPersistedCollectionEvaluationCommandForTargets(std::string storeFileName,
			std::string targetConfigFile);

	AbstractCommandPtr getComposerBasedApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
			std::string cbApprxConfigFile,
			std::string targetConfigFile);
	AbstractCommandPtr getSKApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
			std::string skApprxConfigFile,
			std::string targetConfigFile);
	AbstractCommandPtr getSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
			std::string skApprxConfigFile,
			std::string cadbComposerConfigFile,
			std::string targetConfigFile);
	AbstractCommandPtr getComposerBasedSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile,
			std::string cbApprxConfigFile,
			std::string skApprxConfigFile,
			std::string cadbConfigFile,
			std::string targetConfigFile);

	AbstractCommandPtr getComposerBasedApproximator2EvaluationCommandForTargets(std::string collectionConfigFile,
			std::string cbApprxConfigFile,
			std::string adbComposerConfigFile,
			std::string targetConfigFile);

	AbstractCommandPtr getComposerEvaluationCommandForTargets(std::string collectionConfigFile,
			std::string cbApprxConfigFile,
			std::string adbComposerConfigFile,
			std::string targetConfigFile,
			bool multiComparatorMode);

	AbstractCommandPtr getParallelComposerEvaluationCommandForTargets(std::string collectionConfigFile,
			std::string cbApprxConfigFile,
			std::string parallelConfigFile,
			std::string targetConfigFile);

	AbstractCommandPtr getParallelComposerEvaluationCommandForTargets(std::string collectionConfigFile,
				std::string cbApprxConfigFile,
				std::string adbComposerConfigFile,
				std::string parallelConfigFile,
				std::string targetConfigFile);

	//----------------------------------//
	//Apply config paramters to change containers for concrete instances of collection, approximator, evaluator...
	//----------------------------------//

	void readCollectionConfig(std::string configFile, CollectionConfig* pCollectionConfig);

	void readCollectionAndEvaluatorConfig(std::string configFile);

	void readTargetConfig(std::string targetConfigFile);

	void readComposerBasedApproximatorConfig(std::string collectionConfigFile,
			std::string cbApprxConfigFile,
			std::string targetConfigFile);

	void readComposerBasedApproximatorConfig(std::string collectionConfigFile,
			std::string cbApprxConfigFile,
			std::string targetConfigFile,
			std::string cadbConfigFile);

	void readComposerEvaluationConfig(std::string collectionConfigFile,
			std::string composerEvalConfigFile,
			std::string targetConfigFile,
			std::string cadbConfigFile,
			bool multiComparatorMode);

	void readParallelComposerEvaluationConfig(std::string collectionConfigFile,
			std::string composerEvalConfigFile,
			std::string parallelConfigFile,
			std::string targetConfigFile);

	void readParallelComposerEvaluationConfig(std::string collectionConfigFile,
				std::string composerEvalConfigFile,
				std::string cadbConfigFile,
				std::string parallelConfigFile,
				std::string targetConfigFile);

	void readSKConfig(std::string collectionConfigFile,
			std::string skApprxConfigFile,
			std::string targetConfigFile);

	void readSK2Config(std::string collectionConfigFile,
			std::string cadbComposerConfigFile,
			std::string skApprxConfigFile,
			std::string targetConfigFile);

	void readComposerBasedSK2Config(std::string collectionConfigFile,
			std::string cadbComposerConfigFile,
			std::string skApprxConfigFile,
			std::string cbApprxConfigFile,
			std::string targetConfigFile);

	//-----------------------------------//
	//Reset container for concrete instances of collection, approximator, evaluator...
	//-----------------------------------//
	void resetCollectionContainer(const CollectionConfig& collectionConfig);

	void resetEvaluationContainer(const EvaluatorConfig& evaluatorConfig,
			const CollectionConfig& collectionConfig);

	void resetApproximatorContainer(const NearIdentityApproximatorConfig& approximatorConfig,
			const CollectionConfig& collectionConfig);

	void resetComposerContainer();

	void resetComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig);

	void resetComposerBasedApproximatorContainer(const ComposerBasedApproximatorConfig& approximatorConfig,
			const CollectionConfig& collectionConfig);

	void resetComposerBasedApproximatorContainer(const ComposerBasedApproximatorConfig& approximatorConfig,
			const CoordinateAdditionalBasedComposerConfig& cadbConfig,
			const CollectionConfig& collectionConfig);

	void resetEvaluatingComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig);

	void resetEvaluatingComposerContainer(const ParallelConfig& parallelConfig);

	void resetEvaluatingComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig,
				const CollectionConfig& collectionConfig,
				const ParallelConfig& parallelConfig);

	void resetComposerEvaluatorContainer(const ComposerEvaluatorConfig& composerEvalConfig,
			const CollectionConfig& collectionCofig);

	void resetSKApproximatorContainer(const SKApproximatorConfig& approximatorConfig,
			const CollectionConfig& collectionConfig);

	void resetSK2ApproximatorContainer(const SKApproximatorConfig& approximatorConfig,
			const CollectionConfig& collectionConfig,
			const CoordinateAdditionalBasedComposerConfig& cadbConfig);

	void resetComposerBasedSK2ApproximatorContainer(const SKApproximatorConfig& skApproximatorConfig,
			const ComposerBasedApproximatorConfig& cbApproximatorConfig,
			const CollectionConfig& collectionConfig,
			const CoordinateAdditionalBasedComposerConfig& cadbConfig);

	//----------------------------------//
	//Generate commands from container
	//----------------------------------//
	AbstractCommandPtr generateCollectionEvaluationCommandForIdentity();
	AbstractCommandPtr generateApproximatorEvaluationCommandForTargets();
	AbstractCommandPtr generateComposerEvaluationCommandForTargets();

	CollectionContainerPtr m_pCollectionContainer;
	ApproximatorContainerPtr m_pApproximatorContainer;
	EvaluatorContainerPtr m_pEvaluatorContainer;
	ComposerContainerPtr m_pComposerContainer;
	EvaluatingComposerContainerPtr m_pEvaluatingComposerContainer;
	ComposerEvaluatorContainerPtr m_pComposerEvaluatorContainer;
};

#endif /* COMMANDFACTORY_H_ */
