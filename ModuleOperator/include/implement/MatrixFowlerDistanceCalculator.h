/*
 * MatrixFowlerDistanceCalculator.h
 *
 *  Created on: Jul 21, 2015
 *      Author: pham
 */

#ifndef MATRIXFOWLERDISTANCECALCULATOR_H_
#define MATRIXFOWLERDISTANCECALCULATOR_H_

#include "IDistanceCalculator.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"

class MatrixFowlerDistanceCalculator : public IDistanceCalculator<MatrixPtr> {
public:
	MatrixFowlerDistanceCalculator(MatrixOperatorPtr pMatrixOperator);

	virtual double distance(MatrixPtr p1, MatrixPtr p2);

private:
	MatrixOperatorPtr m_pMatrixOperator;
};


#endif /* MATRIXFOWLERDISTANCECALCULATOR_H_ */
