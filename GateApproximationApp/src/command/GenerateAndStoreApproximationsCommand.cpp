/*
 * GenerateAndStoreApproximationsCommand.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: pham
 */

#include "GenerateAndStoreApproximationsCommand.h"
#include "Gate.h"
#include "IPersistableCollection.h"
#include <iostream>

GenerateAndStoreApproximationsCommand::GenerateAndStoreApproximationsCommand(GateApproximatorPtr pApproximator,
		GateCollectionPtr pCoreCollection,
		GateDistanceCalculatorPtr pGateDistanceCalculator,
		mreal_t approximatorEpsilon,
		const std::vector<GatePtr>& targets,
		PersitableGateCollectionPtr pPersistableGateCollection,
		std::string storingFileName) {

	m_pApproximator = pApproximator;
	m_pCoreCollection = pCoreCollection;
	m_pGateDistanceCalculator = pGateDistanceCalculator;
	m_approximatorEpsilon = approximatorEpsilon;
	m_targets = targets;
	m_pPersistableGateCollection = pPersistableGateCollection;
	m_storingFileName = storingFileName;
}

GenerateAndStoreApproximationsCommand::~GenerateAndStoreApproximationsCommand() {
	_destroy(m_pPersistableGateCollection);
	_destroy(m_pCoreCollection);
	_destroy(m_pApproximator);
}

void GenerateAndStoreApproximationsCommand::doExecute() {
	for(unsigned int i = 0; i < m_targets.size(); i++) {
		GatePtr pTarget = m_targets[i];

		GateIteratorPtr pGateIter = getApproximatorResultIter(pTarget);
		storeResultsToPersistableCollection(pGateIter);
		_destroy(pGateIter);
	}

	rebuildPersistableCollection();

	persistResultsToStorage();
}

GateIteratorPtr GenerateAndStoreApproximationsCommand::getApproximatorResultIter(GatePtr pTarget) {
	return m_pApproximator->getApproximateElements(m_pCoreCollection, pTarget, m_pGateDistanceCalculator, m_approximatorEpsilon);
}

void GenerateAndStoreApproximationsCommand::storeResultsToPersistableCollection(GateIteratorPtr pGateIter) {
	if(pGateIter == NullPtr) {
		return;
	}
	long nbResults = 0;
	while(!pGateIter->isDone()) {
		GatePtr pGate = pGateIter->getObj();
		m_pPersistableGateCollection->addElement(pGate);
		nbResults++;
		pGateIter->next();
	}
	std::cout << "Found " << nbResults << " results" << std::endl;
}

void GenerateAndStoreApproximationsCommand::rebuildPersistableCollection() {
	std::cout << "Begin to rebuild results collection before persist" << std::endl;
	m_pPersistableGateCollection->rebuildStructure(m_pGateDistanceCalculator);
}

void GenerateAndStoreApproximationsCommand::persistResultsToStorage() {
	std::cout << "Begin to persist re-structured results collection" << std::endl;
	m_pPersistableGateCollection->save(m_storingFileName);
}
