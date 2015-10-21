/*
 * EvaluateCollectionCommandExecutor.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef EVALUATECOLLECTIONCOMMANDEXECUTOR_H_
#define EVALUATECOLLECTIONCOMMANDEXECUTOR_H_

#include "ApplicationCommon.h"
#include "ICollectionContainer.h"
#include "IEvaluatorContainer.h"
#include "AbstractChainCommandExecutor.h"

class EvaluateCollectionCommandExecutor : public AbstractChainCommandExecutor {
public:
	EvaluateCollectionCommandExecutor(CollectionContainerPtr pCollectionContainer, EvaluatorContainerPtr pEvaluatorContainer);
	virtual ~EvaluateCollectionCommandExecutor();

protected:
	//Override
	void doExecute();

private:
	CollectionContainerPtr m_pCollectionContainer;
	EvaluatorContainerPtr m_pEvaluatorContainer;
};

#endif /* EVALUATECOLLECTIONCOMMANDEXECUTOR_H_ */
