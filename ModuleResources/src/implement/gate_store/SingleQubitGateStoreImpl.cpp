/*
 * SingleQubitGateStoreImpl.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: pham
 */

#include "SingleQubitGateStoreImpl.h"
#include "GateSpecification.h"
#include <exception>

SingleQubitGateStoreImpl::SingleQubitGateStoreImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	m_pMatrixFactory = pMatrixFactory;
	m_pMatrixOperator = pMatrixOperator;
	setupOrthonormalBasis();
	setupLibraryGates();
	setupIdentityGates();
}

SingleQubitGateStoreImpl::~SingleQubitGateStoreImpl() {
	releaseVector(m_identityGates);
	releaseMap(m_libraryGates);
	releaseVector(m_basis2);
}

void SingleQubitGateStoreImpl::getLibraryGates(std::vector<GatePtr>& libraryGates, LibrarySet librarySet) {
	using namespace gatespec::sgq;
	using namespace gatespec::val;

	const std::string T(T::name);
	const std::string H(H::name);
	const std::string S(S::name);

	switch (librarySet) {
	case L_HT:
		//H, T
		libraryGates.push_back(m_libraryGates[H]);
		libraryGates.push_back(m_libraryGates[T]);
		break;
	case L_HTS:
		//H, T, S
		libraryGates.push_back(m_libraryGates[H]);
		libraryGates.push_back(m_libraryGates[T]);
		libraryGates.push_back(m_libraryGates[S]);
		break;
	default:
		throw std::logic_error("No such a valid library set");
		break;
	}
}

void SingleQubitGateStoreImpl::getIdentityGates(std::vector<GatePtr>& pIdentityGates, bool isPhaseIgnored) {
	pIdentityGates.clear();
	pIdentityGates.push_back(m_identityGates[0]);

	//In the case of 2x2 matrices, only possible phase factor is -1
	if(isPhaseIgnored) {
		pIdentityGates.push_back(m_identityGates[1]);
	}
}

void SingleQubitGateStoreImpl::getMatrixOrthonormalBasis(std::vector<MatrixPtr>& pBasis) {
	pBasis.insert(pBasis.end(), m_basis2.begin(), m_basis2.end());
}

void SingleQubitGateStoreImpl::getRotationTargets(std::vector<GatePtr>& targets, const RotationConfigs& rotationTargetsConfig) {
	targets.clear();
	for(unsigned int i = 0; i < rotationTargetsConfig.size(); i++) {
		RotationConfig rotationTargetConfig = rotationTargetsConfig[i];
		GatePtr pGate = getRotationGate(rotationTargetConfig);
		targets.push_back(pGate);
	}
}

void SingleQubitGateStoreImpl::setupOrthonormalBasis() {
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(2, m_basis2);
}

void SingleQubitGateStoreImpl::setupLibraryGates() {
	using namespace gatespec::sgq;
	using namespace gatespec::val;

	const std::string T(T::name);
	const std::string H(H::name);
	const std::string S(S::name);

	//Set up T,H,S for all cases ?. Currently just use this method

	//T = [e^(-i*pi/8) 0;0 e^(i*pi/8)]
	ComplexVal arrayT[] = {e_pi_8, zero, zero, epi_8};

	//H = 1/sqrt(2)[1 1;1 -1]
	//When rewrite in SU(2) form, H = 1/sqrt(2)[-i -i;-i i]
	ComplexVal arrayH[] = {-i_r_sqrt2, -i_r_sqrt2, -i_r_sqrt2, i_r_sqrt2};

	//S = T^2 = [e^(-i*pi/4) 0;0 e^(i*pi/4)]
	ComplexVal arrayS[] = {e_pi_4, zero, zero, epi_4};

	MatrixPtr pMatrixT = m_pMatrixFactory->getMatrixFromValues(2, 2, arrayT, ROW_SPLICE);
	MatrixPtr pMatrixH = m_pMatrixFactory->getMatrixFromValues(2, 2, arrayH, ROW_SPLICE);
	MatrixPtr pMatrixS = m_pMatrixFactory->getMatrixFromValues(2, 2, arrayS, ROW_SPLICE);

	m_libraryGates[H] = GatePtr(new Gate(pMatrixH, H::cost, H));
	m_libraryGates[T] = GatePtr(new Gate(pMatrixT, T::cost, T));
	m_libraryGates[S] = GatePtr(new Gate(pMatrixS, S::cost, S));
}

void SingleQubitGateStoreImpl::setupIdentityGates() {
	using namespace gatespec::sgq;
	using namespace gatespec::val;

	//Only I and -I
	MatrixPtr pI = m_pMatrixFactory->getIdentityMatrix(2);

	ComplexVal mVals[] = {minus_one, zero,
			zero, minus_one};
	MatrixPtr pMI = m_pMatrixFactory->getMatrixFromValues(2, 2, mVals);

	GatePtr pIGate = GatePtr(new Gate(pI, 0, I::name));
	GatePtr pMIGate = GatePtr(new Gate(pMI, 0, I::inverse_name));

	m_identityGates.push_back(pIGate);
	m_identityGates.push_back(pMIGate);
}

GatePtr SingleQubitGateStoreImpl::getRotationGate(RotationConfig rotationConfig) {
	mreal_t phi = rotationConfig.m_rotationAngle;

	MatrixPtr pBasis = NullPtr;

	switch (rotationConfig.m_rotationType) {
	case R_X:
		pBasis = m_basis2[0];
		break;
	case R_Y:
		pBasis = m_basis2[1];
		break;
	case R_Z:
		pBasis = m_basis2[2];
		break;
	default:
		throw std::logic_error("No such a valid rotation");
		break;
	}
	MatrixPtr pRotationMatrix;
	m_pMatrixOperator->getRotationMatrix(pBasis, phi, pRotationMatrix);

	return GatePtr(new Gate(pRotationMatrix, 0, pRotationMatrix->getLabel()));
}
template<typename T>
void SingleQubitGateStoreImpl::releaseVector(std::vector<T>& pElements) {
	for(typename std::vector<T>::iterator pIter = pElements.begin(); pIter != pElements.end(); ) {
		T pElement = *pIter;
		_destroy(pElement);
		pIter = pElements.erase(pIter);
	}
}

void SingleQubitGateStoreImpl::releaseMap(LibraryGates& pElements) {
	for(typename LibraryGates::iterator pIter = pElements.begin(); pIter != pElements.end(); ) {
		GatePtr pElement = pIter->second;
		_destroy(pElement);
		pIter = pElements.erase(pIter);
	}
}
