/*
 * AbstractChainCommandExecutor.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef ABSTRACTCHAINCOMMANDEXECUTOR_H_
#define ABSTRACTCHAINCOMMANDEXECUTOR_H_

#include "ICommandExecutor.h"
#include "MathConceptsCommon.h"
#include "ApplicationCommon.h"

class AbstractChainCommandExecutor : public ICommandExecutor {
public:
	AbstractChainCommandExecutor(CommandExecutorPtr pSuccessor = NullPtr);
	virtual ~AbstractChainCommandExecutor();

	inline void setSuccessor(CommandExecutorPtr pSuccessor){m_pSuccessor = pSuccessor;};

	//Override
	void execute();

protected:
	virtual void doExecute() = 0;

private:
	CommandExecutorPtr m_pSuccessor;
};

#endif /* ABSTRACTCHAINCOMMANDEXECUTOR_H_ */
