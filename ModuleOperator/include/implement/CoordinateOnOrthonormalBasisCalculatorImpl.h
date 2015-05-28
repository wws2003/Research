/*
 * CoordinateOnOrthonormalBasisCalculatorImpl.h
 *
 *  Created on: May 28, 2015
 *      Author: pham
 */

#ifndef COORDINATEONORTHONORMALBASISCALCULATORIMPL_H_
#define COORDINATEONORTHONORMALBASISCALCULATORIMPL_H_

#include "ICoordinateCalculator.h"
#include "OperatorCommon.h"
#include <vector>

template<typename E, typename Field>
class CoordinateOnOrthonormalBasisCalculatorImpl: public ICoordinateCalculator<E, Field> {
public:
	CoordinateOnOrthonormalBasisCalculatorImpl(InnerProductCalculatorPtr<E, Field> pInnerProductCalculator, std::vector<E> basis);
	virtual ~CoordinateOnOrthonormalBasisCalculatorImpl(){};

	//Override
	virtual void calulateElementCoordinate(E element, CoordinatePtrRef<E, Field> prCoordinate) const ;

private:
	std::vector<E> m_basis;
	InnerProductCalculatorPtr<E, Field> m_pInnerProductCalculator;
};


#endif /* COORDINATEONORTHONORMALBASISCALCULATORIMPL_H_ */
