/*
 * EvaluateCollectionCommand.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "EvaluateCollectionCommand.h"

EvaluateCollectionCommand::EvaluateCollectionCommand(GateCollectionPtr pCollection,
		GateSearchSpaceEvaluatorPtr pEvaluator) : AbstractChainCommand() {
	m_pCollection = pCollection;
	m_pEvaluator = pEvaluator;
}

EvaluateCollectionCommand::~EvaluateCollectionCommand() {
	//TODO Revise if destroy should be here
	_destroy(m_pEvaluator);
	_destroy(m_pCollection);
}

void EvaluateCollectionCommand::doExecute() {
	m_pEvaluator->evaluateCollection(m_pCollection);
}
