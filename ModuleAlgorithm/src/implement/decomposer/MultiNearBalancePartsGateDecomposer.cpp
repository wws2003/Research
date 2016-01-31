/*
 * MultiNearBalancePartsGateDecomposer.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: pham
 */
#include "MultiNearBalancePartsGateDecomposer.h"
#include "Coordinate.hpp"

MultiNearBalancePartsGateDecomposer::MultiNearBalancePartsGateDecomposer(RealCoordinateCalculatorPtr<GatePtr> pCoordinateCalculator,
		MatrixOperatorPtr pMatrixOperator) {
	m_pCoordinateCalculator = pCoordinateCalculator;
	m_pMatrixOperator = pMatrixOperator;
}

//Override
void MultiNearBalancePartsGateDecomposer::decomposeElement(GatePtr pGate,
		std::vector<GatePtr>& subGates,
		int nbSubElements) {
	RealCoordinatePtr<GatePtr> pGateCoordinate = NullPtr;
	m_pCoordinateCalculator->calulateElementCoordinate(pGate, pGateCoordinate);

	subGates.clear();
	getNearOnePartGates(pGateCoordinate, subGates, nbSubElements - 1);

	GatePtr pComplementaryGate = NullPtr;
	getComplementaryGate(pGate, subGates, pComplementaryGate);

	subGates.push_back(pComplementaryGate);

	_destroy(pGateCoordinate);
}

void MultiNearBalancePartsGateDecomposer::getNearOnePartGates(RealCoordinatePtr<GatePtr> pGateCoordinate,
		std::vector<GatePtr>& subGates,
		int nbSubElements) {
	real_coordinate_t nearOnePartCoordinate;
	for(unsigned int i = 0; i < pGateCoordinate->getCoordinates().size(); i++) {
		nearOnePartCoordinate.push_back(pGateCoordinate->getCoordinates()[i] / mreal_t(nbSubElements));
	}

	for(int i = 0; i < nbSubElements; i++) {
		real_coordinate_t onePartCoord = nearOnePartCoordinate;
		modifyCoordinate(onePartCoord);
		RealCoordinate<GatePtr> onePartGateCoordinate(NullPtr, onePartCoord);
		m_pCoordinateCalculator->calculateElementFromCoordinate(&onePartGateCoordinate);
		subGates.push_back(onePartGateCoordinate.getElement());
	}
}

void MultiNearBalancePartsGateDecomposer::modifyCoordinate(real_coordinate_t& coordinate) {
	mreal_t eps = 2e-3;
	size_t size = coordinate.size();
	for(unsigned int i = 0; i < size; i++) {
		coordinate[i] += (mreal::rand() - 0.5) * eps;
	}
}

void MultiNearBalancePartsGateDecomposer::getComplementaryGate(GatePtr pGate, const std::vector<GatePtr>& subGates, GatePtr& rpComplementaryGate) {
	//m1*m2*..*m[k-1]
	MatrixPtr pComposedMatrix = subGates[0]->getMatrix();
	for(unsigned int i = 1; i < subGates.size(); i++) {
		MatrixPtr pProductMatrix = NullPtr;
		m_pMatrixOperator->multiply(pComposedMatrix, subGates[i]->getMatrix(), pProductMatrix);
		if(i > 1) {
			_destroy(pComposedMatrix);
		}
		pComposedMatrix = pProductMatrix;
	}

	//mk = (m1*m2*..*m[k-1])' * m
	MatrixPtr pMatrix = pGate->getMatrix();
	MatrixPtr pComplementaryMatrix = NullPtr;
	m_pMatrixOperator->multiplyConjugateTranspose(pComposedMatrix, pMatrix, pComplementaryMatrix);

	//Destroy pComposedMatrix
	if(subGates.size() > 1) {
		_destroy(pComposedMatrix);
	}

	rpComplementaryGate = GatePtr(new Gate(pComplementaryMatrix, 0, ""));
}

