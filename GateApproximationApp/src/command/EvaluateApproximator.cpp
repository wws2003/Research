/*
 * EvaluateApproximator.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "EvaluateApproximator.h"
#include "ISearchSpaceEvaluator.h"

EvaluateApproximator::EvaluateApproximator(ApproximatorContainerPtr pApproximatorContainer,
		CollectionContainerPtr pCollectionContainer,
		EvaluatorContainerPtr pEvaluatorContainer) : AbstractChainCommandExecutor() {
	m_pApproximatorContainer = pApproximatorContainer;
	m_pCollectionContainer = pCollectionContainer;
	m_pEvaluatorContainer = pEvaluatorContainer;
}

EvaluateApproximator::~EvaluateApproximator() {

}

void EvaluateApproximator::doExecute() {
	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	pEvaluator->evaluateApproximator(pApproximator, pCollection);
}

