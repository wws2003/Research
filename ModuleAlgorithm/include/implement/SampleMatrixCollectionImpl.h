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
#include <vector>

typedef std::vector<MatrixPtr> MatrixPtrVector;

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
	MatrixPtrVector m_pMatrices;

	class InnerVectorMatrixIterator : public MatrixIterator {
	public:
		InnerVectorMatrixIterator(MatrixPtrVector& pMatrices);

		virtual ~InnerVectorMatrixIterator(){};

		virtual void toBegin();

		virtual void next();

		virtual void prev();

		virtual bool isDone();

		virtual MatrixPtr getObj();
	private:
		size_t m_counter;
		MatrixPtrVector& m_pMatrices;
	};

	typedef InnerVectorMatrixIterator* InnerVectorMatrixIteratorPtr;

	MatrixIteratorPtr m_pMatrixIterator;
};



#endif /* SAMPLEMATRIXCOLLECTIONIMPL_H_ */
