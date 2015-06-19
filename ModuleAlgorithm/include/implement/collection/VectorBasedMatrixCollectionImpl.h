/*
 * SampleMatrixCollection.h
 *
 *  Created on: Apr 28, 2015
 *      Author: pham
 */

#ifndef SAMPLEMATRIXCOLLECTIONIMPL_H_
#define SAMPLEMATRIXCOLLECTIONIMPL_H_

#include "IDistanceCalculator.h"
#include "IIterator.h"
#include "AlgoCommon.h"
#include "ICollection.h"
#include "MathConceptsCommon.h"
#include "VectorBasedCollectionImpl.hpp"

typedef VectorBasedCollectionImpl<MatrixPtr> VectorBasedMatrixCollectionImpl;

#if 0
#include "IDistanceCalculator.h"
#include "IIterator.h"
#include "AlgoCommon.h"
#include "ICollection.h"
#include "MathConceptsCommon.h"
#include <vector>

class VectorBasedMatrixCollectionImpl : public IMatrixCollection {
public:
	VectorBasedMatrixCollectionImpl();

	virtual ~VectorBasedMatrixCollectionImpl();

	virtual void addElement(MatrixPtr pm) ;

	virtual void clear() ;

	virtual MatrixIteratorPtr getReadonlyIteratorPtr() ;

	//Return iterator through a set of matrix reflecting the changes in the collection
	virtual MatrixIteratorPtr getIteratorPtr() ;

	virtual MatrixCollectionSize_t size() const ;

	virtual MatrixIteratorPtr findApproxElements(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const ;

private:
	MatrixPtrVector m_matrices;
	MatrixIteratorPtr m_pMatrixIterator;
};

#endif

#endif /* SAMPLEMATRIXCOLLECTIONIMPL_H_ */
