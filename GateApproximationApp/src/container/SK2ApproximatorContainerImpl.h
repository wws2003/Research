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
#include "IComposer.h"
#include "IConverter.h"
#include "IEvaluatingComposerContainer.h"
#include "IElementComparator.h"
#include "IGateStore.h"

class SK2ApproximatorContainerImpl: public IApproximatorContainer {
public:
	SK2ApproximatorContainerImpl(SKApproximatorConfig config,
			CollectionConfig coreCollectionConfig,
			CoordinateAdditionalBasedComposerConfig cadbConfig);

	virtual ~SK2ApproximatorContainerImpl();

	virtual GateApproximatorPtr getGateApproximator();

protected:
	virtual void wireDependencies();
	virtual void releaseDependencies();

	void initGateComposerContainer(const CoordinateAdditionalBasedComposerConfig& cadbConfig);

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	GateStorePtr m_pGateStore;

	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;

	GateDecomposerPtr m_pHarrowGateDecomposer;

	GateComposerPtr m_pGateComposer;
	ComposerContainerPtr m_pGateComposerContainer;

	SKApproximatorConfig m_approximatorConfig;
	CollectionConfig m_coreCollectionConfig;
};


#endif /* SK2APPROXIMATORCONTAINERIMPL_H_ */
