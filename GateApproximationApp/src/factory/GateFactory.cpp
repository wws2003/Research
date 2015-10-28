/*
 * GateFactory.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: pham
 */

#include "GateFactory.h"
#include "IMatrixOperator.h"
#include "IMatrixFactory.h"
#include "IMatrix.h"
#include <stdexcept>

GateFactory::GateFactory(LibraryMatrixStorePtr pLibraryMatrixStore) {
	m_pLibraryMatrixStore = pLibraryMatrixStore;
	initGateNameCodeMap();
}

GatePtr GateFactory::getLibraryGateInstance(std::string gateName, cost_t cost) {
	GatePtr pGate = NullPtr;
	MatrixPtr pMatrix = m_pLibraryMatrixStore->getLibraryMatrixFromLabel(gateName)->clone();

	pGate = GatePtr(new Gate(pMatrix, cost, gateName));
	return pGate;
}

void GateFactory::initGateNameCodeMap() {
	m_gateNameCodeMap["H"] = G_H;
	m_gateNameCodeMap["T"] = G_T;
	m_gateNameCodeMap["t"] = G_T_INVERSE;
	m_gateNameCodeMap["CNOT1"] = G_CNOT1;
	m_gateNameCodeMap["CNOT2"] = G_CNOT2;
	m_gateNameCodeMap["CV1"] = G_CV1;
	m_gateNameCodeMap["CV2"] = G_CV2;
	m_gateNameCodeMap["H1"] = G_H1;
	m_gateNameCodeMap["H2"] = G_H2;
	m_gateNameCodeMap["T1"] = G_T1;
	m_gateNameCodeMap["T2"] = G_T2;
}

