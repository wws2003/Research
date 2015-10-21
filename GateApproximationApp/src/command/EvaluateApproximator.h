/*
 * EvaluateApproximator.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef EVALUATEAPPROXIMATOR_H_
#define EVALUATEAPPROXIMATOR_H_

#include "AbstractChainCommandExecutor.h"
#include "IApproximatorContainer.h"
#include "IEvaluatorContainer.h"
#include "ICollectionContainer.h"
#include "ApplicationCommon.h"

class EvaluateApproximator: public AbstractChainCommandExecutor {
public:
	EvaluateApproximator(ApproximatorContainerPtr pApproximatorContainer,
			CollectionContainerPtr pCollectionContainer,
			EvaluatorContainerPtr pEvaluatorContainer);
	virtual ~EvaluateApproximator();

protected:
	//Override
	void doExecute();

private:
	ApproximatorContainerPtr m_pApproximatorContainer;
	CollectionContainerPtr m_pCollectionContainer;
	EvaluatorContainerPtr m_pEvaluatorContainer;
};

#endif /* EVALUATEAPPROXIMATOR_H_ */
