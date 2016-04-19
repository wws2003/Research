/*
 * CoordinateDistanceCalculatorByElementImpl.hpp
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef COORDINATEDISTANCECALCULATORBYELEMENTIMPL_HPP_
#define COORDINATEDISTANCECALCULATORBYELEMENTIMPL_HPP_

#include "IDistanceCalculator.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"
#include "Coordinate.hpp"

template<typename T, typename F>
class CoordinateDistanceCalculatorByElementImpl : public IDistanceCalculator<CoordinatePtr<T,F> >{
public:
	CoordinateDistanceCalculatorByElementImpl(DistanceCalculatorPtr<T> pElementDistanceCalculator);

	virtual ~CoordinateDistanceCalculatorByElementImpl(){};

	virtual mreal_t distance(CoordinatePtr<T,F> pCoord1, CoordinatePtr<T,F> pCoord2);

private:
	DistanceCalculatorPtr<T> m_pElementDistanceCalculator;
};

template<typename T, typename F>
CoordinateDistanceCalculatorByElementImpl<T,F>::CoordinateDistanceCalculatorByElementImpl(DistanceCalculatorPtr<T> pElementDistanceCalculator) {
	m_pElementDistanceCalculator = pElementDistanceCalculator;
}

template<typename T, typename F>
mreal_t CoordinateDistanceCalculatorByElementImpl<T,F>::distance(CoordinatePtr<T,F> pCoord1, CoordinatePtr<T,F> pCoord2) {
	return m_pElementDistanceCalculator->distance(pCoord1->getElement(), pCoord2->getElement());
}


#endif /* COORDINATEDISTANCECALCULATORBYELEMENTIMPL_HPP_ */
