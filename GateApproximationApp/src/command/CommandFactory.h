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
	AbstractCommandPtr getGenerateAndStoreApproximationsForIdentity(std::string evaluationConfigFile, std::string approximatorConfigFileName, std::string storeFileName);
	AbstractCommandPtr getPersistedCollectionEvaluationForTargets(std::string storeFileName, std::string targetConfigFile);

	void readCollectionAndEvaluatorConfig(ConfigReader configReader, std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);
	void readApproximatorConfig(ConfigReader configReader, std::string configFile, const CollectionConfig&  collectionConfig, NearIdentityApproximatorConfig* pApproximatorConfig);
	void readTargetConfig(ConfigReader configReader, std::string targetConfigFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	void resetCollectionContainer(const CollectionConfig& collectionConfig);
	void resetEvaluationContainer(const EvaluatorConfig& evaluatorConfig, const CollectionConfig& collectionConfig);
	void resetApproximatorContainer(const NearIdentityApproximatorConfig& approximatorConfig, const CollectionConfig& collectionConfig);

	CollectionContainerPtr m_pCollectionContainer;
	ApproximatorContainerPtr m_pApproximatorContainer;
	EvaluatorContainerPtr m_pEvaluatorContainer;

};

#endif /* COMMANDFACTORY_H_ */
