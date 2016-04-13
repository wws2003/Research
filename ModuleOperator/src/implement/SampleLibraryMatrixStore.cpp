/*
 * SampleLibraryMatrixStore.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: pham
 */

#include "SampleLibraryMatrixStore.h"
#include "IMatrixOperator.h"

mreal_t inverseSQRT2 = 1/sqrt(2);
ComplexVal expmPi_8 = std::exp(ComplexVal(0, 1 * M_PI / 8.0));
ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, -1 * M_PI / 8.0));

SampleLibraryMatrixStore::SampleLibraryMatrixStore(MatrixFactoryPtr pMatrixFactory, MatrixOperatorPtr pMatrixOperator) {
	m_pMatrixFactory = pMatrixFactory;
	m_pMatrixOperator = pMatrixOperator;
	initMap();
}

SampleLibraryMatrixStore::~SampleLibraryMatrixStore() {
	//Release library matrices
	for(MatrixLabelMap::iterator mIter = m_map.begin(); mIter != m_map.end();) {
		MatrixPtr pMatrix = mIter->second;
		mIter = m_map.erase(mIter);
		_destroy(pMatrix);
	}
}

void SampleLibraryMatrixStore::initMap() {
	//Belows are 1-qubit gates
	m_map["H"] = getHMatrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["T"] = getTMatrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["t"] = getInverseTMatrix(m_pMatrixOperator, m_pMatrixFactory);

	//Belows are 2-qubit gates
	m_map["CNOT1"] = getCNOT1Matrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["CNOT2"] = getCNOT2Matrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["H1"] = getH1Matrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["H2"] = getH2Matrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["T1"] = getT1Matrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["T2"] = getT2Matrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["CV1"] = getCV1Matrix(m_pMatrixOperator, m_pMatrixFactory);
	m_map["CV2"] = getCV2Matrix(m_pMatrixOperator, m_pMatrixFactory);
}

//Override
MatrixPtr SampleLibraryMatrixStore::getLibraryMatrixFromLabel(std::string label) {
	MatrixLabelMap::const_iterator mIter = m_map.find(label);
	if(mIter != m_map.end()) {
		return mIter->second;
	}
	else {
		throw std::logic_error("Gate name not found");
		return NullPtr;
	}
}

MatrixPtr SampleLibraryMatrixStore::getHMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {

	ComplexVal arrayH[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0),
			ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pHMat = pMatrixFactory->getMatrixFromValues(2, 2, arrayH, ROW_SPLICE);
	MatrixPtr pSHMat = NullPtr;

	pMatrixOperator->specialUnitaryFromUnitary(pHMat, pSHMat);

	_destroy(pHMat);

	return pSHMat;
}

MatrixPtr SampleLibraryMatrixStore::getTMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {

	ComplexVal arrayT[] = {expm_Pi_8, (ComplexVal)0.0
			, (ComplexVal)0.0, expmPi_8};

	MatrixPtr pTMat = pMatrixFactory->getMatrixFromValues(2, 2, arrayT, ROW_SPLICE);
	return pTMat;
}

MatrixPtr SampleLibraryMatrixStore::getCNOT1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayCNOT1[] = {(ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0};

	MatrixPtr pCNOTMat1 = pMatrixFactory->getMatrixFromValues(4, 4, arrayCNOT1, ROW_SPLICE);

	MatrixPtr pSCNOTMat1 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat1, pSCNOTMat1);

	_destroy(pCNOTMat1);

	return pSCNOTMat1;
}

MatrixPtr SampleLibraryMatrixStore::getCNOT2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayCNOT2[] = {(ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0};

	MatrixPtr pCNOTMat2 = pMatrixFactory->getMatrixFromValues(4, 4, arrayCNOT2, ROW_SPLICE);
	MatrixPtr pSCNOTMat2 = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pCNOTMat2, pSCNOTMat2);

	_destroy(pCNOTMat2);

	return pSCNOTMat2;
}

MatrixPtr SampleLibraryMatrixStore::getCV1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayV1[] = {(ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, ComplexVal(0.5, 0.5), ComplexVal(0.5, -0.5)
			, (ComplexVal)0.0, (ComplexVal)0.0, ComplexVal(0.5, -0.5), ComplexVal(0.5, 0.5)};

	MatrixPtr pV1Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayV1, ROW_SPLICE);
	MatrixPtr pSV1Mat = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pV1Mat, pSV1Mat);

	_destroy(pV1Mat);

	return pSV1Mat;
}

MatrixPtr SampleLibraryMatrixStore::getCV2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayV2[] = {ComplexVal(0.5, 0.5), ComplexVal(0.5, -0.5), (ComplexVal)0.0, (ComplexVal)0.0
			, ComplexVal(0.5, -0.5), ComplexVal(0.5, 0.5), (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)1.0};

	MatrixPtr pV2Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayV2, ROW_SPLICE);
	MatrixPtr pSV2Mat = NullPtr;
	pMatrixOperator->specialUnitaryFromUnitary(pV2Mat, pSV2Mat);

	_destroy(pV2Mat);

	return pSV2Mat;
}

MatrixPtr SampleLibraryMatrixStore::getH1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayH1[] = {ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0
			, (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0)
			, ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, ComplexVal(-inverseSQRT2, 0), (ComplexVal)0.0
			, (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0),(ComplexVal) 0.0, ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH1Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayH1, ROW_SPLICE);
	return pH1Mat;
}

MatrixPtr SampleLibraryMatrixStore::getH2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayH2[] = {ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0), (ComplexVal)0.0, (ComplexVal)0.0
			, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0), (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(inverseSQRT2, 0)
			, (ComplexVal)0.0, (ComplexVal)0.0, ComplexVal(inverseSQRT2, 0), ComplexVal(-inverseSQRT2, 0)};

	MatrixPtr pH2Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayH2, ROW_SPLICE);
	return pH2Mat;
}

MatrixPtr SampleLibraryMatrixStore::getT1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayT1[] = {expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8};

	MatrixPtr pT1Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayT1, ROW_SPLICE);

	return pT1Mat;
}

MatrixPtr SampleLibraryMatrixStore::getT2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayT2[] = {expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, expmPi_8, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, expm_Pi_8, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8};

	MatrixPtr pT2Mat = pMatrixFactory->getMatrixFromValues(4, 4, arrayT2, ROW_SPLICE);

	return pT2Mat;
}

MatrixPtr SampleLibraryMatrixStore::SampleLibraryMatrixStore::getInverseTMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	ComplexVal arrayTInverse[] = {expmPi_8, (ComplexVal)0.0
			, (ComplexVal)0.0, expm_Pi_8};

	MatrixPtr pTInverseMat = pMatrixFactory->getMatrixFromValues(2, 2, arrayTInverse, ROW_SPLICE);

	return pTInverseMat;
}


