/*
 * LazyGateDistanceCalculator.h
 *
 *  Created on: Oct 20, 2015
 *      Author: pham
 */

#ifndef LAZYGATEDISTANCECALCULATORIMPL_H_
#define LAZYGATEDISTANCECALCULATORIMPL_H_

#include "IDistanceCalculator.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"

class LazyGateDistanceCalculatorImpl: public IGateDistanceCalculator {
public:
	LazyGateDistanceCalculatorImpl(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator,
			MatrixOperatorPtr pMatrixOperator);

	virtual ~LazyGateDistanceCalculatorImpl(){};

	virtual mreal_t distance(GatePtr pGate1, GatePtr pGate2);

protected:
	virtual void provideMatrixToGate(GatePtr pGate) = 0;

private:
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* LAZYGATEDISTANCECALCULATOR_H_ */
