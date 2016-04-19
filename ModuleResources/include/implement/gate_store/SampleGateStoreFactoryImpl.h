/*
 * SampleGateStoreFactoryImpl.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pham
 */

#include "IGateStoreFactory.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"
#include "IMatrixFactory.h"

class SampleGateStoreFactoryImpl: public IGateStoreFactory {
public:
	SampleGateStoreFactoryImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	virtual ~SampleGateStoreFactoryImpl(){};

	//Implement
	GateStorePtr getGateStore(int nbQubits);

private:
	MatrixOperatorPtr m_pMatrixOperator;
	MatrixFactoryPtr m_pMatrixFactory;
};


