/*
 * CommandFactory.h
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#ifndef COMMANDFACTORY_H_
#define COMMANDFACTORY_H_

#include "ICommand.h"
#include "IEvaluatorContainer.h"
#include "ICollectionContainer.h"
#include "Config.h"
#include "ApplicationCommon.h"

class CommandFactory {
public:
	CommandFactory();
	virtual ~CommandFactory();

	virtual CommandPtr getCommand(int commandCode, const CommandParams& commandParams);

private:
	void resetCollectionContainer(CollectionConfig collectionConfig);
	void resetEvaluationContainer(EvaluatorConfig evaluatorConfig);

	CollectionContainerPtr m_pCollectionContainer;
	EvaluatorContainerPtr m_pEvaluatorContainer;
};

#endif /* COMMANDFACTORY_H_ */
