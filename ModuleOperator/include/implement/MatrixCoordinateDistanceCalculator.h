/*
 * MatrixCoordinateDistanceCalculator.h
 *
 *  Created on: May 18, 2015
 *      Author: pham
 */

#ifndef MATRIXCOORDINATEDISTANCECALCULATOR_H_
#define MATRIXCOORDINATEDISTANCECALCULATOR_H_

#include "IDistanceCalculator.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"
#include "Coordinate.hpp"

class MatrixCoordinateDistanceCalculator: public IDistanceCalculator<MatrixCoordinatePtr> {
public:
	MatrixCoordinateDistanceCalculator(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator);
	virtual double distance(MatrixCoordinatePtr p1, MatrixCoordinatePtr p2);

private:
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
};


#endif /* MATRIXCOORDINATEDISTANCECALCULATOR_H_ */
