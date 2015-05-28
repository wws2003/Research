/*
 * ICoordinateCalculator.h
 *
 *  Created on: May 28, 2015
 *      Author: pham
 */

#ifndef ICOORDINATECALCULATOR_H_
#define ICOORDINATECALCULATOR_H_

#include "MathConceptsCommon.h"
#include "OperatorCommon.h"

template<typename E, typename Field>
class ICoordinateCalculator {
public:
	virtual ~ICoordinateCalculator(){};

	/**
	 *Calculate the coordinate of element (vector) on the vector space on field Field
	 * and store the result to prCoordinate
	 */
	virtual void calulateElementCoordinate(E element, CoordinatePtrRef<E, Field> prCoordinate) const = 0;
};



#endif /* ICOORDINATECALCULATOR_H_ */
