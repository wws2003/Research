/*
 * EvaluateCollectionCommand.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef EVALUATECOLLECTIONCOMMAND_H_
#define EVALUATECOLLECTIONCOMMAND_H_

#include "ApplicationCommon.h"
#include "ICollectionContainer.h"
#include "IEvaluatorContainer.h"
#include "AbstractChainCommandExecutor.h"

class EvaluateCollectionCommand : public AbstractChainCommand {
public:
	EvaluateCollectionCommand(CollectionContainerPtr pCollectionContainer, EvaluatorContainerPtr pEvaluatorContainer);
	virtual ~EvaluateCollectionCommand();

protected:
	//Override
	void doExecute();

private:
	CollectionContainerPtr m_pCollectionContainer;
	EvaluatorContainerPtr m_pEvaluatorContainer;
};

#endif /* EVALUATECOLLECTIONCOMMANDEXECUTOR_H_ */
