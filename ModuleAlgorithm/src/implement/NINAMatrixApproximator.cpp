/*
 * NINAMatrixApproximator.cpp
 *
 *  Created on: May 13, 2015
 *      Author: pham
 */

#include "NINAMatrixApproximator.h"
#include "IIterator.h"
#include "IMatrixCollection.h"
#include "IMatrix.h"

NINAMatrixApproximator::NINAMatrixApproximator(MatrixCollectionPtr pNearIdentityMatrixCollection, MatrixOperatorPtr pMatrixOperator) {
	m_pNearIdentityMatrixCollection = pNearIdentityMatrixCollection;
	m_pMatrixOperator = pMatrixOperator;
}


MatrixIteratorPtr NINAMatrixApproximator::getApproximateMatrices(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) {
	int stepCount = 0;

	//Find first step approximate matrix from core collection

	MatrixIteratorPtr pFirstApproximationMatrixIter = pCoreCollection->findApproxMatrices(pQuery, pDistanceCalculator, epsilon);
	if(pFirstApproximationMatrixIter == NullPtr || pFirstApproximationMatrixIter->isDone()) {
		return pFirstApproximationMatrixIter;
	}
	pFirstApproximationMatrixIter->toBegin();
	MatrixPtr pApproximateCandidate = pFirstApproximationMatrixIter->getObj();

	//Step by step approach the query from first position (i.e. pApproximateCandidate)

	while(canStopApproximating(pApproximateCandidate, pQuery, pDistanceCalculator, stepCount++)) {
		//Calculate "one-step closer" position
		MatrixPtr oneStepCloserToQuery = calculateOneStepCloserToTarget(pApproximateCandidate, pQuery, pDistanceCalculator);

		//Calculate "step" matrix to that closer position
		MatrixPtr pStep = calculateStepMatrix(pApproximateCandidate, oneStepCloserToQuery);

		//Find in near-identity matrices candidate to fit "step" matrix
		MatrixPtr pStepFitMatrix = findMatrixToFitStep(pStep);

		//Calculate current postion
		MatrixPtr pNewApproximate = goOneStepToTarget(pApproximateCandidate, pStepFitMatrix);

		//Destroy immediate results
		_destroy(oneStepCloserToQuery);
		_destroy(pStep);
		_destroy(pApproximateCandidate);

		//Update current position
		pApproximateCandidate = pNewApproximate;
	}

	//TODO Generate result from pApproximateCandidate
	return NullPtr;
}

bool NINAMatrixApproximator::canStopApproximating(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, int stepCount) const {
	//TODO Implement
	return true;
}

MatrixPtr NINAMatrixApproximator::calculateOneStepCloserToTarget(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator) const {
	//TODO Implement
	return NullPtr;
}

MatrixPtr NINAMatrixApproximator::calculateStepMatrix(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pOneStepCloserToQueryMatrix) const {
	//StepMatrix = CurrentApproximateMatrix^-1 * pOneStepCloserToQueryMatrix
	MatrixPtr pStepMatrix = NullPtr;
	MatrixPtr pCurrentApproximateMatrixInverse = NullPtr;
	m_pMatrixOperator->inverse(pCurrentApproximateMatrix, pCurrentApproximateMatrixInverse);
	m_pMatrixOperator->multiply(pCurrentApproximateMatrixInverse, pOneStepCloserToQueryMatrix, pStepMatrix);

	//Release intermediate pointer
	_destroy(pCurrentApproximateMatrixInverse);

	return pStepMatrix;
}

MatrixPtr NINAMatrixApproximator::findMatrixToFitStep(MatrixPtr pStepMatrix) const {
	//TODO Implement
	return NullPtr;
}

MatrixPtr NINAMatrixApproximator::goOneStepToTarget(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pStepFitMatrix) {
	//TODO Implement
	return NullPtr;
}

