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
	~ConfigReader();

	void readCollectionConfig(std::string configFile, CollectionConfig* pCollectionConfig);

	void readCollectionAndEvaluatorConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	void readComposerBasedApproximatorConfig(std::string configFile, ComposerBasedApproximatorConfig* pApproximatorConfig);

	void readSKApproximatorConfig(std::string configFile, SKApproximatorConfig* pApproximatorConfig);

	void readCoordinateAddtionalBasedComposerConfig(std::string configFile, CoordinateAdditionalBasedComposerConfig* pComposerConfig);

	void readMultiComparatorCoordinateAddtionalBasedComposerConfig(std::string configFile, CoordinateAdditionalBasedComposerConfig* pComposerConfig);

	void readTargetsConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

	void readEvaluatorConfigFromTargets(std::string configFile, EvaluatorConfig* pEvaluatorConfig);

	void readComposerEvaluatorConfig(std::string configFile, ComposerEvaluatorConfig* pComposerEvaluatorConfig);

	void readComposerEvaluatorConfigFromTargets(std::string configFile, ComposerEvaluatorConfig* pComposerEvaluatorConfig);

	void readParallelConfig(std::string configFile, ParallelConfig* pParallelConfig);

	void readSelingerComposerEvaluatorConfig(std::string configFile, SelingerComposerEvaluatorConfig* pComposerEvaluatorConfig);

	void readSelingerComposerEvaluatorConfigFromTargets(std::string configFile, SelingerComposerEvaluatorConfig* pComposerEvaluatorConfig);

private:
	ResourceNaming m_resourceNaming;

	void readRotationConfigLine(std::string line, RotationConfig& rotationConfig);

	void readCoordinateComparatorConfig(std::istream& inputStream, CoordinateComparatorConfig* pConfig);
};

#endif /* CONFIGREADER_H_ */
