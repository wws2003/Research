/*
 * ComposerBasedApproximatorContainer.h
 *
 *  Created on: Jan 2, 2016
 *      Author: pham
 */

#ifndef COMPOSERBASEDAPPROXIMATORCONTAINER_H_
#define COMPOSERBASEDAPPROXIMATORCONTAINER_H_

#include "IApproximatorContainer.h"
#include "Config.h"
#include "IComposer.h"
#include "IDecomposer.h"
#include "Gate.h"
#include "ICombiner.h"
#include "IMatrixOperator.h"
#include "IMatrixFactory.h"
#include "IElementSetLog.h"
#include "ICoordinateCalculator.h"
#include "IResourceContainer.h"

class ComposerBasedApproximatorContainer: public IApproximatorContainer {
public:
	ComposerBasedApproximatorContainer(ComposerBasedApproximatorConfig approximatorConfig, CollectionConfig coreCollectionConfig);
	virtual ~ComposerBasedApproximatorContainer();

	GateApproximatorPtr getGateApproximator();

private:
	void wireDependencies();
	void releaseDependencies();

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;
	ResourceContainerPtr m_pResourceContainer;

	CombinerPtr<GatePtr> m_pGateCombiner;
	ElementSetLogPtr<GatePtr> m_pGateSetLog;
	GateComposerPtr m_pGateComposer;

	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;

	GateDecomposerPtr m_pGateDecomposer;

	ComposerBasedApproximatorConfig m_approximatorConfig;
	CollectionConfig m_coreCollectionConfig;
};

#endif /* COMPOSERBASEDAPPROXIMATORCONTAINER_H_ */
