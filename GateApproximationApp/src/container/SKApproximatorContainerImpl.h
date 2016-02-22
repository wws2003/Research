/*
 * SKApproximatorContainerImpl.h
 *
 *  Created on: Nov 18, 2015
 *      Author: pham
 */

#ifndef SKAPPROXIMATORCONTAINERIMPL_H_
#define SKAPPROXIMATORCONTAINERIMPL_H_

#include "IApproximatorContainer.h"
#include "Config.h"
#include "IMatrixOperator.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "GateCoordinateCalculatorImpl.h"
#include "NearIdentityElementBinBasedComposer.h"
#include "IDecomposer.h"
#include "ApplicationCommon.h"
#include "IResourceContainer.h"
#include "IComposer.h"
#include "IComposerContainer.h"

class SKApproximatorContainerImpl: public IApproximatorContainer {
public:
	SKApproximatorContainerImpl(SKApproximatorConfig config, CollectionConfig coreCollectionConfig);
	virtual ~SKApproximatorContainerImpl();

	GateApproximatorPtr getGateApproximator();

private:
	void wireDependencies();
	void releaseDependencies();

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;
	ResourceContainerPtr m_pResourceContainer;

	CombinerPtr<GatePtr> m_pGateCombiner;

	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;

	GateComposerPtr m_pGateLoggingComposer;
	ComposerContainerPtr m_pComposerContainer;
	GateDecomposerPtr m_pHarrowGateDecomposer;

	SKApproximatorConfig m_approximatorConfig;
	CollectionConfig m_coreCollectionConfig;
};

#endif /* SKAPPROXIMATORCONTAINERIMPL_H_ */
