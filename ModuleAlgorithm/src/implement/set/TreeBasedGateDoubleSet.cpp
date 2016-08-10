/*
 * TreeBasedGateDoubleSet.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#include "TreeBasedGateDoubleSet.h"
#include "IDistanceCalculator.h"
#include "AbstractTreeBasedSet.cpp"
#include <cmath>

template class AbstractTreeBasedSet<GatePtr, double>;

TreeBasedGateDoubleSet::TreeBasedGateDoubleSet(const std::vector<GatePtr>& pivots,
		DistanceCalculatorPtr<GatePtr> pDistanceCalculator,
		mreal_t distanceToConsiderAsOne,
		RangeSearchTreePtr<GatePtr, double> pRangeSearchTree,
		const std::vector<double>& coordinateRanges) : AbstractTreeBasedSet<GatePtr, double>(pivots,
				pDistanceCalculator,
				distanceToConsiderAsOne,
				pRangeSearchTree,
				coordinateRanges) {

	for(GatePtr pPivot : m_pivots) {
		ApprxDoubleComplexMatrix* pApprxMatrix = new ApprxDoubleComplexMatrix(pPivot->getMatrix());
		m_apprxPivots.push_back(pApprxMatrix);
	}
}

TreeBasedGateDoubleSet::~TreeBasedGateDoubleSet() {
	for(typename std::vector<ApprxDoubleComplexMatrixPtr>::iterator pIter = m_apprxPivots.begin(); pIter != m_apprxPivots.end();) {
		ApprxDoubleComplexMatrixPtr pApprxPivot = *pIter;
		pIter = m_apprxPivots.erase(pIter);
		delete pApprxPivot;
	}
}

void TreeBasedGateDoubleSet::calculateDistancesToPivots(GatePtr element,
		std::vector<double>& distances) {
	//Just calculate as normal
	distances.clear();
	for(ApprxDoubleComplexMatrixPtr pApprxPivot : m_apprxPivots) {
		double distanceToPivot = getApprxDistance(pApprxPivot, element->getMatrix());
		distances.push_back(distanceToPivot);
	}
}

std::string TreeBasedGateDoubleSet::getElementDistanceCacheKey(GatePtr element) {
	return element->getLabelStr();
}

//MARK: Private methods and class
double TreeBasedGateDoubleSet::getApprxDistance(ApprxDoubleComplexMatrixPtr pApprxPivot, MatrixPtr pMatrix2) {
	//Try directly compute instead to avoid overhead !
	int nbRows, nbColumns;
	pMatrix2->getSize(nbRows, nbColumns);

	DoubleComplex traceM1ConjugateTranpose2(0.0, 0.0);
	for(int r1 = 0; r1 < nbRows; r1++) {
		for(int c1 = 0; c1 < nbColumns; c1++) {
			DoubleComplex v1 = pApprxPivot->getValue(r1, c1);

			ComplexVal cv2 = std::conj(pMatrix2->getValue(r1, c1));
			DoubleComplex v2(mreal::toDouble(cv2.real()), mreal::toDouble(cv2.imag()));

			traceM1ConjugateTranpose2 += v1  * v2;
		}
	}

	double traceM1ConjugateTranpose2Norm = std::sqrt(std::norm(traceM1ConjugateTranpose2));
	double distance = std::sqrt(1.0 - (traceM1ConjugateTranpose2Norm) / (double)nbRows);

	return std::isnan(distance) ? 0.0 : distance;
}

//MARK: Private class
TreeBasedGateDoubleSet::ApprxDoubleComplexMatrix::ApprxDoubleComplexMatrix(MatrixPtr pMatrix) {
	pMatrix->getSize(m_nbRows, m_nbColumns);
	m_array = new DoubleComplex[m_nbRows * m_nbColumns];
	for(int r = 0; r < m_nbRows; r++) {
		for(int c = 0; c < m_nbColumns; c++) {
			ComplexVal cv1 = pMatrix->getValue(r, c);
			m_array[r * m_nbColumns + c] = std::complex<double> (mreal::toDouble(cv1.real()),
					mreal::toDouble(cv1.imag()));
		}
	}
}

TreeBasedGateDoubleSet::ApprxDoubleComplexMatrix::~ApprxDoubleComplexMatrix() {
	delete[] m_array;
}

TreeBasedGateDoubleSet::DoubleComplex TreeBasedGateDoubleSet::ApprxDoubleComplexMatrix::getValue(int row, int col) {
	return m_array[row * m_nbColumns + col];
}
