/*
 * GateCombinerImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "GateCombinerImpl.h"
#include "IGateCombinabilityChecker.h"
#include <iostream>

#if MPFR_REAL
#define IDENTITY_NOISE_THRESOLD 1e-10
#else
#define IDENTITY_NOISE_THRESOLD 1e-8
#endif

bool checkIdentity(MatrixPtr pMatrix);

GateCombinerImpl::GateCombinerImpl(const GateCombinabilityCheckers& combinabilityCheckers, MatrixOperatorPtr pMatrixOperator) {
	m_combinabilityCheckers = combinabilityCheckers;
	m_pMatrixOperator = pMatrixOperator;
}

void GateCombinerImpl::combine(GatePtr pGate1, GatePtr pGate2, GatePtr& result) {
	result = NullPtr;

	for(unsigned int i = 0; i< m_combinabilityCheckers.size(); i++) {
		if(!m_combinabilityCheckers[i]->canCombine(pGate1, pGate2)) {
			return;
		}
	}

	MatrixPtr pCombinedMatrix = NullPtr;
	m_pMatrixOperator->multiply(pGate1->getMatrix(), pGate2->getMatrix(), pCombinedMatrix);

	//If combined matrix is actually identity, abort the combination
	if(!checkIdentity(pCombinedMatrix)) {
		_destroy(pCombinedMatrix);
		return;
	}

	cost_t combinedCost = pGate1->getCost() + pGate2->getCost();

	LabelSeq combinedLabel;
	combinedLabel.insert(combinedLabel.end(), pGate1->getLabelSeq().begin(), pGate1->getLabelSeq().end());
	combinedLabel.insert(combinedLabel.end(), pGate2->getLabelSeq().begin(), pGate2->getLabelSeq().end());

	std::string combinedLabelStr = pGate1->getLabelStr() + pGate2->getLabelStr();

	result = GatePtr(new Gate(pCombinedMatrix, combinedCost, combinedLabel, combinedLabelStr));

}

bool checkIdentity(MatrixPtr pMatrix) {
	int nbRows, nbColums;
	pMatrix->getSize(nbRows, nbColums);

	for(int row = 0; row < nbRows; row++) {
		for(int column = 0; column < nbColums; column++) {
			ComplexVal v = pMatrix->getValue(row, column);

			ComplexVal identityElement = (row == column) ? (ComplexVal)1.0 : (ComplexVal)0.0;

			//If matrix[row][column] differs from I[row][column], consider matrix is not identity
			mreal_t err = std::norm(v - identityElement);
			if(err > IDENTITY_NOISE_THRESOLD) {
				return true;
			}
		}
	}

	return false;
}

