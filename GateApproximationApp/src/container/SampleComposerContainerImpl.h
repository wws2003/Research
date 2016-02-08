/*
 * SampleComposerContainerImpl.h
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#ifndef SAMPLECOMPOSERCONTAINERIMPL_H_
#define SAMPLECOMPOSERCONTAINERIMPL_H_

#include "Config.h"
#include "IComposerContainer.h"
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

class SampleComposerContainerImpl: public IComposerContainer {
public:
	SampleComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig);

	virtual ~SampleComposerContainerImpl();

	//Override
	GateComposerPtr getStandardGateComposer();

	//Override
	GateComposerPtr getEvaluatedGateComposer();

private:
	void wireDependencies();
	void releaseDependencies();

	void initCoordinateAdditionalBasedGateComposerElements();
	void initRealCoordinateComparator();
	void initEpsilonRealCoordinate(RealCoordinate<GatePtr>& epsilonRealCoordinate);

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

	CoordinateAdditionalBasedComposerConfig m_coordinateAdditionalBasedComposerConfig;
	CollectionConfig m_collectionConfig;
};

#endif /* SAMPLECOMPOSERCONTAINERIMPL_H_ */
