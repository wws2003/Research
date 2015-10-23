/*
 * EvaluateApproximator.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "EvaluateApproximatorCommand.h"
#include "ISearchSpaceEvaluator.h"
#include <iostream>

EvaluateApproximatorCommand::EvaluateApproximatorCommand(ApproximatorContainerPtr pApproximatorContainer,
		CollectionContainerPtr pCollectionContainer,
		EvaluatorContainerPtr pEvaluatorContainer) : AbstractChainCommand() {
	m_pApproximatorContainer = pApproximatorContainer;
	m_pCollectionContainer = pCollectionContainer;
	m_pEvaluatorContainer = pEvaluatorContainer;
}

EvaluateApproximatorCommand::~EvaluateApproximatorCommand() {

}

void EvaluateApproximatorCommand::doExecute() {
	std::cout << "Start evaluate approximator" << std::endl;

	GateApproximatorPtr pApproximator = m_pApproximatorContainer->getGateApproximator();
	GateCollectionPtr pCollection = m_pCollectionContainer->getGateCollection();
	GateSearchSpaceEvaluatorPtr pEvaluator = m_pEvaluatorContainer->getGateSearchSpaceEvaluator();
	pEvaluator->evaluateApproximator(pApproximator, pCollection);
}

