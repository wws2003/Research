/*
 * Util.h
 *
 *  Created on: Jun 23, 2015
 *      Author: pham
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "ISearchSpaceConstructor.h"
#include "ISearchSpaceEvaluator.h"
#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "AlgoInternal.h"
#include "EvaluateCommon.h"
#include "IGateCombinabilityChecker.h"

class AssemblerUtil {
public:

	AssemblerUtil();

	virtual ~AssemblerUtil();

	virtual inline void getMatrixOperator(MatrixOperatorPtr& pMatrixOperator) {pMatrixOperator = m_pMatrixOperator;};

	virtual void getLibraryGates(GateCollectionPtr& pLibraryGates, int nbQubits);

	virtual void getSampleGateSearchSpaceCollection(GateCollectionPtr& pGateCollection, int nbQubits, int maxSequenceLength);

	virtual void getSampleQueryGate(GatePtrVector& queries, int nbQubits);


private:
	MatrixOperatorPtr m_pMatrixOperator;
	MatrixFactoryPtr m_pMatrixFactory;

	GateCollectionPtr m_pSingleQubitLibraryGates;
	GateCollectionPtr m_pTwoQubitLibraryGates;

	GateCombinabilityCheckers m_singleQubitGateCombinerCheckers;
	GateCombinabilityCheckers m_twoQubitGateCombinerCheckers;

	CombinerPtr<GatePtr> m_pSingleQubitGateCombiner;
	CombinerPtr<GatePtr> m_pTwoQubitGateCombiner;

	GateSearchSpaceConstructorPtr m_pSingleQubitGateSearchSpaceConstructor;
	GateSearchSpaceConstructorPtr m_pTwoQubitGateSearchSpaceConstructor;

};

typedef AssemblerUtil* AssemblerUtilPtr;

#endif /* UTIL_H_ */
