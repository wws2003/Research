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

class SampleResourceContainerImpl : public IResourceContainer {
public:
	SampleResourceContainerImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);

	virtual ~SampleResourceContainerImpl();

	virtual void getUniversalGates(GateCollectionPtr pLibraryGates, int nbQubits = 2);

	virtual void getGateCombinabilityCheckers(GateCombinabilityCheckers& checkers, int nbQubits = 2);

	virtual void getMatrixOrthonormalBasis(MatrixPtrVector& pBasis, int nbQubits = 2);

	void getTargetGatesAndEpsilon(std::vector<GatePtr>& targets, int nbQubits = 2);

private:
	MatrixOperatorPtr m_pMatrixOperator;
	MatrixFactoryPtr m_pMatrixFactory;

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
