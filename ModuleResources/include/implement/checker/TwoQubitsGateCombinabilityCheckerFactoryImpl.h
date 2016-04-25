/*
 * TwoQubitsGateCombinabilityCheckerFactoryImpl.h
 *
 *  Created on: Apr 21, 2016
 *      Author: pham
 */

#ifndef TWOQUBITSGATECOMBINABILITYCHECKERFACTORYIMPL_H_
#define TWOQUBITSGATECOMBINABILITYCHECKERFACTORYIMPL_H_

#include "IGateCombinabilityCheckerFactory.h"
#include "ResourcesCommon.h"

class TwoQubitsGateCombinabilityCheckerFactoryImpl: public IGateCombinabilityCheckerFactory {
public:
	TwoQubitsGateCombinabilityCheckerFactoryImpl();
	virtual ~TwoQubitsGateCombinabilityCheckerFactoryImpl();

	//Implement
	void getGateCombinabilityCheckers(LibrarySet librarySet, GateCombinabilityCheckers& checkers);

private:
	void initHTCNOTGateCombinabilityCheckers();
	void initHTSCNOTGateCombinabilityCheckers();

	void releaseCheckers(GateCombinabilityCheckers& checkers);

	GateCombinabilityCheckerPtr getGateIdentityCycleCombinabilityChecker(LibrarySet librarySet);
	GateCombinabilityCheckerPtr getGateCancelationCombinabilityChecker(LibrarySet librarySet);

	GateCombinabilityCheckerPtr getHTCNOTGateSelectiveCombinabilityChecker();
	GateCombinabilityCheckerPtr getHTSCNOTGateSelectiveCombinabilityChecker();

	GateCombinabilityCheckers m_htCNOTGateCombinabilityCheckers;
	GateCombinabilityCheckers m_htsCNOTGateCombinabilityCheckers;
};


#endif /* TWOQUBITGATECOMBINABILITYCHECKERFACTORYIMPL_H_ */
