/*
 * NearHalfBalanceGateDecomposer.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: pham
 */

#include "NearHalfBalanceGateDecomposer.h"
#include "Coordinate.hpp"

NearHalfBalanceGateDecomposer::NearHalfBalanceGateDecomposer(RealCoordinateCalculatorPtr<GatePtr> pCoordinateCalculator,
		MatrixOperatorPtr pMatrixOperator) {
	m_pCoordinateCalculator = pCoordinateCalculator;
	m_pMatrixOperator = pMatrixOperator;
}

//Override
void NearHalfBalanceGateDecomposer::decomposeElement(GatePtr pGate, std::vector<GatePtr>& subGates, int nbSubElements) {
	RealCoordinatePtr<GatePtr> pGateCoordinate = NullPtr;
	m_pCoordinateCalculator->calulateElementCoordinate(pGate, pGateCoordinate);

	GatePtr pNearSqrtGate = NullPtr;
	getNearSqrtGate(pGateCoordinate, pNearSqrtGate);

	GatePtr pComplementaryGate = NullPtr;
	getComplementaryGate(pGate, pNearSqrtGate, pComplementaryGate);

	subGates.push_back(pNearSqrtGate);
	subGates.push_back(pComplementaryGate);

	_destroy(pGateCoordinate);
}

void NearHalfBalanceGateDecomposer::getNearSqrtGate(RealCoordinatePtr<GatePtr> pGateCoordinate, GatePtr& rpNearSqrtGate) {
	real_coordinate_t nearHalfCoordinate;
	for(unsigned int i = 0; i < pGateCoordinate->getCoordinates().size(); i++) {
		nearHalfCoordinate.push_back(pGateCoordinate->getCoordinates()[i] / 2.0);
	}

	modifyCoordinate(nearHalfCoordinate);

	RealCoordinate<GatePtr> nearHalfGateCoordinate(NullPtr, nearHalfCoordinate);

	m_pCoordinateCalculator->calculateElementFromCoordinate(&nearHalfGateCoordinate);

	rpNearSqrtGate = nearHalfGateCoordinate.getElement();
}

void NearHalfBalanceGateDecomposer::modifyCoordinate(real_coordinate_t& coordinate) {
	mreal_t eps = 6e-2;
	size_t size = coordinate.size();
	for(unsigned int i = 0; i < size; i++) {
		coordinate[i] += (mreal::rand() - 0.5) * eps;
	}
}

void NearHalfBalanceGateDecomposer::getComplementaryGate(GatePtr pGate, GatePtr pNearSqrtGate, GatePtr& rpComplementaryGate) {
	MatrixPtr pNearSqrtMatrix = pNearSqrtGate->getMatrix();

	//m2 = m * m1'
	MatrixPtr pMatrix = pGate->getMatrix();
	MatrixPtr pComplementaryMatrix = NullPtr;
	m_pMatrixOperator->multiplyConjugateTranspose(pMatrix, pNearSqrtMatrix, pComplementaryMatrix);

	rpComplementaryGate = GatePtr(new Gate(pComplementaryMatrix, 0, ""));
}




