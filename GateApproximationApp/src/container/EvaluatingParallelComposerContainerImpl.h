/*
 * EvaluatingParallelComposerContainerImpl.h
 *
 *  Created on: Feb 23, 2016
 *      Author: pham
 */

#ifndef EVALUATINGPARALLELCOMPOSERCONTAINERIMPL_H_
#define EVALUATINGPARALLELCOMPOSERCONTAINERIMPL_H_

#include "IEvaluatingComposerContainer.h"
#include "ApplicationCommon.h"
#include "IComposerContainer.h"
#include "Config.h"

class EvaluatingParallelComposerContainerImpl: public IEvaluatingComposerContainer {
public:
	EvaluatingParallelComposerContainerImpl(const ParallelConfig& parallelConfig);
	virtual ~EvaluatingParallelComposerContainerImpl();

	//Override
	GateComposerPtr getStandardGateComposer();

	//Override
	GateComposerPtr getEvaluatedGateComposer();

private:
	void wireDependencies(const ParallelConfig& parallelConfig);
	void releaseDependencies();

	ComposerContainerPtr m_pStandardComposerContainer;
	ComposerContainerPtr m_pEvaluatedComposerContainer;
};

#endif /* EVALUATINGPARALLELCOMPOSERCONTAINERIMPL_H_ */
