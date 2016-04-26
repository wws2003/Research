/*
 * TwoQubitsGateStoreImpl.h
 *
 *  Created on: Apr 21, 2016
 *      Author: pham
 */

#ifndef TWOQUBITSGATESTOREIMPL_H_
#define TWOQUBITSGATESTOREIMPL_H_

#include "IGateStore.h"
#include "ResourcesCommon.h"
#include "IMatrixOperator.h"
#include "IMatrixFactory.h"
#include "OperatorCommon.h"
#include <map>

class TwoQubitsGateStoreImpl: public IGateStore {
public:
	TwoQubitsGateStoreImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);

	virtual ~TwoQubitsGateStoreImpl();

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

	void computeMatrixTs(std::vector<MatrixPtr>& matrixTs);

	void computeMatrixHs(std::vector<MatrixPtr>& matrixHs);

	void computeMatrixSs(std::vector<MatrixPtr>& matrixSs);

	void computeMatrixCNOTs(std::vector<MatrixPtr>& matrixCNOTs);

	void computeTensors12(MatrixPtr pMatrix22, std::vector<MatrixPtr>& matrices44);

	GatePtr getRotationGate(RotationConfig rotationConfig);

	template<typename T>
	void releaseVector(std::vector<T>& pElements);

	MatrixOperatorPtr m_pMatrixOperator;
	MatrixFactoryPtr m_pMatrixFactory;

	MatrixPtrVector m_basis2;//Used to calculate controlled-rotation...
	MatrixPtrVector m_basis4;

	typedef std::map<std::string, GatePtr> LibraryGates;

	void releaseMap(LibraryGates& pElements);

	LibraryGates m_libraryGates;

	GatePtrVector m_identityGates;
};



#endif /* TWOQUBITSGATESTOREIMPL_H_ */
