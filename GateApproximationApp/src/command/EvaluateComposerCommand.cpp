/*
 * EvaluateCompserCommand.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#include "EvaluateComposerCommand.h"

EvaluateComposerCommand::EvaluateComposerCommand(GateComposerPtr pEvaluatedComposer,
		GateComposerPtr pStandardComposer,
		GateComposerEvaluatorPtr pEvaluator) {
	m_pEvaluatedComposer = pEvaluatedComposer;
	m_pStandardComposer = pStandardComposer;
	m_pEvaluator = pEvaluator;
}

EvaluateComposerCommand::~EvaluateComposerCommand() {
	_destroy(m_pEvaluator);
	_destroy(m_pEvaluatedComposer);
	_destroy(m_pStandardComposer);
}

void EvaluateComposerCommand::doExecute() {
	m_pEvaluator->evaluateComposers(m_pEvaluatedComposer, m_pStandardComposer);
}
