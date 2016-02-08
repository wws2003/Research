/*
 * EvaluateCompserCommand.h
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#ifndef EVALUATECOMPOSERCOMMAND_H_
#define EVALUATECOMPOSERCOMMAND_H_

#include "AbstractChainCommandExecutor.h"
#include "ApplicationCommon.h"
#include "EvaluateCommon.h"
#include "IComposerEvaluator.h"

class EvaluateComposerCommand: public AbstractChainCommand {
public:
	EvaluateComposerCommand(GateComposerPtr pEvaluatedComposer,
			GateComposerPtr pStandardComposer,
			GateComposerEvaluatorPtr pEvaluator);

	virtual ~EvaluateComposerCommand();

protected:
	//Override
	void doExecute();

private:
	GateComposerPtr m_pEvaluatedComposer;
	GateComposerPtr m_pStandardComposer;
	GateComposerEvaluatorPtr m_pEvaluator;
};

#endif /* EVALUATECOMPSERCOMMAND_H_ */
