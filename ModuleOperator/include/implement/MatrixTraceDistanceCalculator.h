/*
 * MatrixTraceDistanceCalculator.h
 *
 *  Created on: May 14, 2015
 *      Author: pham
 */

#ifndef MATRIXTRACEDISTANCECALCULATOR_H_
#define MATRIXTRACEDISTANCECALCULATOR_H_

#include "IDistanceCalculator.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"

class MatrixTraceDistanceCalculator : public IDistanceCalculator<MatrixPtr> {
public:
	MatrixTraceDistanceCalculator(MatrixOperatorPtr pMatrixOperator);

	virtual double distance(MatrixPtr p1, MatrixPtr p2);

private:
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* MATRIXTRACEDISTANCECALCULATOR_H_ */
