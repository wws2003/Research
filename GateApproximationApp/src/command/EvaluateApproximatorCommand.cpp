/*
 * EvaluateApproximator.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "EvaluateApproximatorCommand.h"
#include "ICollection.h"
#include "IApproximator.h"
#include <iostream>

EvaluateApproximatorCommand::EvaluateApproximatorCommand(GateApproximatorPtr pApproximator,
		GateCollectionPtr pCollection,
		GateSearchSpaceEvaluatorPtr pEvaluator) : AbstractChainCommand() {

	m_pApproximator = pApproximator;
	m_pCoreCollection = pCollection;
	m_pEvaluator = pEvaluator;
}

EvaluateApproximatorCommand::~EvaluateApproximatorCommand() {
	//TODO Revise if destroy should be here
	_destroy(m_pEvaluator);
	_destroy(m_pCoreCollection);
	_destroy(m_pApproximator);
}

void EvaluateApproximatorCommand::doExecute() {
	std::cout << "Start evaluate approximator" << std::endl;

	m_pEvaluator->evaluateApproximator(m_pApproximator, m_pCoreCollection);
}

