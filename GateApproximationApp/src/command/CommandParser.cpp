/*
 * CommandParser.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "CommandParser.h"
#include "NotAvailableCommandExecutor.h"
#include <iostream>

CommandParser::CommandParser() {
	initCommandMap();
}

CommandParser::~CommandParser() {
}

CommandExecutorPtr CommandParser::getCommandExecutor(int argc, char* argv[]) {
	//TODO Implement
	int commandCode = getCommandCodeFromArguments(argc, argv);
	CommandExecutorPtr pCommandExecutor = getCommandExecutorForCommandCode(commandCode);

	if(pCommandExecutor == NullPtr) {
		throw std::runtime_error("Wrong syntax error");
	}

	return pCommandExecutor;
}

void CommandParser::printSyntaxMessage() {
	std::cout << "As of 2015/10/21, acceptable command arguments are:" << std::endl;

	std::cout << "-g conf1 conf2 -o outputFile -> Generate near identity sequences (persist to storage )" << std::endl;

	std::cout << "-e conf1 -I  -> Evaluate collection for identity" << std::endl;
	std::cout << "-e conf1 conf2 -I -> Evaluate collection, approximator for identity" << std::endl;

	std::cout << "For future purposes" << std::endl;

	std::cout << "-e conf1 -t targetConf -> Evaluate collection for target" << std::endl;
	std::cout << "-e conf1 -c dbconf -t targetConf -> Evaluate collection for target given candidates file" << std::endl;
	std::cout << "-e conf1 conf2 -t targetConf -> Evaluate collection, approximator for target" << std::endl;
}

void CommandParser::initCommandMap() {
	//For -g conf1 conf2 -o outputFile
	provideArgumentPattern(6, {1, 4}, {"-g", "-o"}, GENERATE_NEAR_IDENTITY);

	//For -e conf1 -I
	provideArgumentPattern(4, {1, 3}, {"-e", "-I"}, EVALUATE_COLLECTION_TO_IDENTITY);

	//For -e conf1 conf2 -I
	provideArgumentPattern(5, {1, 4}, {"-e", "-I"}, EVALUATE_COLLECTION_APPROXIMATOR_TO_IDENTITY);
}

void CommandParser::provideArgumentPattern(int argc, std::vector<int> argvPositions, std::vector<std::string> argvs, int commandCode) {
	ArgumentPattern argumentPattern;
	for(unsigned int i = 0; i < argvPositions.size(); i++) {
		argumentPattern[argvPositions[i]] = argvs[i];
	}
	m_commandMap[argc].push_back(argumentPattern);
	m_commandCodeMap[argc][m_commandMap[argc].size() - 1] = commandCode;
}

int CommandParser::getCommandCodeFromArguments(int argc, char* argv[]) {
	CommandPatternMap::const_iterator cIter= m_commandMap.find(argc);
	if(cIter != m_commandMap.end()) {
		ArgumentPatternSet argumentPatternSet = cIter->second;
		for(unsigned int i = 0; i < argumentPatternSet.size(); i++) {
			ArgumentPattern argumentPattern = argumentPatternSet[i];
			if(isArgumentsMatched(argv, argumentPattern)) {
				return m_commandCodeMap[argc][i];
			}
		}
	}
	return UNKNOWN_COMMAND;
}

CommandExecutorPtr CommandParser::getCommandExecutorForCommandCode(int commandCode) {
	//TODO Implement properly
	switch (commandCode) {
	case UNKNOWN_COMMAND:
		return NullPtr;
	default:
		return CommandExecutorPtr(new NotAvailableCommandExecutor());
	}
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

