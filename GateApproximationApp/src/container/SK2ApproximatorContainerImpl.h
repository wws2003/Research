/*
 * SK2ApproximatorContainerImpl.h
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#ifndef SK2APPROXIMATORCONTAINERIMPL_H_
#define SK2APPROXIMATORCONTAINERIMPL_H_

#include "IApproximatorContainer.h"
#include "Config.h"
#include "IMatrixOperator.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "GateCoordinateCalculatorImpl.h"
#include "CoordinateAdditionBasedGateComposer.h"
#include "IDecomposer.h"
#include "ApplicationCommon.h"
#include "IResourceContainer.h"
#include "IComposer.h"
#include "IConverter.h"
#include "IElementComparator.h"

class SK2ApproximatorContainerImpl: public IApproximatorContainer {
public:
	SK2ApproximatorContainerImpl(SKApproximatorConfig2 config,
			CollectionConfig coreCollectionConfig);

	virtual ~SK2ApproximatorContainerImpl();

	virtual GateApproximatorPtr getGateApproximator();

protected:
	virtual void wireDependencies();
	virtual void releaseDependencies();

	void initRealCoordinateComparator();
	void initEpsilonRealCoordinate(RealCoordinate<GatePtr>& epsilonRealCoordinate);

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;
	ResourceContainerPtr m_pResourceContainer;

	CombinerPtr<GatePtr> m_pGateCombiner;

	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;

	GateDecomposerPtr m_pHarrowGateDecomposer;

	GateComposerPtr m_pGateComposer;
	ComposerPtr<RealCoordinate<GatePtr> > m_pGateCoordinateComposer;
	ComparatorPtr<RealCoordinate<GatePtr> > m_pRealCoordinateComparator;
	CombinerPtr<RealCoordinate<GatePtr> > m_pGateCoordinateCombiner;
	ConverterPtr<GatePtr, RealCoordinate<GatePtr> > m_pGateCoordinateConveter;
	DistanceCalculatorPtr<RealCoordinate<GatePtr> > m_pGateCoordinateDistanceCalculator;

	SKApproximatorConfig2 m_approximatorConfig;
	CollectionConfig m_coreCollectionConfig;
};


#endif /* SK2APPROXIMATORCONTAINERIMPL_H_ */
