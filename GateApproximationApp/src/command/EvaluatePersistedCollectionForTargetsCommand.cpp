/*
 * EvaluatePersistedCollectionForTargetsCommand.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: pham
 */

#include "EvaluatePersistedCollectionForTargetsCommand.h"
#include <iostream>

EvaluatePersistedCollectionForTargetsCommand::EvaluatePersistedCollectionForTargetsCommand(PersitableGateCollectionPtr pCollection,
		GateSearchSpaceEvaluatorPtr pEvaluator,
		std::string storingFileName) {
	m_pCollection = pCollection;
	m_pEvaluator = pEvaluator;
	m_storingFileName = storingFileName;
}

EvaluatePersistedCollectionForTargetsCommand::~EvaluatePersistedCollectionForTargetsCommand() {

}

void EvaluatePersistedCollectionForTargetsCommand::doExecute() {
	std::cout << "Loading persisted collection...." << std::endl;
	m_pCollection->load(m_storingFileName);
	std::cout << "Evaluating persisted collection for targets" << std::endl;
	m_pEvaluator->evaluateCollection(m_pCollection);
}

