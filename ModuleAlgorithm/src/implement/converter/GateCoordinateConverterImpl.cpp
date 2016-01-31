/*
 * GateCoordinateConverterImpl.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#include "GateCoordinateConverterImpl.h"
#include "Coordinate.hpp"
#include <algorithm>

template<typename T>
class CoordinateNormComparator {
public:
	bool operator()(const RealCoordinatePtr<T>& pLhs, const RealCoordinatePtr<T>& pRhs) {
		real_coordinate_t lCoord = pLhs->getCoordinates();
		real_coordinate_t rCoord = pRhs->getCoordinates();
		return getAbsCoordSum(lCoord) < getAbsCoordSum(rCoord);
	}

	inline mreal_t getAbsCoordSum(const real_coordinate_t& coord) const {
		mreal_t sqrNorm = 0.0;
		for(unsigned int i = 0; i < coord.size(); i++) {
			sqrNorm += mreal::abs(coord[i]);
		}
		return sqrNorm;
	}
};

GateCoordinateConverterImpl::GateCoordinateConverterImpl(GateRealCoordinateCalculatorPtr pGateCoordinateCalculator,
		MatrixOperatorPtr pMatrixOperator,
		bool phaseIgnored) {
	m_pGateCoordinateCalculator = pGateCoordinateCalculator;
	m_pMatrixOperator = pMatrixOperator;
	m_phaseIgnored = phaseIgnored;
}

//Override
void GateCoordinateConverterImpl::convert12(const GatePtr& pGate, RealCoordinate<GatePtr>& gateCoord) {
	std::vector<RealCoordinatePtr<GatePtr> > equivalentCoordinates;
	getEquivalentCoordinates(pGate, equivalentCoordinates);

	//Consider the smallest coordinate of equivalent ones to be the coordinate to be calculated;
	std::sort(equivalentCoordinates.begin(), equivalentCoordinates.end(), CoordinateNormComparator<GatePtr>());

	gateCoord = *equivalentCoordinates[0];

	//Release equivalent coordinate pointers
	releaseEquivalentCoordinates(equivalentCoordinates);
}

//Override
void GateCoordinateConverterImpl::convert21(const RealCoordinate<GatePtr>& gateCoord, GatePtr& pGate) {
	pGate = gateCoord.getElement();
}

void GateCoordinateConverterImpl::getEquivalentCoordinates(const GatePtr& pGate, std::vector<RealCoordinatePtr<GatePtr> >& equivalentCoordinates) {
	equivalentCoordinates.clear();

	RealCoordinatePtr<GatePtr> pGateCoord = NullPtr;
	m_pGateCoordinateCalculator->calulateElementCoordinate(pGate, pGateCoord);
	equivalentCoordinates.push_back(pGateCoord);

	std::vector<GatePtr> equivalentGates;
	getEquivalentGates(pGateCoord, equivalentGates);

	for(GatePtr pEqGate : equivalentGates) {
		RealCoordinatePtr<GatePtr> pEqGateCoord = NullPtr;
		m_pGateCoordinateCalculator->calulateElementCoordinate(pEqGate, pEqGateCoord);

		//Reset gate instance for pEqGateCoord
		pEqGateCoord->setElement(pGate);

		equivalentCoordinates.push_back(pEqGateCoord);
	}

	//Release all equivalent gates
	releaseEquivalentGates(equivalentGates);
}

void GateCoordinateConverterImpl::getEquivalentGates(const RealCoordinatePtr<GatePtr> pGateCoord,
		std::vector<GatePtr> & equivalentGates) {
	if(m_phaseIgnored) {
		GatePtr pGate = pGateCoord->getElement();
		int nbRows, nbColumns;
		pGate->getMatrix()->getSize(nbRows, nbColumns);
		if(nbRows == 2) {
			//Add minus matrix only if neccessary
			CoordinateNormComparator<GatePtr> comp;
			mreal_t absCoordSum = comp.getAbsCoordSum(pGateCoord->getCoordinates());
			if(absCoordSum > 0.5) {
				MatrixPtr pMinusMatrix = NullPtr;
				m_pMatrixOperator->multiplyScalar(pGate->getMatrix(), ComplexVal(-1.0, 0), pMinusMatrix);
				GatePtr pMinusGate(new Gate(pMinusMatrix, pGate->getCost(), pGate->getLabelSeq()));
				equivalentGates.push_back(pMinusGate);
			}
		}
	}
}

void GateCoordinateConverterImpl::releaseEquivalentGates(std::vector<GatePtr> & equivalentGates) {
	for(typename std::vector<GatePtr>::iterator gIter = equivalentGates.begin(); gIter != equivalentGates.end(); ) {
		GatePtr pEqGate = *gIter;
		gIter = equivalentGates.erase(gIter);
		_destroy(pEqGate);
	}
}

void GateCoordinateConverterImpl::releaseEquivalentCoordinates(std::vector<RealCoordinatePtr<GatePtr> >& equivalentCoordinates) {
	for(typename std::vector<RealCoordinatePtr<GatePtr> >::iterator cIter = equivalentCoordinates.begin(); cIter != equivalentCoordinates.end(); ) {
		RealCoordinatePtr<GatePtr> pCoord = *cIter;
		cIter = equivalentCoordinates.erase(cIter);
		_destroy(pCoord);
	}
}



