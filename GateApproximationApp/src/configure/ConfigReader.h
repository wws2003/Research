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

	virtual void readCollectionAndEvaluatorConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig);

private:
	void initLibrarySetNameMap();

	LibrarySetNameMap m_librarySetNameMap;
};

#endif /* CONFIGREADER_H_ */
