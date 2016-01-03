/*
 * GateCoordinateConverterImpl.h
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#ifndef GATECOORDINATECONVERTERIMPL_H_
#define GATECOORDINATECONVERTERIMPL_H_

#include "Gate.h"
#include "Coordinate.hpp"
#include "IConverter.h"
#include "ICoordinateCalculator.h"

class GateCoordinateConverterImpl: public IConverter<GatePtr, RealCoordinate<GatePtr> >{
public:
	GateCoordinateConverterImpl(GateRealCoordinateCalculatorPtr pGateCoordinateCalculator);

	//Override
	void convert12(const GatePtr& t1, RealCoordinate<GatePtr>& t2);

	//Override
	void convert21(const RealCoordinate<GatePtr>& t2, GatePtr& t1);

private:
	GateRealCoordinateCalculatorPtr m_pGateCoordinateCalculator;
};



#endif /* GATECOORDINATECONVERTERIMPL_H_ */
