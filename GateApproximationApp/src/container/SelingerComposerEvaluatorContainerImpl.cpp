/*
 * SelingerComposerEvaluatorContainerImpl.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: pham
 */

#include "SelingerComposerEvaluatorContainerImpl.h"
#include "SampleComposerEvaluatorContainerImpl.h"
#include "GateComposerEvaluatorImpl.h"
#include "SampleEvaluatorContainerImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "SampleMatrixOperator.h"
#include "BinaryGateWriterImpl.h"
#include "BinaryGateReaderImpl.h"
#include "BinaryMatrixWriterImpl.h"
#include "BinaryMatrixReaderImpl.h"
#include "MatrixFowlerDistanceCalculator.h"
#include "GateDistanceCalculatorByMatrixImpl.h"
#include "GateSearchSpaceConstructorImpl.h"
#include "VectorBasedCollectionImpl.hpp"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "GateCoordinateCalculatorImpl.h"
#include "SampleRealCoordinateWriterImpl.hpp"
#include "GateCombinerImpl.h"
#include "LabelOnlyGateWriterImpl.h"
#include "CpuTimer.h"
#include "SystemTimer.h"
#include "GateSearchSpaceTimerEvaluatorImpl.h"
#include "MapBasedGateBinCollectionImpl.h"
#include "SQLiteLazyGateDistanceCalculator.h"
#include "SampleCollectionContainerImpl.h"
#include "VectorBasedMatrixCollectionImpl.h"
#include "MultiNearBalancePartsGateDecomposer.h"
#include "HarrowGateDecomposer.h"
#include "FilePathConfig.h"
#include "SampleGateStoreFactoryImpl.h"
#include "GateCombinerImpl.h"
#include "GateSelingerComposerEvaluatorImpl.h"
#include "FullGateWriterImpl.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

SelingerComposerEvaluatorContainerImpl::SelingerComposerEvaluatorContainerImpl(const SelingerComposerEvaluatorConfig& composerEvaluatorConfig) : m_composerEvaluatorConfig(composerEvaluatorConfig) {
	wireDependencies();
}

SelingerComposerEvaluatorContainerImpl::~SelingerComposerEvaluatorContainerImpl() {
	releaseDependencies();
}

GateComposerEvaluatorPtr SelingerComposerEvaluatorContainerImpl::getGateComposerEvaluator() {
	return GateComposerEvaluatorPtr(new GateSelingerComposerEvaluatorImpl(m_partialTargets,
			m_modifierGates,
			m_composerEvaluatorConfig.m_nbCandidates,
			m_composerEvaluatorConfig.m_composeEpsilon,
			m_composerEvaluatorConfig.m_initialEpsilon,
			m_pTimer,
			m_pGateDistanceCalculator,
			m_pGateCombiner,
			m_pGateRealCoordinateCalculator,
			m_pCoordinateWriter,
			m_pGateWriterInEvaluator,
			m_pHelperCollection,
			std::cout,
			m_pMatrixFactory,
			m_composerEvaluatorConfig.m_selingerProgramPath));
}

//MARK: Private methods
void SelingerComposerEvaluatorContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	SampleGateStoreFactoryImpl gateStoreFactory(m_pMatrixOperator, m_pMatrixFactory);
	m_pGateStore = gateStoreFactory.getGateStore(1);

	//Setup partial targets
	SelingerZRotationTargets zRotationTargets = m_composerEvaluatorConfig.m_selingerZRotationTargets;
	for(unsigned int i = 0; i < zRotationTargets.size(); i++) {
		GatePtrVector partialTargets;
		getPartialTargets(partialTargets, zRotationTargets[i]);
		m_partialTargets.push_back(partialTargets);
	}

	GatePtr pModifierGate = getModifierGate();
	for(unsigned int i = 0; i < m_composerEvaluatorConfig.m_selingerZRotationTargets.size(); i++) {
		m_modifierGates.push_back(pModifierGate);
	}

	//Setup combiner
	GateCombinabilityCheckers checkers;
	m_pGateCombiner = CombinerPtr<GatePtr>(new GateCombinerImpl(checkers, m_pMatrixOperator));

	//Setup distance calculators
	FilePathConfig pathConfig;
	m_pMatrixDistanceCalculator = MatrixDistanceCalculatorPtr(new MatrixFowlerDistanceCalculator(NullPtr));
	m_pGateDistanceCalculator = GateDistanceCalculatorPtr(new GateDistanceCalculatorByMatrixImpl(m_pMatrixDistanceCalculator));

	//Setup helper collection
	m_pHelperCollection = GateCollectionPtr(new VectorBasedCollectionImpl<GatePtr>(NullPtr));

	//Setup decomposers
	MatrixPtrVector pBasis;
	m_pGateStore->getMatrixOrthonormalBasis(pBasis);

	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));

	//For evaluator itself
	m_pGateWriterInEvaluator = GateWriterPtr(new FullGateWriterImpl());
	m_pCoordinateWriter = RealCoordinateWriterPtr<GatePtr>(new SampleRealCoordinateWriterImpl<GatePtr>(20, ","));
	m_pTimer = TimerPtr(new SystemTimer());
}

void SelingerComposerEvaluatorContainerImpl::releaseDependencies() {
	_destroy(m_pTimer);
	_destroy(m_pCoordinateWriter);
	_destroy(m_pGateWriterInEvaluator);

	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);

	_destroy(m_pHelperCollection);

	_destroy(m_pGateDistanceCalculator);
	_destroy(m_pMatrixDistanceCalculator);

	_destroy(m_pGateCombiner);

	//Release modifier (release once as all instance are the same)
	GatePtr pModifierGate = m_modifierGates[0];
	m_modifierGates.clear();
	_destroy(pModifierGate);

	for(GatePtrVector partialTargets : m_partialTargets) {
		std::for_each(partialTargets.begin(), partialTargets.end(), [](GatePtr pGate){_destroy(pGate);});
		partialTargets.clear();
	}

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

GatePtr SelingerComposerEvaluatorContainerImpl::getModifierGate() {
	const std::string H("H");
	const mreal_t r_sqrt2 = 1.0 / mreal::sqrt(2.0); // 1/sqrt(2)
	const ComplexVal i_r_sqrt2 = ComplexVal(0.0, r_sqrt2); // 1/sqrt(2) * i

	ComplexVal arrayH[] = {-i_r_sqrt2, -i_r_sqrt2, -i_r_sqrt2, i_r_sqrt2};
	MatrixPtr pMatrixH = m_pMatrixFactory->getMatrixFromValues(2, 2, arrayH, ROW_SPLICE);

	return GatePtr(new Gate(pMatrixH, 10, H));
}

void SelingerComposerEvaluatorContainerImpl::getPartialTargets(std::vector<GatePtr>& partialTargets,
		SelingerZRotationTarget zRotations) {

	partialTargets.clear();

	mreal_t theta1PiDenominators = zRotations.m_theta1PiDenominators;
	mreal_t theta2PiDenominators = zRotations.m_theta2PiDenominators;
	mreal_t theta3PiDenominators = zRotations.m_theta3PiDenominators;

	RotationConfig z1(R_Z, M_PI / theta1PiDenominators);
	RotationConfig z2(R_Z, M_PI / theta2PiDenominators);
	RotationConfig z3(R_Z, M_PI / theta3PiDenominators);

	RotationConfigs rotationConfigs = {z1, z2, z3};

	GatePtrVector tmpPartialTargets;
	m_pGateStore->getRotationTargets(tmpPartialTargets, rotationConfigs);

	//Copy tmpPartialTargets but change gate labels to meet the requirement of evaluator
	std::stringstream ss1;
	ss1 << "pi/" << theta1PiDenominators;
	std::stringstream ss2;
	ss2 << "pi/" << theta2PiDenominators;
	std::stringstream ss3;
	ss3 << "pi/" << theta3PiDenominators;

	std::string partialLabels[] = {ss1.str(), ss2.str(), ss3.str()};

	for(int i = 0; i < 3; i++) {
		GatePtr pTmpGate = tmpPartialTargets[i];

		GatePtr pPartialTarget = GatePtr(new Gate(pTmpGate->getMatrix()->clone(),
				pTmpGate->getCost(),
				partialLabels[i]));

		partialTargets.push_back(pPartialTarget);
		_destroy(pTmpGate);
	}
}
