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

void readLineAndLog(std::istream& inputStream, std::string& line, std::ostream& outstream);

ConfigReader::ConfigReader() {
	initLibrarySetNameMap();
	initRotationSetNameMap();
}

ConfigReader::~ConfigReader() {
}

void ConfigReader::readCollectionConfig(std::string configFile, CollectionConfig* pCollectionConfig) {
	std::cout << "Collection config " << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		char librarySetName[128];
		std::string line;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%[^\n]", prefix, librarySetName);
		std::string librarySetNameStr(librarySetName);
		pCollectionConfig->m_librarySet = (LibrarySet)m_librarySetNameMap[librarySetNameStr];

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pCollectionConfig->m_maxSequenceLength));

		int nbQubits;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbQubits);
		pCollectionConfig->m_nbQubits = nbQubits;

	}
	else {
		throw std::logic_error("Can not read config file!");
	}
}

void ConfigReader::readCollectionAndEvaluatorConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig) {
	std::cout << "ReadCollectionAndEvaluator config " << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		char librarySetName[128];
		std::string line;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%[^\n]", prefix, librarySetName);
		std::string librarySetNameStr(librarySetName);
		pCollectionConfig->m_librarySet = (LibrarySet)m_librarySetNameMap[librarySetNameStr];

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pCollectionConfig->m_maxSequenceLength));

		int nbQubits;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbQubits);
		pCollectionConfig->m_nbQubits = nbQubits;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pEvaluatorConfig->m_collectionEpsilon));

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pEvaluatorConfig->m_approximatorEpsilon));
	}
	else {
		throw std::logic_error("Can not read config file!");
	}
}

void ConfigReader::readNearIdentityApproximatorConfig(std::string configFile, NearIdentityApproximatorConfig* pApproximatorConfig) {
	std::cout << "NearIdentityApproximator config " << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		readLineAndLog(inputStream, line, std::cout);
		double initialEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &initialEpsilon);
		pApproximatorConfig->m_initialEpsilon = (mreal_t)initialEpsilon;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_maxMergedBinDistance));

		readLineAndLog(inputStream, line, std::cout);
		double maxCandidateEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &maxCandidateEpsilon);
		pApproximatorConfig->m_maxCandidateEpsilon = (mreal_t)maxCandidateEpsilon;

		readLineAndLog(inputStream, line, std::cout);
		double maxCandidateEpsilonDecreaseFactor;
		sscanf(line.data(), "%[^:]:%lf", prefix, &maxCandidateEpsilonDecreaseFactor);
		pApproximatorConfig->m_maxCandidateEpsilonDecreaseFactor = (mreal_t)maxCandidateEpsilonDecreaseFactor;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_iterationMaxSteps));

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_maxResultNumber));
	}
	else {
		throw std::logic_error("Can not read config file for approximator!");
	}
}

void ConfigReader::readComposerBasedApproximatorConfig(std::string configFile, ComposerBasedApproximatorConfig* pApproximatorConfig) {
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_nbPartialQueries));

		readLineAndLog(inputStream, line, std::cout);
		double initialEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &initialEpsilon);
		pApproximatorConfig->m_initialEpsilon = (mreal_t)initialEpsilon;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_nbCandidates));

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_composerBasedApproximatorType));

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_queryDecomposerType));

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_buildingBlockComposerType));

		readLineAndLog(inputStream, line, std::cout);
		int useFilter = 0;
		sscanf(line.data(), "%[^:]:%d", prefix, &(useFilter));
		pApproximatorConfig->m_userFilter = !(useFilter == 0);
	}
	else {
		throw std::logic_error("Can not read config file for approximator!");
	}
}

void ConfigReader::readSKApproximatorConfig(std::string configFile, SKApproximatorConfig* pApproximatorConfig) {
	std::cout << "SK config " << configFile << " <<:\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		readLineAndLog(inputStream, line, std::cout);
		double initialEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &initialEpsilon);
		pApproximatorConfig->m_initialEpsilon = (mreal_t)initialEpsilon;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_recursiveLevels));

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_nbCandidates));
	}
	else {
		throw std::logic_error("Can not read config file for approximator!");
	}
}

void ConfigReader::readSKApproximatorConfig2(std::string configFile, SKApproximatorConfig2* pApproximatorConfig) {
	std::cout << "SK2 config " << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		readLineAndLog(inputStream, line, std::cout);
		double initialEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &initialEpsilon);
		pApproximatorConfig->m_initialEpsilon = (mreal_t)initialEpsilon;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_recursiveLevels));

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_nbCandidates));

		readLineAndLog(inputStream, line, std::cout);
		double coordinateEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &coordinateEpsilon);
		pApproximatorConfig->m_coordinateEpsilon = coordinateEpsilon;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_coordinateComparatorType));
	}
	else {
		throw std::logic_error("Can not read config file for approximator!");
	}
}


void ConfigReader::readTargetsConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig) {
	std::cout << "Target config" << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		int nbQubits;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbQubits);
		pCollectionConfig->m_nbQubits = nbQubits;

		int nbTargets;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbTargets);

		for(int i = 0; i < nbTargets; i++) {
			RotationConfig rotationConfig;
			readLineAndLog(inputStream, line, std::cout);
			readRotationConfigLine(line, rotationConfig);
			pEvaluatorConfig->m_rotationTargets.push_back(rotationConfig);
		}

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pEvaluatorConfig->m_collectionEpsilon));

		//Set approximation epsilon to collection epsilon to avoid empty field
		pEvaluatorConfig->m_approximatorEpsilon = pEvaluatorConfig->m_collectionEpsilon;

		//Since library gates set are not required in the config file, set as unspecified
		pCollectionConfig->m_librarySet = L_UNSPECIFIED;
	}
	else {
		throw std::logic_error("Can not read config file for targets!");
	}
}

void ConfigReader::readEvaluatorConfigFromTargets(std::string configFile, EvaluatorConfig* pEvaluatorConfig) {
	std::cout << "Evaluator config" << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		int nbQubits;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbQubits);

		int nbTargets;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbTargets);

		for(int i = 0; i < nbTargets; i++) {
			RotationConfig rotationConfig;
			readLineAndLog(inputStream, line, std::cout);
			readRotationConfigLine(line, rotationConfig);
			pEvaluatorConfig->m_rotationTargets.push_back(rotationConfig);
		}

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pEvaluatorConfig->m_collectionEpsilon));

		//Set approximation epsilon to collection epsilon to avoid empty field
		pEvaluatorConfig->m_approximatorEpsilon = pEvaluatorConfig->m_collectionEpsilon;
	}
	else {
		throw std::logic_error("Can not read config file for targets!");
	}
}

void ConfigReader::initLibrarySetNameMap() {
	m_librarySetNameMap["H-T"] = L_HT;
	m_librarySetNameMap["H-CV"] = L_HCV;
	m_librarySetNameMap["H-T-CNOT"] = L_HTCNOT;
	m_librarySetNameMap["H-T-CV"] = L_HTCV;
}

void ConfigReader::initRotationSetNameMap() {
	m_rotationSetNameMap["X"] = R_X;
	m_rotationSetNameMap["Y"] = R_Y;
	m_rotationSetNameMap["Z"] = R_Z;
	m_rotationSetNameMap["CX"] = C_RX;
	m_rotationSetNameMap["CY"] = C_RY;
	m_rotationSetNameMap["CZ"] = C_RZ;
}

void ConfigReader::readRotationConfigLine(std::string line, RotationConfig& rotationConfig) {
	char prefix[256];
	char axis[256];
	int piDenominator;
	sscanf(line.data(), "%[^:]:%[^ ]%d", prefix, axis, &piDenominator);
	rotationConfig.m_rotationType = (RotationType)m_rotationSetNameMap[axis];
	rotationConfig.m_rotationAngle = (mreal_t)(M_PI / piDenominator);
}

void readLineAndLog(std::istream& inputStream, std::string& line, std::ostream& outstream) {
	std::getline(inputStream, line);
	outstream << "--" << line << "\n";
}

