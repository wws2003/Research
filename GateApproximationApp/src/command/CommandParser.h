/*
 * CommandParser.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include "ICommand.h"
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

	void provideArgumentPatternForCommandCode(int argc, std::vector<int> argvPositions, std::vector<std::string> argvs, int commandCode);

	void getCommandCodeAndParams(int argc, char* argv[], int& commandCode, CommandParams& rCommandParams);

private:

	bool isArgumentsMatched(char* argv[], const ArgumentPattern& argumentPattern);

	void parseParametersFromArguments(const ArgumentPattern& argumentPattern, int argc, char* argv[], CommandParams& rCommandParams);

	CommandPatternMap m_commandMap;

	CommandCodeMap m_commandCodeMap;

};

#endif /* COMMANDPARSER_H_ */
