/*
 * SampleComposerEvaluatorContainerImpl.h
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#ifndef SAMPLECOMPOSEREVALUATORCONTAINERIMPL_H_
#define SAMPLECOMPOSEREVALUATORCONTAINERIMPL_H_

#include "IComposerEvaluatorContainer.h"
#include "ITimer.h"
#include "ICoordinateCalculator.h"
#include "IWriter.h"
#include "ICoordinateWriter.h"
#include "ICollectionContainer.h"
#include "IDecomposer.h"
#include "Config.h"
#include "ApplicationCommon.h"
#include "IGateStore.h"

class SampleComposerEvaluatorContainerImpl: public IComposerEvaluatorContainer {
public:
	SampleComposerEvaluatorContainerImpl(const ComposerEvaluatorConfig& composerEvaluatorConfig,
			const CollectionConfig& collectionConfig);

	virtual ~SampleComposerEvaluatorContainerImpl();

	//Override
	GateComposerEvaluatorPtr getGateComposerEvaluator();

private:
	void wireDependencies();
	void releaseDependencies();

	void getTargetsForEvaluation(std::vector<GatePtr>& targets);

	GateDecomposerPtr generateDecomposerFromConfig(ComposerEvaluatorConfig evalConfig);

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	GateStorePtr m_pGateStore;

	//For targets
	std::vector<GatePtr> m_targetGates;

	//For collection
	CollectionContainerPtr m_pCollectionContainer;
	GateCollectionPtr m_pCoreCollection;
	GateCollectionPtr m_pHelperCollection;

	//For distance calculator
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	GateDistanceCalculatorPtr m_pGateDistanceCalculator;

	//For decomposer
	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;
	GateDecomposerPtr m_pGateDecomposer;

	//For the evaluator itself
	GateWriterPtr m_pGateWriterInEvaluator;
	RealCoordinateWriterPtr<GatePtr> m_pCoordinateWriter;
	TimerPtr m_pTimer;

	//Config params
	ComposerEvaluatorConfig m_composerEvaluatorConfig;
	CollectionConfig m_collectionConfig;
};

#endif /* SAMPLECOMPOSEREVALUATORCONTAINERIMPL_H_ */
