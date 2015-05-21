/*
 * Coordinate.h
 *
 *  Created on: May 18, 2015
 *      Author: pham
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "MathConceptsCommon.h"

template<typename E>
class Coordinate {
public:
	Coordinate(VectorPtr coordinates, E element):m_pCoordinates(coordinates), m_element(element){};

	virtual ~Coordinate(){};

	VectorPtr getCoordinates(){return m_pCoordinates;}
	E getElement(){return m_element;}

private:
	VectorPtr m_pCoordinates;
	E m_element;
};


#endif /* COORDINATE_H_ */
