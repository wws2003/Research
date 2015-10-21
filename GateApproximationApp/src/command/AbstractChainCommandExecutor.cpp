/*
 * AbstractChainCommandExecutor.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "AbstractChainCommandExecutor.h"

AbstractChainCommandExecutor::AbstractChainCommandExecutor(CommandExecutorPtr pSuccessor) {
	m_pSuccessor = pSuccessor;
}

AbstractChainCommandExecutor::~AbstractChainCommandExecutor() {
}

void AbstractChainCommandExecutor::execute() {
	doExecute();
	if(m_pSuccessor != NullPtr) {
		m_pSuccessor->execute();
	}
}
