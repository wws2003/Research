/*
 * SKGateApproximator.h
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#ifndef SKGATEAPPROXIMATOR_H_
#define SKGATEAPPROXIMATOR_H_


#include "SKElementApproximator.h"
#include "IMatrixOperator.h"
#include "Gate.h"

class SKGateApproximator : public SKElementApproximator<GatePtr> {
public:
	SKGateApproximator(DecomposerPtr<GatePtr> pQueryDecomposer,
				ComposerPtr<GatePtr> pBuildingBlockComposer,
				mreal_t initialEpsilon,
				int nbCandidates,
				int recursiveLevel,
				MatrixOperatorPtr pMatrixOperator,
				ApproximatorPtr<GatePtr> pHelperApproximator);

protected:
	//Override
	void calculateResidual(GatePtr apprx, GatePtr query, GatePtr& residual);

private:
	MatrixOperatorPtr m_pMatrixOperator;

};

#endif /* SKGATEAPPROXIMATOR_H_ */
