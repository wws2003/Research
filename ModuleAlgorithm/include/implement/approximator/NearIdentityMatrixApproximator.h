/*
 * NearIdentityMatrixApproximator.h
 *
 *  Created on: Jun 15, 2015
 *      Author: pham
 */

#ifndef NEARIDENTITYMATRIXAPPROXIMATOR_H_
#define NEARIDENTITYMATRIXAPPROXIMATOR_H_

#include "IMatrixApproximator.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"
#include "IMatrixCollection.h"
#include "IMatrixBinCollection.h"

class NearIdentityMatrixApproximator: public IMatrixApproximator {
public:
	NearIdentityMatrixApproximator(MatrixRealCoordinateCalculatorPtr pMatrixRealCoordinateCalculator, MatrixOperatorPtr pMatrixOperator, MatrixCollectionPtr pResultCollecton, MatrixBinCollectionPtr pMatrixBinCollection);
	virtual ~NearIdentityMatrixApproximator(){};

	virtual MatrixIteratorPtr getApproximateMatrices(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon);

private:
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	MatrixOperatorPtr m_pMatrixOperator;
	MatrixCollectionPtr m_pResultCollection;
	MatrixBinCollectionPtr m_pMatrixBinCollection;
};


#endif /* NEARIDENTITYMATRIXAPPROXIMATOR_H_ */