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

double MatrixFowlerDistanceCalculator::distance(MatrixPtr p1, MatrixPtr p2) {
	int nbRows, nbColumns;
	p1->getSize(nbRows, nbColumns);

	/**Fowler distance
	 * d = size(m2) = size(m1)
	 * dist(m1, m2) = sqrt((d - |trace(m1 * m2')|) / d);
	 */

	//m2'
	MatrixPtr pConjugateTranpose2 = NullPtr;
	m_pMatrixOperator->conjugateTranpose(p2, pConjugateTranpose2);

	//m1 * m2'
	MatrixPtr pM1ConjugateTranpose2 = NullPtr;
	m_pMatrixOperator->multiply(p1, pConjugateTranpose2, pM1ConjugateTranpose2);

	//Trace(m1 * m2')
	ComplexVal traceM1ConjugateTranpose2 = m_pMatrixOperator->trace(pM1ConjugateTranpose2);

	double distance = std::sqrt(((double)nbRows - std::sqrt(std::norm(traceM1ConjugateTranpose2))) / (double)nbRows);

	_destroy(pM1ConjugateTranpose2);
	_destroy(pConjugateTranpose2);

	return distance;
}


