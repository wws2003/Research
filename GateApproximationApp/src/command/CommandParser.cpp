/*
 * CommandParser.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "CommandParser.h"
#include "NotAvailableCommand.h"
#include <iostream>

CommandParser::CommandParser() {
}

CommandParser::~CommandParser() {
}

void CommandParser::provideArgumentPatternForCommandCode(int argc, std::vector<int> argvPositions, std::vector<std::string> argvs, int commandCode) {
	ArgumentPattern argumentPattern;
	for(unsigned int i = 0; i < argvPositions.size(); i++) {
		//Add to map of (argument position -> argument value)
		argumentPattern[argvPositions[i]] = argvs[i];
	}
	//Add new argument pattern to the list of patterns having the same argc
	m_commandMap[argc].push_back(argumentPattern);

	//Set command code corresponding to argc and the order of argument pattern
	m_commandCodeMap[argc][m_commandMap[argc].size() - 1] = commandCode;
}

void CommandParser::getCommandCodeAndParams(int argc, char* argv[], int& rCommandCode, CommandParams& rCommandParams) {
	CommandPatternMap::const_iterator cIter= m_commandMap.find(argc);
	if(cIter != m_commandMap.end()) {
		ArgumentPatternSet argumentPatternSet = cIter->second;
		for(unsigned int i = 0; i < argumentPatternSet.size(); i++) {
			ArgumentPattern argumentPattern = argumentPatternSet[i];
			if(isArgumentsMatched(argv, argumentPattern)) {
				rCommandCode = m_commandCodeMap[argc][i];

				//Retrieve from argument list arguments those not fixed by position, i.e. not in the patterns list of argc
				parseParametersFromArguments(argumentPattern, argc, argv, rCommandParams);
				return;
			}
		}
	}
	throw std::runtime_error("Wrong syntax error");
}

bool CommandParser::isArgumentsMatched(char* argv[], const ArgumentPattern& argumentPattern) {
	for(ArgumentPattern::const_iterator pIter = argumentPattern.begin(); pIter != argumentPattern.end(); pIter++) {
		int argumentPosition = pIter->first;
		std::string fixedArgument = pIter->second;
		char* givenArgument = argv[argumentPosition];
		if(fixedArgument.compare(givenArgument)) {
			return false;
		}
	}
	return true;
}

void CommandParser::parseParametersFromArguments(const ArgumentPattern& argumentPattern, int argc, char* argv[], CommandParams& rCommandParams) {
	rCommandParams.clear();

	//Set i starts from 1 to ignore the first argument, which should be executable file name.
	//May require a better solution
	for(int i = 1; i < argc; i++) {
		if(argumentPattern.count(i) == 0) {
			rCommandParams.push_back(argv[i]);
		}
	}
}


