/*
 * EvaluateApproximatorCommand.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef EVALUATEAPPROXIMATORCOMMAND_H_
#define EVALUATEAPPROXIMATORCOMMAND_H_

#include "AbstractChainCommandExecutor.h"
#include "IApproximatorContainer.h"
#include "IEvaluatorContainer.h"
#include "ICollectionContainer.h"
#include "ApplicationCommon.h"

class EvaluateApproximatorCommand: public AbstractChainCommand {
public:
	EvaluateApproximatorCommand(ApproximatorContainerPtr pApproximatorContainer,
			CollectionContainerPtr pCollectionContainer,
			EvaluatorContainerPtr pEvaluatorContainer);
	virtual ~EvaluateApproximatorCommand();

protected:
	//Override
	void doExecute();

private:
	ApproximatorContainerPtr m_pApproximatorContainer;
	CollectionContainerPtr m_pCollectionContainer;
	EvaluatorContainerPtr m_pEvaluatorContainer;
};

#endif /* EVALUATEAPPROXIMATOR_H_ */
