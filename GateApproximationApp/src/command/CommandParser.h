/*
 * CommandParser.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include "ICommandExecutor.h"
#include "ApplicationCommon.h"
#include <stdexcept>
#include <map>
#include <vector>

typedef std::map<int, std::string> ArgumentPattern;
typedef std::vector<ArgumentPattern> ArgumentPatternSet;
typedef std::map<int, ArgumentPatternSet> CommandPatternMap;

typedef std::map<int, std::map<unsigned int, int> > CommandCodeMap;

class CommandParser {
public:
	CommandParser();
	virtual ~CommandParser();

	CommandExecutorPtr getCommandExecutor(int argc, char* argv[]);

	void printSyntaxMessage();

private:
	void initCommandMap();

	void provideArgumentPattern(int argc, std::vector<int> argvPositions, std::vector<std::string> argvs, int commandCode);

	int getCommandCodeFromArguments(int argc, char* argv[]);

	CommandExecutorPtr getCommandExecutorForCommandCode(int commandCode);

	bool isArgumentsMatched(char* argv[], const ArgumentPattern& argumentPattern);

	CommandPatternMap m_commandMap;

	CommandCodeMap m_commandCodeMap;

	enum CommandCode {
		UNKNOWN_COMMAND,
		GENERATE_NEAR_IDENTITY,
		EVALUATE_COLLECTION_TO_IDENTITY,
		EVALUATE_COLLECTION_APPROXIMATOR_TO_IDENTITY,
		//TODO Add more command code
	};

};

#endif /* COMMANDPARSER_H_ */
