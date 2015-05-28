/*
 * VectorizationMatrixCollectionImpl.h
 *
 *  Created on: May 22, 2015
 *      Author: pham
 */

#ifndef VECTORIZATIONMATRIXCOLLECTIONIMPL_H_
#define VECTORIZATIONMATRIXCOLLECTIONIMPL_H_

#include "IMatrixCollection.h"
#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "IDistanceCalculator.h"
#include "IIterator.h"
#include "ICollection.h"
#include "ICoordinateCalculator.h"

class VectorizationMatrixCollectionImpl : IMatrixCollection {
public:
	VectorizationMatrixCollectionImpl(MatrixCoordinateCalculatorPtr pMatrixCoordinateCalculator, MatrixCoordinateCollectionPtr pInternalMatrixCoordinateCollection, MatrixCoordinateDistanceCalculatorPtr pMatrixCoordinateDistanceCalculator);

	virtual ~VectorizationMatrixCollectionImpl(){};

	virtual void addMatrix(MatrixPtr pm) ;

	virtual void clear() ;

	virtual MatrixIteratorPtr getReadonlyIteratorPtr() ;

	//Return iterator through a set of matrix reflecting the changes in the collection
	virtual MatrixIteratorPtr getIteratorPtr() ;

	virtual MatrixCollectionSize_t size() const ;

	virtual MatrixIteratorPtr findApproxMatrices(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const ;

private:
	MatrixCoordinateCalculatorPtr m_pMatrixCoordinateCalculator;
	MatrixCoordinateCollectionPtr m_pInternalMatrixCoordinateCollection;
	MatrixCoordinateDistanceCalculatorPtr m_pMatrixCoordinateDistanceCalculator;

	class InnerVectorMatrixIterator : public MatrixIterator {
	public:
		InnerVectorMatrixIterator(const MatrixPtrVector& pMatrices);

		virtual ~InnerVectorMatrixIterator();

		virtual void toBegin();

		virtual void next();

		virtual void prev();

		virtual bool isDone();

		virtual MatrixPtr getObj();
	private:
		counter_t m_counter;
		MatrixPtrVector m_pMatrices;
	};
};



#endif /* VECTORIZATIONMATRIXCOLLECTIONIMPL_H_ */
