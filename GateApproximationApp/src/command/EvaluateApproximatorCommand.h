/*
 * EvaluateApproximatorCommand.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef EVALUATEAPPROXIMATORCOMMAND_H_
#define EVALUATEAPPROXIMATORCOMMAND_H_

#include "AbstractChainCommandExecutor.h"
#include "ApplicationCommon.h"
#include "ISearchSpaceEvaluator.h"
#include "EvaluateCommon.h"

class EvaluateApproximatorCommand: public AbstractChainCommand {
public:
	EvaluateApproximatorCommand(GateApproximatorPtr pApproximator,
			GateCollectionPtr pCollection,
			GateSearchSpaceEvaluatorPtr pEvaluator);

	virtual ~EvaluateApproximatorCommand();

protected:
	//Override
	void doExecute();

private:
	GateApproximatorPtr m_pApproximator;
	GateCollectionPtr m_pCoreCollection;
	GateSearchSpaceEvaluatorPtr m_pEvaluator;
};

#endif /* EVALUATEAPPROXIMATOR_H_ */
