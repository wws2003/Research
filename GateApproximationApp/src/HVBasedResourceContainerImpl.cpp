/*
 * HVBasedResourceContainerImpl.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: pham
 */

#include "HVBasedResourceContainerImpl.h"
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

HVBasedResourceContainerImpl::HVBasedResourceContainerImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) : SampleResourceContainerImpl(pMatrixOperator, pMatrixFactory) {

}

void HVBasedResourceContainerImpl::initTwoQubitsLibGates(GateNameCostMap& gateNameCostMap) {
	gateNameCostMap.clear();
	gateNameCostMap["CV1"] = 100;
	gateNameCostMap["CV2"] = 100;
	gateNameCostMap["H1"] = 10;
	gateNameCostMap["H2"] = 10;
}

void HVBasedResourceContainerImpl::getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker) {
	CombinableGateLabelMap combinableLabelMap;

	combinableLabelMap.insert(CombinableGateLabelPair("H1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","CV1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H1","CV2"));

	combinableLabelMap.insert(CombinableGateLabelPair("H2","CV1"));
	combinableLabelMap.insert(CombinableGateLabelPair("H2","CV2"));

	combinableLabelMap.insert(CombinableGateLabelPair("CV1","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV1","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV1","CV2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV1","CV2"));

	combinableLabelMap.insert(CombinableGateLabelPair("CV2","H1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV2","H2"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV2","CV1"));
	combinableLabelMap.insert(CombinableGateLabelPair("CV2","CV2"));

	pGateSelectiveCombinabilityChecker = GateCombinabilityCheckerPtr(new GateSelectiveCombinabilityCheckerImpl(combinableLabelMap));

}

void HVBasedResourceContainerImpl::getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker) {
	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair("CV1", 4));
	identityCycleMap.insert(GateLabelIdentityCyclePair("CV2", 4));

	pGateIdentityCycleCombinabilityChecker = new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap);
}

void HVBasedResourceContainerImpl::getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker) {
	GateLabelCancelationMap cancelationMap;
	pGateCancelationCombinabilityChecker = GateCombinabilityCheckerPtr(new GateCancelationCombinabilityCheckerImpl(cancelationMap));

}

