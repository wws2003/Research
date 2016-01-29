/*
 * SampleEvaluatorContainerImpl.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

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
#include "SampleResourceContainerImpl.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "MatrixCoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include "GateCoordinateCalculatorImpl.h"
#include "SampleRealCoordinateWriterImpl.hpp"
#include "GateCombinerImpl.h"
#include "LabelOnlyGateWriterImpl.h"
#include "CpuTimer.h"
#include "GateSearchSpaceTimerEvaluatorImpl.h"
#include "MapBasedGateBinCollectionImpl.h"
#include "DuplicateGateCancelationCombinerImpl.h"
#include "ContainerResourceFactory.h"
#include "SampleLibraryMatrixStore.h"
#include "SQLiteLazyGateDistanceCalculator.h"
#include <stdexcept>

SampleEvaluatorContainerImpl::SampleEvaluatorContainerImpl(const EvaluatorConfig& config, const CollectionConfig& collectionConfig) : m_evaluatorConfig(config), m_collectionConfig(collectionConfig) {
	wireDependencies();
}

SampleEvaluatorContainerImpl::~SampleEvaluatorContainerImpl() {
	releaseDependencies();
}

GateSearchSpaceEvaluatorPtr SampleEvaluatorContainerImpl::getGateSearchSpaceEvaluator() {

	GateSearchSpaceEvaluatorPtr pGateSearchSpaceEvaluator = new GateSearchSpaceTimerEvaluatorImpl(m_targetGates,
			m_evaluatorConfig.m_collectionEpsilon,
			m_evaluatorConfig.m_approximatorEpsilon,
			m_pGateRealCoordinateCalculator,
			m_pCoordinateWriter,
			m_pGateWriterInEvaluator,
			m_pTimer,
			std::cout);

	return pGateSearchSpaceEvaluator;
}

void SampleEvaluatorContainerImpl::getTargetsForEvaluation(std::vector<GatePtr>& targets) {
	targets.clear();
	if(m_evaluatorConfig.m_rotationTargets.empty()) {
		//No specified target -> use identity as default target
		m_pResourceContainer->getIdentityTargets(targets, m_collectionConfig.m_nbQubits);
	}
	else {
		m_pResourceContainer->getRotationTargets(targets, m_evaluatorConfig.m_rotationTargets, m_collectionConfig.m_nbQubits);
	}
}

GateDistanceCalculatorPtr SampleEvaluatorContainerImpl::getGateDistanceCalculatorForEvaluation() {
	return m_pGateDistanceCalculator;
}

void SampleEvaluatorContainerImpl::wireDependencies() {
	m_pMatrixFactory = MatrixFactoryPtr(new SimpleDenseMatrixFactoryImpl());
	m_pMatrixOperator = MatrixOperatorPtr(new SampleMatrixOperator(m_pMatrixFactory));

	ResourceContainerFactory resourceContainerFactory;
	m_pResourceContainer = resourceContainerFactory.getResourceContainer(m_collectionConfig.m_librarySet,
			m_pMatrixFactory,
			m_pMatrixOperator);

	getTargetsForEvaluation(m_targetGates);

	m_pMatrixDistanceCalculator = MatrixDistanceCalculatorPtr(new MatrixFowlerDistanceCalculator(NullPtr));
	m_pLibraryMatrixStore = LibraryMatrixStorePtr(new SampleLibraryMatrixStore(m_pMatrixFactory, m_pMatrixOperator));

	m_pGateDistanceCalculator = GateDistanceCalculatorPtr(new SQLiteLazyGateDistanceCalculator(m_pMatrixDistanceCalculator,
			m_pMatrixFactory,
			getMatrixDBFileName(m_collectionConfig),
			m_collectionConfig.m_nbQubits == 1 ? 2 : 4));

	/*m_pGateDistanceCalculator = GateDistanceCalculatorPtr(new LazyGateDistanceCalculatorImpl(m_pMatrixDistanceCalculator,
				m_pMatrixOperator,
				m_pLibraryMatrixStore));*/

	MatrixPtrVector pBasis;
	m_pResourceContainer->getMatrixOrthonormalBasis(pBasis, m_collectionConfig.m_nbQubits);
	m_pGateWriterInEvaluator = GateWriterPtr(new LabelOnlyGateWriterImpl(","));
	m_pMatrixRealInnerProductCalculator = MatrixRealInnerProductCalculatorPtr(new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator));
	m_pHermitiaRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new MatrixCoordinateOnOrthonormalBasisCalculatorImpl(m_pMatrixRealInnerProductCalculator, pBasis));
	m_pMatrixRealCoordinateCalculator = MatrixRealCoordinateCalculatorPtr(new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pHermitiaRealCoordinateCalculator));
	m_pGateRealCoordinateCalculator = GateRealCoordinateCalculatorPtr(new GateCoordinateCalculatorImpl(m_pMatrixRealCoordinateCalculator));
	m_pCoordinateWriter = RealCoordinateWriterPtr<GatePtr>(new SampleRealCoordinateWriterImpl<GatePtr>(20, ","));

	m_pTimer = TimerPtr(new CpuTimer());
}

std::string SampleEvaluatorContainerImpl::getMatrixDBFileName(const CollectionConfig& config) {
#if MPFR_REAL
	std::string precisionPostFix = "_mpfr";
#else
	std::string precisionPostFix = "";
#endif

	char fullName[256];
	sprintf(fullName, "db_%d%s.sqlite",
			config.m_nbQubits,
			precisionPostFix.c_str());

	return std::string(fullName);
}


void SampleEvaluatorContainerImpl::releaseDependencies() {
	_destroy(m_pTimer);
	_destroy(m_pCoordinateWriter);
	_destroy(m_pGateRealCoordinateCalculator);
	_destroy(m_pMatrixRealCoordinateCalculator);
	_destroy(m_pHermitiaRealCoordinateCalculator);
	_destroy(m_pMatrixRealInnerProductCalculator);
	_destroy(m_pGateWriterInEvaluator);

	_destroy(m_pGateDistanceCalculator);

	_destroy(m_pLibraryMatrixStore);

	_destroy(m_pMatrixDistanceCalculator);

	//Release generate targets
	/*for(std::vector<GatePtr>::iterator gIter = m_targetGates.begin(); gIter != m_targetGates.end();) {
		GatePtr pGate = *gIter;
		gIter = m_targetGates.erase(gIter);
		_destroy(pGate);
	}*/
	//Try foreach !
	std::for_each(m_targetGates.begin(), m_targetGates.end(), [](GatePtr pGate){_destroy(pGate);});

	_destroy(m_pResourceContainer);

	_destroy(m_pMatrixOperator);
	_destroy(m_pMatrixFactory);
}

