/*
 * SimpleParallelComposerContainer.h
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#ifndef SIMPLEPARALLELCOMPOSERCONTAINER_H_
#define SIMPLEPARALLELCOMPOSERCONTAINER_H_

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

class SimpleParallelComposerContainerImpl: public IComposerContainer {
public:
	SimpleParallelComposerContainerImpl(int nbCandidates, const ParallelConfig& parallelConfig);
	virtual ~SimpleParallelComposerContainerImpl();

	//Override
	GateComposerPtr getGateComposer();

private:
	void wireDependencies();
	void releaseDependencies();

	int m_nbCandidates;
	ParallelConfig m_parallelConfig;

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	CombinerPtr<GatePtr> m_pGateCombiner;
	ElementSetLogPtr<GatePtr> m_pGateSetLog;

	TaskExecutorPtr<LookupResult<GatePtr> > m_pTaskExecutor;
	TaskQueuePtr<QueuedTask<LookupResult<GatePtr> > > m_pTaskQueue;
};


#endif /* SIMPLEPARALLELCOMPOSERCONTAINER_H_ */
