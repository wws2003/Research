/*
 * HVBasedResourceContainerImpl.h
 *
 *  Created on: Aug 27, 2015
 *      Author: pham
 */

#ifndef HVBASEDRESOURCECONTAINERIMPL_H_
#define HVBASEDRESOURCECONTAINERIMPL_H_

#include "SampleResourceContainerImpl.h"

class HVBasedResourceContainerImpl: public SampleResourceContainerImpl {
public:
	HVBasedResourceContainerImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);

protected:

	//Library (universal) gates: H and CV
	virtual void initTwoQubitsLibGates(GateNameCostMap& gateNameCostMap);

	/* ----------------------2-qubits gate combination checker----------------------*/
	virtual void getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker);

	virtual void getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

	virtual void getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);
	/*----------------------End of 2-qubits gate combination checker----------------------*/

};



#endif /* HVBASEDRESOURCECONTAINERIMPL_H_ */
