/*
 * CoordinateOnOrthonormalBasisCalculatorImpl.cpp
 *
 *  Created on: May 28, 2015
 *      Author: pham
 */

#include "CoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "Coordinate.hpp"

template<typename E, typename Field>
CoordinateOnOrthonormalBasisCalculatorImpl<E, Field>::CoordinateOnOrthonormalBasisCalculatorImpl(InnerProductCalculatorPtr<E, Field> pInnerProductCalculator, std::vector<E> basis) {
	m_basis.insert(m_basis.end(), basis.begin(), basis.end());
	m_pInnerProductCalculator = pInnerProductCalculator;
}

template<typename E, typename Field>
void CoordinateOnOrthonormalBasisCalculatorImpl<E, Field>::calulateElementCoordinate(E element, CoordinatePtrRef<E, Field> prCoordinate) const  {
	std::vector<Field> coordinates;

	Field basisSelfInnerProduct;
	m_pInnerProductCalculator->innerProduct(m_basis[0], m_basis[0], basisSelfInnerProduct);

	for(int i = 0; i < m_basis.size(); i++) {
		Field coordinate;
		Field innerProduct1;
		m_pInnerProductCalculator->innerProduct(element, m_basis[i]);
		coordinate = innerProduct1 / coordinate; //This compiles, surprisingly
		coordinates.push_back(coordinate);
	}

	prCoordinate = new Coordinate<E, Field>(element, coordinates);
}



