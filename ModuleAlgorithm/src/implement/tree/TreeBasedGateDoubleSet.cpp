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

}

void TreeBasedGateDoubleSet::calculateDistancesToPivots(GatePtr element, std::vector<double>& distances) {
	//Just calculate as normal
	distances.clear();
	for(GatePtr pPivot : m_pivots) {
		double distanceToPivot = getApprxDistance(pPivot->getMatrix(), element->getMatrix());
		distances.push_back(distanceToPivot);
	}
}

double TreeBasedGateDoubleSet::getApprxDistance(MatrixPtr pMatrix1, MatrixPtr pMatrix2) {
	//TODO Implement
	//Try directly compute instead to avoid overhead !
	int nbRows, nbColumns;
	pMatrix1->getSize(nbRows, nbColumns);

	std::complex<double> traceM1ConjugateTranpose2(0.0, 0.0);
	for(int r1 = 0; r1 < nbRows; r1++) {
		for(int c1 = 0; c1 < nbColumns; c1++) {
			ComplexVal cv1 = pMatrix1->getValue(r1, c1);
			ComplexVal cv2 = std::conj(pMatrix2->getValue(r1, c1));
			std::complex<double> v1(mreal::toDouble(cv1.real()), mreal::toDouble(cv1.imag()));
			std::complex<double> v2(mreal::toDouble(cv2.real()), mreal::toDouble(cv2.imag()));
			traceM1ConjugateTranpose2 += v1  * v2;
		}
	}

	double traceM1ConjugateTranpose2Norm = std::sqrt(std::norm(traceM1ConjugateTranpose2));
	double distance = std::sqrt(1.0 - (traceM1ConjugateTranpose2Norm) / (double)nbRows);

	return std::isnan(distance) ? 0.0 : distance;
}
