/*
 * NINAMatrixApproximator.h
 *
 *  Created on: May 13, 2015
 *      Author: pham
 */

#ifndef NINAMATRIXAPPROXIMATOR_H_
#define NINAMATRIXAPPROXIMATOR_H_

#include "AlgoCommon.h"
#include "IMatrixApproximator.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"

//NINA = Near Indentity Nearbour Analysis

class NINAMatrixApproximator : public IMatrixApproximator {
public:

	/**
	 * Provide to constructor
	 * - A collection of matrices near indetity matrix
	 * - A matrix operator
	 */
	NINAMatrixApproximator(MatrixCollectionPtr pNearIdentityMatrixCollection, MatrixOperatorPtr pMatrixOperator);

	virtual ~NINAMatrixApproximator(){};

	/**
	 * Find in given collection, and moreover, if possible generate from it
		 approximate matrices to query (distance smaller than epsilon)
	 */
	virtual MatrixIteratorPtr getApproximateMatrices(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon);

protected:

	virtual bool canStopApproximating(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, int stepCount) const;

	virtual MatrixPtr calculateOneStepCloserToTarget(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator) const;

	virtual MatrixPtr calculateStepMatrix(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pOneStepCloserToQueryMatrix) const;

	virtual MatrixPtr findMatrixToFitStep(MatrixPtr pStepMatrix) const;

	virtual MatrixPtr goOneStepToTarget(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pStepFitMatrix);

	MatrixCollectionPtr m_pNearIdentityMatrixCollection;
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* NINAMATRIXAPPROXIMATOR_H_ */
