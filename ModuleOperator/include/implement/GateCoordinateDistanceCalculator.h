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

#define USE_TEMPLATE 1

#include "CoordinateDistanceCalculatorByElementImpl.hpp"

#if USE_TEMPLATE

typedef CoordinateDistanceCalculatorByElementImpl<GatePtr, double> GateCoordinateDistanceCalculator;

#endif /* GATECOORDINATEDISTANCECALCULATOR_H_ */
