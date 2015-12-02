/*
 * SampleEvaluatorContainerImpl.h
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#ifndef SAMPLEEVALUATORCONTAINERIMPL_H_
#define SAMPLEEVALUATORCONTAINERIMPL_H_

#include "IEvaluatorContainer.h"
#include "EvaluateCommon.h"
#include "OperatorCommon.h"
#include "IOCommon.h"
#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "IResourceContainer.h"
#include "AlgoInternal.h"
#include "ILibraryMatrixStore.h"
#include "Config.h"

class SampleEvaluatorContainerImpl: public IEvaluatorContainer {
public:
	SampleEvaluatorContainerImpl(const EvaluatorConfig& config, const CollectionConfig& collectionConfig);
	virtual ~SampleEvaluatorContainerImpl();

	//Override
	GateSearchSpaceEvaluatorPtr getGateSearchSpaceEvaluator();

	//Override
	void getTargetsForEvaluation(std::vector<GatePtr>& targets);

	//Override
	GateDistanceCalculatorPtr getGateDistanceCalculatorForEvaluation();

private:
	void wireDependencies();
	void releaseDependencies();

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	ResourceContainerPtr m_pResourceContainer;

	std::vector<GatePtr> m_targetGates;

	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	LibraryMatrixStorePtr m_pLibraryMatrixStore;
	GateDistanceCalculatorPtr m_pGateDistanceCalculator;

	GateWriterPtr m_pGateWriterInEvaluator;
	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;
	RealCoordinateWriterPtr<GatePtr> m_pCoordinateWriter;
	TimerPtr m_pTimer;

	EvaluatorConfig m_evaluatorConfig;
	CollectionConfig m_collectionConfig;
};

#endif /* SAMPLEEVALUATORCONTAINERIMPL_H_ */
