/*
 * SelingerComposerEvaluatorContainerImpl.h
 *
 *  Created on: Jun 8, 2016
 *      Author: pham
 */

#ifndef SELINGERCOMPOSEREVALUATORCONTAINERIMPL_H_
#define SELINGERCOMPOSEREVALUATORCONTAINERIMPL_H_

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
#include "ICombiner.h"

class SelingerComposerEvaluatorContainerImpl: public IComposerEvaluatorContainer {
public:
	SelingerComposerEvaluatorContainerImpl(const SelingerComposerEvaluatorConfig& composerEvaluatorConfig);

	virtual ~SelingerComposerEvaluatorContainerImpl();

	//Override
	GateComposerEvaluatorPtr getGateComposerEvaluator();

private:
	void wireDependencies();
	void releaseDependencies();

	GatePtr getModifierGate();
	void getPartialTargets(std::vector<GatePtr>& partialTargets, SelingerZRotationTarget zRotations);

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	GateStorePtr m_pGateStore;

	//For targets
	std::vector<GatePtrVector> m_partialTargets;

	//For modifiers
	std::vector<GatePtr> m_modifierGates;

	//For combiner
	CombinerPtr<GatePtr> m_pGateCombiner;

	//For collection
	GateCollectionPtr m_pHelperCollection;

	//For distance calculator
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	GateDistanceCalculatorPtr m_pGateDistanceCalculator;

	//For coordinate calculator
	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;

	//For the evaluator itself
	GateWriterPtr m_pGateWriterInEvaluator;
	RealCoordinateWriterPtr<GatePtr> m_pCoordinateWriter;
	TimerPtr m_pTimer;

	//Config params
	SelingerComposerEvaluatorConfig m_composerEvaluatorConfig;
};
#endif /* SELINGERCOMPOSEREVALUATORCONTAINERIMPL_H_ */
