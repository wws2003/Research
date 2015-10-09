/*
 * GateMatrixDistanceCalculatorImpl.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef GATEMATRIXDISTANCECALCULATORIMPL_H_
#define GATEMATRIXDISTANCECALCULATORIMPL_H_

#include "IDistanceCalculator.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"

class GateDistanceCalculatorByMatrixImpl : public IGateDistanceCalculator {
public:
	GateDistanceCalculatorByMatrixImpl(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator);
	virtual ~GateDistanceCalculatorByMatrixImpl(){};

	virtual mreal_t distance(GatePtr pGate1, GatePtr pGate2);

private:
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
};

#endif /* GATEMATRIXDISTANCECALCULATORIMPL_H_ */
