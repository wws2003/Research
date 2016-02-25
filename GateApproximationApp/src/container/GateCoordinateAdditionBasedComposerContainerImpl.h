/*
 * GateCoordinateAdditionBasedComposerContainerImpl.h
 *
 *  Created on: Feb 22, 2016
 *      Author: pham
 */

#ifndef GATECOORDINATEADDITIONBASEDCOMPOSERCONTAINERIMPL_H_
#define GATECOORDINATEADDITIONBASEDCOMPOSERCONTAINERIMPL_H_

#include "Config.h"
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
#include "IComposerContainer.h"

class GateCoordinateAdditionBasedComposerContainerImpl: public IComposerContainer {
public:
	GateCoordinateAdditionBasedComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig);

	virtual ~GateCoordinateAdditionBasedComposerContainerImpl();

	//Override
	GateComposerPtr getGateComposer();

protected:
	void wireDependencies();
	void releaseDependencies();

	virtual void initCoordinateAdditionalBasedGateComposer();

	void initCoordinateAdditionalBasedGateComposerElements();

	typedef ComparatorPtr<RealCoordinate<GatePtr> > GateRealCoordinateComparatorPtr;
	typedef std::vector<GateRealCoordinateComparatorPtr> GateRealCoordinateComparatorList;

	GateRealCoordinateComparatorPtr getRealCoordinateComparator(CoordinateComparatorConfig coordinateComparatorConfig);
	void initEpsilonRealCoordinate(RealCoordinate<GatePtr>& epsilonRealCoordinate, mreal_t coordinateEpsilon);

	void releaseSecondaryCoordinateComparators();

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;
	ResourceContainerPtr m_pResourceContainer;

	CombinerPtr<GatePtr> m_pGateCombiner;

	ComposerPtr<RealCoordinate<GatePtr> > m_pGateCoordinateComposer;
	GateRealCoordinateComparatorPtr m_pPrimaryRealCoordinateComparator;
	GateRealCoordinateComparatorList m_secondaryRealCoordinateComparators;

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

#endif /* GATECOORDINATEADDITIONBASEDCOMPOSERCONTAINERIMPL_H_ */
