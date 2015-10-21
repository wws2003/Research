/*
 * EvaluateCollectionCommandExecutor.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "EvaluateCollectionCommandExecutor.h"
#include "ICollection.h"
#include "ISearchSpaceEvaluator.h"

EvaluateCollectionCommandExecutor::EvaluateCollectionCommandExecutor(CollectionContainerPtr pCollectionContainer, EvaluatorContainerPtr pEvaluatorContainer) : AbstractChainCommandExecutor() {
	m_pCollectionContainer = pCollectionContainer;
	m_pEvaluatorContainer = pEvaluatorContainer;
}

EvaluateCollectionCommandExecutor::~EvaluateCollectionCommandExecutor() {
	// TODO Auto-generated destructor stub
}

void EvaluateCollectionCommandExecutor::doExecute() {
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	pEvaluator->evaluateCollection(pCollection);
}
