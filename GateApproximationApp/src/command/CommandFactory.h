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
#include "IComposerContainer.h"
#include "Config.h"
#include "ApplicationCommon.h"
#include "ConfigReader.h"

class CommandFactory {
public:
	CommandFactory();
	virtual ~CommandFactory();

	virtual CommandPtr getCommand(int commandCode, const CommandParams& commandParams);

private:
	AbstractCommandPtr getCollectionEvaluationCommandForIdentity(std::string configFileName);
	AbstractCommandPtr getApproximatorEvaluationCommandForIdentity(std::string evaluationConfigFile, std::string approximatorConfigFileName);
	AbstractCommandPtr getGenerateAndStoreApproximationsCommandForIdentity(std::string evaluationConfigFile, std::string approximatorConfigFileName, std::string storeFileName);
	AbstractCommandPtr getPersistedCollectionEvaluationCommandForTargets(std::string storeFileName, std::string targetConfigFile);

	AbstractCommandPtr getComposerBasedApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string cbApprxConfigFile, std::string targetConfigFile);
	AbstractCommandPtr getSKApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string skApprxConfigFile, std::string targetConfigFile);
	AbstractCommandPtr getSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string skApprxConfigFile, std::string targetConfigFile);
	AbstractCommandPtr getComposerBasedSK2ApproximatorEvaluationCommandForTargets(std::string collectionConfigFile, std::string cbApprxConfigFile, std::string skApprxConfigFile, std::string targetConfigFile);

	AbstractCommandPtr getComposerBasedApproximator2EvaluationCommandForTargets(std::string collectionConfigFile, std::string cbApprxConfigFile, std::string adbComposerConfigFile, std::string targetConfigFile);

	AbstractCommandPtr getComposerEvaluationCommandForTargets(std::string collectionConfigFile,
			std::string cbApprxConfigFile,
			std::string adbComposerConfigFile,
			std::string targetConfigFile,
			bool multiComparatorMode);

	//Apply config paramters to change containers for concrete instances of collection, approximator, evaluator...
	void readCollectionConfig(ConfigReader configReader, std::string configFile, CollectionConfig* pCollectionConfig);

	void readCollectionAndEvaluatorConfig(ConfigReader configReader, std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	void readApproximatorConfig(ConfigReader configReader, std::string configFile, const CollectionConfig&  collectionConfig, NearIdentityApproximatorConfig* pApproximatorConfig);

	void readTargetConfig(ConfigReader configReader, std::string targetConfigFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	void readComposerBasedApproximatorConfig(ConfigReader configReader,
			std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
			std::string cbApprxConfigFile, ComposerBasedApproximatorConfig* pApproximatorConfig,
			std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig);

	void readComposerBasedApproximatorConfig(ConfigReader configReader,
				std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
				std::string cbApprxConfigFile, ComposerBasedApproximatorConfig* pApproximatorConfig,
				std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig,
				std::string cadbConfigFile, CoordinateAdditionalBasedComposerConfig* pCadbConfig);

	void readComposerEvaluationConfig(ConfigReader configReader,
					std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
					std::string composerEvalConfigFile,
					std::string targetConfigFile, ComposerEvaluatorConfig* pComposerEvalConfig,
					std::string cadbConfigFile, CoordinateAdditionalBasedComposerConfig* pCadbConfig, bool multiComparatorMode);

	void readSKConfig(ConfigReader configReader,
			std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
			std::string skApprxConfigFile, SKApproximatorConfig* pApproximatorConfig,
			std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig);

	void readSK2Config(ConfigReader configReader,
				std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
				std::string skApprxConfigFile, SKApproximatorConfig2* pApproximatorConfig,
				std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig);

	void readComposerBasedSK2Config(ConfigReader configReader,
					std::string collectionConfigFile, CollectionConfig* pCollectionConfig,
					std::string skApprxConfigFile, SKApproximatorConfig2* pSkApproximatorConfig,
					std::string cbApprxConfigFile, ComposerBasedApproximatorConfig* pCbApproximatorConfig,
					std::string targetConfigFile, EvaluatorConfig* pEvaluatorConfig);

	//Reset container for concrete instances of collection, approximator, evaluator...
	void resetCollectionContainer(const CollectionConfig& collectionConfig);

	void resetEvaluationContainer(const EvaluatorConfig& evaluatorConfig, const CollectionConfig& collectionConfig);

	void resetApproximatorContainer(const NearIdentityApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig);

	void resetComposerBasedApproximatorContainer(const ComposerBasedApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig);

	void resetComposerBasedApproximatorContainer(const ComposerBasedApproximatorConfig& approximatorConfig, const CoordinateAdditionalBasedComposerConfig& cadbConfig, const CollectionConfig& collectionConfig);

	void resetComposerContainer(const CoordinateAdditionalBasedComposerConfig& cabConfig, const CollectionConfig& collectionConfig);

	void resetComposerEvaluatorContainer(const ComposerEvaluatorConfig& composerEvalConfig, const CollectionConfig& collectionCofig);

	void resetSKApproximatorContainer(const SKApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig);

	void resetSK2ApproximatorContainer(const SKApproximatorConfig2& approximatorConfig, const CollectionConfig& collectionConfig);

	void resetComposerBasedSK2ApproximatorContainer(const SKApproximatorConfig2& skApproximatorConfig, const ComposerBasedApproximatorConfig& cbApproximatorConfig, const CollectionConfig& collectionConfig);

	CollectionContainerPtr m_pCollectionContainer;
	ApproximatorContainerPtr m_pApproximatorContainer;
	EvaluatorContainerPtr m_pEvaluatorContainer;
	ComposerContainerPtr m_pComposerContainer;
	ComposerEvaluatorContainerPtr m_pComposerEvaluatorContainer;
};

#endif /* COMMANDFACTORY_H_ */
