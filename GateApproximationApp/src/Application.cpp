//============================================================================
// Name        : Application.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include "Gate.h"
#include "ICollection.h"
#include "AlgoCommon.h"
#include "ISearchSpaceEvaluator.h"
#include "EvaluateCommon.h"
#include "SampleAppContainerImpl.h"
#include "IAppContainer.h"
#include "CommandParser.h"
#include "ApplicationCommon.h"
#include "CommandFactory.h"
#include "ICommand.h"

using namespace std;

void initCommands(CommandParser* pCommandParser);

void printSyntaxMessage();

int main(int argc, char* argv[]) {
	mreal::initPrecision();

	CommandParser commandParser;
	initCommands(&commandParser);

	CommandFactory commandFactory;

	try {
		int commandCode = UNKNOWN_COMMAND;
		CommandParams commandParams;
		commandParser.getCommandCodeAndParams(argc, argv, commandCode, commandParams);

		CommandPtr pCommand = commandFactory.getCommand(commandCode, commandParams);
		pCommand->execute();
	}
	catch(std::exception const& e) {
		std::cout << e.what() << std::endl;
		printSyntaxMessage();
	}
}

void initCommands(CommandParser* pCommandParser) {
	//For -g conf1 conf2 -o outputFile
	pCommandParser->provideArgumentPatternForCommandCode(6, {1, 4}, {"-g", "-o"}, GENERATE_NEAR_IDENTITY);

	//For -e conf1 -I
	pCommandParser->provideArgumentPatternForCommandCode(4, {1, 3}, {"-e", "-I"}, EVALUATE_COLLECTION_TO_IDENTITY);

	//For -e conf1 conf2 -I
	pCommandParser->provideArgumentPatternForCommandCode(5, {1, 4}, {"-e", "-I"}, EVALUATE_COLLECTION_APPROXIMATOR_TO_IDENTITY);

}

void printSyntaxMessage() {
	std::cout << "As of 2015/10/21, acceptable command arguments are:" << std::endl;

	std::cout << "-g conf1 conf2 -o outputFile -> Generate near identity sequences (persist to storage )" << std::endl;

	std::cout << "-e conf1 -I  -> Evaluate collection for identity" << std::endl;
	std::cout << "-e conf1 conf2 -I -> Evaluate collection, approximator for identity" << std::endl;

	std::cout << "For future purposes" << std::endl;

	std::cout << "-e conf1 -t targetConf -> Evaluate collection for target" << std::endl;
	std::cout << "-e conf1 -c dbconf -t targetConf -> Evaluate collection for target given candidates file" << std::endl;
	std::cout << "-e conf1 conf2 -t targetConf -> Evaluate collection, approximator for target" << std::endl;

}
