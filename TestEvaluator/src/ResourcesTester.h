/*
 * ResourcesTester.h
 *
 *  Created on: Apr 25, 2016
 *      Author: pham
 */

#ifndef RESOURCESTESTER_H_
#define RESOURCESTESTER_H_

#include "ResourcesCommon.h"
#include "IGateStore.h"
#include "IGateCombinabilityCheckerFactory.h"
#include "ISearchSpaceConstructor.h"
#include "IMatrixOperator.h"
#include "IMatrixFactory.h"
#include "GateSearchSpaceConstructorImpl.h"

class ResourcesTester {
public:
	ResourcesTester();
	virtual ~ResourcesTester();

	void testSimpleHTCombinationChecker();

	void testSimpleHTSCombinationChecker();

private:
	MatrixOperatorPtr m_pMatrixOperator;
	MatrixFactoryPtr m_pMatrixFactory;
	GateStorePtr m_pGateStore;
	GateCombinabilityCheckerFactoryPtr m_pGateCombinabilityCheckerFactory;
};

#endif /* RESOURCESTESTER_H_ */
