/*
 * IInnerProductCalculator.h
 *
 *  Created on: May 28, 2015
 *      Author: pham
 */

#ifndef IINNERPRODUCTCALCULATOR_H_
#define IINNERPRODUCTCALCULATOR_H_

#include "OperatorCommon.h"

template<typename E, typename Field>
class IInnerProductCalculator {
public:
	virtual ~IInnerProductCalculator(){};

	/**
	 * Calculate inner product of 2 element (vector) on field Field
	 * and store result to innerProduct
	 */
	virtual void innerProduct(E e1, E e2, Field& innerProduct) = 0;
};

#endif /* IINNERPRODUCTCALCULATOR_H_ */
