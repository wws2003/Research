/*
 * ConfigReader.h
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include "Config.h"
#include "ApplicationCommon.h"
#include <string>

class ConfigReader {
public:
	ConfigReader();
	virtual ~ConfigReader();

	virtual void readCollectionConfig(std::string configFile, CollectionConfig* pCollectionConfig);

	virtual void readCollectionAndEvaluatorConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	virtual void readNearIdentityApproximatorConfig(std::string configFile, NearIdentityApproximatorConfig* pApproximatorConfig);

	virtual void readComposerBasedApproximatorConfig(std::string configFile, ComposerBasedApproximatorConfig* pApproximatorConfig);

	virtual void readSKApproximatorConfig(std::string configFile, SKApproximatorConfig* pApproximatorConfig);

	virtual void readSKApproximatorConfig2(std::string configFile, SKApproximatorConfig2* pApproximatorConfig);

	virtual void readTargetsConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	virtual void readEvaluatorConfigFromTargets(std::string configFile, EvaluatorConfig* pEvaluatorConfig);

private:
	void initLibrarySetNameMap();
	void initRotationSetNameMap();

	void readRotationConfigLine(std::string line, RotationConfig& rotationConfig);

	LibrarySetNameMap m_librarySetNameMap;
	RotationTypeNameMap m_rotationSetNameMap;
};

#endif /* CONFIGREADER_H_ */
