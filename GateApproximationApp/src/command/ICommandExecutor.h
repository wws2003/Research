/*
 * ICommandExecutor.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef ICOMMANDEXECUTOR_H_
#define ICOMMANDEXECUTOR_H_

class ICommandExecutor {
public:
	virtual ~ICommandExecutor(){};

	virtual void execute() = 0;
};


#endif /* ICOMMANDEXECUTOR_H_ */
