/*
 * GenerateAndStoreApproximationsCommand.h
 *
 *  Created on: Oct 26, 2015
 *      Author: pham
 */

#ifndef GENERATEANDSTOREAPPROXIMATIONSCOMMAND_H_
#define GENERATEANDSTOREAPPROXIMATIONSCOMMAND_H_

#include "AbstractChainCommandExecutor.h"
#include "IApproximator.h"
#include "IWriter.h"
#include "IOCommon.h"
#include "AlgoCommon.h"
#include "ApplicationCommon.h"
#include "IDistanceCalculator.h"
#include "IIterator.h"

class GenerateAndStoreApproximationsCommand: public AbstractChainCommand {
public:
	GenerateAndStoreApproximationsCommand(GateApproximatorPtr pApproximator,
			GateCollectionPtr pCoreCollection,
			GateDistanceCalculatorPtr pGateDistanceCalculator,
			mreal_t approximatorEpsilon,
			const std::vector<GatePtr>& targets,
			PersitableGateCollectionPtr pPersistableGateCollection,
			std::string storingFileName);

	virtual ~GenerateAndStoreApproximationsCommand();

	//Override
	void doExecute();

private:
	GateLookupResultIteratorPtr getApproximatorResultIter(GatePtr pTarget);

	void storeResultsToPersistableCollection(GateLookupResultIteratorPtr pGateIter);

	void rebuildPersistableCollection();

	void persistResultsToStorage();

	GateApproximatorPtr m_pApproximator;
	GateCollectionPtr m_pCoreCollection;

	GateDistanceCalculatorPtr m_pGateDistanceCalculator;
	mreal_t m_approximatorEpsilon;
	std::vector<GatePtr> m_targets;

	PersitableGateCollectionPtr m_pPersistableGateCollection;

	std::string m_storingFileName;
};

#endif /* GENERATEANDSTOREAPPROXIMATIONSCOMMAND_H_ */
