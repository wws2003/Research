/*
 * EvaluateCollectionCommand.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "EvaluateCollectionCommand.h"
#include "ICollection.h"
#include "ISearchSpaceEvaluator.h"

EvaluateCollectionCommand::EvaluateCollectionCommand(CollectionContainerPtr pCollectionContainer, EvaluatorContainerPtr pEvaluatorContainer) : AbstractChainCommand() {
	m_pCollectionContainer = pCollectionContainer;
	m_pEvaluatorContainer = pEvaluatorContainer;
}

EvaluateCollectionCommand::~EvaluateCollectionCommand() {
	// TODO Auto-generated destructor stub
}

void EvaluateCollectionCommand::doExecute() {
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	pEvaluator->evaluateCollection(pCollection);
}
