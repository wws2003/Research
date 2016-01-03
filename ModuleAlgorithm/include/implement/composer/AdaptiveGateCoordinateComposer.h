/*
 * AdaptiveGateCoordinateComposer.h
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#ifndef ADAPTIVEGATECOORDINATECOMPOSER_H_
#define ADAPTIVEGATECOORDINATECOMPOSER_H_

#include "Gate.h"
#include "AdaptiveElementComposer.h"
#include "Coordinate.hpp"

typedef class AdaptiveElementComposer<GatePtr, RealCoordinate<GatePtr> > AdaptiveGateCoordinateComposer;

#endif /* ADAPTIVEGATECOORDINATECOMPOSER_H_ */
