/*
 * EvaluatePersistedCollectionForTargetsCommand.h
 *
 *  Created on: Oct 28, 2015
 *      Author: pham
 */

#ifndef EVALUATEPERSISTEDCOLLECTIONFORTARGETSCOMMAND_H_
#define EVALUATEPERSISTEDCOLLECTIONFORTARGETSCOMMAND_H_

#include "AbstractChainCommandExecutor.h"
#include "ICollection.h"
#include "AlgoCommon.h"
#include "EvaluateCommon.h"
#include "ISearchSpaceEvaluator.h"
#include "IPersistableCollection.h"

class EvaluatePersistedCollectionForTargetsCommand: public AbstractChainCommand {
public:
	EvaluatePersistedCollectionForTargetsCommand(PersitableGateCollectionPtr pCollection,
			GateSearchSpaceEvaluatorPtr pEvaluator,
			std::string storingFileName);

	virtual ~EvaluatePersistedCollectionForTargetsCommand();

protected:
	//Override
	void doExecute();

private:
	PersitableGateCollectionPtr m_pCollection;
	GateSearchSpaceEvaluatorPtr m_pEvaluator;
	std::string m_storingFileName;
};

#endif /* EVALUATEPERSISTEDCOLLECTIONFORTARGETSCOMMAND_H_ */
