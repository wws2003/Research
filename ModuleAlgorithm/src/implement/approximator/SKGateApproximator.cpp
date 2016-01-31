/*
 * SKGateApproximator.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#include "SKGateApproximator.h"
#include "SKElementApproximator.cpp"

SKGateApproximator::SKGateApproximator(DecomposerPtr<GatePtr> pQueryDecomposer,
				ComposerPtr<GatePtr> pBuildingBlockComposer,
				mreal_t initialEpsilon,
				int nbCandidates,
				int recursiveLevel,
				MatrixOperatorPtr pMatrixOperator,
				ApproximatorPtr<GatePtr> pHelperApproximator) : SKElementApproximator<GatePtr>(pQueryDecomposer,
						pBuildingBlockComposer,
						initialEpsilon,
						nbCandidates,
						recursiveLevel,
						pHelperApproximator) {

	m_pMatrixOperator = pMatrixOperator;
}

void SKGateApproximator::calculateResidual(GatePtr apprx, GatePtr query, GatePtr& residual) {
	MatrixPtr pResidualMatrix;
	MatrixPtr pInverseApprx;
	m_pMatrixOperator->conjugateTranpose(apprx->getMatrix(), pInverseApprx);
	m_pMatrixOperator->multiply(pInverseApprx, query->getMatrix(), pResidualMatrix);
	_destroy(pInverseApprx);
	residual = GatePtr(new Gate(pResidualMatrix, apprx->getCost(), ""));
}




