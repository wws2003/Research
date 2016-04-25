/*
 * FilePathConfig.h
 *
 *  Created on: Apr 13, 2016
 *      Author: pham
 */

#ifndef FILEPATHCONFIG_H_
#define FILEPATHCONFIG_H_

#include "Config.h"
#include "ApplicationCommon.h"

class FilePathConfig {
public:
	FilePathConfig(){};
	virtual ~FilePathConfig(){};

	std::string getMatrixDBFilePath(const CollectionConfig& config);

	std::string getGateCollectionPersistenceFilePath(const CollectionConfig& config);

private:
	std::string getLibrarySetPrefix(LibrarySet librarySet);

	static const std::string DEFAULT_COLLECTION_PERSISTED_FILE_PREFIX;
	static const std::string DEFAULT_COLLECTION_PERSISTED_FILE_EXTENSION;
};

#endif /* FILEPATHCONFIG_H_ */
