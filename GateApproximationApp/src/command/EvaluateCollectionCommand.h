/*
 * EvaluateCollectionCommand.h
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#ifndef EVALUATECOLLECTIONCOMMAND_H_
#define EVALUATECOLLECTIONCOMMAND_H_

#include "ApplicationCommon.h"
#include "AbstractChainCommandExecutor.h"
#include "ICollection.h"
#include "AlgoCommon.h"
#include "EvaluateCommon.h"
#include "ISearchSpaceEvaluator.h"

class EvaluateCollectionCommand : public AbstractChainCommand {
public:
	EvaluateCollectionCommand(GateCollectionPtr pCollection,
			GateSearchSpaceEvaluatorPtr pEvaluator);
	virtual ~EvaluateCollectionCommand();

protected:
	//Override
	void doExecute();

private:
	GateCollectionPtr m_pCollection;
	GateSearchSpaceEvaluatorPtr m_pEvaluator;
};

#endif /* EVALUATECOLLECTIONCOMMANDEXECUTOR_H_ */
