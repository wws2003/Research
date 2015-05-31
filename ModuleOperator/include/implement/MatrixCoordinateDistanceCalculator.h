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

class MatrixCoordinateDistanceCalculator: public IDistanceCalculator<MatrixRealCoordinatePtr> {
public:
	MatrixCoordinateDistanceCalculator(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator);
	virtual double distance(MatrixRealCoordinatePtr p1, MatrixRealCoordinatePtr p2);

private:
	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
};


#endif /* MATRIXCOORDINATEDISTANCECALCULATOR_H_ */
