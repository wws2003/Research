/*
 * FowlerSearchSpaceGeneratorTester.h
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#ifndef FOWLERSEARCHSPACEGENERATORTESTER_H_
#define FOWLERSEARCHSPACEGENERATORTESTER_H_

#include "GateSearchSpaceConstructorFowlerImpl.h"
#include "ICombiner.h"
#include "IGateStore.h"
#include "IMatrixFactory.h"
#include "IMatrixOperator.h"
#include "IDistanceCalculator.h"
#include "IGateCombinabilityCheckerFactory.h"

class FowlerSearchSpaceConstructorTester {
public:
	FowlerSearchSpaceConstructorTester();
	virtual ~FowlerSearchSpaceConstructorTester();

	void testFowlerSearchSpaceConstructor();

private:
	void assertEquivCollections(CollectionPtr<GatePtr> pCollectionConstructedByFowler, CollectionPtr<GatePtr> pSampleCollection);

	void assertUniqueElementsCollections(CollectionPtr<GatePtr> pCollectionConstructedByFowler);

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	CombinerPtr<GatePtr> m_pGateCombiner;
	GateStorePtr m_pGateStore;
	GateCombinabilityCheckerFactoryPtr m_pGateCombinabilityCheckerFactory;

	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	GateDistanceCalculatorPtr m_pGateDistanceCalculator;
};

#endif /* FOWLERSEARCHSPACEGENERATORTESTER_H_ */
