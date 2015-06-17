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
#include "IMatrixCombiner.h"

#ifndef MatrixPtrArray
typedef MatrixPtr* MatrixPtrArray;
#endif

class SearchSpaceConstructorImpl : public ISearchSpaceConstructor {
public:
	SearchSpaceConstructorImpl(MatrixCombinerPtr pMatrixCombiner);
	virtual ~SearchSpaceConstructorImpl();

	//Override
	virtual void constructSearchSpace(MatrixCollectionPtr pMatrixCollection, MatrixCollectionPtr pUniversalSet, int maxSequenceLength);

private:
	MatrixCombinerPtr m_pMatrixCombiner;
};


#endif /* SEARCHSPACECONSTRUCTORIMPL_H_ */
