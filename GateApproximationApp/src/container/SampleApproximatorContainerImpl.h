/*
 * SampleApproximatorContainerImpl.h
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#ifndef SAMPLEAPPROXIMATORCONTAINERIMPL_H_
#define SAMPLEAPPROXIMATORCONTAINERIMPL_H_

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
#include "IElementSetLog.h"
#include "ILookupResultFilter.h"

class SampleApproximatorContainerImpl: public IApproximatorContainer {
public:
	SampleApproximatorContainerImpl(NearIdentityApproximatorConfig approximatorConfig,
			CollectionConfig coreCollectionConfig);

	virtual ~SampleApproximatorContainerImpl();

	GateApproximatorPtr getGateApproximator();

private:
	void wireDependencies();
	void releaseDependencies();

	NearIdentityElementBinBasedComposer<GatePtr>::Config getComposerConfig();

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	ResourceContainerPtr m_pResourceContainer;

	CombinerPtr<GatePtr> m_pGateCombiner;

	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;

	CombinerPtr<GatePtr> m_pGateInBinCombiner;
	BinCollectionPtr<GatePtr> m_pBinCollection;

	GateComposerPtr m_pGateComposer;
	GateDecomposerPtr m_pGateDecomposer;

	GateLookupResultFilterPtr m_pGateLookupResultFilter;

	NearIdentityApproximatorConfig m_approximatorConfig;
	CollectionConfig m_coreCollectionConfig;
};

#endif /* SAMPLEAPPROXIMATORCONTAINERIMPL_H_ */
