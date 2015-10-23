/*
 * ICommand.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include <vector>
#include <string>

typedef std::vector<std::string> CommandParams;

class ICommand {
public:
	virtual ~ICommand(){};

	virtual void execute() = 0;
};



#endif /* ICOMMANDEXECUTOR_H_ */
