/*
 * IMatrixDistanceCalculator.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IMATRIXDISTANCECALCULATOR_H_
#define IMATRIXDISTANCECALCULATOR_H_

#include "../math_concept/IMatrix.h"

class IMatrixDistanceCalculator {
public:
	virtual ~IMatrixDistanceCalculator(){};

	virtual double distance(MatrixPtr pm1, MatrixPtr pm2) = 0;
};

#endif /* IMATRIXDISTANCECALCULATOR_H_ */
