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
#include "CommandParser.h"
#include "ApplicationCommon.h"
#include "CommandFactory.h"
#include "ICommand.h"

using namespace std;

void initCommands(CommandParser* pCommandParser);

void printSyntaxMessage();

typedef std::vector<int> ArgumentPositions;
typedef std::vector<std::string> Arguments;

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
		_destroy(pCommand);
	}
	catch(std::exception const& e) {
		std::cout << e.what() << std::endl;
		printSyntaxMessage();
	}
}

void initCommands(CommandParser* pCommandParser) {
	//For -g conf1 conf2 -o outputFile
	pCommandParser->provideArgumentPatternForCommandCode(6, ArgumentPositions{1, 4}, Arguments{"-g", "-o"}, GENERATE_NEAR_IDENTITY);

	//For -e conf1 -I
	pCommandParser->provideArgumentPatternForCommandCode(4, ArgumentPositions{1, 3}, Arguments{"-e", "-I"}, EVALUATE_COLLECTION_TO_IDENTITY);

	//For -e conf1 conf2 -I
	pCommandParser->provideArgumentPatternForCommandCode(5, ArgumentPositions{1, 4}, Arguments{"-e", "-I"}, EVALUATE_COLLECTION_APPROXIMATOR_TO_IDENTITY);

	//For -e -i db -t targetConf.
	pCommandParser->provideArgumentPatternForCommandCode(6, ArgumentPositions{1, 2, 4}, Arguments{"-e", "-i", "-t"}, EVALUATE_PERSISTED_COLLECTION_TO_TARGET);

	//For -e conf1 -cb conf2 -t targetConf
	pCommandParser->provideArgumentPatternForCommandCode(7, ArgumentPositions{1, 3, 5}, Arguments{"-e", "-cb", "-t"}, EVALUATE_CB_APPROXIMATOR_TO_TARGET);

	//For -e conf1 -sk conf2 -t targetConf
	pCommandParser->provideArgumentPatternForCommandCode(7, ArgumentPositions{1, 3, 5}, Arguments{"-e", "-sk", "-t"}, EVALUATE_SK_APPROXIMATOR_TO_TARGET);

	//For -e conf1 -sk2 conf2 -t targetConf
	pCommandParser->provideArgumentPatternForCommandCode(7, ArgumentPositions{1, 3, 5}, Arguments{"-e", "-sk2", "-t"}, EVALUATE_SK2_APPROXIMATOR_TO_TARGET);

	//For -e conf1 -cb conf2 -sk2 conf3 -t targetConf
	pCommandParser->provideArgumentPatternForCommandCode(9, ArgumentPositions{1, 3, 5, 7}, Arguments{"-e", "-cb", "-sk2", "-t"}, EVALUATE_CB_SK2_APPROXIMATOR_TO_TARGET);

	//For -e conf1 -cb conf21 conf22 -t targetConf
	pCommandParser->provideArgumentPatternForCommandCode(8, ArgumentPositions{1, 3, 6}, Arguments{"-e", "-cb", "-t"}, EVALUATE_CB2_APPROXIMATOR_TO_TARGET);
}

void printSyntaxMessage() {
	std::cout << "As of 2016/01/02, acceptable command arguments are:" << std::endl;

	std::cout << "========For Near-Identity stuffs========" << std::endl;
	std::cout << "-g conf1 conf2 -o outputFile -> Generate near identity sequences (then persist to storage)." << std::endl;
	std::cout << "-e conf1 -I  -> Evaluate collection for identity." << std::endl;
	std::cout << "-e conf1 conf2 -I -> Evaluate collection, approximator for identity." << std::endl;

	std::cout << "\n========For specified-targets stuffs========" << std::endl;
	std::cout << "-e -i db -t targetConf -> Evaluate collection for target given candidates in database file." << std::endl;
	std::cout << "-e conf1 -cb conf2 -t targetConf -> Evaluate composer-based approximator" << std::endl;
	std::cout << "-e conf1 -sk conf2 -t targetConf -> Evaluate SK approximator based on the configured collection." << std::endl;
	std::cout << "-e conf1 -sk2 conf2 -t targetConf -> Evaluate SK approximator based on the configured collection, with a smart filter for intermediate results. " << std::endl;
	std::cout << "-e conf1 -cb conf2 -sk2 conf3 -t targetConf -> Evaluate SK approximator based on the configured collection, with a smart filter for intermediate results. Initial stage is based on composer-based approximator instead of collection." << std::endl;

	std::cout << "\n========For future purposes========" << std::endl;

	std::cout << "-e conf1 -t targetConf -> Evaluate collection for target" << std::endl;
	std::cout << "-e conf1 conf2 -t targetConf -> Evaluate collection, approximator for target" << std::endl;

}
