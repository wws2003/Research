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
#include "ResourceNaming.h"
#include <string>
#include <istream>

class ConfigReader {
public:
	virtual ~ConfigReader();

	virtual void readCollectionConfig(std::string configFile, CollectionConfig* pCollectionConfig);

	virtual void readCollectionAndEvaluatorConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	virtual void readComposerBasedApproximatorConfig(std::string configFile, ComposerBasedApproximatorConfig* pApproximatorConfig);

	virtual void readSKApproximatorConfig(std::string configFile, SKApproximatorConfig* pApproximatorConfig);

	virtual void readCoordinateAddtionalBasedComposerConfig(std::string configFile, CoordinateAdditionalBasedComposerConfig* pComposerConfig);

	virtual void readMultiComparatorCoordinateAddtionalBasedComposerConfig(std::string configFile, CoordinateAdditionalBasedComposerConfig* pComposerConfig);

	virtual void readTargetsConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	virtual void readEvaluatorConfigFromTargets(std::string configFile, EvaluatorConfig* pEvaluatorConfig);

	virtual void readComposerEvaluatorConfig(std::string configFile, ComposerEvaluatorConfig* pComposerEvaluatorConfig);

	virtual void readComposerEvaluatorConfigFromTargets(std::string configFile, ComposerEvaluatorConfig* pComposerEvaluatorConfig);

	virtual void readParallelConfig(std::string configFile, ParallelConfig* pParallelConfig);

private:
	ResourceNaming m_resourceNaming;

	void readRotationConfigLine(std::string line, RotationConfig& rotationConfig);

	void readCoordinateComparatorConfig(std::istream& inputStream, CoordinateComparatorConfig* pConfig);
};

#endif /* CONFIGREADER_H_ */
