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
#include "ITaskExecutor.h"
#include "ITaskQueue.h"
#include "TaskCommon.h"
#include "ICollection.h"
#include "ILookupResultFilter.h"
#include "ApplicationCommon.h"
#include "IComposerContainer.h"
#include "QueueBasedTaskExecutorImpl.h"

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

	TaskExecutorPtr<LookupResult<GatePtr> > m_pTaskExecutor;
	TaskQueuePtr<QueuedTask<LookupResult<GatePtr> > > m_pTaskQueue;
};

#endif /* SIMPLECOMPOSERCONTAINERIMPL_H_ */
