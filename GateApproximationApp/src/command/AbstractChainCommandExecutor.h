/*
 * AbstractChainCommandExecutor.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef ABSTRACTCHAINCOMMANDEXECUTOR_H_
#define ABSTRACTCHAINCOMMANDEXECUTOR_H_

#include "ICommand.h"
#include "MathConceptsCommon.h"
#include "ApplicationCommon.h"

class AbstractChainCommand : public ICommand {
public:
	AbstractChainCommand(CommandPtr pSuccessor = NullPtr);
	virtual ~AbstractChainCommand();

	inline void setSuccessor(CommandPtr pSuccessor){m_pSuccessor = pSuccessor;};

	//Override
	void execute();

protected:
	virtual void doExecute() = 0;

private:
	CommandPtr m_pSuccessor;
};

#endif /* ABSTRACTCHAINCOMMANDEXECUTOR_H_ */
