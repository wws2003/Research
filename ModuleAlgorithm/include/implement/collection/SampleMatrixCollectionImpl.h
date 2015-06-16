/*
 * SampleMatrixCollection.h
 *
 *  Created on: Apr 28, 2015
 *      Author: pham
 */

#ifndef SAMPLEMATRIXCOLLECTIONIMPL_H_
#define SAMPLEMATRIXCOLLECTIONIMPL_H_

#include "IMatrixCollection.h"
#include "IDistanceCalculator.h"
#include "IIterator.h"
#include "AlgoCommon.h"
#include "MathConceptsCommon.h"
#include <vector>

class SampleMatrixCollectionImpl : public IMatrixCollection {
public:
	SampleMatrixCollectionImpl();

	virtual ~SampleMatrixCollectionImpl();

	virtual void addMatrix(MatrixPtr pm) ;

	virtual void clear() ;

	virtual MatrixIteratorPtr getReadonlyIteratorPtr() ;

	//Return iterator through a set of matrix reflecting the changes in the collection
	virtual MatrixIteratorPtr getIteratorPtr() ;

	virtual MatrixCollectionSize_t size() const ;

	virtual MatrixIteratorPtr findApproxMatrices(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const ;

private:
	MatrixPtrVector m_matrices;
	MatrixIteratorPtr m_pMatrixIterator;
};



#endif /* SAMPLEMATRIXCOLLECTIONIMPL_H_ */
