/*
 * GateCoordinateCalculator.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef GATECOORDINATECALCULATORIMPL_H_
#define GATECOORDINATECALCULATORIMPL_H_

#include "ICoordinateCalculator.h"
#include "OperatorCommon.h"
#include "MathConceptsCommon.h"

class GateCoordinateCalculatorImpl: public IGateRealCoordinateCalculator {
public:
	GateCoordinateCalculatorImpl(MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator);
	virtual ~GateCoordinateCalculatorImpl(){};

	//Override
	void calulateElementCoordinate(GatePtr pGate, GateRealCoordinatePtrRef& prGateRealCoordinatePtr) const;

	//Override
	void calculateElementFromCoordinate(GateRealCoordinatePtr pCoordinate) const;

private:
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
};



#endif /* GATECOORDINATECALCULATOR_H_ */
