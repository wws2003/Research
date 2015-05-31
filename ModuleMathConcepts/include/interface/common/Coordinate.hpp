/*
 * Coordinate.h
 *
 *  Created on: May 18, 2015
 *      Author: pham
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "MathConceptsCommon.h"
#include <vector>

template<typename E, typename Field>
class Coordinate {
public:
	Coordinate(E element, const std::vector<Field>& coordinates):m_element(element){
		m_coordinates.clear();
		m_coordinates.insert(m_coordinates.end(), coordinates.begin(), coordinates.end());
	};

	virtual ~Coordinate(){};

	const std::vector<Field>& getCoordinates(){return m_coordinates;}
	E getElement(){return m_element;}

private:
	std::vector<Field> m_coordinates;
	E m_element;
};


#endif /* COORDINATE_H_ */
