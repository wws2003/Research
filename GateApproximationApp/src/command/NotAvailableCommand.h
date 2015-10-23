/*
 * NotAvailableCommand.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef NOTAVAILABLECOMMAND_H_
#define NOTAVAILABLECOMMAND_H_

#include "ICommand.h"

class NotAvailableCommand: public ICommand {
public:
	NotAvailableCommand();
	virtual ~NotAvailableCommand();

	//Override
	void execute();
};

#endif /* NOTAVAILABLECOMMANDEXECUTOR_H_ */
