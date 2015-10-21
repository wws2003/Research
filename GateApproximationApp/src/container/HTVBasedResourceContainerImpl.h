/*
 * HTVBasedResourceContainerImpl.h
 *
 *  Created on: Sep 1, 2015
 *      Author: pham
 */

#ifndef HTVBASEDRESOURCECONTAINERIMPL_H_
#define HTVBASEDRESOURCECONTAINERIMPL_H_

#include "SampleResourceContainerImpl.h"

class HTVBasedResourceContainerImpl: public SampleResourceContainerImpl {
public:
	HTVBasedResourceContainerImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);

protected:

	//Library (universal) gates: H and CV
	virtual void initTwoQubitsLibGates(GateNameCostMap& gateNameCostMap);

	/* ----------------------2-qubits gate combination checker----------------------*/
	virtual void getTwoQubitsGateSelectiveCombinabilityChecker(GateCombinabilityCheckerPtr& pGateSelectiveCombinabilityChecker);

	virtual void getTwoQubitsGateIdentityCycleCombinabilityChecker(GateCombinabilityCheckerPtr& pGateIdentityCycleCombinabilityChecker);

	virtual void getTwoQubitsGateCancelationCombinabilityChecker(GateCombinabilityCheckerPtr& pGateCancelationCombinabilityChecker);
	/*----------------------End of 2-qubits gate combination checker----------------------*/

};




#endif /* HTVBASEDRESOURCECONTAINERIMPL_H_ */
