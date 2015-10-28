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
	initRotationSetNameMap();
}

ConfigReader::~ConfigReader() {
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

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pCollectionConfig->m_maxSequenceLength));

		int nbQubits;
		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbQubits);
		pCollectionConfig->m_nbQubits = nbQubits;

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pEvaluatorConfig->m_collectionEpsilon));

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%lf", prefix, &(pEvaluatorConfig->m_approximatorEpsilon));
	}
	else {
		throw std::logic_error("Can not read config file!");
	}
}

void ConfigReader::readApproximatorConfig(std::string configFile, NearIdentityApproximatorConfig* pApproximatorConfig) {
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		std::getline(inputStream, line);
		double initialEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &initialEpsilon);
		pApproximatorConfig->m_initialEpsilon = (mreal_t)initialEpsilon;

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_maxMergedBinDistance));

		std::getline(inputStream, line);
		double maxCandidateEpsilon;
		sscanf(line.data(), "%[^:]:%lf", prefix, &maxCandidateEpsilon);
		pApproximatorConfig->m_maxCandidateEpsilon = (mreal_t)maxCandidateEpsilon;

		std::getline(inputStream, line);
		double maxCandidateEpsilonDecreaseFactor;
		sscanf(line.data(), "%[^:]:%lf", prefix, &maxCandidateEpsilonDecreaseFactor);
		pApproximatorConfig->m_maxCandidateEpsilonDecreaseFactor = (mreal_t)maxCandidateEpsilonDecreaseFactor;

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_iterationMaxSteps));

		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &(pApproximatorConfig->m_maxResultNumber));
	}
	else {
		throw std::logic_error("Can not read config file for approximator!");
	}
}

void ConfigReader::readTargetsConfig(std::string configFile, CollectionConfig* pCollectionConfig, EvaluatorConfig* pEvaluatorConfig) {
	std::ifstream inputStream(configFile, std::ifstream::in);
	if(inputStream.is_open()) {
		char prefix[128];
		std::string line;

		int nbQubits;
		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbQubits);
		pCollectionConfig->m_nbQubits = nbQubits;

		int nbTargets;
		std::getline(inputStream, line);
		sscanf(line.data(), "%[^:]:%d", prefix, &nbTargets);

		for(int i = 0; i < nbTargets; i++) {
			RotationConfig rotationConfig;
			std::getline(inputStream, line);
			readRotationConfigLine(line, rotationConfig);
			pEvaluatorConfig->m_rotationTargets.push_back(rotationConfig);
		}

		std::getline(inputStream, line);
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
