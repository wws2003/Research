/*
 * SearchSpaceConstructorImpl.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef SEARCHSPACECONSTRUCTORIMPL_H_
#define SEARCHSPACECONSTRUCTORIMPL_H_

#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "ISearchSpaceConstructor.h"

#ifndef MatrixPtrArray
typedef MatrixPtr* MatrixPtrArray;
#endif

class SearchSpaceConstructorImpl : public ISearchSpaceConstructor {
public:
	SearchSpaceConstructorImpl(MatrixOperatorPtr pMatrixOperator);
	virtual ~SearchSpaceConstructorImpl();

	//Override
	virtual void constructSearchSpace(MatrixCollectionPtr pMatrixCollection, MatrixCollectionPtr pUniversalSet, int maxSequenceLength);

private:
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* SEARCHSPACECONSTRUCTORIMPL_H_ */