/*
 * TwoQubitsGateStoreImpl.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: pham
 */

#include "TwoQubitsGateStoreImpl.h"
#include "GateSpecification.h"
#include <exception>
#include <map>

TwoQubitsGateStoreImpl::TwoQubitsGateStoreImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	m_pMatrixFactory = pMatrixFactory;
	m_pMatrixOperator = pMatrixOperator;
	setupOrthonormalBasis();
	setupLibraryGates();
	setupIdentityGates();
}

TwoQubitsGateStoreImpl::~TwoQubitsGateStoreImpl() {
	releaseVector(m_identityGates);
	releaseMap(m_libraryGates);
	releaseVector(m_basis4);
}

void TwoQubitsGateStoreImpl::getLibraryGates(std::vector<GatePtr>& libraryGates, LibrarySet librarySet) {
	using namespace gatespec::val;
	using namespace gatespec::twq;

	const std::string Ts[] = {T1::name, T2::name};
	const std::string Hs[] = {H1::name, H2::name};
	const std::string Ss[] = {S1::name, S2::name};
	const std::string CNOTs[] = {CNOT1::name, CNOT2::name};

	const std::vector<GatePtr> GateTs = {m_libraryGates[Ts[0]], m_libraryGates[Ts[1]]};
	const std::vector<GatePtr> GateHs = {m_libraryGates[Hs[0]], m_libraryGates[Hs[1]]};
	const std::vector<GatePtr> GateSs = {m_libraryGates[Ss[0]], m_libraryGates[Ss[1]]};
	const std::vector<GatePtr> GateCNOTs = {m_libraryGates[CNOTs[0]], m_libraryGates[CNOTs[1]]};

	switch (librarySet) {
	case L_HTCNOT:
		//H, T, CNOT
		libraryGates.insert(libraryGates.end(), GateTs.begin(), GateTs.end());
		libraryGates.insert(libraryGates.end(), GateHs.begin(), GateHs.end());
		libraryGates.insert(libraryGates.end(), GateCNOTs.begin(), GateCNOTs.end());
		break;
	case L_HTSCNOT:
		//H, T, S, CNOT
		libraryGates.insert(libraryGates.end(), GateTs.begin(), GateTs.end());
		libraryGates.insert(libraryGates.end(), GateHs.begin(), GateHs.end());
		libraryGates.insert(libraryGates.end(), GateSs.begin(), GateSs.end());
		libraryGates.insert(libraryGates.end(), GateCNOTs.begin(), GateCNOTs.end());
		break;
	default:
		throw std::logic_error("No such a valid library set");
		break;
	}
}

void TwoQubitsGateStoreImpl::getIdentityGates(std::vector<GatePtr>& pIdentityGates, bool isPhaseIgnored) {
	pIdentityGates.clear();
	pIdentityGates.push_back(m_identityGates[0]);

	if(isPhaseIgnored) {
		for(unsigned int i = 1; i < m_identityGates.size(); i++) {
			pIdentityGates.push_back(m_identityGates[i]);
		}
	}
}

void TwoQubitsGateStoreImpl::getMatrixOrthonormalBasis(std::vector<MatrixPtr>& pBasis) {
	pBasis.insert(pBasis.end(), m_basis4.begin(), m_basis4.end());
}

void TwoQubitsGateStoreImpl::getRotationTargets(std::vector<GatePtr>& targets, const RotationConfigs& rotationTargetsConfig) {
	targets.clear();
	for(unsigned int i = 0; i < rotationTargetsConfig.size(); i++) {
		RotationConfig rotationTargetConfig = rotationTargetsConfig[i];
		GatePtr pGate = getRotationGate(rotationTargetConfig);
		targets.push_back(pGate);
	}
}

void TwoQubitsGateStoreImpl::setupOrthonormalBasis() {
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, m_basis4);
}

void TwoQubitsGateStoreImpl::setupLibraryGates() {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	typedef std::pair<std::string, int> GateExtInfo;
	typedef std::vector<GateExtInfo> GateExtInfos;

	GateExtInfos gateTs = {GateExtInfo(T1::name, T1::cost), GateExtInfo(T2::name, T2::cost)};
	GateExtInfos gateHs = {GateExtInfo(H1::name, H1::cost), GateExtInfo(H2::name, H2::cost)};
	GateExtInfos gateSs = {GateExtInfo(S1::name, S1::cost), GateExtInfo(S2::name, S2::cost)};
	GateExtInfos gateCNOTs = {GateExtInfo(CNOT1::name, CNOT1::cost), GateExtInfo(CNOT2::name, CNOT2::cost)};

	std::vector<MatrixPtr> matrixTs;
	std::vector<MatrixPtr> matrixHs;
	std::vector<MatrixPtr> matrixSs;
	std::vector<MatrixPtr> matrixCNOTs;

	computeMatrixTs(matrixTs);
	computeMatrixHs(matrixHs);
	computeMatrixSs(matrixSs);
	computeMatrixCNOTs(matrixCNOTs);

	auto composeGate = [this](GateExtInfo extInfo, MatrixPtr pMatrix) {
		m_libraryGates[extInfo.first] = GatePtr(new Gate(pMatrix,
				extInfo.second,
				extInfo.first));
	};

	for(int i = 0; i < 2; i++) {
		composeGate(gateTs[i], matrixTs[i]);
		composeGate(gateHs[i], matrixHs[i]);
		composeGate(gateSs[i], matrixSs[i]);
		composeGate(gateCNOTs[i], matrixCNOTs[i]);
	}
}

void TwoQubitsGateStoreImpl::setupIdentityGates() {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	//In the case of 4x4 matrices, only possible phase factor is p | p^4 = 1
	// p = 1
	// p = -1
	// p = +i
	// p = -i
	ComplexVal phaseFactors[] = {one, minus_one, image_unit, minus_image_unit};
	std::string gateNames[] = {I::name, I::equiv_name1, I::equiv_name2, I::equiv_name3};

	for(int i = 0; i < 4; i++) {
		MatrixPtr pI0 = m_pMatrixFactory->getIdentityMatrix(4);
		MatrixPtr pI = i == 0 ? pI0 : NullPtr;
		if(i > 0) {
			m_pMatrixOperator->multiplyScalar(pI0, phaseFactors[i], pI);
			_destroy(pI0);
		}
		GatePtr pIGate = GatePtr(new Gate(pI, 0, gateNames[i]));
		m_identityGates.push_back(pIGate);
	}
}

GatePtr TwoQubitsGateStoreImpl::getRotationGate(RotationConfig rotationConfig) {
	//TODO Implement
	throw std::logic_error("Method hasn't been implemented yet");

	return NullPtr;
}
template<typename T>
void TwoQubitsGateStoreImpl::releaseVector(std::vector<T>& pElements) {
	for(typename std::vector<T>::iterator pIter = pElements.begin(); pIter != pElements.end(); ) {
		T pElement = *pIter;
		_destroy(pElement);
		pIter = pElements.erase(pIter);
	}
}

void TwoQubitsGateStoreImpl::releaseMap(LibraryGates& pElements) {
	for(typename LibraryGates::iterator pIter = pElements.begin(); pIter != pElements.end(); ) {
		GatePtr pElement = pIter->second;
		_destroy(pElement);
		pIter = pElements.erase(pIter);
	}
}

void TwoQubitsGateStoreImpl::computeMatrixTs(std::vector<MatrixPtr>& matrixTs) {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	//T = [e^(-i*pi/8) 0;0 e^(i*pi/8)]
	ComplexVal arrayT[] = {e_pi_8, zero, zero, epi_8};
	MatrixPtr pMatrixT = m_pMatrixFactory->getMatrixFromValues(2, 2, arrayT, ROW_SPLICE);

	computeTensors12(pMatrixT, matrixTs);
}

void TwoQubitsGateStoreImpl::computeMatrixHs(std::vector<MatrixPtr>& matrixHs) {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	//H = 1/sqrt(2)[1 1;1 -1]
	//When rewrite in SU(2) form, H = 1/sqrt(2)[-i -i;-i i]
	ComplexVal arrayH[] = {-i_r_sqrt2, -i_r_sqrt2, -i_r_sqrt2, i_r_sqrt2};
	MatrixPtr pMatrixH = m_pMatrixFactory->getMatrixFromValues(2, 2, arrayH, ROW_SPLICE);

	computeTensors12(pMatrixH, matrixHs);
}

void TwoQubitsGateStoreImpl::computeMatrixSs(std::vector<MatrixPtr>& matrixSs) {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	//S = T^2 = [e^(-i*pi/4) 0;0 e^(i*pi/4)]
	ComplexVal arrayS[] = {e_pi_4, zero, zero, epi_4};
	MatrixPtr pMatrixS = m_pMatrixFactory->getMatrixFromValues(2, 2, arrayS, ROW_SPLICE);

	computeTensors12(pMatrixS, matrixSs);
}

void TwoQubitsGateStoreImpl::computeMatrixCNOTs(std::vector<MatrixPtr>& matrixCNOTs) {
	//CNOT1 = [1 0 0 0;0 1 0 0;0 0 0 1;0 0 1 0]
	//Rewrite in SU(4)
	//CNOT1 = CNOT1 * e^(-i*pi/4)
	using namespace gatespec::twq;
	using namespace gatespec::val;

	ComplexVal arrayCNOT1[] = {e_pi_4, zero, zero, zero, zero,
			zero, e_pi_4, zero, zero,
			zero, zero, zero, e_pi_4,
			zero, zero, e_pi_4, zero};

	MatrixPtr pMatrixCNOT1 = m_pMatrixFactory->getMatrixFromValues(4, 4, arrayCNOT1, ROW_SPLICE);

	//CNOT2 = [0 1 0 0;1 0 0 0;0 0 1 0;0 0 0 1]
	//Rewrite in SU(4)
	//CNOT2 = CNOT2 * e^(-i*pi/4)
	ComplexVal arrayCNOT2[] = {zero, e_pi_4, zero, zero, zero,
			e_pi_4, zero, zero, zero,
			zero, zero, e_pi_4, zero,
			zero, zero, zero, e_pi_4};
	MatrixPtr pMatrixCNOT2 = m_pMatrixFactory->getMatrixFromValues(4, 4, arrayCNOT2, ROW_SPLICE);

	matrixCNOTs.push_back(pMatrixCNOT1);
	matrixCNOTs.push_back(pMatrixCNOT2);
}

void TwoQubitsGateStoreImpl::computeTensors12(MatrixPtr pMatrix22, std::vector<MatrixPtr>& matrices44) {
	MatrixPtr pI2 = m_pMatrixFactory->getIdentityMatrix(2);

	MatrixPtr pMatrixT1;
	m_pMatrixOperator->kron(pMatrix22, pI2, pMatrixT1);

	MatrixPtr pMatrixT2;
	m_pMatrixOperator->kron(pI2, pMatrix22, pMatrixT2);

	matrices44.push_back(pMatrixT1);
	matrices44.push_back(pMatrixT2);

	_destroy(pI2);
}
