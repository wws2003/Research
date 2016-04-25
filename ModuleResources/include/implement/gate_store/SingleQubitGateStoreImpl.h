/*
 * SingleQubitGateStoreImpl.h
 *
 *  Created on: Apr 13, 2016
 *      Author: pham
 */

#ifndef SINGLEQUBITGATESTOREIMPL_H_
#define SINGLEQUBITGATESTOREIMPL_H_

#include "IGateStore.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"
#include "IMatrixFactory.h"
#include <map>

class SingleQubitGateStoreImpl: public IGateStore {
public:
	SingleQubitGateStoreImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);

	virtual ~SingleQubitGateStoreImpl();

	//Implement
	void getLibraryGates(std::vector<GatePtr>& pLibraryGates, LibrarySet librarySet);

	//Implement
	void getIdentityGates(std::vector<GatePtr>& pIdentityGates, bool isPhaseIgnored);

	//Implement
	void getMatrixOrthonormalBasis(std::vector<MatrixPtr>& pBasis);

	//Implement
	void getRotationTargets(std::vector<GatePtr>& targets,
			const RotationConfigs& rotationTargetsConfig);

private:
	void setupOrthonormalBasis();
	void setupLibraryGates();
	void setupIdentityGates();

	GatePtr getRotationGate(RotationConfig rotationConfig);

	template<typename T>
	void releaseVector(std::vector<T>& pElements);

	MatrixOperatorPtr m_pMatrixOperator;
	MatrixFactoryPtr m_pMatrixFactory;

	MatrixPtrVector m_basis2;

	typedef std::map<std::string, GatePtr> LibraryGates;

	void releaseMap(LibraryGates& pElements);

	LibraryGates m_libraryGates;

	GatePtrVector m_identityGates;
};


#endif /* SINGLEQUBITGATESTOREIMPL_H_ */
