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
	void getGateCombinabilityCheckers(R_LibrarySet librarySet, GateCombinabilityCheckers& checkers);

private:
	void initHTGateCombinabilityCheckers();
	void initHTSGateCombinabilityCheckers();

	void releaseCheckers(GateCombinabilityCheckers& checkers);

	GateCombinabilityCheckerPtr getGateIdentityCycleCombinabilityChecker(R_LibrarySet librarySet);
	GateCombinabilityCheckerPtr getGateCancelationCombinabilityChecker(R_LibrarySet librarySet);

	GateCombinabilityCheckers m_htGateCombinabilityCheckers;
	GateCombinabilityCheckers m_htsGateCombinabilityCheckers;
};

#endif /* SAMPLEGATECOMBINABILITYCHECKERFACTORY_H_ */
