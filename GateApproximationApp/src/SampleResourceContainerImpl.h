/*
 * SampleResourceContainerImpl.h
 *
 *  Created on: Aug 15, 2015
 *      Author: pham
 */

#ifndef SAMPLERESOURCECONTAINERIMPL_H_
#define SAMPLERESOURCECONTAINERIMPL_H_

#include "IResourceContainer.h"
#include "OperatorCommon.h"
#include "AlgoInternal.h"
#include "GateFactory.h"
#include "ILibraryMatrixStore.h"
#include <map>

typedef std::map<std::string, cost_t> GateNameCostMap;

class SampleResourceContainerImpl : public IResourceContainer {
public:
	SampleResourceContainerImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);

	virtual ~SampleResourceContainerImpl();

	void setup();

	virtual void getUniversalGates(GateCollectionPtr pLibraryGates, int nbQubits = 2);

	virtual void getGateCombinabilityCheckers(GateCombinabilityCheckers& checkers, int nbQubits = 2);

	virtual void getMatrixOrthonormalBasis(MatrixPtrVector& pBasis, int nbQubits = 2);

	void getTargetGatesAndEpsilon(std::vector<GatePtr>& targets, int nbQubits = 2);

protected:

	//Library (universal) gates: H, T, and T'
	virtual void initSingleQubitLibGates(GateNameCostMap& gateNameCostMap);

	//Library (universal) gates: CNOT, H and T
	virtual void initTwoQubitsLibGates(GateNameCostMap& gateNameCostMap);

	/* ----------------------1-qubit gate combination checker----------------------*/
	virtual void initSingleQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

	virtual void getSingleQubitGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

	virtual void getSingleQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);
	/*----------------------End of 1-qubit gate combination checker----------------------*/

	/* ----------------------2-qubits gate combination checker----------------------*/
	virtual void initTwoQubitGateCombinabilityCheckers(GateCombinabilityCheckers& checkers);

	virtual void getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker);

	virtual void getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

	virtual void getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);
	/*----------------------End of 2-qubits gate combination checker----------------------*/

private:
	void collectLibraryGatesFromMap(const GateNameCostMap& gateNameCostMap, GateFactoryPtr pGateFactory, CollectionPtr<GatePtr> pLibraryGates);

	LibraryMatrixStorePtr m_pLibraryMatrixStore;

	MatrixOperatorPtr m_pMatrixOperator;
	MatrixFactoryPtr m_pMatrixFactory;
	GateFactoryPtr m_pGateFactory;

	GateCollectionPtr m_pSingleQubitLibraryGates;
	GateCollectionPtr m_pTwoQubitLibraryGates;

	GateCombinabilityCheckers m_singleQubitGateCombinerCheckers;
	GateCombinabilityCheckers m_twoQubitGateCombinerCheckers;

	MatrixPtrVector m_basis2;
	MatrixPtrVector m_basis4;

	GatePtr m_pIdentity2;
	GatePtr m_pIdentity4;
};


#endif /* SAMPLERESOURCECONTAINERIMPL_H_ */
