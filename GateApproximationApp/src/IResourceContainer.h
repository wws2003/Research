/*
 * IResourceContainer.h
 *
 *  Created on: Aug 15, 2015
 *      Author: pham
 */

#ifndef IRESOURCECONTAINER_H_
#define IRESOURCECONTAINER_H_

#include "ISearchSpaceConstructor.h"
#include "ISearchSpaceEvaluator.h"
#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "EvaluateCommon.h"
#include "IGateCombinabilityChecker.h"
#include "AlgoInternal.h"

/**
 * Classes to manage (supply/release) instances for universal set and combiners checkers.
 */

class IResourceContainer {
public:
	virtual ~IResourceContainer(){};

	virtual void getUniversalGates(GateCollectionPtr pUniversalGates, int nbQubits = 2) = 0;

	virtual void getGateCombinabilityCheckers(GateCombinabilityCheckers& checkers, int nbQubits = 2) = 0;

	virtual void getMatrixOrthonormalBasis(MatrixPtrVector& pBasis, int nbQubits = 2) = 0;

	virtual void getTargetGatesAndEpsilon(std::vector<GatePtr>& targets, int nbQubits = 2) = 0;
};

typedef IResourceContainer* ResourceContainerPtr;

#endif /* IRESOURCECONTAINER_H_ */
