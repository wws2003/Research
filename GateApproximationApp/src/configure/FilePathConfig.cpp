/*
 * FilePathConfig.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: pham
 */

#include "FilePathConfig.h"
#include "Context.h"
#include "ResourceNaming.h"

const std::string FilePathConfig::DEFAULT_COLLECTION_PERSISTED_FILE_PREFIX = "gnat";
const std::string FilePathConfig::DEFAULT_COLLECTION_PERSISTED_FILE_EXTENSION = "dat";

std::string FilePathConfig::getMatrixDBFilePath(const CollectionConfig& config) {
#if MPFR_REAL
	std::string precisionPostFix = "mpfr";
#else
	std::string precisionPostFix = "";
#endif

	char fullName[256];
	ContextPtr pContext = Context::getInstance();

	//E.g. .data/db_ht_mpfr_1.sqlite
	sprintf(fullName, "%s/db_%s_%s_%d.sqlite",
			pContext->getAppDataFolder().c_str(),
			getLibrarySetPrefix(config.m_librarySet).c_str(),
			precisionPostFix.c_str(),
			config.m_nbQubits);

	return std::string(fullName);
}

std::string FilePathConfig::getGateCollectionPersistenceFilePath(const CollectionConfig& config) {

	int nbQubits = config.m_nbQubits;
	int maxSequenceLength = config.m_maxSequenceLength;

#if MPFR_REAL
	std::string precisionPostFix = "mpfr";
#else
	std::string precisionPostFix = "";
#endif

	char fullName[256];
	ContextPtr pContext = Context::getInstance();

	//E.g. .data/gnat_ht_mpfr_1_16.dat
	sprintf(fullName, "%s/%s_%s_%s_%d_%d.%s",
			pContext->getAppDataFolder().c_str(),
			DEFAULT_COLLECTION_PERSISTED_FILE_PREFIX.c_str(),
			getLibrarySetPrefix(config.m_librarySet).c_str(),
			precisionPostFix.c_str(),
			nbQubits,
			maxSequenceLength,
			DEFAULT_COLLECTION_PERSISTED_FILE_EXTENSION.c_str());

	return std::string(fullName);
}

std::string FilePathConfig::getLibrarySetPrefix(LibrarySet librarySet) {
	ResourceNaming resourceNaming;
	return resourceNaming.getLibrarySetDisplayName(librarySet);
}
