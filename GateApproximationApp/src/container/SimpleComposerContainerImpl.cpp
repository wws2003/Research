/*
 * SimpleComposerContainerImpl.cpp
 *
 *  Created on: Feb 22, 2016
 *      Author: pham
 */

#include "SimpleComposerContainerImpl.h"
#include "SimpleGateComposer.h"
#include "GateSetLogImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "GateCombinerImpl.h"
#include "SimpleGateComposer.h"
#include "GateSetLogImpl.h"

SimpleComposerContainerImpl::SimpleComposerContainerImpl(int nbCandidates) {
	m_nbCandidates = nbCandidates;
	wireDependencies();
}

SimpleComposerContainerImpl::~SimpleComposerContainerImpl() {
	releaseDependencies();
}

GateComposerPtr SimpleComposerContainerImpl::getGateComposer() {
	return GateComposerPtr(new SimpleGateComposer(m_pGateCombiner,
			m_nbCandidates,
			m_pGateSetLog));
}

void SimpleComposerContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));
	m_pGateSetLog = ElementSetLogPtr<GatePtr>(new GateSetLogImpl());
	GateCombinabilityCheckers checkers;
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));
}

void SimpleComposerContainerImpl::releaseDependencies() {
	_destroy(m_pGateCombiner);
	_destroy(m_pGateSetLog);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}
