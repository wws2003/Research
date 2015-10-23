/*
 * AbstractChainCommandExecutor.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "AbstractChainCommandExecutor.h"

AbstractChainCommand::AbstractChainCommand(CommandPtr pSuccessor) {
	m_pSuccessor = pSuccessor;
}

AbstractChainCommand::~AbstractChainCommand() {
}

void AbstractChainCommand::execute() {
	doExecute();
	if(m_pSuccessor != NullPtr) {
		m_pSuccessor->execute();
	}
}
