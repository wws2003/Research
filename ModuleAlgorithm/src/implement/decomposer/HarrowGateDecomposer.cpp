/*
 * HarrowGateDecomposer.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#include "HarrowGateDecomposer.h"
#include "HarrowElementDecomposer.cpp"

HarrowGateDecomposer::HarrowGateDecomposer(RealCoordinateCalculatorPtr<GatePtr> pSphereCoordinateCalculator,
		MatrixOperatorPtr pMatrixOperator) : HarrowElementDecomposer<GatePtr>(pSphereCoordinateCalculator) {
	m_pMatrixOperator = pMatrixOperator;
}

void HarrowGateDecomposer::getInverseElement(GatePtr element, GatePtr& inverseElement) {
	MatrixPtr pMatrix = element->getMatrix();
	MatrixPtr pInverseMatrix = NullPtr;
	m_pMatrixOperator->conjugateTranpose(pMatrix, pInverseMatrix);
	inverseElement = GatePtr(new Gate(pInverseMatrix, element->getCost(), ""));
}

