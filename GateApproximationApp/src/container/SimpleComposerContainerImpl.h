/*
 * SimpleComposerContainerImpl.h
 *
 *  Created on: Feb 22, 2016
 *      Author: pham
 */

#ifndef SIMPLECOMPOSERCONTAINERIMPL_H_
#define SIMPLECOMPOSERCONTAINERIMPL_H_

#include "Config.h"
#include "IComposer.h"
#include "Gate.h"
#include "ICombiner.h"
#include "IMatrixOperator.h"
#include "IMatrixFactory.h"
#include "IElementSetLog.h"
#include "ICollection.h"
#include "ILookupResultFilter.h"
#include "ApplicationCommon.h"
#include "IComposerContainer.h"

class SimpleComposerContainerImpl: public IComposerContainer {
public:
	SimpleComposerContainerImpl(int nbCandidates = 0);
	virtual ~SimpleComposerContainerImpl();

	//Override
	GateComposerPtr getGateComposer();

private:
	void wireDependencies();
	void releaseDependencies();

	int m_nbCandidates;

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	CombinerPtr<GatePtr> m_pGateCombiner;
	ElementSetLogPtr<GatePtr> m_pGateSetLog;
};

#endif /* SIMPLECOMPOSERCONTAINERIMPL_H_ */
