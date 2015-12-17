/*
 * MatrixFowlerDistanceCalculator.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: pham
 */

#include "MatrixFowlerDistanceCalculator.h"
#include "IMatrixOperator.h"
#include "IMatrix.h"
#include <cmath>

MatrixFowlerDistanceCalculator::MatrixFowlerDistanceCalculator(MatrixOperatorPtr pMatrixOperator) {
	m_pMatrixOperator = pMatrixOperator;
}

mreal_t MatrixFowlerDistanceCalculator::distance(MatrixPtr p1, MatrixPtr p2) {
	int nbRows, nbColumns;
	p1->getSize(nbRows, nbColumns);

	/**Fowler distance
	 * d = size(m2) = size(m1)
	 * dist(m1, m2) = sqrt((d - |trace(m1 * m2')|) / d);
	 */
	ComplexVal traceM1ConjugateTranpose2(0.0, 0.0);

	if(m_pMatrixOperator == NullPtr) {
		//Try directly compute instead to avoid overhead !
		for(int r1 = 0; r1 < nbRows; r1++) {
			for(int c1 = 0; c1 < nbColumns; c1++) {
				traceM1ConjugateTranpose2 += p1->getValue(r1, c1) * std::conj(p2->getValue(r1, c1));
			}
		}
	}
	else {
		//m1 * m2'
		MatrixPtr pM1ConjugateTranpose2 = NullPtr;
		m_pMatrixOperator->multiplyConjugateTranspose(p1, p2, pM1ConjugateTranpose2);

		//Trace(m1 * m2')
		traceM1ConjugateTranpose2 = m_pMatrixOperator->trace(pM1ConjugateTranpose2);
		_destroy(pM1ConjugateTranpose2);
	}

	mreal_t traceM1ConjugateTranpose2Norm = mreal::norm(traceM1ConjugateTranpose2);
	mreal_t distance = mreal::sqrt(1.0 - (traceM1ConjugateTranpose2Norm) / (mreal_t)nbRows);

	return mreal::isNAN(distance) ? 0.0 : distance;
}



