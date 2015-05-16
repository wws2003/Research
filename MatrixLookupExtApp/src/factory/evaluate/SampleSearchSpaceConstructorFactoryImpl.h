/*
 * SampleSearchSpaceConstructorFactoryImpl.h
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#ifndef SAMPLESEARCHSPACECONSTRUCTORFACTORYIMPL_H_
#define SAMPLESEARCHSPACECONSTRUCTORFACTORYIMPL_H_

#include "ISearchSpaceConstructorFactory.h"
#include "IMatrixOperator.h"
#include "OperatorCommon.h"

class SampleSeachSpaceConstructorFactoryImpl: public ISearchSpaceConstructorFactory {
public:
	SampleSeachSpaceConstructorFactoryImpl(MatrixOperatorPtr pMatrixOperator);
	virtual SearchSpaceConstructorPtr getSearchSpaceConstructorPtr();

private:
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* SAMPLESEARCHSPACECONSTRUCTORFACTORYIMPL_H_ */
