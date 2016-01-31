/*
 * MultiNearBalancePartsGateDecomposer.h
 *
 *  Created on: Jan 30, 2016
 *      Author: pham
 */

#ifndef MULTINEARBALANCEPARTSGATEDECOMPOSER_H_
#define MULTINEARBALANCEPARTSGATEDECOMPOSER_H_

#include "IDecomposer.h"
#include "Gate.h"
#include "ICoordinateCalculator.h"
#include "IMatrixOperator.h"

class MultiNearBalancePartsGateDecomposer : public IDecomposer<GatePtr>  {
public:
	MultiNearBalancePartsGateDecomposer(RealCoordinateCalculatorPtr<GatePtr> pCoordinateCalculator,
			MatrixOperatorPtr pMatrixOperator);

	virtual ~MultiNearBalancePartsGateDecomposer(){};

	//Override
	void decomposeElement(GatePtr pGate, std::vector<GatePtr>& subGates, int nbSubElements = 0);

private:
	void getNearOnePartGates(RealCoordinatePtr<GatePtr> pGateCoordinate, std::vector<GatePtr>& subGates, int nbSubElements);

	void modifyCoordinate(real_coordinate_t& coordinate);

	void getComplementaryGate(GatePtr pGate, const std::vector<GatePtr>& subGates, GatePtr& rpComplementaryGate);

	MatrixOperatorPtr m_pMatrixOperator;
	RealCoordinateCalculatorPtr<GatePtr> m_pCoordinateCalculator;
};

#endif /* MULTINEARBALANCEPARTSGATEDECOMPOSER_H_ */
