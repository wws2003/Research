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
		int enumEval;
		sscanf(line.data(), "%[^:]:%d", prefix, &enumEval);
		pApproximatorConfig->m_composerBasedApproximatorType = (ComposerType)enumEval;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(enumEval));
		pApproximatorConfig->m_queryDecomposerType = (DecomposerType)enumEval;

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

void ConfigReader::readCoordinateAddtionalBasedComposerConfig(std::string configFile, CoordinateAdditionalBasedComposerConfig* pComposerConfig) {
	std::cout << "Coordinate addition-based config " << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		CoordinateComparatorConfig primaryCoordinateComparatorConfig;
		readCoordinateComparatorConfig(inputStream, &primaryCoordinateComparatorConfig);
		pComposerConfig->m_primaryCoordinateComparatorConfig = primaryCoordinateComparatorConfig;
	}
	else {
		throw std::logic_error("Can not read config file for coordinate additional-based composer!");
	}
}

void ConfigReader::readMultiComparatorCoordinateAddtionalBasedComposerConfig(std::string configFile, CoordinateAdditionalBasedComposerConfig* pComposerConfig) {
	std::cout << "Multiple comparator coordinate addition-based config " << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		readLineAndLog(inputStream, line, std::cout);
		int nbComparators;
		sscanf(line.data(), "%[^:]:%d", prefix, &nbComparators);

		//Read primary comparator
		CoordinateComparatorConfig primaryCoordinateComparatorConfig;
		readCoordinateComparatorConfig(inputStream, &primaryCoordinateComparatorConfig);
		pComposerConfig->m_primaryCoordinateComparatorConfig = primaryCoordinateComparatorConfig;

		//Read secondary comparators
		for(int i = 0; i < nbComparators - 1; i++) {
			CoordinateComparatorConfig secondaryCoordinateComparatorConfig;
			readCoordinateComparatorConfig(inputStream, &secondaryCoordinateComparatorConfig);
			pComposerConfig->m_secondaryCoordinateComparatorConfigs.push_back(secondaryCoordinateComparatorConfig);
		}
	}
	else {
		throw std::logic_error("Can not read config file for coordinate additional-based composer!");
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
	std::cout << "Evaluator config " << configFile << ":\n";
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

void ConfigReader::readComposerEvaluatorConfig(std::string configFile, ComposerEvaluatorConfig* pComposerEvaluatorConfig) {
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pComposerEvaluatorConfig->m_nbPartialQueries));

		readLineAndLog(inputStream, line, std::cout);
		double initialEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &initialEpsilon);
		pComposerEvaluatorConfig->m_initialEpsilon = (mreal_t)initialEpsilon;

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pComposerEvaluatorConfig->m_nbCandidates));

		readLineAndLog(inputStream, line, std::cout);
		int enumEval;
		sscanf(line.data(), "%[^:]:%d", prefix, &(enumEval));
		pComposerEvaluatorConfig->m_queryDecomposerType = (DecomposerType)enumEval;
	}
	else {
		throw std::logic_error("Can not read config file for approximator!");
	}
}

void ConfigReader::readComposerEvaluatorConfigFromTargets(std::string configFile, ComposerEvaluatorConfig* pComposerEvaluatorConfig) {
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
			pComposerEvaluatorConfig->m_rotationTargets.push_back(rotationConfig);
		}

		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pComposerEvaluatorConfig->m_composeEpsilon));
	}
	else {
		throw std::logic_error("Can not read config file for targets!");
	}
}

void ConfigReader::readParallelConfig(std::string configFile, ParallelConfig* pParallelConfig) {
	std::cout << "Parallel config" << configFile << ":\n";
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		int nbThreads;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbThreads);

		int taskBufferSize;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &taskBufferSize);

		int taskFutureBufferSize;
		readLineAndLog(inputStream, line, std::cout);
		sscanf(line.data(), "%[^:]:%d", prefix, &taskFutureBufferSize);

		pParallelConfig->m_nbThreads = nbThreads;
		pParallelConfig->m_taskBufferSize = taskBufferSize;
		pParallelConfig->m_taskFutureBufferSize = taskFutureBufferSize;
	}
	else {
		throw std::logic_error("Can not read parallel config file!");
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

void ConfigReader::readCoordinateComparatorConfig(std::istream& inputStream, CoordinateComparatorConfig* pConfig) {
	char prefix[128];
	std::string line;

	readLineAndLog(inputStream, line, std::cout);
	int enumEval;
	sscanf(line.data(), "%[^:]:%d", prefix, &enumEval);
	pConfig->m_coordinateComparatorType = (CoordinateComparatorTypes)enumEval;

	readLineAndLog(inputStream, line, std::cout);
	double coordinateEpsilon;
	sscanf(line.data(), "%[^:]:%lf", prefix, &coordinateEpsilon);
	pConfig->m_coordinateEpsilon = coordinateEpsilon;

	if(pConfig->m_coordinateComparatorType == CMP_ONE_ELEMENT) {
		readLineAndLog(inputStream, line, std::cout);
		int comparedElementIndex;
		sscanf(line.data(), "%[^:]:%d", prefix, &comparedElementIndex);
		pConfig->m_appliedCoordinateIndices.push_back(comparedElementIndex);
	}
}


void readLineAndLog(std::istream& inputStream, std::string& line, std::ostream& outstream) {
	std::getline(inputStream, line);
	outstream << "--" << line << "\n";
}

