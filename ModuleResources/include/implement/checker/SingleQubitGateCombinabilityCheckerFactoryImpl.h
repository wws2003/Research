/*
 * SingleQubitGateCombinabilityCheckerFactoryImpl.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pham
 */

#ifndef SINGLEQUBITGATECOMBINABILITYCHECKERFACTORYIMPL_H_
#define SINGLEQUBITGATECOMBINABILITYCHECKERFACTORYIMPL_H_

#include "IGateCombinabilityCheckerFactory.h"

class SingleQubitGateCombinabilityCheckerFactoryImpl: public IGateCombinabilityCheckerFactory {
public:
	SingleQubitGateCombinabilityCheckerFactoryImpl();
	virtual ~SingleQubitGateCombinabilityCheckerFactoryImpl();

	//Implement
	void getGateCombinabilityCheckers(LibrarySet librarySet, GateCombinabilityCheckers& checkers);

private:
	void initHTGateCombinabilityCheckers();
	void initHTSGateCombinabilityCheckers();

	void releaseCheckers(GateCombinabilityCheckers& checkers);

	GateCombinabilityCheckerPtr getGateIdentityCycleCombinabilityChecker(LibrarySet librarySet);
	GateCombinabilityCheckerPtr getGateCancelationCombinabilityChecker(LibrarySet librarySet);

	GateCombinabilityCheckers m_htGateCombinabilityCheckers;
	GateCombinabilityCheckers m_htsGateCombinabilityCheckers;
};

#endif /* SAMPLEGATECOMBINABILITYCHECKERFACTORY_H_ */
