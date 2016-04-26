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
#include "IMatrixOperator.h"

class GateCoordinateConverterImpl: public IConverter<GatePtr, RealCoordinate<GatePtr> >{
public:
	GateCoordinateConverterImpl(GateRealCoordinateCalculatorPtr pGateCoordinateCalculator, MatrixOperatorPtr pMatrixOperator, bool phaseIgnored = false);

	//Override
	void convert12(const GatePtr& t1, RealCoordinate<GatePtr>& t2);

	//Override
	void convert21(const RealCoordinate<GatePtr>& t2, GatePtr& t1);

private:
	void getEquivalentCoordinates(const GatePtr& pGate, std::vector<RealCoordinatePtr<GatePtr> >& equivalentCoordinates);

	void getEquivalentGates(const RealCoordinatePtr<GatePtr> pGateCoord, std::vector<GatePtr> & equivalentGates);

	void getEquivalentPhaseFactors(std::vector<ComplexVal>& phaseFactors, int matrixSize);

	void releaseEquivalentGates(std::vector<GatePtr> & equivalentGates);

	void releaseEquivalentCoordinates(std::vector<RealCoordinatePtr<GatePtr> >& equivalentCoordinates);

	GateRealCoordinateCalculatorPtr m_pGateCoordinateCalculator;
	MatrixOperatorPtr m_pMatrixOperator;
	bool m_phaseIgnored;
};



#endif /* GATECOORDINATECONVERTERIMPL_H_ */
