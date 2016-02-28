/*
 * GateCoordinateDistanceCalculator.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef GATECOORDINATEDISTANCECALCULATOR_H_
#define GATECOORDINATEDISTANCECALCULATOR_H_

#include "IDistanceCalculator.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"
#include "Coordinate.hpp"

#include "CoordinateDistanceCalculatorByElementImpl.hpp"

typedef CoordinateDistanceCalculatorByElementImpl<GatePtr, mreal_t> GateCoordinateDistanceCalculator;

#endif /* GATECOORDINATEDISTANCECALCULATOR_H_ */
