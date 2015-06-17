/*
 * VectorizationMatrixCollectionImpl.h
 *
 *  Created on: May 22, 2015
 *      Author: pham
 */

#ifndef COORDINATEBASEDMATRIXCOLLECTIONIMPL_H_
#define COORDINATEBASEDMATRIXCOLLECTIONIMPL_H_

#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "IDistanceCalculator.h"
#include "IIterator.h"
#include "ICollection.h"
#include "ICoordinateCalculator.h"

class CoordinateBasedMatrixCollectionImpl : IMatrixCollection {
public:
	CoordinateBasedMatrixCollectionImpl(MatrixRealCoordinateCalculatorPtr pMatrixCoordinateCalculator, MatrixRealCoordinateCollectionPtr pInternalMatrixCoordinateCollection, MatrixRealCoordinateDistanceCalculatorPtr pMatrixCoordinateDistanceCalculator);

	virtual ~CoordinateBasedMatrixCollectionImpl(){};

	virtual void addElement(MatrixPtr pm) ;

	virtual void clear() ;

	virtual MatrixIteratorPtr getReadonlyIteratorPtr() ;

	//Return iterator through a set of matrix reflecting the changes in the collection
	virtual MatrixIteratorPtr getIteratorPtr() ;

	virtual MatrixCollectionSize_t size() const ;

	virtual MatrixIteratorPtr findApproxElements(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const ;

private:
	MatrixRealCoordinateCalculatorPtr m_pMatrixCoordinateCalculator;
	MatrixRealCoordinateCollectionPtr m_pInternalMatrixCoordinateCollection;
	MatrixRealCoordinateDistanceCalculatorPtr m_pMatrixCoordinateDistanceCalculator;

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
