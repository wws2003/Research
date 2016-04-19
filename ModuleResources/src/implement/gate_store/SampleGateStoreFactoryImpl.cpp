/*
 * SampleGateStoreFactoryImpl.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: pham
 */

#include "SampleGateStoreFactoryImpl.h"
#include "SingleQubitGateStoreImpl.h"
#include <exception>

SampleGateStoreFactoryImpl::SampleGateStoreFactoryImpl(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory) {
	m_pMatrixFactory = pMatrixFactory;
	m_pMatrixOperator = pMatrixOperator;
}

GateStorePtr SampleGateStoreFactoryImpl::getGateStore(int nbQubits) {
	//TODO Implement properly
	if(nbQubits == 1) {
		return GateStorePtr(new SingleQubitGateStoreImpl(m_pMatrixOperator, m_pMatrixFactory));
	}
	throw std::logic_error("Sorry gate store not available yet");
	return NullPtr;
}



