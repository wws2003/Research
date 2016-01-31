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
#include "ILookupResultFilter.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "GateCoordinateCalculatorImpl.h"
#include "CoordinateAdditionBasedGateComposer.h"

class ComposerBasedApproximatorContainer: public IApproximatorContainer {
public:
	ComposerBasedApproximatorContainer(ComposerBasedApproximatorConfig approximatorConfig,
			CollectionConfig coreCollectionConfig);

	ComposerBasedApproximatorContainer(ComposerBasedApproximatorConfig approximatorConfig,
			CoordinateAdditionalBasedComposerConfig coordinateAdditionalBasedComposerConfig,
			CollectionConfig coreCollectionConfig);

	virtual ~ComposerBasedApproximatorContainer();

	GateApproximatorPtr getGateApproximator();

private:
	void wireDependencies();
	void releaseDependencies();

	void initCoordinateAdditionalBasedGateComposerElements();
	void initRealCoordinateComparator();
	void initEpsilonRealCoordinate(RealCoordinate<GatePtr>& epsilonRealCoordinate);

	GateComposerPtr generateComposerFromConfig(ComposerBasedApproximatorConfig approximatorConfig);

	GateDecomposerPtr generateDecomposerFromConfig(ComposerBasedApproximatorConfig approximatorConfig);

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;
	ResourceContainerPtr m_pResourceContainer;

	CombinerPtr<GatePtr> m_pGateCombiner;
	ElementSetLogPtr<GatePtr> m_pGateSetLog;

	GateComposerPtr m_pGateComposer;
	ComposerPtr<RealCoordinate<GatePtr> > m_pGateCoordinateComposer;
	ComparatorPtr<RealCoordinate<GatePtr> > m_pRealCoordinateComparator;
	CombinerPtr<RealCoordinate<GatePtr> > m_pGateCoordinateCombiner;
	ConverterPtr<GatePtr, RealCoordinate<GatePtr> > m_pGateCoordinateConveter;
	DistanceCalculatorPtr<RealCoordinate<GatePtr> > m_pGateCoordinateDistanceCalculator;

	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;

	GateDecomposerPtr m_pGateDecomposer;

	GateLookupResultFilterPtr m_pGateLookupResultFilter;

	ComposerBasedApproximatorConfig m_approximatorConfig;
	CoordinateAdditionalBasedComposerConfig m_coordinateAdditionalBasedComposerConfig;
	CollectionConfig m_coreCollectionConfig;
};

#endif /* COMPOSERBASEDAPPROXIMATORCONTAINER_H_ */
