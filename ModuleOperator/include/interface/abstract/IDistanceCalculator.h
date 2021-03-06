/*
 * IDistanceCalculator.h
 *
 *  Created on: May 12, 2015
 *      Author: pham
 */

#ifndef IDISTANCECALCULATOR_H_
#define IDISTANCECALCULATOR_H_

#include "OperatorCommon.h"

template<typename T>
class IDistanceCalculator {
public:
	virtual ~IDistanceCalculator(){};

	/**
	 * Calculate distance between to generic-typed element. Possibly vector, matrix or scalar
	 */
	virtual mreal_t distance(T p1, T p2) = 0;
};



#endif /* IDISTANCECALCULATOR_H_ */
