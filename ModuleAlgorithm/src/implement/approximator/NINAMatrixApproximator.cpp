/*
 * NINAMatrixApproximator.cpp
 *
 *  Created on: May 13, 2015
 *      Author: pham
 */

#include "NINAMatrixApproximator.h"
#include "IIterator.h"
#include "IMatrix.h"
#include "ICollection.h"
#include "IDistanceCalculator.h"

NINAMatrixApproximator::NINAMatrixApproximator(MatrixCollectionPtr pNearIdentityMatrixCollection, MatrixOperatorPtr pMatrixOperator) {
	m_pNearIdentityMatrixCollection = pNearIdentityMatrixCollection;
	m_pMatrixOperator = pMatrixOperator;
}

MatrixIteratorPtr NINAMatrixApproximator::getApproximateElements(MatrixCollectionPtr pCoreCollection, MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, mreal_t epsilon) {
	int stepCount = 0;

	//Find first step approximate matrix from core collection

	MatrixIteratorPtr pFirstApproximationMatrixIter = pCoreCollection->findNearestNeighbour(pQuery, pDistanceCalculator, epsilon);
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
		MatrixPtr pStepFitMatrix = findMatrixToFitStep(pStep, pApproximateCandidate, pQuery, pDistanceCalculator);

		//Calculate current position
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

MatrixPtr NINAMatrixApproximator::findMatrixToFitStep(MatrixPtr pStepMatrix, MatrixPtr pCurrentApproximateMatrix, MatrixPtr pTarget, MatrixDistanceCalculatorPtr pDistanceCalculator) const {
	//TODO Modify proper epsilonForStepMatrix
	mreal_t epsilonForStepMatrix = 1e-4;

	MatrixIteratorPtr pNearStepMatrixIterator = m_pNearIdentityMatrixCollection->findNearestNeighbour(pStepMatrix, pDistanceCalculator, epsilonForStepMatrix);
	pNearStepMatrixIterator->toBegin();
	if(pNearStepMatrixIterator->isDone()) {
		return NullPtr;
	}

	//Find in candidates for step matrix the one can move currentApproximateMatrix closest to target
	MatrixPtr pStepFitCandidate = NullPtr;
	mreal_t minDistanceToTarget = 1e3;
	while(!pNearStepMatrixIterator->isDone()) {
		MatrixPtr pStepFitCandidate = pNearStepMatrixIterator->getObj();

		MatrixPtr pTargetCandidate = NullPtr;
		m_pMatrixOperator->multiply(pCurrentApproximateMatrix, pStepFitCandidate, pTargetCandidate);

		mreal_t candidateDistanceToTarget = pDistanceCalculator->distance(pTarget, pTargetCandidate);
		if(candidateDistanceToTarget < minDistanceToTarget) {
			minDistanceToTarget = candidateDistanceToTarget;
			pStepFitCandidate = pTargetCandidate;
		}

		_destroy(pTargetCandidate);

		pNearStepMatrixIterator->next();
	}

	return pStepFitCandidate;
}

MatrixPtr NINAMatrixApproximator::goOneStepToTarget(MatrixPtr pCurrentApproximateMatrix, MatrixPtr pStepFitMatrix) {
	//OneStepCloserToTarget = CurrentApproximateMatrix * StepFitMatrix
	MatrixPtr pOneStepToTarget = NullPtr;
	m_pMatrixOperator->multiply(pCurrentApproximateMatrix, pStepFitMatrix, pOneStepToTarget);
	return pOneStepToTarget;
}

