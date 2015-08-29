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

double inverseSQRT2 = 1/sqrt(2);
ComplexVal expmPi_8 = std::exp(ComplexVal(0, 1) * M_PI / 8.0);
ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, -1) * M_PI / 8.0);

MatrixPtr getHMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getTMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getCNOT1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getCNOT2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getCV1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getCV2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getH1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getH2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getT1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getT2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
MatrixPtr getInverseTMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);

GateFactory::GateFactory(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	m_pMatrixOperator = pMatrixOperator;
	m_pMatrixFactory = pMatrixFactory;
	initGateNameCodeMap();
}

GatePtr GateFactory::getLibraryGateInstance(std::string gateName, cost_t cost) {
	GatePtr pGate = NullPtr;

	if(m_gateNameCodeMap.find(gateName) == m_gateNameCodeMap.end()) {
		throw std::logic_error("Gate name not found");
	}

	int gateCode = m_gateNameCodeMap[gateName];
	MatrixPtr pMatrix = NullPtr;

	switch(gateCode) {
	case G_H:
		pMatrix = getHMatrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_T:
		pMatrix = getTMatrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_T_INVERSE:
		pMatrix = getInverseTMatrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_CNOT1:
		pMatrix = getCNOT1Matrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_CNOT2:
		pMatrix = getCNOT2Matrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_CV1:
		pMatrix = getCV1Matrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_CV2:
		pMatrix = getCV2Matrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_H1:
		pMatrix = getH1Matrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_H2:
		pMatrix = getH2Matrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_T1:
		pMatrix = getT1Matrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	case G_T2:
		pMatrix = getT2Matrix(m_pMatrixOperator, m_pMatrixFactory);
		break;
	default:
		break;
	}

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

MatrixPtr getHMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {

	ComplexVal arrayH[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0),
			ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pHMat = pMatrixFactory->getMatrixFromValues(2, 2, arrayH, ROW_SPLICE);
	MatrixPtr pSHMat = NullPtr;

	pMatrixOperator->specialUnitaryFromUnitary(pHMat, pSHMat);

	_destroy(pHMat);

	return pSHMat;
}

MatrixPtr getTMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {

	ComplexVal arrayT[] = {expm_Pi_8, 0.0
			, 0.0, expmPi_8};

	MatrixPtr pTMat = pMatrixFactory->getMatrixFromValues(2, 2, arrayT, ROW_SPLICE);
	return pTMat;
}

MatrixPtr getCNOT1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayCNOT1[] = {1.0, 0.0, 0.0, 0.0
			, 0.0, 1.0, 0.0, 0.0
			, 0.0, 0.0, 0.0, 1.0
			, 0.0, 0.0, 1.0, 0.0};

	MatrixPtr pCNOTMat1 = pMatrixFactory->getMatrixFromValues(2, 2, arrayCNOT1, ROW_SPLICE);

	MatrixPtr pSCNOTMat1 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat1, pSCNOTMat1);

	_destroy(pCNOTMat1);

	return pSCNOTMat1;
}

MatrixPtr getCNOT2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayCNOT2[] = {0.0, 1.0, 0.0, 0.0
			, 1.0, 0.0, 0.0, 0.0
			, 0.0, 0.0, 1.0, 0.0
			, 0.0, 0.0, 0.0, 1.0};

	MatrixPtr pCNOTMat2 = pMatrixFactory->getMatrixFromValues(2, 2, arrayCNOT2, ROW_SPLICE);
	MatrixPtr pSCNOTMat2 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat2, pSCNOTMat2);

	_destroy(pCNOTMat2);

	return pSCNOTMat2;
}

MatrixPtr getCV1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayV1[] = {1.0, 0.0, 0.0, 0.0
			, 0.0, 1.0, 0.0, 0.0
			, 0.0, 0.0, ComplexVal(0.5, 0.5), ComplexVal(0.5, -0.5)
			, 0.0, 0.0, ComplexVal(0.5, -0.5), ComplexVal(0.5, 0.5)};

	MatrixPtr pV1Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayV1, ROW_SPLICE);
	MatrixPtr pSV1Mat = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pV1Mat, pSV1Mat);

	_destroy(pV1Mat);

	return pSV1Mat;
}

MatrixPtr getCV2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayV2[] = {ComplexVal(0.5, 0.5), ComplexVal(0.5, -0.5), 0.0, 0.0
			, ComplexVal(0.5, -0.5), ComplexVal(0.5, 0.5), 0.0, 0.0
			, 0.0, 0.0, 1.0, 0.0
			, 0.0, 0.0, 0.0, 1.0};

	MatrixPtr pV2Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayV2, ROW_SPLICE);
	MatrixPtr pSV2Mat = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pV2Mat, pSV2Mat);

	_destroy(pV2Mat);

	return pSV2Mat;
}

MatrixPtr getH1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayH1[] = {ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(inverseSQRT2, 0), 0.0
			, 0.0, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(inverseSQRT2, 0)
			, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(-inverseSQRT2, 0), 0.0
			, 0.0, ComplexVal(inverseSQRT2, 0), 0.0, ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH1Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayH1, ROW_SPLICE);
	return pH1Mat;
}

MatrixPtr getH2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayH2[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0), 0.0, 0.0
			, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0), 0.0, 0.0
			, 0.0, 0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0)
			, 0.0, 0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH2Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayH2, ROW_SPLICE);
	return pH2Mat;
}

MatrixPtr getT1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayT1[] = {expm_Pi_8, 0.0, 0.0, 0.0
			, 0.0, expm_Pi_8, 0.0, 0.0
			, 0.0, 0.0, expmPi_8, 0.0
			, 0.0, 0.0, 0.0, expmPi_8};

	MatrixPtr pT1Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayT1, ROW_SPLICE);

	return pT1Mat;
}

MatrixPtr getT2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayT2[] = {expm_Pi_8, 0.0, 0.0, 0.0
			, 0.0, expmPi_8, 0.0, 0.0
			, 0.0, 0.0, expm_Pi_8, 0.0
			, 0.0, 0.0, 0.0, expmPi_8};

	MatrixPtr pT2Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayT2, ROW_SPLICE);

	return pT2Mat;
}

MatrixPtr getInverseTMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayTInverse[] = {expmPi_8, 0.0
			, 0.0, expm_Pi_8};

	MatrixPtr pTInverseMat = pMatrixFactory->getMatrixFromValues(2, 2, arrayTInverse, ROW_SPLICE);

	return pTInverseMat;
}


