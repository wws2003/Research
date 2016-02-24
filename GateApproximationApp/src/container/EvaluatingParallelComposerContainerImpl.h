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

class EvaluatingParallelComposerContainerImpl: public IEvaluatingComposerContainer {
public:
	EvaluatingParallelComposerContainerImpl(int nbThreads);
	virtual ~EvaluatingParallelComposerContainerImpl();

	//Override
	GateComposerPtr getStandardGateComposer();

	//Override
	GateComposerPtr getEvaluatedGateComposer();

private:
	void wireDependencies();
	void releaseDependencies();

	int m_nbThreads;

	ComposerContainerPtr m_pStandardComposerContainer;
	ComposerContainerPtr m_pEvaluatedComposerContainer;
};

#endif /* EVALUATINGPARALLELCOMPOSERCONTAINERIMPL_H_ */
