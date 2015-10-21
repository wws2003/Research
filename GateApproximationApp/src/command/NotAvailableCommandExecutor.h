/*
 * NotAvailableCommandExecutor.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef NOTAVAILABLECOMMANDEXECUTOR_H_
#define NOTAVAILABLECOMMANDEXECUTOR_H_

#include "ICommandExecutor.h"

/*
 *
 */
class NotAvailableCommandExecutor: public ICommandExecutor {
public:
	NotAvailableCommandExecutor();
	virtual ~NotAvailableCommandExecutor();

	//Override
	void execute();
};

#endif /* NOTAVAILABLECOMMANDEXECUTOR_H_ */
