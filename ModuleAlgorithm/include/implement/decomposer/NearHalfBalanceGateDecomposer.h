/*
 * NearHalfBalanceGateDecomposer.h
 *
 *  Created on: Jan 2, 2016
 *      Author: pham
 */

#ifndef NEARHALFBALANCEGATEDECOMPOSER_H_
#define NEARHALFBALANCEGATEDECOMPOSER_H_

#include "IDecomposer.h"
#include "Gate.h"
#include "ICoordinateCalculator.h"
#include "IMatrixOperator.h"

class NearHalfBalanceGateDecomposer : public IDecomposer<GatePtr> {
public:
	NearHalfBalanceGateDecomposer(RealCoordinateCalculatorPtr<GatePtr> pCoordinateCalculator,
			MatrixOperatorPtr pMatrixOperator);
	virtual ~NearHalfBalanceGateDecomposer(){};

	//Override
	void decomposeElement(GatePtr pGate, std::vector<GatePtr>& subGates, int nbSubElements = 0);

private:
	void modifyCoordinate(real_coordinate_t& coordinate);

	void getNearSqrtGate(RealCoordinatePtr<GatePtr> pGateCoordinate, GatePtr& rpNearSqrtGate);

	void getComplementaryGate(GatePtr pGate, GatePtr pNearSqrtGate, GatePtr& rpComplementaryGate);

	MatrixOperatorPtr m_pMatrixOperator;
	RealCoordinateCalculatorPtr<GatePtr> m_pCoordinateCalculator;
};



#endif /* NEARHALFBALANCEGATEDECOMPOSER_H_ */
