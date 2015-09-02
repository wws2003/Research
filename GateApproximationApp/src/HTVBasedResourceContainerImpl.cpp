/*
 * HTVBasedResourceContainerImpl.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: pham
 */

#include "HTVBasedResourceContainerImpl.h"

#include "IMatrixOperator.h"
#include "VectorBasedCollectionImpl.hpp"
#include "GateSearchSpaceConstructorImpl.h"
#include "GateCombinerImpl.h"
#include "ICombiner.h"
#include "SimpleDenseMatrixImpl.h"
#include "GateSelectiveCombinabilityCheckerImpl.h"
#include "GateCancelationCombinabilityCheckerImpl.h"
#include "GateIdentityCycleCombinabilityCheckerImpl.h"
#include "SampleMatrixOperator.h"
#include "SimpleDenseMatrixFactoryImpl.h"

HTVBasedResourceContainerImpl::HTVBasedResourceContainerImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) : SampleResourceContainerImpl(pMatrixOperator, pMatrixFactory) {

}

void HTVBasedResourceContainerImpl::initTwoQubitsLibGates(GateNameCostMap& gateNameCostMap) {
	gateNameCostMap.clear();
	gateNameCostMap["CV1"] = 100;
	gateNameCostMap["CV2"] = 100;
	gateNameCostMap["H1"] = 10;
	gateNameCostMap["H2"] = 10;
	gateNameCostMap["T1"] = 20;
	gateNameCostMap["T2"] = 20;
}

void HTVBasedResourceContainerImpl::getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker) {
	CombinableGateLabelMap combinableLabelMap;

	combinableLabelMap.insert(CombinableGateLabelPair("H1","CV1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","CV2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","T2"));

	combinableLabelMap.insert(CombinableGateLabelPair("H2","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H2","CV1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H2","CV2"));

	combinableLabelMap.insert(CombinableGateLabelPair("T1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","CV1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T1","CV2"));

	combinableLabelMap.insert(CombinableGateLabelPair("T2","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","CV1"));
	combinableLabelMap.insert(CombinableGateLabelPair("T2","CV2"));

	combinableLabelMap.insert(CombinableGateLabelPair("CV1","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV1","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV1","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV1","CV2"));

	combinableLabelMap.insert(CombinableGateLabelPair("CV2","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV2","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV2","T1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV2","T2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV2","CV1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV2","CV2"));

	pGateSelectiveCombinabilityChecker = GateCombinabilityCheckerPtr(new GateSelectiveCombinabilityCheckerImpl(combinableLabelMap));

}

void HTVBasedResourceContainerImpl::getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker) {
	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair("CV1", 4));
	identityCycleMap.insert(GateLabelIdentityCyclePair("CV2", 4));
	identityCycleMap.insert(GateLabelIdentityCyclePair("T1", 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair("T2", 8));

	pGateIdentityCycleCombinabilityChecker = new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap);
}

void HTVBasedResourceContainerImpl::getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker) {
	GateLabelCancelationMap cancelationMap;
	pGateCancelationCombinabilityChecker = GateCombinabilityCheckerPtr(new GateCancelationCombinabilityCheckerImpl(cancelationMap));
}



