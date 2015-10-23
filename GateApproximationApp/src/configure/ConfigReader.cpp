/*
 * ConfigReader.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#include "ConfigReader.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdexcept>

ConfigReader::ConfigReader() {
	initLibrarySetNameMap();
}

ConfigReader::~ConfigReader() {
}

void ConfigReader::initLibrarySetNameMap() {
	m_librarySetNameMap["H-T"] = L_HT;
	m_librarySetNameMap["H-CV"] = L_HCV;
	m_librarySetNameMap["H-T-CNOT"] = L_HTCNOT;
	m_librarySetNameMap["H-T-CV"] = L_HTCV;
}

void ConfigReader::readCollectionAndEvaluatorConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig) {
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		char librarySetName[128];
		std::string line;

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%[^\n]", prefix, librarySetName);
		std::string librarySetNameStr(librarySetName);
		pCollectionConfig->m_librarySet = (LibrarySet)m_librarySetNameMap[librarySetNameStr];
		pEvaluatorConfig->m_librarySet = pCollectionConfig->m_librarySet;

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pCollectionConfig->m_maxSequenceLength));

		int nbQubits;
		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbQubits);
		pCollectionConfig->m_nbQubits = nbQubits;
		pEvaluatorConfig->m_nbQubits = nbQubits;

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pEvaluatorConfig->m_collectionEpsilon));

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pEvaluatorConfig->m_approximatorEpsilon));
	}
	else {
		throw std::logic_error("Can not read config file!");
	}
}
