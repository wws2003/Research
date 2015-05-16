/*
 * SampleSeachSpaceConstructorFactoryImpl.cpp
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#include "SampleSearchSpaceConstructorFactoryImpl.h"
#include "SearchSpaceConstructorImpl.h"

SampleSeachSpaceConstructorFactoryImpl::SampleSeachSpaceConstructorFactoryImpl(MatrixOperatorPtr pMatrixOperator):m_pMatrixOperator(pMatrixOperator) {

}

SearchSpaceConstructorPtr SampleSeachSpaceConstructorFactoryImpl::getSearchSpaceConstructorPtr() {
	return new SearchSpaceConstructorImpl(m_pMatrixOperator);
}



